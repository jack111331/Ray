//
// Created by Edge on 2020/10/17.
//
#include "BVH.h"
#include "OBJ_Loader.h"
#include <cmath>
#include <queue>
#include <GL/glew.h>
#include <Lambertian.h>

using namespace std;

const Velocity Extent::m_planeSetNormals[7]{
        {1,                  0,                  0},
        {0,                  1,                  0},
        {0,                  0,                  1},
        {sqrtf(3.0) / 3.0f,  sqrtf(3.0) / 3.0f,  sqrtf(3.0) / 3.0f},
        {-sqrtf(3.0) / 3.0f, sqrtf(3.0) / 3.0f,  sqrtf(3.0) / 3.0f},
        {-sqrtf(3.0) / 3.0f, -sqrtf(3.0) / 3.0f, sqrtf(3.0) / 3.0f},
        {sqrtf(3.0) / 3.0f,  -sqrtf(3.0) / 3.0f, sqrtf(3.0) / 3.0f}
};

Object::Object(const objl::Mesh &mesh) {
    for (int j = 0; j < mesh.Vertices.size(); j++) {
        m_vertices.push_back({mesh.Vertices[j].Position.X, mesh.Vertices[j].Position.Y,
                              mesh.Vertices[j].Position.Z});
        m_texCoords.push_back({mesh.Vertices[j].TextureCoordinate.X, mesh.Vertices[j].TextureCoordinate.Y});
        m_normals.push_back(
                {mesh.Vertices[j].Normal.X, mesh.Vertices[j].Normal.Y, mesh.Vertices[j].Normal.Z});

    }
    for (int j = 0; j < mesh.Indices.size(); j++) {
        m_indices.push_back(mesh.Indices[j]);
    }
}

bool Object::isHit(double tmin, const Ray &ray, HitRecord &record) {
    bool isHitted = false;
    for (uint32_t j = 0; j < m_indices.size(); j += 3) {
        isHitted = max(isHitted, isHitSingle(tmin, j, ray, record));
    }
    return isHitted;
}

bool Object::isHitSingle(float tmin, uint32_t index, const Ray &ray, HitRecord &record) const {
    const double EPSILON = 1e-7;
    const Coord (&point)[3] = {m_vertices[m_indices[index * 3]], m_vertices[m_indices[index * 3 + 1]],
                               m_vertices[m_indices[index * 3 + 2]]};
    Velocity planeVector[2] = {point[1] - point[0],
                               point[2] - point[0]};
    Velocity h = ray.velocity.cross(planeVector[1]);
    double a = planeVector[0].dot(h);
    if (std::abs(a) < EPSILON)
        return false;
    double f = 1.0 / a;
    Velocity s = ray.origin - point[0];
    double u = f * s.dot(h);
    if (u < 0.0 || u > 1.0)
        return false;
    Velocity q = s.cross(planeVector[0]);
    double v = f * ray.velocity.dot(q);
    if (v < 0.0 || v + u > 1.0)
        return false;

    double t = f * planeVector[1].dot(q);
    if (t > EPSILON && t > tmin) {
        if (record.t < 0.0f || record.t > t) {
            // ray.origin + t * ray.velocity is intersection point
            record.t = t;
            record.point = ray.pointAt(t);
            record.normal = planeVector[0].cross(planeVector[1]).normalize();
            record.texCoord = m_texCoords[m_indices[index * 3]] + u * (m_texCoords[m_indices[index * 3+1]] - m_texCoords[m_indices[index * 3]]) +
                              v * (m_texCoords[m_indices[index * 3 + 2]] - m_texCoords[m_indices[index * 3]]);
            record.material = m_material;
            return true;
        }
    }
    return false;
}


std::vector<ObjectInfo> Object::createVAO(const Material *material) {
    // VAO
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Mesh VBO
    uint32_t meshVbo;
    glGenBuffers(1, &meshVbo);
    glBindBuffer(GL_ARRAY_BUFFER, meshVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Coord) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Normal VBO
    uint32_t normalVbo;
    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Velocity) * m_normals.size(), m_normals.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(1);

    // Texture Coordinatte VBO
    uint32_t texCoordVbo;
    glGenBuffers(1, &texCoordVbo);
    glBindBuffer(GL_ARRAY_BUFFER, texCoordVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Coord2D) * m_texCoords.size(), m_texCoords.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(2);

    // EBO
    uint32_t ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

    vector<ObjectInfo> result;
    result.push_back({vao, (int) m_indices.size()});

    return result;
}

