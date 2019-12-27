/***************************/
/*  FILE NAME: vector.hpp  */
/***************************/
#ifndef _ORCA_VECTOR_HPP_
#define _ORCA_VECTOR_HPP_

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace orca
{
    using vec2i = vec2<int>;
    using vec2f = vec2<float>;
    using vec2d = vec2<double>;

    using vec3i = vec3<int>;
    using vec3f = vec3<float>;
    using vec3d = vec3<double>;

    using vec4i = vec4<int>;
    using vec4f = vec4<float>;
    using vec4d = vec4<double>;
} // namespace orca
#endif // !_ORCA_VECTOR_HPP_