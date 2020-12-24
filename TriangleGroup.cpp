//
// Created by Edge on 2020/12/25.
//

#include "TriangleGroup.h"


bool TriangleGroup::fromObj(const objl::Mesh &mesh, const Scene *scene) {

    for (int j = 0; j < mesh.Vertices.size(); j++) {
        m_vertices.push_back({mesh.Vertices[j].Position.X, mesh.Vertices[j].Position.Y,
                              mesh.Vertices[j].Position.Z});
        m_boundingBox.updateBoundingBox(*--m_vertices.end());
        m_texCoords.push_back(
                {mesh.Vertices[j].TextureCoordinate.X, mesh.Vertices[j].TextureCoordinate.Y});
        m_normals.push_back(
                {mesh.Vertices[j].Normal.X, mesh.Vertices[j].Normal.Y, mesh.Vertices[j].Normal.Z});
    }
    for (int j = 0; j < mesh.Indices.size(); j++) {
        m_indices.push_back(mesh.Indices[j]);
    }
}

bool TriangleGroup::readObjectInfo(const YAML::Node &node, const Scene *scene) {
    if(!node["vertices"] || !node["triangles"]) {
        return false;
    }
    for (YAML::const_iterator it = node["vertices"].begin(); it != node["vertices"].end(); ++it) {
        m_vertices.push_back(Coord::toCoord(it->as<std::vector<float>>()));
        m_boundingBox.updateBoundingBox(*--m_vertices.end());
    }
    // Divide Triangle
    for (YAML::const_iterator it = node["triangles"].begin(); it != node["triangles"].end(); ++it) {
        std::vector<float> triangleIndices = it->second.as<std::vector<float>>();
        m_indices.insert(m_indices.end(), triangleIndices.begin(), triangleIndices.end());
    }
    // TODO post processing normal or use generated normal
    return true;
}

ObjectBoundingBox TriangleGroup::getBoundingBox() {
    return m_boundingBox;
}