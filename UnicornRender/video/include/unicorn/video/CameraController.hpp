/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_CAMERACONTROLLER_HPP
#define UNICORN_VIDEO_CAMERACONTROLLER_HPP

#include <unicorn/utility/SharedMacros.hpp>
#include <unicorn/video/Transform.hpp>

namespace unicorn
{
namespace video
{

/** @brief Camera controller class to create new controllers for cameras */
class CameraController : public Transform
{
public:
    /**
     * @brief      { function_description }
     */
    void Calculate();
protected:
    /**
     * @brief      { function_description }
     *
     * @param      cameraView  The camera view
     */
    CameraController(glm::mat4& cameraView);

    glm::mat4& m_cameraView;
};

} // namespace video
} // namespace unicorn

#endif // UNICORN_VIDEO_CAMERACONTROLLER_HPP
