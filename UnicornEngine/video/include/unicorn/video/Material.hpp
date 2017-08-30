/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MATERIAL_HPP
#define UNICORN_VIDEO_MATERIAL_HPP

#include <unicorn/video/Color.hpp>

#include <glm/glm.hpp>

namespace unicorn
{
namespace video
{
class Texture;

/*
 * @brief Material class holds all data which needed to render any object
 */
class Material
{
public:
    UNICORN_EXPORT Material();

    /**
     * @brief Sets new albedo texture
     * @param[in] texture albedo texture
     * @attention Updates flags IsColored to false
     */
    UNICORN_EXPORT void SetAlbedo(Texture const* texture);

    /**
     * @brief Sets rendering mode to wireframe
     * @param[in] wireframe true if wired and false if not
     * 
     * @attention sets IsColored to false to prevent rendering color instead albedo
     */
    UNICORN_EXPORT void SetIsWired(bool wireframe);

    /**
     * @brief Sets colored mode
     * @param[in] colored true if must render with color and false if render with albedo
     * 
     * @attention if colored mode enabled and no albedo provided, default engine texture will be used
     */
    UNICORN_EXPORT void SetIsColored(bool colored);

    /**
     * @brief Sets IsColored mode to true and removes binded texture
     */
    UNICORN_EXPORT void RemoveAlbedo();

    /**
     * @brief Returns state of object rendering mode
     * @return true if colored mode enabled and false if not
     */
    UNICORN_EXPORT bool IsColored() const;

    /**
     * @brief Returns state of object rendering mode
     * @return true if wired mode enabled and false if not
     */
    UNICORN_EXPORT bool IsWired() const;

    /**
     * @brief Returns const reference to albedo texture
     * @return const reference to binded albdeo texture
     */
    UNICORN_EXPORT Texture const* GetAlbedo() const;

    /*
     * @brief Color of object
     */
    glm::vec3 color;
private:
    bool m_isColored;
    bool m_isWired;
    Texture const* m_albedo;
};
}
}

#endif // UNICORN_VIDEO_MATERIAL_HPP
