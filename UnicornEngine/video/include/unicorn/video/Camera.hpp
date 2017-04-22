/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERA_HPP
#define UNICORN_VIDEO_CAMERA_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace unicorn
{
    namespace video
    {
        class Camera
        {
        public:
            Camera();

            void SetPerspective(float fov, float aspect, float znear, float zfar);

            void UpdateAspectRatio(float aspect);

            void SetPosition(glm::vec3 position);

            void SetRotation(glm::vec3 rotation);

            void Rotate(glm::vec3 delta);

            void SetTranslation(glm::vec3 translation);

            void Translate(glm::vec3 delta);
        private:
            glm::vec3 m_rotation;
            glm::vec3 m_position;
            struct
            {
                glm::mat4 m_perspective;
                glm::mat4 m_view;
            } m_matrices;

            float m_fov;
            float m_znear, m_zfar;

            void UpdateViewMatrix();
        };
    }
}

#endif // UNICORN_VIDEO_CAMERA_HPP
