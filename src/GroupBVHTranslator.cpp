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

int GroupBVHTranslator::traverseInitBLASGroupSecondStage(ObjectNode *node, ObjectNode *rightNode,
                                                         const glm::mat4 &transformMat) {
    if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::TLAS) {
        TLASNode *tlasNode = (TLASNode *) node;

        bool isTransformObj = false;
        glm::mat4 newTransformMat;

        if (tlasNode->getTlasNodeType() == TLASNode::TLASNodeType::TRANSFORM) {
            isTransformObj = true;
            TransformObj *transformObj = (TransformObj *) tlasNode;
            newTransformMat = transformObj->getTransformMat() * transformMat;
        }

        for (int memberId = tlasNode->m_groupMemberList.size() - 1; memberId >= 0; --memberId) {
            traverseInitBLASGroupSecondStage(tlasNode->m_groupMemberList[memberId],
                                             memberId + 1 >= tlasNode->m_groupMemberList.size() ? nullptr
                                                                                                : tlasNode->m_groupMemberList[
                                                     memberId + 1],
                                             isTransformObj ? newTransformMat
                                                            : transformMat);
        }
    } else if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
        BLASNode *blasNode = (BLASNode *) node;
        if (blasNode->getBLASNodeType() == BLASNode::BLASNodeType::TRIANGLE_GROUP) {
            m_transformMatList[m_curMeshInstance] = transformMat;
            m_materialInstanceList[m_curMeshInstance] = m_materialIdTable[blasNode->getMaterial()];
            TriangleGroup *triangleGroup = (TriangleGroup *) blasNode;

            triangleGroup->m_inTwoLevelBVHId = m_curBlas++;
            if (rightNode) {
                BLASNode *rightBlasNode = (BLASNode *) rightNode;
                int intermediateNodeId = triangleGroup->m_inTwoLevelBVHId;
                m_blasNodeList.insert(m_blasNodeList.begin() + triangleGroup->m_inTwoLevelBVHId, Node());
                triangleGroup->m_inTwoLevelBVHId = m_curBlas++;
                m_blasNodeList[intermediateNodeId].lrLeaf = Vec4i(m_curMesh + triangleGroup->m_inTwoLevelBVHId,
                                                                  m_curMesh +
                                                                  (rightBlasNode->m_intermediateNodeIdx == -1
                                                                   ? rightBlasNode->m_inTwoLevelBVHId
                                                                   : rightBlasNode->m_intermediateNodeIdx), 0, 0);
                triangleGroup->m_intermediateNodeIdx = intermediateNodeId;
            }
            m_blasNodeList[triangleGroup->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                    triangleGroup->m_accel->m_octree->m_root->m_inTwoLevelBVHId, -1, -m_curMeshInstance - 1, 1);
            m_curMeshInstance++;
        }
    }
}

int GroupBVHTranslator::traverseInitTLASGroupSecondStage(ObjectNode *node, ObjectNode *rightNode) {
    if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::TLAS) {
        TLASNode *tlasNode = (TLASNode *) node;

        for (int memberId = tlasNode->m_groupMemberList.size() - 1; memberId >= 0; --memberId) {
            traverseInitTLASGroupSecondStage(tlasNode->m_groupMemberList[memberId],
                                             memberId + 1 >= tlasNode->m_groupMemberList.size() ? nullptr
                                                                                                : tlasNode->m_groupMemberList[
                                                     memberId + 1]);
        }
        tlasNode->m_inTwoLevelBVHId = m_curTlas++;

        if (tlasNode->m_groupMemberList[0]->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
            BLASNode *blasNode = (BLASNode *) tlasNode->m_groupMemberList[0];
            if (rightNode && rightNode->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
                BLASNode *rightBlasNode = (BLASNode *) rightNode;
                m_tlasNodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                        m_meshNodeList.size() + (blasNode->m_intermediateNodeIdx == -1 ? blasNode->m_inTwoLevelBVHId
                                                                                       : blasNode->m_intermediateNodeIdx),
                        m_meshNodeList.size() + (rightBlasNode->m_intermediateNodeIdx == -1 ? rightBlasNode->m_inTwoLevelBVHId
                                                                                            : rightBlasNode->m_intermediateNodeIdx), 0, 0);
            } else {
                m_tlasNodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                        m_meshNodeList.size() + (blasNode->m_intermediateNodeIdx == -1 ? blasNode->m_inTwoLevelBVHId
                                                                                       : blasNode->m_intermediateNodeIdx),
                        rightNode ? (m_meshNodeList.size() + m_blasNodeList.size() + rightNode->m_inTwoLevelBVHId) : -1, 0, 0);
            }
        } else {
            TLASNode *tlasNodeMember = (TLASNode *) tlasNode->m_groupMemberList[0];
            if (rightNode && rightNode->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
                BLASNode *rightBlasNode = (BLASNode *) rightNode;
                m_tlasNodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                        m_meshNodeList.size() + m_blasNodeList.size() + tlasNodeMember->m_inTwoLevelBVHId,
                        m_meshNodeList.size() + (rightBlasNode->m_intermediateNodeIdx == -1 ? rightBlasNode->m_inTwoLevelBVHId
                                                                                            : rightBlasNode->m_intermediateNodeIdx), 0, 0);
            } else {
                m_tlasNodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                        m_meshNodeList.size() + m_blasNodeList.size() + tlasNodeMember->m_inTwoLevelBVHId,
                        rightNode ? (m_meshNodeList.size() + m_blasNodeList.size() + rightNode->m_inTwoLevelBVHId) : -1, 0, 0);
            }
        }
    }
}