void Cell::insert(Object *object, uint32_t index) {
    m_triangles.push_back({object, index});
}

bool Cell::intersect(float tmin, const Ray &ray, HitRecord &record) const {
    bool isHitted = false;
    for (auto triangle:m_triangles) {
        triangle.object->isHitSingle(tmin, triangle.index, ray, record);
    }
    return isHitted;
}

Grid::Grid(const objl::Mesh &mesh) {
    // TODO calculate m_gridRes
    int totalNumTriangles = 0;
    for (int j = 0; j < mesh.Vertices.size(); j++) {
        m_vertices.push_back({mesh.Vertices[j].Position.X, mesh.Vertices[j].Position.Y,
                              mesh.Vertices[j].Position.Z});

        m_normals.push_back(
                {mesh.Vertices[j].Normal.X, mesh.Vertices[j].Normal.Y, mesh.Vertices[j].Normal.Z});
        if (!j) {
            m_aabb.assign(m_vertices[j]);
        } else {
            m_aabb.extendBy(m_vertices[j]);
        }
    }
    m_cell.resize(m_gridDim.x);
    for (int i = 0; i < m_gridDim.x; ++i) {
        m_cell[i].resize(m_gridDim.y);
        for (int j = 0; j < m_gridDim.y; ++j) {
            m_cell[i][j].resize(m_gridDim.z);
        }
    }
    m_gridMin = m_aabb.m_bounding[0];
    Vec3i triangleMin, triangleMax;
    for (int j = 0; j < mesh.Indices.size(); j++) {
        m_indices.push_back(mesh.Indices[j]);

        if (j % 3 == 2) {
            BoundingBox aabb;
            aabb.assign(m_vertices[j]);
            aabb.extendBy(m_vertices[j - 1]);
            aabb.extendBy(m_vertices[j - 2]);
            triangleMin = calculateCoordInCell(aabb.m_bounding[0]);
            triangleMax = calculateCoordInCell(aabb.m_bounding[1]);
            for (int k = triangleMin.x; k <= triangleMax.x; ++k)
                for (int l = triangleMin.y; l <= triangleMax.y; ++l)
                    for (int m = triangleMin.z; m <= triangleMax.z; ++m) {
                        m_cell[k][l][m].push_back(j / 3);
                    }
        }
    }
}

