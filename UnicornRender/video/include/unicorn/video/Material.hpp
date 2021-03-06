/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MATERIAL_HPP
#define UNICORN_VIDEO_MATERIAL_HPP

#include <unicorn/video/Color.hpp>

#include <wink/signal.hpp>
#include <glm/glm.hpp>

#include <memory>

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
    Material();

    /**
     * @brief Sets new albedo texture
     * @param[in] texture albedo texture
     * @attention Updates flags IsColored to false
     */
    void SetAlbedo(std::shared_ptr<Texture> texture);

    /**
     * @brief Sets rendering mode to wireframe
     * @param[in] wireframe true if wired and false if not
     *
     * @attention sets IsColored to false
     */
    void SetIsWired(bool wireframe);

    /**
     * @brief Sets material visibility flag
     *
     * @param[in] visible visibility flag
     */
    void SetIsVisible(bool visible);

    /**
     * @brief Returns visibility flag
     *
     * @return True if visible, False otherwise.
     */
    bool IsVisible() const;

    /**
     * @brief Sets colored mode
     *
     * @param[in] colored true if must render with color and false if render with albedo
     *
     * @attention if colored mode enabled and no albedo provided, default engine "replace me" texture is used
     */
    void SetIsColored(bool colored);

    /** @brief Sets IsColored mode to true and removes bound texture */
    void RemoveAlbedo();

    /** Returns @c true if colored mode is enabled and @false otherwise */
    bool IsColored() const;

    /** Returns @c true if wired mode is enabled and @false otherwise */
    bool IsWired() const;

    /**
     * @brief Returns pointer to albedo texture
     *
     * @return pointer to binded albedo texture
     */
    std::shared_ptr<Texture> GetAlbedo() const;

    /** @brief Sets color */
    void SetColor(glm::vec3 color);

    /** @brief Returns color */
    glm::vec3 GetColor() const;

    /**
     * @brief Sets sprite area
     *
     * @param x texture offset by x in pixels
     * @param y texture offset by y in pixels
     * @param width sprite width in pixels
     * @param height sprite height in pixels
     */
    void SetSpriteArea(int32_t x, int32_t y, int32_t width, int32_t height);

    /**
     * @brief Returns sprite area
     *
     * @return sprite area: {x, y, width, height}
    */
    glm::vec4 GetSpriteArea() const;

    /**
    * @brief Returns normalized sprite area
    *
    * @return normalized sprite ares: {x, y, width, height}
    */
    glm::vec4 GetNormalizedSpriteArea() const;

    /** @brief Signal for material update notification */
    wink::signal<wink::slot<void()>> DataUpdated;
protected:
    void NormalizeSpriteArea();

    glm::vec3 m_color;
    glm::vec4 m_normSpriteArea;
    glm::vec4 m_spriteArea;

    bool m_isColored;
    bool m_isWired;
    bool m_isVisible;

    std::shared_ptr<Texture> m_albedo;
};
}
}

#endif // UNICORN_VIDEO_MATERIAL_HPP
