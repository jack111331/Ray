//
// Created by Edge on 2020/12/25.
//

#include "TriangleGroup.h"
#include <queue>
#include <GL/glew.h>
#include "OBJ_Loader.h"


using namespace std;

BVH::BVH(TriangleGroup *triangleGroup) : m_octree(nullptr) {
    updateBVH(triangleGroup);
}

bool BVH::isHit(double tmin, const Ray &ray, HitRecord &record) {
    if (!m_octree->m_root->m_nodeBoundingBox.isHit(ray)) {
        return false;
    }
    priority_queue<Octree<TriangleGroup>::QueueElement> pq;
    pq.push({Octree<TriangleGroup>::QueueElement(m_octree->m_root, 0)});
    bool isHitted = false;
    while (!pq.empty() && (pq.top().t < record.t || record.t < 0)) {
        const OctreeNode<TriangleGroup> *node = pq.top().node;
        pq.pop();
        if (node->m_isLeaf) {
            for (uint32_t i = 0; i < node->m_data.size(); ++i) {
                if (node->m_data[i]->isHit(tmin, ray, record)) {
                    record.material = node->m_data[i]->m_material;
                    isHitted = true;
                }
            }
        } else {
            for (int i = 0; i < 8; ++i) {
                if (node->m_child[i]) {
                    float tNearChild = 0, tFarChild = 1e9;
                    if (node->m_child[i]->m_nodeBoundingBox.isHit(ray)) {
                        float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
                        pq.push(Octree<TriangleGroup>::QueueElement(node->m_child[i], t));
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
    m_octree = new Octree<TriangleGroup>(triangleGroup->m_boundingBox);
    m_octree->build();
}

std::vector<Hittable *> TriangleGroup::fromObj(const YAML::Node &node, const Scene *scene) {
    std::vector<Hittable *> hittableList;
    objl::Loader Loader;

    if(!node["filename"])
        return {};

    bool result = Loader.LoadFile(node["filename"].as<std::string>());

    if (result) {
        for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
            TriangleGroup *triangleGroup = new TriangleGroup();
            triangleGroup->fromObj(Loader.LoadedMeshes[i], scene);
            hittableList.push_back(triangleGroup);
        }
    } else {
        exit(1);
        // TODO require better resolve
    }
    return hittableList;
}

bool TriangleGroup::fromObj(const objl::Mesh &mesh, const Scene *scene) {
    std::vector<TriangleNode *> nodeList;
    for (int j = 0; j < mesh.Vertices.size(); j++) {
        TriangleNode *node = new TriangleNode{{mesh.Vertices[j].Position.X, mesh.Vertices[j].Position.Y,
                                                      mesh.Vertices[j].Position.Z},
                                              {mesh.Vertices[j].TextureCoordinate.X,
                                                                            mesh.Vertices[j].TextureCoordinate.Y},
                                              {mesh.Vertices[j].Normal.X,   mesh.Vertices[j].Normal.Y,
                                                      mesh.Vertices[j].Normal.Z}};
        nodeList.push_back(node);
        m_boundingBox.updateBoundingBox((*--nodeList.end())->m_coord);
    }
    for (int j = 0; j < mesh.Indices.size(); j += 3) {
        Triangle *triangle = new Triangle();
        triangle->fromTriangleNode(mesh.MeshMaterial.name, scene, nodeList[mesh.Indices[j]],
                                   nodeList[mesh.Indices[j + 1]],
                                   nodeList[mesh.Indices[j + 2]]);
        m_triangles.push_back(triangle);
    }
    m_accel = new BVH(this);
    return true;
}

bool TriangleGroup::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if (!node["vertices"] || !node["triangles"]) {
        return false;
    }
    std::vector<TriangleNode *> nodeList;
    for (YAML::const_iterator it = node["vertices"].begin(); it != node["vertices"].end(); ++it) {
        TriangleNode *node = new TriangleNode{Coord::toCoord(it->as<std::vector<float>>()), {0, 0}, {0, 0, 0}};
        nodeList.push_back(node);
        m_boundingBox.updateBoundingBox((*--nodeList.end())->m_coord);
    }
    for (YAML::const_iterator it = node["triangles"].begin(); it != node["triangles"].end(); ++it) {
        std::vector<float> triangleIndices = it->second["indices"].as<std::vector<float>>();
        Triangle *triangle = new Triangle();
        triangle->fromTriangleNode(it->second["material"].as<std::string>(), scene, nodeList[triangleIndices[0]],
                                   nodeList[triangleIndices[1]],
                                   nodeList[triangleIndices[2]]);
        m_triangles.push_back(triangle);
    }
    m_accel = new BVH(this);
    return true;
}

bool TriangleGroup::isHit(double tmin, const Ray &ray, HitRecord &record) {
    if (m_accel) {
        return m_accel->isHit(tmin, ray, record);
    }
    bool isHitted = false;
    for (auto triangle: m_triangles) {
        isHitted = std::max(isHitted, triangle->isHit(tmin, ray, record));
    }
    return isHitted;
}

std::vector<ObjectInfo> TriangleGroup::createVAO(const Material *material) {
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

    glBindVertexArray(0);

    vector<ObjectInfo> result;
    result.push_back({vao, (int) indices.size()});

    return result;
}

ObjectBoundingBox TriangleGroup::getBoundingBox() {
    return m_boundingBox;
}