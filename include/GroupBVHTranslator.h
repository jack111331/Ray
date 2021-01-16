//
// Created by Edge on 2021/1/9.
//

#ifndef RAY_GROUPBVHTRANSLATOR_H
#define RAY_GROUPBVHTRANSLATOR_H


#include <GL/glew.h>
#include "GroupObj.h"

class GroupBVHTranslator {
    // TODO flatten transform to texture
    // TODO flatten material to texture
    // their leaf node of tlas is transform, my transform can be in any node
public:

    struct Node {
        Node() : boundingBox(), lrLeaf() {}
        ObjectBoundingBox boundingBox;
        Vec3i lrLeaf;
    };

    GroupBVHTranslator(Scene *scene) : m_group(&scene->m_group[0]), m_tlasSize(0), m_meshTable(scene->m_meshTable), m_materialTable(scene->m_materialTable) {
        int nodeCount = 0;
        for(auto mesh: m_meshTable) {
            BLASNode *blasNode = (BLASNode *)mesh.second;
            if(blasNode->getBLASNodeType() == BLASNode::BLASNodeType::TRIANGLE_GROUP) {
                nodeCount += blasNode->m_accel->m_octree->m_nodeCount;
                m_blasRootStartIndicesList.push_back(nodeCount);
            }
        }
        m_tlasStartNodeIndex = nodeCount;
        traverseInitGroupFirstStage(m_group);
        m_nodeList.resize(m_tlasStartNodeIndex + 2 * m_blasSize);
        traverseInitGroupSecondStage(m_group, nullptr);

        // initialize ssbo
        glGenBuffers(1, &m_meshSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_meshSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_flattenBVHVertices.size() * sizeof(float), m_flattenBVHVertices.data(), GL_DYNAMIC_DRAW);

        glGenBuffers(1, &m_bvhSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bvhSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, m_nodeList.size() * sizeof(Node), m_nodeList.data(), GL_DYNAMIC_DRAW);

    }

    void updateTranslator() {
        for(auto member: m_group->m_groupMemberList) {
            traverseUpdateGroup(member);
        }
        glGenBuffers(1, &m_bvhSSBO);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_bvhSSBO);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, m_tlasStartNodeIndex * sizeof(Node), (m_nodeList.size() - m_tlasStartNodeIndex) * sizeof(Node), &m_nodeList[m_tlasStartNodeIndex]);
    }
private:
    void traverseInitGroupFirstStage(ObjectNode *node);

    int traverseInitGroupSecondStage(ObjectNode *node, ObjectNode *rightNode);

    int traverseInitBLASNode(OctreeNode *node, OctreeNode *rightNode, int &curBlasNode);

    void traverseUpdateGroup(ObjectNode *node);

    GroupObj * m_group;
    int m_tlasSize = 0;
    int m_blasSize = 0;

    std::vector<Node> m_nodeList;
    int m_tlasStartNodeIndex = 0;
    int m_curTlas = 0;
    int m_curBlas = 0;

    std::vector<uint32_t> m_flattenBVHVertices;

    // Define currently in which mesh
    int m_curBlasBVHNodeIndex = 0;

    std::vector<int> m_blasRootStartIndicesList;

    const std::map<std::string, Material *> &m_materialTable;
    const std::map<std::string, ObjectNode *> &m_meshTable;

    // TODO SSBO buffer
    uint32_t m_meshSSBO;
    uint32_t m_bvhSSBO;
};


#endif //RAY_GROUPBVHTRANSLATOR_H
