//
// Created by Edge on 2021/1/9.
//

#ifndef RAY_GROUPBVHTRANSLATOR_H
#define RAY_GROUPBVHTRANSLATOR_H


#include "GroupObj.h"
#include "Lambertian.h"
#include "GLUtil.h"


class GroupBVHTranslator {
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
                TriangleGroup *triangleGroup = (TriangleGroup *) mesh.second;
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

        for (auto material: m_materialTable) {
            m_materialList.push_back(material.second->getProperty());
            m_materialIdTable[material.second] = m_materialList.size()-1;
        }

        traverseInitGroupFirstStage(m_group);
        m_meshNodeList.resize(nodeCount);
        m_blasNodeList.resize(m_meshInstanceSize);
        m_tlasNodeList.resize(m_tlasSize);
        int meshId = 0;
        for (auto mesh: m_meshTable) {
            BLASNode *blasNode = (BLASNode *) mesh.second;
            if (blasNode->getBLASNodeType() == BLASNode::BLASNodeType::TRIANGLE_GROUP) {
                TriangleGroup *triangleGroup = (TriangleGroup *) mesh.second;
                traverseInitMeshNode(triangleGroup->m_accel->m_octree->m_root, nullptr,
                                     m_meshRootStartIndicesList[meshId]);
                meshId++;
            }
        }
        m_transformMatList.resize(m_meshInstanceSize);
        m_materialInstanceList.resize(m_meshInstanceSize);
        traverseInitBLASGroupSecondStage(m_group, nullptr);
        traverseInitTLASGroupSecondStage(m_group, nullptr);
        m_nodeList.insert(m_nodeList.end(), m_meshNodeList.begin(), m_meshNodeList.end());
        m_nodeList.insert(m_nodeList.end(), m_blasNodeList.begin(), m_blasNodeList.end());
        m_nodeList.insert(m_nodeList.end(), m_tlasNodeList.begin(), m_tlasNodeList.end());
        m_tlasStartNodeIndex = m_meshNodeList.size() + m_blasNodeList.size() + m_group->m_inTwoLevelBVHId;
        reduceNode(m_tlasStartNodeIndex);
        // TODO compress mesh and blas space
#ifdef DEBUG_FLAG
        std::cout << "TLAS Start Node Index " << m_tlasStartNodeIndex << ", mesh instance size " << m_meshInstanceSize
                  << " , tlas size " << m_tlasSize << std::endl;
        std::cout << "flattenBVHIndices " << m_flattenBVHIndices.size() << std::endl;
        for (int i = 0; i < m_flattenBVHIndices.size(); i += 3) {
            std::cout << m_flattenBVHIndices[i] << " " << m_flattenBVHIndices[i + 1] << " "
                      << m_flattenBVHIndices[i + 2]
                      << std::endl;
        }

        std::cout << "node" << std::endl;
        for (int i = 0; i < m_nodeList.size(); ++i) {
            std::cout << "(" << m_nodeList[i].boundingBoxMin.x << ", " << m_nodeList[i].boundingBoxMin.y << ", "
                      << m_nodeList[i].boundingBoxMin.z << ") (" << m_nodeList[i].boundingBoxMax.x << ", "
                      << m_nodeList[i].boundingBoxMax.y << ", " << m_nodeList[i].boundingBoxMax.z << ") " << " "
                      << m_nodeList[i].lrLeaf.x << " " << m_nodeList[i].lrLeaf.y << " " << m_nodeList[i].lrLeaf.z
                      << std::endl;
        }
#endif
        // initialize ssbo
        RayUtil::generateStaticSSBO(m_meshIndicesSSBO, m_flattenBVHIndices.data(),
                                    m_flattenBVHIndices.size() * sizeof(uint32_t));
        RayUtil::generateDynamicSSBO(m_bvhSSBO, m_nodeList.data(), m_nodeList.size() * sizeof(Node));
        RayUtil::generateStaticSSBO(m_meshVerticesSSBO, m_flattenBVHVertices.data(),
                                    m_flattenBVHVertices.size() * sizeof(Vec4f));
        RayUtil::generateStaticSSBO(m_meshNormalsSSBO, m_flattenBVHNormals.data(),
                                    m_flattenBVHNormals.size() * sizeof(Vec4f));
        RayUtil::generateDynamicSSBO(m_transformSSBO, m_transformMatList.data(),
                                     m_transformMatList.size() * sizeof(glm::mat4));
        RayUtil::generateDynamicSSBO(m_materialSSBO, m_materialList.data(), m_materialList.size() * sizeof(Material::MaterialProperty));
        RayUtil::generateDynamicSSBO(m_materialInstanceSSBO, m_materialInstanceList.data(),
                                     m_materialInstanceList.size() * sizeof(uint32_t));

