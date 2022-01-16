#include "Utilities/IOHelper.h"

#include "Utilities/Logger.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <tinyobjloader/tiny_obj_loader.h>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/string_cast.hpp>
#include <unordered_map>

namespace std {
    template <>
    struct hash<Yare::Vertex> {
        size_t operator()(Yare::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^
                   (hash<glm::vec2>()(vertex.uv) << 1);
        }
    };
}  // namespace std

namespace Yare::Utilities {

    std::vector<std::string> readFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file) {
            YZ_ERROR("File '" + filename + "' was unable to open.");
            throw std::runtime_error("File '" + filename + "' was unable to open.");
        }

        // captures lines not separated by whitespace
        std::vector<std::string> myLines;
        std::copy(std::istream_iterator<std::string>(file), std::istream_iterator<std::string>(),
                  std::back_inserter(myLines));

        return myLines;
    }

    void loadMesh(const std::string& filePath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
        tinyobj::attrib_t                attrib;
        std::vector<tinyobj::shape_t>    shapes;
        std::vector<tinyobj::material_t> materials;
        std::string                      warn, err;

        if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath.c_str())) {
            YZ_ERROR(warn + err);
        }

        std::unordered_map<Vertex, uint32_t> uniqueVertices = {};

        for (size_t s = 0; s < shapes.size(); s++) {
            // Loop over faces(polygon)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
                // hardcode loading to triangles
                int fv = 3;

                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                    // vertex position
                    tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                    tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                    tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                    // vertex normal
                    tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
                    tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
                    tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];

                    tinyobj::real_t ux = attrib.texcoords[2 * idx.texcoord_index + 0];
                    tinyobj::real_t uy = attrib.texcoords[2 * idx.texcoord_index + 1];

                    // copy it into our vertex
                    Vertex new_vert;
                    new_vert.pos.x = vx;
                    new_vert.pos.y = vy;
                    new_vert.pos.z = vz;

                    new_vert.normal.x = nx;
                    new_vert.normal.y = ny;
                    new_vert.normal.z = nz;

                    new_vert.uv.x = ux;
                    new_vert.uv.y = uy;

                    // we are setting the vertex color as the vertex normal. This is just for display purposes
                    new_vert.color = new_vert.normal;

                    if (uniqueVertices.count(new_vert) == 0) {
                        uniqueVertices[new_vert] = static_cast<uint32_t>(vertices.size());
                        vertices.push_back(new_vert);
                    }

                    indices.push_back(uniqueVertices[new_vert]);
                }
                index_offset += fv;
            }
        }
    }
}  // namespace Yare::Utilities
