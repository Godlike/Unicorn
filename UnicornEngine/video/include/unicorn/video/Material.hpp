/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_VIDEO_MATERIAL_HPP
#define UNICORN_VIDEO_MATERIAL_HPP

#include <unicorn/utility/asset/SimpleStorage.hpp>
#include <unicorn/utility/SharedMacros.hpp>

#include <string>
#include <unordered_map>

namespace unicorn
{
namespace video
{
class Material
{
    public:
        UNICORN_EXPORT Material(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
        UNICORN_EXPORT const std::vector<uint8_t>& GetVertexShaderContent() const;
        UNICORN_EXPORT const std::vector<uint8_t>& GetFragmentShaderContent() const;
        UNICORN_EXPORT bool IsInitialized() const;
        UNICORN_EXPORT void AddUniform(uint32_t bindingLocation, const std::string& uniformName);
        UNICORN_EXPORT unicorn::utility::asset::Handler const& GetVertShaderHandler() const;
        UNICORN_EXPORT unicorn::utility::asset::Handler const& GetFragShaderHandler() const;
    private:
        bool m_isInitialized;
        unicorn::utility::asset::Handler m_vertShaderHandler;
        unicorn::utility::asset::Handler m_fragShaderHandler;

        std::unordered_map<uint32_t, std::string> m_uniforms;
};
}
}

#endif // UNICORN_VIDEO_MATERIAL_HPP
