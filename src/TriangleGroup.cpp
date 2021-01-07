//
// Created by Edge on 2020/12/25.
//

#include "TriangleGroup.h"
#include <queue>
#include <GL/glew.h>

#include "OBJ_Loader.h"
#include "TriMesh.h"


using namespace std;

BVH::BVH(TriangleGroup *triangleGroup) : m_octree(nullptr) {
    updateBVH(triangleGroup);
}

bool BVH::isHit(const Ray &ray, HitRecord &record, float tmin) {
    float tNear = 1e9, tFar = -1e9;
    if (!m_octree->m_root->m_nodeBoundingBox.isHit(ray, tNear, tFar)) {
        return false;
    }
    priority_queue<Octree::QueueElement> pq;
    pq.push({Octree::QueueElement(m_octree->m_root, 0)});
    bool isHitted = false;
    while (!pq.empty() && (pq.top().t < record.t || record.t < 0)) {
        const OctreeNode *node = pq.top().node;
        pq.pop();
        if (node->m_isLeaf) {
            for (uint32_t i = 0; i < node->m_data.size(); ++i) {
                if (node->m_data[i]->isHit(ray, record, tmin)) {
                    record.material = node->m_data[i]->m_material;
                    isHitted = true;
                }
            }
        } else {
            for (int i = 0; i < 8; ++i) {
                if (node->m_child[i]) {
                    float tNearChild = 0, tFarChild = tFar;
                    if (node->m_child[i]->m_nodeBoundingBox.isHit(ray, tNearChild, tFarChild)) {
                        float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
                        pq.push(Octree::QueueElement(node->m_child[i], t));
                    }
                }
            }
        }
    }
    return isHitted;
}

void BVH::updateBVH(TriangleGroup *triangleGroup) {
    if (m_octree) {
        delete m_octree;
    }
    m_octree = new Octree(triangleGroup->m_boundingBox);
    for (auto triangle: triangleGroup->m_triangles) {
        m_octree->insert(triangle);
    }
    m_octree->build();
}

std::vector<Hittable *> TriangleGroup::fromObj(const YAML::Node &node, const Scene *scene) {

    std::vector<Hittable *> hittableList;
    objl::Loader Loader;

    if (!node["filename"])
        return {};

    bool result = Loader.LoadFile(node["filename"].as<std::string>());
    if (result) {
        for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
            TriangleGroup *triangleGroup = new TriangleGroup();

            if (node["material"]) {
                triangleGroup->fromObj(Loader.LoadedMeshes[i], scene, node["material"].as<std::string>());
            } else {
                triangleGroup->fromObj(Loader.LoadedMeshes[i], scene);
            }


            triangleGroup->m_objFilename = node["filename"].as<std::string>();
            hittableList.push_back(triangleGroup);
        }
    } else {
        std::cerr << "Load obj failed" << std::endl;
        exit(1);
    }
    return hittableList;
}

bool TriangleGroup::fromObj(const objl::Mesh &mesh, const Scene *scene, const std::string &materialName) {
    std::vector<TriangleNode *> nodeList;
    for (int j = 0; j < mesh.Vertices.size(); j++) {
        TriangleNode *node = new TriangleNode{{mesh.Vertices[j].Position.X, mesh.Vertices[j].Position.Y,
                                               mesh.Vertices[j].Position.Z},
                                              {mesh.Vertices[j].TextureCoordinate.X,
                                               mesh.Vertices[j].TextureCoordinate.Y},
                                              {mesh.Vertices[j].Normal.X, mesh.Vertices[j].Normal.Y,
                                               mesh.Vertices[j].Normal.Z}, true};
        nodeList.push_back(node);
        m_boundingBox.updateBoundingBox(node->m_coord);
    }
    for (int j = 0; j < mesh.Indices.size(); j += 3) {
        Triangle *triangle = new Triangle();
        if (materialName.empty()) {
            triangle->fromTriangleNode(mesh.MeshMaterial.name, scene, nodeList[mesh.Indices[j]],
                                       nodeList[mesh.Indices[j + 1]],
                                       nodeList[mesh.Indices[j + 2]]);
        } else {
            triangle->fromTriangleNode(materialName, scene, nodeList[mesh.Indices[j]],
                                       nodeList[mesh.Indices[j + 1]],
                                       nodeList[mesh.Indices[j + 2]]);
        }
        m_triangles.push_back(triangle);
    }
    auto it = scene->m_materialTable.find(materialName);
    if (it != scene->m_materialTable.end()) {
        setMaterial(it->second);
    }
    m_accel = new BVH(this);
    return true;
}

