/***********************/
/*  FILE NAME: matrix  */
/***********************/
#ifndef _ORCA_MATRIX_HPP_
#define _ORCA_MATRIX_HPP_

#include "vector.hpp"
#include "mat2.hpp"
#include "mat3.hpp"
#include "mat4.hpp"

namespace orca
{
    using mat2i = mat2<int>;
    using mat2f = mat2<float>;
    using mat2d = mat2<double>;
    
    using mat3i = mat3<int>;
    using mat3f = mat3<float>;
    using mat3d = mat3<double>;
    
    using mat4i = mat4<int>;
    using mat4f = mat4<float>;
    using mat4d = mat4<double>;
} // namespace orca
#endif // !_ORCA_MATRIX_HPP_