bool Grid::isHit(double tmin, const Ray &ray, HitRecord &record) {
    bool isHitted = false;
    // first test if hit grid
    if (!m_aabb.isHit(tmin, ray, record)) {
        return false;
    }
    // second test grid
    // calculate incident grid
    Coord originInGrid = (ray.origin + (-m_gridMin));
    Coord originInCell = originInGrid / m_gridRes;
    Velocity normalizedVelocity = ray.velocity;
    normalizedVelocity = normalizedVelocity.normalize();
    float deltaTx = fabsf(m_gridRes.x / normalizedVelocity.x);
    float deltaTy = fabsf(m_gridRes.y / normalizedVelocity.y);
    float deltaTz = fabsf(m_gridRes.z / normalizedVelocity.z);
    float tx, ty, tz;
    if (normalizedVelocity.x < 0) {
        tx = (floor(originInCell.x) * m_gridRes.x - originInGrid.x) / normalizedVelocity.x;
    } else {
        tx = ((floor(originInCell.x) + 1) * m_gridRes.x - originInGrid.x) / normalizedVelocity.x;
    }
    if (normalizedVelocity.y < 0) {
        ty = (floor(originInCell.y) * m_gridRes.y - originInGrid.y) / normalizedVelocity.y;
    } else {
        ty = ((floor(originInCell.y) + 1) * m_gridRes.y - originInGrid.y) / normalizedVelocity.y;
    }
    if (normalizedVelocity.z < 0) {
        tz = (floor(originInCell.z) * m_gridRes.z - originInGrid.z) / normalizedVelocity.z;
    } else {
        tz = ((floor(originInCell.z) + 1) * m_gridRes.z - originInGrid.z) / normalizedVelocity.z;
    }
    Vec3i cellIndex = calculateCoordInCell(ray.origin);
    while (1) {
        if (tx < ty) {
            if (tx < tz) {
                tx += deltaTx;
                if (normalizedVelocity.x < 0) {
                    cellIndex.x--;
                } else {
                    cellIndex.x++;
                }
            } else {
                tz += deltaTz;
                if (normalizedVelocity.z < 0) {
                    cellIndex.z--;
                } else {
                    cellIndex.z++;
                }
            }
        } else {
            if (ty < tz) {
                ty += deltaTy;
                if (normalizedVelocity.y < 0) {
                    cellIndex.y--;
                } else {
                    cellIndex.y++;
                }
            } else {
                tz += deltaTz;
                if (normalizedVelocity.z < 0) {
                    cellIndex.z--;
                } else {
                    cellIndex.z++;
                }
            }
        }
        // TODO test triangle in grid
        if (cellIndex.x < 0 || cellIndex.x >= m_gridDim.x || cellIndex.y < 0 || cellIndex.y >= m_gridDim.y ||
            cellIndex.z < 0 || cellIndex.z >= m_gridDim.z) {
            break;
        }
    }
    for (uint32_t j = 0; j < m_indices.size(); j += 3) {
        const double EPSILON = 1e-7;
        const Coord (&point)[3] = {m_vertices[m_indices[j]], m_vertices[m_indices[j + 1]],
                                   m_vertices[m_indices[j + 2]]};
        Velocity planeVector[2] = {point[1] - point[0],
                                   point[2] - point[0]};
        Velocity h = ray.velocity.cross(planeVector[1]);
        double a = planeVector[0].dot(h);
        if (std::abs(a) < EPSILON)
            continue;
        double f = 1.0 / a;
        Velocity s = ray.origin - point[0];
        double u = f * s.dot(h);
        if (u < 0.0 || u > 1.0)
            continue;
        Velocity q = s.cross(planeVector[0]);
        double v = f * ray.velocity.dot(q);
        if (v < 0.0 || v + u > 1.0)
            continue;

        double t = f * planeVector[1].dot(q);
        if (t > EPSILON && t > tmin) {
            if (record.t < 0.0f || record.t > t) {
                // ray.origin + t * ray.velocity is intersection point
                record.t = t;
                record.point = ray.pointAt(t);
                record.normal = planeVector[0].cross(planeVector[1]).normalize();
                record.material = m_material;
                isHitted = true;
            }
        }
    }
    return isHitted;
}

vector<ObjectInfo> Grid::createVAO(const Material *material) {
    // TODO
    vector<ObjectInfo> result;
    result.push_back({0, 0});

    return result;
}

Vec3i Grid::calculateCoordInCell(const Coord &coord) {
    Coord result = {(coord.x - m_gridMin.x) / m_gridRes.x, (coord.y - m_gridMin.y) / m_gridRes.y,
                    (coord.z - m_gridMin.z) / m_gridRes.z};
    return {(int) result.x, (int) result.y, (int) result.z};
}

void Extent::extendBy(const Extent &extent) {
    for (int i = 0; i < 7; ++i) {
        m_d[i][0] = min(m_d[i][0], extent.m_d[i][0]);
        m_d[i][1] = max(m_d[i][1], extent.m_d[i][1]);
    }
}

bool
Extent::isHit(const Ray &ray, float precomputedNumerator[], float precomputeDenominator[], float &tNear, float &tFar,
              uint8_t &planeIndex) {
    for (int i = 0; i < 7; ++i) {
        float tn = (m_d[i][0] - precomputedNumerator[i]) / precomputeDenominator[i];
        float tf = (m_d[i][1] - precomputedNumerator[i]) / precomputeDenominator[i];
        if (precomputeDenominator[i] < 0) swap(tn, tf);
        if (tn > tNear) tNear = tn, planeIndex = i;
        if (tf < tFar) tFar = tf;
        if (tNear > tFar) return false;
    }
    return true;

}