        // reserve all transform node and selector node, but transform will goes to concrete geometry
    }

    void updateTranslator() {
        m_curTlas = 0;
        traverseInitTLASGroupSecondStage(m_group, nullptr);
        m_nodeList.resize(m_meshNodeList.size() + m_blasNodeList.size());
        m_nodeList.insert(m_nodeList.end(), m_tlasNodeList.begin(), m_tlasNodeList.end());
        m_tlasStartNodeIndex = m_meshNodeList.size() + m_blasNodeList.size() + m_group->m_inTwoLevelBVHId;
        reduceNode(m_tlasStartNodeIndex);

        RayUtil::updateSSBO(m_bvhSSBO, &m_nodeList[m_meshNodeList.size() + m_blasNodeList.size()],
                            (m_meshNodeList.size() + m_blasNodeList.size()) * sizeof(Node),
                            m_tlasNodeList.size() * sizeof(Node));
    }

private:
    void traverseInitGroupFirstStage(ObjectNode *node);

    void traverseInitBLASGroupSecondStage(ObjectNode *node, ObjectNode *rightNode,
                                         const glm::mat4 &transformMat = glm::mat4(1.0f));

    void traverseInitTLASGroupSecondStage(ObjectNode *node, ObjectNode *rightNode);

    void traverseInitMeshNode(OctreeNode *node, OctreeNode *rightNode, int meshStartIndices);

    void reduceNode(int nodeIdx);

    GroupObj *m_group;
    int m_tlasSize = 0;
    int m_meshInstanceSize = 0;

    std::vector<Node> m_meshNodeList;
    std::vector<Node> m_blasNodeList;
    std::vector<Node> m_tlasNodeList;
    // node for all 3 above nodes
    std::vector<Node> m_nodeList;
    // final layout is [meshNode, blasNode, tlasNode]
    int m_curMesh = 0;
    int m_curBlas = 0;
    int m_curTlas = 0;
    int m_curMeshInstance = 0;

    std::vector<uint32_t> m_flattenBVHIndices;
    std::vector<Vec4f> m_flattenBVHVertices;
    std::vector<Vec4f> m_flattenBVHNormals;
    std::vector<glm::mat4> m_transformMatList;
    std::vector<uint32_t> m_materialInstanceList;

    std::vector<int> m_blasRootStartIndicesList;
    std::vector<int> m_meshRootStartIndicesList;

    const std::map<std::string, Material *> &m_materialTable;
    const std::map<std::string, ObjectNode *> &m_meshTable;

    std::vector<Material::MaterialProperty> m_materialList;
    std::map<Material *, int> m_materialIdTable;

public:
    int m_tlasStartNodeIndex = 0;

    uint32_t m_meshIndicesSSBO;
    uint32_t m_meshVerticesSSBO;
    uint32_t m_meshNormalsSSBO;
    uint32_t m_transformSSBO;
    uint32_t m_materialInstanceSSBO;
    uint32_t m_bvhSSBO;
    uint32_t m_materialSSBO;
};


#endif //RAY_GROUPBVHTRANSLATOR_H
