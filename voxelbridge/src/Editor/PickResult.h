//
// Created by antonscereskevics on 07/11/2026.
//

#ifndef VOXELBRIDGE_PICKRESULT_H
#define VOXELBRIDGE_PICKRESULT_H
#include "glm/vec3.hpp"

struct PickResult {
    bool hit = false;
    bool solid = false;
    glm::ivec3 voxel = {0, 0, 0};
    glm::ivec3 faceNormal = {0, 0, 0};
    float distance = 0.0f;
};

#endif //VOXELBRIDGE_PICKRESULT_H