Octree::Octree(const Extent &extent) {
    // constructor compute bound by first computing centroid and then add dimension to form bound
    float xDiff = extent.m_d[0][1] - extent.m_d[0][0];
    float yDiff = extent.m_d[1][1] - extent.m_d[1][0];
    float zDiff = extent.m_d[2][1] - extent.m_d[2][0];
    float dim = max(xDiff, max(yDiff, zDiff));
    // technically, this is not true centroid, just use this for computational efficiency
    Coord centroid = {extent.m_d[0][0] + extent.m_d[0][1], extent.m_d[1][0] + extent.m_d[1][1],
                      extent.m_d[2][0] + extent.m_d[2][1]};
    m_bound[0] = (centroid - (Velocity) {dim, dim, dim});
    m_bound[1] = (centroid + (Velocity) {dim, dim, dim});
    m_root = new OctreeNode;
}

void Octree::insert(OctreeNode *node, const Extent *extent, const Coord &boundMin,
                    const Coord &boundMax, int depth) {
    if (node->m_isLeaf) {
        // insert extent into node or reallocate node extents' to child node
        if (node->m_data.empty() || depth == 16) {
            node->m_data.push_back(extent);
        } else {
            node->m_isLeaf = false;
            while (!node->m_data.empty()) {
                // recursion and go to non leaf handling part
                insert(node, node->m_data.back(), boundMin, boundMax, depth);
                node->m_data.pop_back();
            }
            insert(node, extent, boundMin, boundMax, depth);
        }
    } else {
        // compute centroid and determine which child this extent should go
        Coord extentCentroid = (Coord) {extent->m_d[0][0] + extent->m_d[0][1], extent->m_d[1][0] + extent->m_d[1][1],
                                        extent->m_d[2][0] + extent->m_d[2][1]} * 0.5f;
        Coord nodeCentroid = (boundMin + boundMax) * 0.5f;
        uint32_t childIndex = 0;
        if (extentCentroid.x > nodeCentroid.x) childIndex += 4;
        if (extentCentroid.y > nodeCentroid.y) childIndex += 2;
        if (extentCentroid.z > nodeCentroid.z) childIndex += 1;
        Coord childBoundMin, childBoundMax;
        // compute child bound and pass down
        computeChildBound(childIndex, nodeCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
        if (node->m_child[childIndex] == nullptr) node->m_child[childIndex] = new OctreeNode;
        insert(node->m_child[childIndex], extent, childBoundMin, childBoundMax, depth + 1);
    }
}

void Octree::computeChildBound(uint32_t index, const Coord &nodeCentroid, const Coord &boundMin, const Coord &boundMax,
                               Coord &pMin, Coord &pMax) {
    pMin.x = (index & 4) ? nodeCentroid.x : boundMin.x;
    pMin.y = (index & 2) ? nodeCentroid.y : boundMin.y;
    pMin.z = (index & 1) ? nodeCentroid.z : boundMin.z;
    pMax.x = (index & 4) ? boundMax.x : nodeCentroid.x;
    pMax.y = (index & 2) ? boundMax.y : nodeCentroid.y;
    pMax.z = (index & 1) ? boundMax.z : nodeCentroid.z;
}

void Octree::build(OctreeNode *node, const Coord &boundMin, const Coord &boundMax) {
    if (node->m_isLeaf) {
        // update node extent
        for (auto extent:node->m_data) {
            node->m_nodeExtent.extendBy(*extent);
        }
    } else {
        for (int i = 0; i < 8; ++i) {
            if (node->m_child[i]) {
                Coord nodeCentroid = (boundMin + boundMax) * 0.5f;
                Coord childBoundMin, childBoundMax;
                // compute child bound and pass down
                computeChildBound(i, nodeCentroid, boundMin, boundMax, childBoundMin, childBoundMax);
                build(node->m_child[i], childBoundMin, childBoundMax);
                // collect child extent compute result
                node->m_nodeExtent.extendBy(node->m_child[i]->m_nodeExtent);
            }
        }
    }
}

PolygonMeshBVH::PolygonMeshBVH(const std::string &objPath, Material *material) {
    m_material = material;
    // Initialize Loader
    objl::Loader Loader;

    // Load .obj File
    bool loadout = Loader.LoadFile(objPath);

    if (loadout) {
        Extent sceneExtent;
        m_extents = new Extent *[Loader.LoadedMeshes.size()];
        for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
            m_objects.push_back(new Object(Loader.LoadedMeshes[i]));
            m_extents[i] = new Extent[m_objects[i]->m_indices.size() / 3];
            if (!material) {
                LambertianMaterial *newMaterial = new LambertianMaterial;
                newMaterial->m_ambientColor =
                        (Color) {Loader.LoadedMaterials[i].Ka.X, Loader.LoadedMaterials[i].Ka.Y,
                                 Loader.LoadedMaterials[i].Ka.Z};
                newMaterial->m_diffuseColor =
                        (Color) {Loader.LoadedMaterials[i].Kd.X, Loader.LoadedMaterials[i].Kd.Y,
                                 Loader.LoadedMaterials[i].Kd.Z};
                newMaterial->m_specularColor =
                        (Color) {Loader.LoadedMaterials[i].Ks.X, Loader.LoadedMaterials[i].Ks.Y,
                                 Loader.LoadedMaterials[i].Ks.Z};
                newMaterial->m_constantSpecularExp =
                        Loader.LoadedMaterials[i].Ns;
                // FIXME reference index
                newMaterial->m_constantRoughness =
                        Loader.LoadedMaterials[i].Ni;
                if(!Loader.LoadedMaterials[i].map_Kd.empty()) {
                    newMaterial->m_diffuseTexture = new Texture(objPath.substr(0, objPath.find_last_of('/') + 1) + Loader.LoadedMaterials[i].map_Kd);
                }
                m_objects[i]->m_material = newMaterial;
            } else {
                m_objects[i]->m_material = material;
            }
        }
        for (int i = 0; i < Loader.LoadedMeshes.size(); ++i) {
            for (int j = 0; j < 7; ++j)
                for (int k = 0; k < m_objects[i]->m_indices.size() / 3; ++k) {
                    computeTriangleBound(Extent::m_planeSetNormals[j], i, k, m_extents[i][k].m_d[j][0],
                                         m_extents[i][k].m_d[j][1]);
                }
            for (int j = 0; j < m_objects[i]->m_indices.size() / 3; ++j) {
                m_extents[i][j].m_object = m_objects[i];
                m_extents[i][j].m_triangleIndex = j;
                sceneExtent.extendBy(m_extents[i][j]);
            }
        }
        m_octree = new Octree(sceneExtent);
        for (int i = 0; i < Loader.LoadedMeshes.size(); ++i)
            for (int k = 0; k < m_objects[i]->m_indices.size() / 3; ++k) {
                m_octree->insert(&m_extents[i][k]);
            }
        m_octree->build();
    }
}