bool TriangleGroup::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if (!node["vertices"] || !node["triangles"] || !node["individual_triangle"]) {
        return false;
    }

    m_individualTriangle = node["individual_triangle"].as<bool>();

    if (!m_individualTriangle && !node["material"]) {
        return false;
    }
    std::vector<TriangleNode *> nodeList;
    auto verticesNode = node["vertices"];
    for (uint32_t i = 0; i < verticesNode.size(); ++i) {
        TriangleNode *node = new TriangleNode(Coord::toCoord(verticesNode[i].as<std::vector<float>>()), {0, 0},
                                              {0, 0, 0}, false);
        nodeList.push_back(node);
        m_boundingBox.updateBoundingBox((*--nodeList.end())->m_coord);
    }
    auto trianglesNode = node["triangles"];
    for (uint32_t i = 0; i < trianglesNode.size(); ++i) {
        std::vector<uint32_t> triangleIndices = trianglesNode[i]["indices"].as<std::vector<uint32_t>>();
        Triangle *triangle = new Triangle();
        triangle->fromTriangleNode(trianglesNode[i]["material"].as<std::string>(), scene, nodeList[triangleIndices[0]],
                                   nodeList[triangleIndices[1]],
                                   nodeList[triangleIndices[2]]);
        m_triangles.push_back(triangle);
    }
    if (node["bvh_acceleration"] && node["bvh_acceleration"].as<bool>()) {
        m_accel = new BVH(this);
    }
    return true;
}

bool TriangleGroup::isHit(const Ray &ray, HitRecord &record, float tmin) const {
    if (m_accel) {
        return m_accel->isHit(ray, record, tmin);
    }
    bool isHitted = false;
    for (auto triangle: m_triangles) {
        isHitted = std::max(isHitted, triangle->isHit(ray, record, tmin));
    }
    return isHitted;
}

