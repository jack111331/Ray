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
        if (tlasNode->m_groupMemberList[0]->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
            BLASNode *blasNode = (BLASNode *) tlasNode->m_groupMemberList[0];
            if (rightNode && rightNode->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::BLAS) {
                BLASNode *rightBlasNode = (BLASNode *) rightNode;
                m_meshNodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                        blasNode->m_intermediateNodeIdx == -1 ? blasNode->m_inTwoLevelBVHId
                                                              : blasNode->m_intermediateNodeIdx,
                        rightBlasNode->m_intermediateNodeIdx == -1 ? rightBlasNode->m_inTwoLevelBVHId
                                                                   : rightBlasNode->m_intermediateNodeIdx, 0, 0);
            } else {
                m_meshNodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                        blasNode->m_intermediateNodeIdx == -1 ? blasNode->m_inTwoLevelBVHId
                                                              : blasNode->m_intermediateNodeIdx,
                        rightNode ? rightNode->m_inTwoLevelBVHId : -1, 0, 0);
            }
        } else {
            m_meshNodeList[tlasNode->m_inTwoLevelBVHId].lrLeaf = Vec4i(tlasNode->m_groupMemberList[0]->m_inTwoLevelBVHId,
                                                                   rightNode ? rightNode->m_inTwoLevelBVHId : -1, 0, 0);
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
                m_tlasStartNodeIndex++;
                int intermediateNodeId = triangleGroup->m_inTwoLevelBVHId;
                m_meshNodeList.insert(m_meshNodeList.begin() + triangleGroup->m_inTwoLevelBVHId, Node());
                triangleGroup->m_inTwoLevelBVHId = m_curBlas++;
                m_meshNodeList[intermediateNodeId].lrLeaf = Vec4i(triangleGroup->m_inTwoLevelBVHId,
                                                              rightBlasNode->m_intermediateNodeIdx == -1
                                                              ? rightBlasNode->m_inTwoLevelBVHId
                                                              : rightBlasNode->m_intermediateNodeIdx, 0, 0);
                triangleGroup->m_intermediateNodeIdx = intermediateNodeId;
            }
            m_meshNodeList[triangleGroup->m_inTwoLevelBVHId].lrLeaf = Vec4i(
                    triangleGroup->m_accel->m_octree->m_root->m_inTwoLevelBVHId, -1, -m_curMeshInstance - 1, 1);
            m_curMeshInstance++;
        }
    }
}

int GroupBVHTranslator::traverseInitBLASNode(OctreeNode *node, OctreeNode *rightNode, int meshStartIndices) {
    node->m_inTwoLevelBVHId = m_curBlas++;

    if (node->m_isLeaf) {
        if (rightNode) {
            m_tlasStartNodeIndex++;
            int intermediateNodeId = node->m_inTwoLevelBVHId;
            m_meshNodeList.insert(m_meshNodeList.begin() + node->m_inTwoLevelBVHId, Node());
            node->m_inTwoLevelBVHId = m_curBlas++;
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
                traverseInitBLASNode(node->m_child[i], latestNode, meshStartIndices);
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
    while (m_meshNodeList[nodeIdx].lrLeaf.z <= 0 && m_meshNodeList[nodeIdx].lrLeaf.y == -1) {
        if (m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.x].lrLeaf.z > 0) {
            m_meshNodeList[nodeIdx].boundingBoxMin = m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.x].boundingBoxMin;
            m_meshNodeList[nodeIdx].boundingBoxMax = m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.x].boundingBoxMax;
        }
        if (m_meshNodeList[nodeIdx].lrLeaf.z < 0) {
            int meshInstanceIdx = m_meshNodeList[nodeIdx].lrLeaf.z;
            m_meshNodeList[nodeIdx].lrLeaf = m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.x].lrLeaf;
            m_meshNodeList[nodeIdx].lrLeaf.z = meshInstanceIdx;
        } else {
            m_meshNodeList[nodeIdx].lrLeaf = m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.x].lrLeaf;
        }
    }
    if (m_meshNodeList[nodeIdx].lrLeaf.z <= 0) {
        // what if i become mesh node?
        reduceNode(m_meshNodeList[nodeIdx].lrLeaf.x);
        reduceNode(m_meshNodeList[nodeIdx].lrLeaf.y);
        m_meshNodeList[nodeIdx].boundingBoxMin = m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.x].boundingBoxMin;
        m_meshNodeList[nodeIdx].boundingBoxMin.min(m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.y].boundingBoxMin);
        m_meshNodeList[nodeIdx].boundingBoxMax = m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.x].boundingBoxMax;
        m_meshNodeList[nodeIdx].boundingBoxMax.max(m_meshNodeList[m_meshNodeList[nodeIdx].lrLeaf.y].boundingBoxMax);
    }
}

void GroupBVHTranslator::traverseUpdateGroup(ObjectNode *node) {
    // FIXME
    if (node->getTypeInTwoLevelBVH() == ObjectNode::TwoLevelBVHType::TLAS) {
        TLASNode *tlasNode = (TLASNode *) node;
        ObjectBoundingBox boundingBox;
        // TODO should update right node
        for (int memberId = tlasNode->m_groupMemberList.size() - 1; memberId >= 0; --memberId) {
            boundingBox.updateBoundingBox(
                    tlasNode->m_groupMemberList[memberId]->getBoundingBox());
            traverseUpdateGroup(tlasNode->m_groupMemberList[memberId]);
        }
        m_meshNodeList[tlasNode->m_inTwoLevelBVHId].boundingBoxMin = Vec4f(boundingBox.m_bounding[0], 1.0f);
        m_meshNodeList[tlasNode->m_inTwoLevelBVHId].boundingBoxMax = Vec4f(boundingBox.m_bounding[1], 1.0f);
    }
}