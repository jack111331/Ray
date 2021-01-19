//
// Created by Edge on 2021/1/9.
//

#include <TransformObj.h>
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
            m_meshInstanceSize++;
        }
    }
}

int GroupBVHTranslator::traverseInitGroupSecondStage(ObjectNode *node, ObjectNode *rightNode,
                                                     const glm::mat4 &transformMat) {
    if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::TLAS) {
        node->m_inTwoLevelBVHId = m_tlasStartNodeIndex + m_curTlas;
        m_curTlas++;
        TLASNode *tlasNode = (TLASNode *) node;
        bool isTransformObj = false;
        glm::mat4 newTransformMat;

        if (tlasNode->getTlasNodeType() == TLASNode::TLASNodeType::TRANSFORM) {
            isTransformObj = true;
            TransformObj *transformObj = (TransformObj *) tlasNode;
            newTransformMat = transformObj->getTransformMat() * transformMat;
        }

        for (int memberId = tlasNode->m_groupMemberList.size() - 1; memberId >= 0; --memberId) {
            traverseInitGroupSecondStage(tlasNode->m_groupMemberList[memberId],
                                         memberId + 1 >= tlasNode->m_groupMemberList.size() ? nullptr
                                                                                            : tlasNode->m_groupMemberList[
                                                 memberId + 1],
                                         isTransformObj ? newTransformMat
                                                        : transformMat);
        }
        m_nodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec4i(tlasNode->m_groupMemberList[0]->m_inTwoLevelBVHId,
                                                               rightNode ? rightNode->m_inTwoLevelBVHId : -1, 0, 0);
    } else if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
        BLASNode *blasNode = (BLASNode *) node;
        if (blasNode->getBLASNodeType() == BLASNode::BLASNodeType::TRIANGLE_GROUP) {
            m_transformMatList[m_curMeshInstance] = transformMat;
            TriangleGroup *triangleGroup = (TriangleGroup *) blasNode;

            triangleGroup->m_inTwoLevelBVHId = m_curBlas++;
            m_nodeList[triangleGroup->m_inTwoLevelBVHId].lrLeaf = Vec4i(triangleGroup->m_accel->m_octree->m_root->m_inTwoLevelBVHId, rightNode?rightNode->m_inTwoLevelBVHId:-1, -m_curMeshInstance - 1, 1);
            m_curMeshInstance++;
        }
    }
}

int GroupBVHTranslator::traverseInitBLASNode(OctreeNode *node, OctreeNode *rightNode, int meshStartIndices) {
    node->m_inTwoLevelBVHId = m_curBlas++;

    if (node->m_isLeaf) {
        if(rightNode) {
            m_tlasStartNodeIndex++;
            int intermediateNodeId = node->m_inTwoLevelBVHId;
            node->m_inTwoLevelBVHId = m_curBlas++;
            m_nodeList.insert(m_nodeList.begin()+node->m_inTwoLevelBVHId, Node());
            m_nodeList[intermediateNodeId].lrLeaf = Vec4i(node->m_inTwoLevelBVHId,
                                                          rightNode->m_intermediateNodeIdx==-1?rightNode->m_inTwoLevelBVHId:rightNode->m_intermediateNodeIdx, 0, 0);
            node->m_intermediateNodeIdx = intermediateNodeId;
        }
        m_nodeList[node->m_inTwoLevelBVHId].boundingBoxMin = Vec4f(node->m_nodeBoundingBox.m_bounding[0], 1.0f);
        m_nodeList[node->m_inTwoLevelBVHId].boundingBoxMax = Vec4f(node->m_nodeBoundingBox.m_bounding[1], 1.0f);
        m_nodeList[node->m_inTwoLevelBVHId].lrLeaf = Vec4i(meshStartIndices + node->m_startIdx,
                                                           node->m_data.size(), 1, 0);
    } else {
        OctreeNode *latestNode = nullptr;
        for (int i = 7; i >= 0; --i) {
            if (node->m_child[i]) {
                traverseInitBLASNode(node->m_child[i], latestNode, meshStartIndices);
                latestNode = node->m_child[i];
            }
        }
        m_nodeList[node->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                latestNode->m_intermediateNodeIdx==-1?latestNode->m_inTwoLevelBVHId:latestNode->m_intermediateNodeIdx, rightNode ? (rightNode->m_intermediateNodeIdx==-1?rightNode->m_inTwoLevelBVHId:rightNode->m_intermediateNodeIdx) : -1, 0, 0);
    }
}

void GroupBVHTranslator::reduceNode(int nodeIdx) {
    // backward?
    while(m_nodeList[nodeIdx].lrLeaf.z <= 0 && m_nodeList[nodeIdx].lrLeaf.y == -1) {
        if(m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].lrLeaf.z > 0) {
            m_nodeList[nodeIdx].boundingBoxMin = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].boundingBoxMin;
            m_nodeList[nodeIdx].boundingBoxMax = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].boundingBoxMax;
        }
        if(m_nodeList[nodeIdx].lrLeaf.z < 0) {
            int meshInstanceIdx = m_nodeList[nodeIdx].lrLeaf.z;
            m_nodeList[nodeIdx].lrLeaf = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].lrLeaf;
            m_nodeList[nodeIdx].lrLeaf.z = meshInstanceIdx;
        } else {
            m_nodeList[nodeIdx].lrLeaf = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].lrLeaf;
        }
    }
    if(m_nodeList[nodeIdx].lrLeaf.z <= 0) {
        // what if i become mesh node?
        reduceNode(m_nodeList[nodeIdx].lrLeaf.x);
        reduceNode(m_nodeList[nodeIdx].lrLeaf.y);
        m_nodeList[nodeIdx].boundingBoxMin = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].boundingBoxMin;
        m_nodeList[nodeIdx].boundingBoxMin.min(m_nodeList[m_nodeList[nodeIdx].lrLeaf.y].boundingBoxMin);
        m_nodeList[nodeIdx].boundingBoxMax = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].boundingBoxMax;
        m_nodeList[nodeIdx].boundingBoxMax.max(m_nodeList[m_nodeList[nodeIdx].lrLeaf.y].boundingBoxMax);
    }
}

void GroupBVHTranslator::traverseUpdateGroup(ObjectNode *node) {
    if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::TLAS) {
        TLASNode *tlasNode = (TLASNode *) node;
        ObjectBoundingBox boundingBox;
        // TODO should update right node
        for (int memberId = tlasNode->m_groupMemberList.size() - 1; memberId >= 0; --memberId) {
            boundingBox.updateBoundingBox(
                    tlasNode->m_groupMemberList[memberId]->getBoundingBox());
            traverseUpdateGroup(tlasNode->m_groupMemberList[memberId]);
        }
        m_nodeList[tlasNode->m_inTwoLevelBVHId].boundingBoxMin = Vec4f(boundingBox.m_bounding[0], 1.0f);
        m_nodeList[tlasNode->m_inTwoLevelBVHId].boundingBoxMax = Vec4f(boundingBox.m_bounding[1], 1.0f);
    }
}