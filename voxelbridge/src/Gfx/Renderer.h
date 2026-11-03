//
// Created by antonscereskevics on 29/10/2026.
//

#ifndef VOXELBRIDGE_RENDERER_H
#define VOXELBRIDGE_RENDERER_H
#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "glm/vec3.hpp"


class Renderer {
public:
    Renderer();
    void Clear(const glm::vec3& color);

    void BeginScene(const Camera& camera, float aspectRatio);
    void Submit(const Mesh& mesg, const glm::mat4& model = glm::mat4(1.0f));

    void SubmitLines(const Mesh& mesh, const glm::mat4& model = glm::mat4(1.0f));
private:
    Shader m_Shader;
    Shader m_LineShader;
};


#endif //VOXELBRIDGE_RENDERER_H
