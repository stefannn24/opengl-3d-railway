#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/Camera.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderTrain(glm::vec3 position, float angleY);
        void RenderWagon(glm::vec3 position, float angleY);

        void RenderStation(const std::string& type, glm::vec3 position, float rotationY = 0.0f);
        void RenderPassenger(const std::string& type, glm::vec3 position, float rotationY = 0.0f);
        void RenderTrack(const std::string& type, glm::vec3 position, float rotationY = 0.0f);
        void Tema2::RenderTrainRotated(glm::vec3 position, float angleY);
        void Tema2::GetPositionOnPath(float dist, glm::vec3& outPos, float& outRotY);
        void DrawScene();
        void RenderMinimap();

    protected:
        tema2_camera::Camera* camera;
        glm::mat4 projectionMatrix;

        tema2_camera::Camera* minimapCamera;
        glm::mat4 orthoProjection;

        float wheelRotationAngle;
        
		// variables for the 3rd view camera
        bool cameraLock;
        glm::vec3 trainPosition;
        glm::vec3 cameraOffset;

        float distanceTraveled;
        const int trackLenX = 4; 
        const int trackLenZ = 2;
        bool trainRunning;
        bool renderingMinimap;
    };
}