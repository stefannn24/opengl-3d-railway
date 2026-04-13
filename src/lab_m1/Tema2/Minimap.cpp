#include "lab_m1/Tema2/minimap.h"
#include <vector>
#include <cmath>

using namespace std;

void minimap::CreateMinimapMeshes(std::unordered_map<std::string, Mesh*>& meshes)
{
    // helper to create a 3d Box
    // creates a unit box 1x1x1 centered at 0
    auto CreateBox = [&](string name, glm::vec3 color) {
        vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(-0.5f, -0.5f,  0.5f), color),
            VertexFormat(glm::vec3(0.5f, -0.5f,  0.5f), color),
            VertexFormat(glm::vec3(-0.5f,  0.5f,  0.5f), color),
            VertexFormat(glm::vec3(0.5f,  0.5f,  0.5f), color),
            VertexFormat(glm::vec3(-0.5f, -0.5f, -0.5f), color),
            VertexFormat(glm::vec3(0.5f, -0.5f, -0.5f), color),
            VertexFormat(glm::vec3(-0.5f,  0.5f, -0.5f), color),
            VertexFormat(glm::vec3(0.5f,  0.5f, -0.5f), color)
        };
        vector<unsigned int> indices = {
            0, 1, 2, 1, 3, 2, 2, 3, 7, 2, 7, 6, 1, 7, 3, 1, 5, 7,
            6, 7, 4, 7, 5, 4, 0, 4, 1, 1, 4, 5, 2, 6, 4, 0, 2, 4
        };
        Mesh* mesh = new Mesh(name);
        mesh->InitFromData(vertices, indices);
        meshes[mesh->GetMeshID()] = mesh;
        };

    CreateBox("map_track", glm::vec3(0.2f, 0.2f, 0.2f));
    CreateBox("map_bridge", glm::vec3(0.8f, 0.5f, 0.2f));

}