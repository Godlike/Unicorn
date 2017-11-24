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

/** @brief Material class holds all data which needed to render any mesh */
class Material
{
public:
    /** @brief Default constructor */
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
     * @attention sets IsColored to false
     */
    UNICORN_EXPORT void SetIsWired(bool wireframe);

    /**
     * @brief Sets mesh in visible state
     *
     * @param[in] visible visibility flag
     */
    UNICORN_EXPORT void SetIsVisible(bool visible);

    /**
     * @brief Determines if visible
     *
     * @return True if visible, False otherwise.
     */
    UNICORN_EXPORT bool IsVisible() const;

    /**
     * @brief Sets colored mode
     *
     * @param[in] colored true if must render with color and false if render with albedo
     *
     * @attention if colored mode enabled and no albedo provided, default engine "replace me" texture is used
     */
    UNICORN_EXPORT void SetIsColored(bool colored);

    /** @brief Sets IsColored mode to true and removes binded texture */
    UNICORN_EXPORT void RemoveAlbedo();

    /** Returns @c true if colored mode is enabled and @false otherwise */
    UNICORN_EXPORT bool IsColored() const;

    /** Returns @c true if wired mode is enabled and @false otherwise */
    UNICORN_EXPORT bool IsWired() const;

    /**
     * @brief Returns pointer to albedo texture
     *
     * @return pointer to binded albdeo texture
     */
    UNICORN_EXPORT Texture const* GetAlbedo() const;

    /** @brief Color of object */
    glm::vec3 color;
private:
    bool m_isColored;
    bool m_isWired;
    bool m_isVisible;

    Texture const* m_albedo;
};
}
}

#endif // UNICORN_VIDEO_MATERIAL_HPP