void
PolygonMeshBVH::computeTriangleBound(const Velocity &planeNormal, uint32_t objectIndex, uint32_t triangleIndex,
                                     float &dNear,
                                     float &dFar) {
    for (uint32_t i = 0; i < 3; ++i) {
        const Coord &triangleCoord = m_objects[objectIndex]->m_vertices[m_objects[objectIndex]->m_indices[
                3 * triangleIndex + i]];
        float d = planeNormal.dot({triangleCoord.x, triangleCoord.y, triangleCoord.z});
        if (d < dNear) dNear = d;
        if (d > dFar) dFar = d;
    }
}

void PolygonMeshBVH::computeBounds(const Velocity &planeNormal, const Object *object, float &dNear, float &dFar) {
    for (uint32_t i = 0; i < object->m_vertices.size(); ++i) {
        float d = planeNormal.dot({object->m_vertices[i].x, object->m_vertices[i].y, object->m_vertices[i].z});
        if (d < dNear) dNear = d;
        if (d > dFar) dFar = d;
    }
}

bool PolygonMeshBVH::isHit(double tmin, const Ray &ray, HitRecord &record) {
    float precomputedNumerator[7], precomputedDenominator[7];
    for (int i = 0; i < 7; ++i) {
        precomputedNumerator[i] = Extent::m_planeSetNormals[i].dot({ray.origin.x, ray.origin.y, ray.origin.z});
        precomputedDenominator[i] = Extent::m_planeSetNormals[i].dot(ray.velocity);
    }
    float tNear = -1e9, tFar = 1e9;
    uint8_t planeIndex;
    // FIXME
    if (!m_octree->m_root->m_nodeExtent.isHit(ray, precomputedNumerator, precomputedDenominator, tNear, tFar,
                                              planeIndex)) {
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
                if (node->m_data[i]->m_object->isHitSingle(tmin, node->m_data[i]->m_triangleIndex, ray, record)) {
                    record.material = node->m_data[i]->m_object->m_material;
                    isHitted = true;
                }
            }
        } else {
            for (int i = 0; i < 8; ++i) {
                if (node->m_child[i]) {
                    float tNearChild = 0, tFarChild = tFar;
                    if (node->m_child[i]->m_nodeExtent.isHit(ray, precomputedNumerator, precomputedDenominator,
                                                             tNearChild, tFarChild, planeIndex)) {
                        float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
                        pq.push(Octree::QueueElement(node->m_child[i], t));
                    }
                }
            }
        }
    }
    return isHitted;
}

