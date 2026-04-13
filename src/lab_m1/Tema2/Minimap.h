#pragma once

#include <unordered_map>
#include <string>
#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

namespace minimap
{
    // generates the 2D meshes required for the minimap icons
    void CreateMinimapMeshes(std::unordered_map<std::string, Mesh*>& meshes);
}