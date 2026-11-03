//
// Created by antonscereskevics on 03/11/2026.
//

#ifndef VOXELBRIDGE_PICKING_H
#define VOXELBRIDGE_PICKING_H
#include "glm/fwd.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;
};

class Picking {
public:
    Picking();
    ~Picking();

    Ray ScreenToRay(glm::vec2 ndc, const glm::mat4& view, const glm::mat4& projection);
};


#endif //VOXELBRIDGE_PICKING_H