vector<ObjectInfo> PolygonMeshBVH::createVAO(const Material *material) {
    vector<ObjectInfo> result;
    vector<vector<ObjectInfo>> tmp;
    int totalObjectSize = 0;
    for (auto object : m_objects) {
        tmp.push_back(object->createVAO(m_material));
        totalObjectSize += tmp[tmp.size() - 1].size();
    }
    result.resize(totalObjectSize);
    for (auto object:tmp) {
        result.insert(result.end(), object.begin(), object.end());
    }
    return result;
}

PolygonMeshBV::PolygonMeshBV(const std::string &objPath) {
    // Initialize Loader
    objl::Loader Loader;

    // Load .obj File
    bool loadout = Loader.LoadFile(objPath);

    if (loadout) {
        m_objectSize = Loader.LoadedMeshes.size();
        m_extent = new Extent[m_objectSize];

        typedef std::aligned_storage<sizeof(Object), std::alignment_of<objl::Mesh>::value>::type storage_type;
        //first get the aligned uninitialized memory!
        m_objects = reinterpret_cast<Object *>(new storage_type[m_objectSize]);

        for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
            objl::Mesh curMesh = Loader.LoadedMeshes[i];

            // then use placement new to construct the objects
            new(m_objects + i) Object(curMesh);
        }
        for (uint32_t i = 0; i < m_objectSize; ++i) {
            for (int j = 0; j < 7; ++j)
                computeBound(Extent::m_planeSetNormals[j], m_objects[i], m_extent[i].m_d[j][0],
                             m_extent[i].m_d[j][1]);
        }
    }

}

void PolygonMeshBV::computeBound(const Velocity &planeNormal, const Object &object, float &dNear, float &dFar) {
    float d;
    for (uint32_t i = 0; i < object.m_vertices.size(); ++i) {
        d = planeNormal.dot({object.m_vertices[i].x, object.m_vertices[i].y, object.m_vertices[i].z});
        if (d < dNear) dNear = d;
        if (d > dFar) dFar = d;
    }
}

bool PolygonMeshBV::intersect(uint32_t index, const Ray &ray) {
    float precomputedNumerator[7], precomputedDenominator[7];
    for (int i = 0; i < 7; ++i) {
        precomputedNumerator[i] = Extent::m_planeSetNormals[i].dot({ray.origin.x, ray.origin.y, ray.origin.z});
        precomputedDenominator[i] = Extent::m_planeSetNormals[i].dot(ray.velocity);
    }
    float tNear = -1e9, tFar = 1e9;
    uint8_t planeIndex;
    return m_extent[index].isHit(ray, precomputedNumerator, precomputedDenominator, tNear, tFar,
                                 planeIndex);
}

bool PolygonMeshBV::isHit(double tmin, const Ray &ray, HitRecord &record) {
    bool isHitted = false;
    for (uint32_t i = 0; i < m_objectSize; ++i) {
        if (!intersect(i, ray)) {
            continue;
        }
        isHitted = max(isHitted, m_objects[i].isHit(tmin, ray, record));
    }
    return isHitted;
}

vector<ObjectInfo> PolygonMeshBV::createVAO(const Material *material) {
    vector<ObjectInfo> result;
    vector<vector<ObjectInfo>> tmp;
    int totalObjectSize = 0;
    for (int i = 0; i < m_objectSize; ++i) {
        tmp.push_back(m_objects[i].createVAO(m_material));
        totalObjectSize += tmp[i].size();
    }
    result.resize(totalObjectSize);
    for (auto object:tmp) {
        result.insert(result.end(), object.begin(), object.end());
    }
    return result;
}

