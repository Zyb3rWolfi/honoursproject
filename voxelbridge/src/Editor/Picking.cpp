//
// Created by antonscereskevics on 03/11/2026.
//

#include "Picking.h"
#include <glm/glm.hpp>

Picking::Picking() {}
Picking::~Picking() {}

Ray Picking::ScreenToRay(glm::vec2 ndc, const glm::mat4 &view, const glm::mat4 &projection) {

    const glm::mat4 &mvp = glm::inverse(projection * view);

    glm::vec4 nearTarget(ndc.x, ndc.y, -1.0f, 1.0f);
    glm::vec4 farTarget(ndc.x, ndc.y, 1.0f, 1.0f);

    glm::vec4 worldNear = mvp * nearTarget;
    glm::vec4 worldFar = mvp * farTarget;

    worldNear /= worldNear.w;
    worldFar /= worldFar.w;

    Ray ray;
    ray.origin = glm::vec3(worldNear);
    ray.direction = normalize(glm::vec3(worldFar - worldNear));

    return ray;

}

bool IntersectRayPlaneY(const Ray& ray, float planeY, glm::vec3& outHitPoint) {
    // Avoid division by zero if ray is parallel to the plane
    if (std::abs(ray.direction.y) < 1e-6f)
        return false;

    float t = (planeY - ray.origin.y) / ray.direction.y;
    if (t < 0.0f) // Intersection is behind the camera
        return false;

    outHitPoint = ray.origin + t * ray.direction;
    return true;
}