std::vector<ShadeObject *> TriangleGroup::createVAO() {
    vector<ShadeObject *> result;
    if (m_individualTriangle) {
        for (auto triangle: m_triangles) {
            vector<ShadeObject *> triangleVAO = triangle->createVAO();
            result.insert(result.end(), triangleVAO.begin(), triangleVAO.end());
        }
    } else {
        // VAO
        uint32_t vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        std::vector<Coord> vertices;
        std::vector<Velocity> normals;
        std::vector<Coord2D> texCoords;
        std::vector<uint32_t> indices;


        std::map<TriangleNode *, size_t> triangleTable;
        int indicesCount = 0;
        for (size_t i = 0; i < m_triangles.size(); ++i) {
            for (int j = 0; j < 3; ++j) {
                if (triangleTable.find(m_triangles[i]->m_point[j]) == triangleTable.end()) {
                    triangleTable[m_triangles[i]->m_point[j]] = indicesCount++;
                    vertices.push_back(m_triangles[i]->m_point[j]->m_coord);
                    normals.push_back(m_triangles[i]->m_point[j]->m_normal);
                    texCoords.push_back(m_triangles[i]->m_point[j]->m_texCoord);
                }
                indices.push_back(triangleTable[m_triangles[i]->m_point[j]]);
            }
        }

        // Mesh VBO
        uint32_t meshVbo;
        glGenBuffers(1, &meshVbo);
        glBindBuffer(GL_ARRAY_BUFFER, meshVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Coord) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        // Normal VBO
        uint32_t normalVbo;
        glGenBuffers(1, &normalVbo);
        glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Velocity) * normals.size(), normals.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);

        // Texture Coordinate VBO
        uint32_t texCoordVbo;
        glGenBuffers(1, &texCoordVbo);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Coord2D) * texCoords.size(), texCoords.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
        glEnableVertexAttribArray(2);

        // EBO
        uint32_t ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_STATIC_DRAW);

        if (!m_objFilename.empty()) {
            trimesh::TriMesh *themesh = trimesh::TriMesh::read(m_objFilename);
            themesh->need_tstrips();
            themesh->need_curvatures();
            themesh->need_dcurv();

            std::cout << "feature size : " << themesh->feature_size() << std::endl;

            glBindBuffer(GL_ARRAY_BUFFER, meshVbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Coord) * themesh->vertices.size(), themesh->vertices.data(),
                         GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

            glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Velocity) * themesh->normals.size(), themesh->normals.data(),
                         GL_DYNAMIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

            uint32_t pdir1Vbo;
            glGenBuffers(1, &pdir1Vbo);
            glBindBuffer(GL_ARRAY_BUFFER, pdir1Vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Coord) * themesh->pdir1.size(), themesh->pdir1.data(),
                         GL_DYNAMIC_DRAW);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(3);

            uint32_t pdir2Vbo;
            glGenBuffers(1, &pdir2Vbo);
            glBindBuffer(GL_ARRAY_BUFFER, pdir2Vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Coord) * themesh->pdir2.size(), themesh->pdir2.data(),
                         GL_DYNAMIC_DRAW);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(4);

            uint32_t curv1Vbo;
            glGenBuffers(1, &curv1Vbo);
            glBindBuffer(GL_ARRAY_BUFFER, curv1Vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * themesh->curv1.size(), themesh->curv1.data(),
                         GL_DYNAMIC_DRAW);
            glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);
            glEnableVertexAttribArray(5);

            uint32_t curv2Vbo;
            glGenBuffers(1, &curv2Vbo);
            glBindBuffer(GL_ARRAY_BUFFER, curv2Vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * themesh->curv2.size(), themesh->curv2.data(),
                         GL_DYNAMIC_DRAW);
            glVertexAttribPointer(6, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);
            glEnableVertexAttribArray(6);

            uint32_t dcurvVbo;
            glGenBuffers(1, &dcurvVbo);
            glBindBuffer(GL_ARRAY_BUFFER, dcurvVbo);
            glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(float) * themesh->dcurv.size(), themesh->dcurv.data(),
                         GL_DYNAMIC_DRAW);
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
            glEnableVertexAttribArray(7);

            std::vector<uint32_t> newIndices;
            const int *t = &themesh->tstrips[0];
            const int *end = t + themesh->tstrips.size();
            while (likely(t < end)) {
                int striplen = *t++;
                uint32_t tmpList[2];
                for (int i = 0; i < striplen; ++i) {
                    if (i < 2) {
                        tmpList[i] = *t++;
                    } else {
                        newIndices.push_back(tmpList[(i - 2) % 2]);
                        newIndices.push_back(tmpList[(i - 1) % 2]);
                        tmpList[i % 2] = *t++;
                        newIndices.push_back(tmpList[i % 2]);
                    }
                }
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * newIndices.size(), newIndices.data(),
                         GL_STATIC_DRAW);

            glBindVertexArray(0);
            result.push_back(new ShadeObject({vao, (int) newIndices.size()}, m_material));
            return result;
        }

        glBindVertexArray(0);

        result.push_back(new ShadeObject({vao, (int) indices.size()}, m_material));

    }
    return result;
}

ObjectBoundingBox TriangleGroup::getBoundingBox() const {
    return m_boundingBox;
}