//
// Created by Edge on 2021/1/9.
//

#include "GroupBVHTranslator.h"

void GroupBVHTranslator::traverseInitGroupFirstStage(ObjectNode *node) {
    if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::TLAS) {
        TLASNode *tlasNode = (TLASNode *) node;
        m_tlasSize++;
        for (auto member: tlasNode->m_groupMemberList) {
            traverseInitGroupFirstStage(member);
        }
    } else if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
        BLASNode *blasNode = (BLASNode *) node;
        if (blasNode->getBLASNodeType() == BLASNode::BLASNodeType::TRIANGLE_GROUP) {
            m_blasSize++;
        }
    }
}

int GroupBVHTranslator::traverseInitGroupSecondStage(ObjectNode *node, ObjectNode *rightNode) {
    if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::TLAS) {
        node->m_inTwoLevelBVHId = m_tlasStartNodeIndex + m_curTlas;
        m_curTlas++;
        TLASNode *tlasNode = (TLASNode *) node;
        for (int memberId = tlasNode->m_groupMemberList.size() - 1; memberId >= 0; --memberId) {
            m_nodeList[tlasNode->m_inTwoLevelBVHId].boundingBox.updateBoundingBox(
                    tlasNode->m_groupMemberList[memberId]->getBoundingBox());
            traverseInitGroupSecondStage(tlasNode->m_groupMemberList[memberId],
                                         memberId + 1 >= tlasNode->m_groupMemberList.size() ? nullptr
                                                                                            : tlasNode->m_groupMemberList[
                                                 memberId + 1]);
        }
        if (tlasNode->getTlasNodeType() == TLASNode::TLASNodeType::TRANSFORM) {
            // TODO maybe i can use non zero of z to express transform index?
//            m_nodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec3i(tlasNode->m_groupMemberList[0]->m_inTwoLevelBVHId,
//            rightNode ? rightNode->m_inTwoLevelBVHId : -1, 0);
        } else {
            m_nodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec3i(tlasNode->m_groupMemberList[0]->m_inTwoLevelBVHId,
                                                                   rightNode ? rightNode->m_inTwoLevelBVHId : -1, 0);
        }
    } else if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
        BLASNode *blasNode = (BLASNode *) node;
        if (blasNode->getBLASNodeType() == BLASNode::BLASNodeType::TRIANGLE_GROUP) {
            TriangleGroup *triangleGroup = (TriangleGroup *) blasNode;
            triangleGroup->m_accel->flattenBVH();
            m_flattenBVHVertices.insert(m_flattenBVHVertices.end(),
                                        triangleGroup->m_accel->m_octree->m_packedIndices.begin(),
                                        triangleGroup->m_accel->m_octree->m_packedIndices.end());
            traverseInitBLASNode(triangleGroup->m_accel->m_octree->m_root, nullptr, m_curBlas);
            m_curBlasBVHNodeIndex += triangleGroup->m_accel->m_octree->m_packedIndices.size();
        }
    }
}

int GroupBVHTranslator::traverseInitBLASNode(OctreeNode *node, OctreeNode *rightNode, int &curBlasNode) {
    node->m_inTwoLevelBVHId = curBlasNode++;
    m_nodeList[node->m_inTwoLevelBVHId].boundingBox = node->m_nodeBoundingBox;
    OctreeNode *latestNode = nullptr;
    for (uint32_t i = 7; i >= 0; --i) {
        if (node->m_child[i]) {
            traverseInitBLASNode(node->m_child[i], latestNode, curBlasNode);
            latestNode = node->m_child[i];
        }
    }
    if (node->m_isLeaf) {
        m_nodeList[node->m_inTwoLevelBVHId].lrLeaf = Vec3i(m_curBlasBVHNodeIndex + node->m_startIdx,
                                                           node->m_data.size(), 1);
    } else {
        m_nodeList[node->m_inTwoLevelBVHId].lrLeaf = Vec3i(
                latestNode ? latestNode->m_inTwoLevelBVHId : -1, rightNode ? rightNode->m_inTwoLevelBVHId : -1, 0);
    }
}

void GroupBVHTranslator::traverseUpdateGroup(ObjectNode *node) {
    if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::TLAS) {
        TLASNode *tlasNode = (TLASNode *) node;
        m_nodeList[tlasNode->m_inTwoLevelBVHId].boundingBox = ObjectBoundingBox();
        for (int memberId = tlasNode->m_groupMemberList.size() - 1; memberId >= 0; --memberId) {
            m_nodeList[tlasNode->m_inTwoLevelBVHId].boundingBox.updateBoundingBox(
                    tlasNode->m_groupMemberList[memberId]->getBoundingBox());
            traverseUpdateGroup(tlasNode->m_groupMemberList[memberId]);
        }
    }
}