int GroupBVHTranslator::traverseInitMeshNode(OctreeNode *node, OctreeNode *rightNode, int meshStartIndices) {
    node->m_inTwoLevelBVHId = m_curMesh++;

    if (node->m_isLeaf) {
        if (rightNode) {
            int intermediateNodeId = node->m_inTwoLevelBVHId;
            m_meshNodeList.insert(m_meshNodeList.begin() + node->m_inTwoLevelBVHId, Node());
            node->m_inTwoLevelBVHId = m_curMesh++;
            m_meshNodeList[intermediateNodeId].lrLeaf = Vec4i(node->m_inTwoLevelBVHId,
                                                              rightNode->m_intermediateNodeIdx == -1
                                                              ? rightNode->m_inTwoLevelBVHId
                                                              : rightNode->m_intermediateNodeIdx, 0, 0);
            node->m_intermediateNodeIdx = intermediateNodeId;
        }
        m_meshNodeList[node->m_inTwoLevelBVHId].boundingBoxMin = Vec4f(node->m_nodeBoundingBox.m_bounding[0], 1.0f);
        m_meshNodeList[node->m_inTwoLevelBVHId].boundingBoxMax = Vec4f(node->m_nodeBoundingBox.m_bounding[1], 1.0f);
        m_meshNodeList[node->m_inTwoLevelBVHId].lrLeaf = Vec4i(meshStartIndices + node->m_startIdx,
                                                               node->m_data.size(), 1, 0);
    } else {
        OctreeNode *latestNode = nullptr;
        for (int i = 7; i >= 0; --i) {
            if (node->m_child[i]) {
                traverseInitMeshNode(node->m_child[i], latestNode, meshStartIndices);
                latestNode = node->m_child[i];
            }
        }
        m_meshNodeList[node->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                latestNode->m_intermediateNodeIdx == -1 ? latestNode->m_inTwoLevelBVHId
                                                        : latestNode->m_intermediateNodeIdx,
                rightNode ? (rightNode->m_intermediateNodeIdx == -1 ? rightNode->m_inTwoLevelBVHId
                                                                    : rightNode->m_intermediateNodeIdx) : -1, 0, 0);
    }
}

void GroupBVHTranslator::reduceNode(int nodeIdx) {
    // backward?
    while (m_nodeList[nodeIdx].lrLeaf.z == 0 && m_nodeList[nodeIdx].lrLeaf.y == -1) {
        if (m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].lrLeaf.z > 0) {
            m_nodeList[nodeIdx].boundingBoxMin = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].boundingBoxMin;
            m_nodeList[nodeIdx].boundingBoxMax = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].boundingBoxMax;
        }
        m_nodeList[nodeIdx].lrLeaf = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].lrLeaf;
    }
    if (m_nodeList[nodeIdx].lrLeaf.z <= 0) {
        reduceNode(m_nodeList[nodeIdx].lrLeaf.x);
        m_nodeList[nodeIdx].boundingBoxMin = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].boundingBoxMin;
        m_nodeList[nodeIdx].boundingBoxMax = m_nodeList[m_nodeList[nodeIdx].lrLeaf.x].boundingBoxMax;
        if(m_nodeList[nodeIdx].lrLeaf.z == 0) {
            reduceNode(m_nodeList[nodeIdx].lrLeaf.y);
            m_nodeList[nodeIdx].boundingBoxMin.min(m_nodeList[m_nodeList[nodeIdx].lrLeaf.y].boundingBoxMin);
            m_nodeList[nodeIdx].boundingBoxMax.max(m_nodeList[m_nodeList[nodeIdx].lrLeaf.y].boundingBoxMax);
        }
    }
}