/*
* Copyright (C) 2017 by Grapefruit Tech
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef VORPAL_GRAPHICS_VULKANSUBSYSTEM_HPP
#define VORPAL_GRAPHICS_VULKANSUBSYSTEM_HPP

#include <vorpal/utility/templates/Singleton.hpp>

namespace vp
{
    namespace graphics
    {
        class VulkanSubsystem : public utility::templates::Singleton<VulkanSubsystem>
        {
        public:
            void Init();
            void Deinit();
        private:
            friend class utility::templates::Singleton<VulkanSubsystem>;
            VulkanSubsystem();

            ~VulkanSubsystem() = default;

            VulkanSubsystem(const VulkanSubsystem& other) = delete;
            VulkanSubsystem(const VulkanSubsystem&& other) = delete;
            VulkanSubsystem& operator=(const VulkanSubsystem& other) = delete;
            VulkanSubsystem& operator=(const VulkanSubsystem&& other) = delete;
        };
    }
}
#endif // VORPAL_GRAPHICS_VULKANSUBSYSTEM_HPP
