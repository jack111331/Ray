//
// Created by Edge on 2021/1/9.
//

#ifndef RAY_GROUPBVHTRANSLATOR_H
#define RAY_GROUPBVHTRANSLATOR_H


#include <GL/glew.h>
#include "GroupObj.h"

class GroupBVHTranslator {
    // TODO flatten material to texture
    // their leaf node of tlas is transform, my transform can be in any node
public:

    struct Node {
        Node() : boundingBoxMin(), boundingBoxMax(), lrLeaf() {}

        Vec4f boundingBoxMin;
        Vec4f boundingBoxMax;
        Vec4i lrLeaf;
    };

    GroupBVHTranslator(Scene *scene) : m_group(&scene->m_group[0]), m_tlasSize(0), m_meshTable(scene->m_meshTable),
                                       m_materialTable(scene->m_materialTable) {
        int nodeCount = 0;
        int totalVertices = 0;
        for (auto mesh: m_meshTable) {
            BLASNode *blasNode = (BLASNode *) mesh.second;
            if (blasNode->getBLASNodeType() == BLASNode::BLASNodeType::TRIANGLE_GROUP) {
                TriangleGroup *triangleGroup = (TriangleGroup *)mesh.second;
                nodeCount += blasNode->m_accel->m_octree->m_nodeCount;
                m_blasRootStartIndicesList.push_back(nodeCount);

                triangleGroup->m_accel->flattenBVH(totalVertices);
                m_meshRootStartIndicesList.push_back(m_flattenBVHIndices.size());
                m_flattenBVHIndices.insert(m_flattenBVHIndices.end(),
                                           triangleGroup->m_accel->m_octree->m_packedIndices.begin(),
                                           triangleGroup->m_accel->m_octree->m_packedIndices.end());
                for (auto primitive: triangleGroup->m_nodeList) {
                    m_flattenBVHVertices.push_back(Vec4f(primitive->m_coord, 1.0f));
                    m_flattenBVHNormals.push_back(Vec4f(primitive->m_normal, 0.0f));
                }
                totalVertices = m_flattenBVHVertices.size();
            }
        }

        traverseInitGroupFirstStage(m_group);
        m_tlasStartNodeIndex = nodeCount + m_meshInstanceSize;
        // TODO tag active mesh to store space?
        m_nodeList.resize(m_tlasStartNodeIndex + m_tlasSize);
        int meshId = 0;
        for (auto mesh: m_meshTable) {
            BLASNode *blasNode = (BLASNode *) mesh.second;
            if (blasNode->getBLASNodeType() == BLASNode::BLASNodeType::TRIANGLE_GROUP) {
                TriangleGroup *triangleGroup = (TriangleGroup *)mesh.second;
                traverseInitBLASNode(triangleGroup->m_accel->m_octree->m_root, nullptr, m_meshRootStartIndicesList[meshId]);
                meshId++;
            }
        }
        m_transformMatList.resize(m_meshInstanceSize);
        traverseInitGroupSecondStage(m_group, nullptr);

        reduceNode(m_tlasStartNodeIndex);
        // TODO update original node idx from nodeList
        // TODO compress space

        std::cout << "TLAS Start Node Index " << m_tlasStartNodeIndex << ", mesh instance size " << m_meshInstanceSize << " , tlas size " << m_tlasSize << std::endl;
        std::cout << "flattenBVHIndices " << m_flattenBVHIndices.size() << std::endl;
        for (int i = 0; i < m_flattenBVHIndices.size(); i += 3) {
            std::cout << m_flattenBVHIndices[i] << " " << m_flattenBVHIndices[i + 1] << " " << m_flattenBVHIndices[i + 2]
                      << std::endl;
        }

        std::cout << "node" << std::endl;
        for (int i = 0; i < m_nodeList.size(); ++i) {
            std::cout << "(" << m_nodeList[i].boundingBoxMin.x << ", " << m_nodeList[i].boundingBoxMin.y << ", " << m_nodeList[i].boundingBoxMin.z << ") (" << m_nodeList[i].boundingBoxMax.x << ", " << m_nodeList[i].boundingBoxMax.y << ", " << m_nodeList[i].boundingBoxMax.z << ") " << " " << m_nodeList[i].lrLeaf.x << " " << m_nodeList[i].lrLeaf.y << " " << m_nodeList[i].lrLeaf.z
                      << std::endl;
        }

        // initialize ssbo
        glGenBuffers(1, &m_meshIndicesSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_meshIndicesSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_flattenBVHIndices.size() * sizeof(uint32_t), m_flattenBVHIndices.data(),
                     GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_bvhSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bvhSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_nodeList.size() * sizeof(Node), m_nodeList.data(), GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_meshVerticesSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_meshVerticesSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_flattenBVHVertices.size() * sizeof(Vec4f), m_flattenBVHVertices.data(), GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_meshNormalsSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_meshNormalsSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_flattenBVHNormals.size() * sizeof(Vec4f), m_flattenBVHNormals.data(), GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_transformSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_transformSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_transformMatList.size() * sizeof(glm::mat4), m_transformMatList.data(), GL_DYNAMIC_DRAW);
// reserve all transform node and selector node, but transform will goes to concrete geometry
    }

    void updateTranslator() {
        for (auto member: m_group->m_groupMemberList) {
            traverseUpdateGroup(member);
        }
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bvhSSBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_tlasStartNodeIndex * sizeof(Node),
                        (m_nodeList.size() - m_tlasStartNodeIndex) * sizeof(Node), &m_nodeList[m_tlasStartNodeIndex]);
    }

private:
    void traverseInitGroupFirstStage(ObjectNode *node);

    int traverseInitGroupSecondStage(ObjectNode *node, ObjectNode *rightNode, const glm::mat4 &transformMat = glm::mat4(1.0f));

    int traverseInitBLASNode(OctreeNode *node, OctreeNode *rightNode, int meshStartIndices);

    void reduceNode(int nodeIdx);

    void traverseUpdateGroup(ObjectNode *node);

    GroupObj *m_group;
    int m_tlasSize = 0;
    int m_meshInstanceSize = 0;

    std::vector<Node> m_nodeList;
    int m_curTlas = 0;
    int m_curBlas = 0;
    int m_curMeshInstance = 0;

    std::vector<uint32_t> m_flattenBVHIndices;
    std::vector<Vec4f> m_flattenBVHVertices;
    std::vector<Vec4f> m_flattenBVHNormals;
    std::vector<glm::mat4> m_transformMatList;

    std::vector<int> m_blasRootStartIndicesList;
    std::vector<int> m_meshRootStartIndicesList;

    const std::map<std::string, Material *> &m_materialTable;
    const std::map<std::string, ObjectNode *> &m_meshTable;

public:
    int m_tlasStartNodeIndex = 0;

    uint32_t m_meshIndicesSSBO;
    uint32_t m_meshVerticesSSBO;
    uint32_t m_meshNormalsSSBO;
    uint32_t m_transformSSBO;
    uint32_t m_bvhSSBO;
};


#endif //RAY_GROUPBVHTRANSLATOR_H
