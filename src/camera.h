#pragma once

#include <glm/glm.hpp>

namespace engine {
    class Camera {
    public:
        Camera(
            float moveSpeed,
            float lookSensitivity,

            float fovy, 
            float aspectRatio, 
            float nearClip, 
            float farClip
        );

        const glm::vec3& getPosition();
        const glm::mat4& getViewMatrix();
        const glm::mat4& getProjectionMatrix();

        void updateMovement(const glm::vec3& input, float deltaTime);
        void updateMouselook(int xDelta, int yDelta);

    private:
        // Camera options
        float moveSpeed;
        float lookSensitivity;

        // Projection options
        float fovy;
        float aspectRatio;
        float nearClip;
        float farClip;

        // Camera transforms
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        float pitch;
        float yaw;

        // Matrices
        glm::mat4 projection;
        glm::mat4 view;
        bool projectionDirty;
        bool viewDirty;

        // Calculate front, up & right vectors from camera yaw & pitch
        void updateVectors();
    };
}