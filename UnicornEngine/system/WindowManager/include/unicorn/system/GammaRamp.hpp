/*
* Copyright (C) 2017 by Godlike
* This code is licensed under the MIT license (MIT)
* (http://opensource.org/licenses/MIT)
*/

#ifndef UNICORN_SYSTEM_GAMMA_RAMP_HPP
#define UNICORN_SYSTEM_GAMMA_RAMP_HPP

#include <cstdint>

namespace unicorn
{
namespace system
{

/** @brief  Contains gamma ramp information */
struct GammaRamp
{
    /** @brief  Constructs gamma ramp object */
    GammaRamp();

    /** @brief  Constructs a copy of @p other
     *
     *  Allocates own storage for @p red, @p green and @p blue channels
     *
     *  @param  other   GammaRamp object to copy data from
     */
    GammaRamp(const GammaRamp& other);

    /** @brief  Copies all data from @p other
     *
     *  Reallocates own storage for @p red, @p green and @p blue channels
     *
     *  @param  other   GammaRamp object to copy data from
     *
     *  @return reference to this
     */
    GammaRamp& operator=(const GammaRamp& other);

    /** @brief  Constructs a new object from @p other moving all data
     *
     *  @param  other   GammaRamp object to move data from
     */
    GammaRamp(GammaRamp&& other);

    /** @brief  Moves all data from @p other
     *
     *  @param  other   GammaRamp object to move data from
     *
     *  @return reference to this
     */
    GammaRamp& operator=(GammaRamp&& other);

    /** @brief  Destructs gamma ramp object and deallocates all allocated space */
    ~GammaRamp();

    //! Size of each channel
    uint32_t size;

    //! Red channel
    uint16_t* red;

    //! Green channel
    uint16_t* green;

    //! Blue channel
    uint16_t* blue;
};

}
}

#endif /* UNICORN_SYSTEM_GAMMA_RAMP_HPP */
