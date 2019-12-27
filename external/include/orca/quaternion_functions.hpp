/*****************************************/
/*  FILE NAME: quaternion_functions.hpp  */
/*****************************************/
#ifndef _ORCA_QUATERNION_FUNCTIONS_HPP_
#define _ORCA_QUATERNION_FUNCTIONS_HPP_

/**************/
/*  INCLUDES  */
/**************/
#include <cmath>
#include <limits>
#include <iostream>
#include <algorithm>
#include <stdexcept>

#include "vector.hpp"
#include "matrix.hpp"
#include "quaternion.hpp"

namespace orca
{
    /* print quaternion data */
    template<typename T>
    inline void PrintQuaternion(const quaternion<T>& quaternion)
    {
        std::cout << "(" << quaternion.r << ", " << quaternion.x << ", " << quaternion.y << ", " << quaternion.z << ")" << std::endl;
    }

    template<typename T>
    inline T DegreeToRadian(T degree)
    {
        return degree * static_cast<T>(3.14159265358979323846 / 180.0);
    }

    template<typename T>
    inline T RadianToDegree(T radian)
    {
        return radian * static_cast<T>(180.0 / 3.14159265358979323846);
    }

    /* return quaternion conjugate */
    template<typename T>
    inline quaternion<T> Conjugate(const quaternion<T>& quat)
    {
        quaternion<T> result;
        result.r = quat.r;
        result.x = quat.x * -1;
        result.y = quat.y * -1;
        result.z = quat.z * -1;
        return result;
    }

    /* return quaternion inverse */
    template<typename T>
    inline quaternion<T> Inverse(const quaternion<T>& quat)
    {
        T squared = static_cast<T>(0.0);
        for(unsigned int i = 0; i < quat.Size(); ++i)
            squared += quat.data[i] * quat.data[i];
        if(std::abs(squared) <= std::numeric_limits<T>::epsilon())
            return quat;
        else
            return Conjugate(quat) / squared;
    }

    /* return quaternion matrix */
    template<typename T>
    inline mat4<T> QuaternionToMatrix(const quaternion<T>& quat)
    {
        const T xx = quat.x * quat.x;
        const T yy = quat.y * quat.y;
        const T zz = quat.z * quat.z;
        const T xy = quat.x * quat.y;
        const T xz = quat.x * quat.z;
        const T xw = quat.x * quat.r;
        const T yz = quat.y * quat.z;
        const T yw = quat.y * quat.r;
        const T zw = quat.z * quat.r;

        mat4<T> result;
		result[0][0] = static_cast<T>(1.0) - static_cast<T>(2.0) * (yy + zz);
		result[0][1] = static_cast<T>(2.0) * (xy + zw);
		result[0][2] = static_cast<T>(2.0) * (xz - yw);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(2.0) * (xy - zw);
		result[1][1] = static_cast<T>(1.0) - static_cast<T>(2.0) * (xx + zz);
		result[1][2] = static_cast<T>(2.0) * (yz + xw);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(2.0) * (xz + yw);
		result[2][1] = static_cast<T>(2.0) * (yz - xw);
		result[2][3] = static_cast<T>(0.0);
		result[2][2] = static_cast<T>(1.0) - static_cast<T>(2.0) * (xx + yy);

		result[3][0] = static_cast<T>(0.0);
		result[3][1] = static_cast<T>(0.0);
		result[3][3] = static_cast<T>(1.0);
		result[3][2] = static_cast<T>(0.0);
        return result;
    }
    
    /* return the coordinates of vector ratated using quaternion */
    template<typename T>
    inline vec3<T> Rotate(T angle_rad, const vec3<T>& axis, const vec3<T>& src)
    {
        quaternion<T> quat(angle_rad, axis);
        quaternion<T> orgin_vec(src);
        quaternion<T> result_vec = quat * orgin_vec * Conjugate(quat);
        vec3<T> result;
        result.x = result_vec.x;
        result.y = result_vec.y;
        result.z = result_vec.z;
        return result;
    }

    /* return rotate matrix (count clock wise) */
    template<typename T>
    inline mat4<T> Rotate(T angle_rad, T x, T y, T z)
    {
        return QuaternionToMatrix(quaternion<T>(angle_rad, vec3<T>(x, y, z)));
    }

    /* return rotate matrix (count clock wise) */
    template<typename T>
    inline mat4<T> Rotate(T angle_rad, const vec3<T>& vec)
    {
        return QuaternionToMatrix(quaternion<T>(angle_rad, vec));
    }

    /* return rotate matrix (count clock wise) */
    template<typename T>
    inline mat4<T> Rotate(const vec4<T>& vec)
    {
        return QuaternionToMatrix(quaternion<T>(vec.x, orca::vec3<T>(vec.y, vec.z, vec.w)));
    }

    /* return rotate matrix (count clock wise) */
    template<typename T>
    inline mat4<T> Rotate(T angle_rad_x, T angle_rad_y, T angle_rad_z)
    {
        return Rotate(angle_rad_x, vec3<T>(1, 0, 0)) * Rotate(angle_rad_y, vec3<T>(0, 1, 0)) * Rotate(angle_rad_z, vec3<T>(0, 0, 1));
    }

    /* return rotate matrix (count clock wise) */
    template<typename T>
    inline mat4<T> Rotate(const vec3<T>& angle_rads)
    {
        return Rotate(angle_rads.x, vec3<T>(1, 0, 0)) * Rotate(angle_rads.y, vec3<T>(0, 1, 0)) * Rotate(angle_rads.z, vec3<T>(0, 0, 1));
    }

} // namespace orca
#endif // !_ORCA_QUATERNION_FUNCTIONS_HPP_