bool BoundingBox::isHit(double tmin, const Ray &ray, HitRecord &record) {
    float tMin = (m_bounding[0].x - ray.origin.x) / ray.velocity.x;
    float tMax = (m_bounding[1].x - ray.origin.x) / ray.velocity.x;

    if (tMin > tMax) swap(tMin, tMax);

    float tyMin = (m_bounding[0].y - ray.origin.y) / ray.velocity.y;
    float tyMax = (m_bounding[1].y - ray.origin.y) / ray.velocity.y;

    if (tyMin > tyMax) swap(tyMin, tyMax);

    // Test if it intersect in 2d
    if ((tMin > tyMax) || (tyMin > tMax))
        return false;

    if (tyMin > tMin)
        tMin = tyMin;

    if (tyMax < tMax)
        tMax = tyMax;

    float tzMin = (m_bounding[0].z - ray.origin.z) / ray.velocity.z;
    float tzMax = (m_bounding[1].z - ray.origin.z) / ray.velocity.z;

    if (tzMin > tzMax) swap(tzMin, tzMax);

    if ((tMin > tzMax) || (tzMin > tMax))
        return false;

    return true;
}

void BoundingBox::assign(const Coord &coord) {
    m_bounding[0] = m_bounding[1] = coord;
}

void BoundingBox::extendBy(const Coord &coord) {
    m_bounding[0].x = min(m_bounding[0].x, coord.x);
    m_bounding[0].y = min(m_bounding[0].y, coord.y);
    m_bounding[0].z = min(m_bounding[0].z, coord.z);
    m_bounding[1].x = max(m_bounding[1].x, coord.x);
    m_bounding[1].y = max(m_bounding[1].y, coord.y);
    m_bounding[1].z = max(m_bounding[1].z, coord.z);
}

PolygonMesh::PolygonMesh(const std::string &objPath) {
    // Initialize Loader
    objl::Loader Loader;

    // Load .obj File
    bool loadout = Loader.LoadFile(objPath);

    if (loadout) {
        m_objectSize = Loader.LoadedMeshes.size();
        m_aabb = new BoundingBox[m_objectSize];
        typedef std::aligned_storage<sizeof(Object), std::alignment_of<objl::Mesh>::value>::type storage_type;
        //first get the aligned uninitialized memory!
        m_objects = reinterpret_cast<Object *>(new storage_type[m_objectSize]);

        for (int i = 0; i < Loader.LoadedMeshes.size(); i++) {
            objl::Mesh curMesh = Loader.LoadedMeshes[i];

            // then use placement new to construct the objects
            new(m_objects + i) Object(curMesh);
        }
        buildBoundingBox();
    }
}

void PolygonMesh::buildBoundingBox() {
    for (int i = 0; i < m_objectSize; ++i) {
        if (!m_objects[i].m_vertices.empty()) {
            m_aabb[i].assign(m_objects[i].m_vertices[0]);
        }
        for (uint32_t j = 1; j < m_objects[i].m_vertices.size(); ++j) {
            m_aabb[i].extendBy(m_objects[i].m_vertices[j]);
        }
    }
}

bool PolygonMesh::isHit(double tmin, const Ray &ray, HitRecord &record) {
    bool isHitted = false;
    for (uint32_t i = 0; i < m_objectSize; ++i) {
        if (!m_aabb[i].isHit(tmin, ray, record)) {
            continue;
        }
        isHitted = max(isHitted, m_objects[i].isHit(tmin, ray, record));
    }
    return isHitted;
}

vector<ObjectInfo> PolygonMesh::createVAO(const Material *material) {
    vector<ObjectInfo> result;
    vector<vector<ObjectInfo>> tmp;
    int totalObjectSize = 0;
    for (int i = 0; i < m_objectSize; ++i) {
        tmp.push_back(m_objects[i].createVAO(m_material));
        totalObjectSize += tmp[i].size();
    }
    result.resize(totalObjectSize);
    for (auto object:tmp) {
        result.insert(result.end(), object.begin(), object.end());
    }
    return result;
}

