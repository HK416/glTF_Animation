/*************************************/
/*  FILE NAME: vector_functions.hpp  */
/*************************************/
#ifndef _ORCA_VECTOR_FUNCTIONS_HPP_
#define _ORCA_VECTOR_FUNCTIONS_HPP_

/**************/
/*  INCLUDES  */
/**************/
#include <cmath>
#include <limits>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "vector.hpp"

namespace orca
{
    /* print vec2<T> data */
    template<typename T>
    inline void PrintVector(const vec2<T>& vec)
    {
        std::cout << "(" << vec.x << ", " << vec.y << ")" << std::endl;
    }

    /* print vec3<T> data */
    template<typename T>
    inline void PrintVector(const vec3<T>& vec)
    {
        std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")" << std::endl;
    }

    /* print vec4<T> data */
    template<typename T>
    inline void PrintVector(const vec4<T>& vec)
    {
        std::cout << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")" << std::endl;
    }

    /* make vec2<T> type */
    template<typename T>
    inline orca::vec2<T> MakeVector2(const T* data)
    {
        orca::vec2<T> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = data[i];
        return result;
    }

    /* make vec2<T> type */
    template<typename Out, typename In>
    inline orca::vec2<Out> MakeVector2(const In* data)
    {
        orca::vec2<Out> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = static_cast<Out>(data[i]);
        return result;
    }

    /* make vec3<T> type */
    template<typename T>
    inline orca::vec3<T> MakeVector3(const T* data) 
    {
        orca::vec3<T> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = data[i];
        return result;
    }

    /* make vec3<T> type */
    template<typename Out, typename In>
    inline orca::vec3<Out> MakeVector3(const In* data)
    {
        orca::vec3<Out> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = static_cast<Out>(data[i]);
        return result;
    }

    /* make vec4<T> type */
    template<typename T>
    inline orca::vec4<T> MakeVector4(const T* data)
    {
        orca::vec4<T> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = data[i];
        return result;
    }
    
    /* make vec4<T> type */
    template<typename Out, typename In>
    inline orca::vec4<Out> MakeVector4(const In* data)
    {
        orca::vec4<Out> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = static_cast<Out>(data[i]);
        return result;
    }

    /* returns the length of a vec2<T> */
    template<typename T>
    inline T Length(const vec2<T>& vec)
    {
        T sum = static_cast<T>(0.0);
        for(unsigned int i = 0; i < vec.Size(); ++i)
            sum += vec.data[i] * vec.data[i];

        if(std::abs(sum) <= std::numeric_limits<T>::epsilon())
            return static_cast<T>(0.0);
        else
            return std::sqrt(sum);
    }

    /* returns the length of a vec3<T> */
    template<typename T>
    inline T Length(const vec3<T>& vec)
    {
        T sum = static_cast<T>(0.0);
        for(unsigned int i = 0; i < vec.Size(); ++i)
            sum += vec.data[i] * vec.data[i];

        if(std::abs(sum) <= std::numeric_limits<T>::epsilon())
            return static_cast<T>(0.0);
        else
            return std::sqrt(sum);
    }

    /* returns the length of a vec4<T> */
    template<typename T>
    inline T Length(const vec4<T>& vec)
    {
        T sum = static_cast<T>(0.0);
        for(unsigned int i = 0; i < vec.Size(); ++i)
            sum += vec.data[i] * vec.data[i];

        if(std::abs(sum) <= std::numeric_limits<T>::epsilon())
            return static_cast<T>(0.0);
        else
            return std::sqrt(sum);
    }

    /* returns a normalized vec2<T> */
    template<typename T>
    inline vec2<T> Normalize(const vec2<T>& vec)
    {
        vec2<T> result(vec);
        const T len = Length(vec);
        if(std::abs(len) > std::numeric_limits<T>::epsilon())
        {
            result /= len;
        }
        return result;
    }

    /* returns a normalized vec3<T> */
    template<typename T>
    inline vec3<T> Normalize(const vec3<T>& vec)
    {
        vec3<T> result(vec);
        const T len = Length(vec);
        if(std::abs(len) > std::numeric_limits<T>::epsilon())
        {
            result /= len;
        }
        return result;
    }
    
    /* returns a normalized vec4<T> */
    template<typename T>
    inline vec4<T> Normalize(const vec4<T>& vec)
    {
        vec4<T> result(vec);
        const T len = Length(vec);
        if(std::abs(len) > std::numeric_limits<T>::epsilon())
        {
            result /= len;
        }
        return result;
    }

    /* returns the result of the dot product */
    template<typename T>
    inline T Dot(const vec2<T>& a, const vec2<T>& b)
    {
        T dot = static_cast<T>(0.0);
        for(unsigned int i = 0; i < a.Size(); ++i)
            dot += a.data[i] * b.data[i];
        return dot;
    }

    /* returns the result of the dot product */
    template<typename T>
    inline T Dot(const vec3<T>& a, const vec3<T>& b)
    {
        T dot = static_cast<T>(0.0);
        for(unsigned int i = 0; i < a.Size(); ++i)
            dot += a.data[i] * b.data[i];
        return dot;
    }
    
    /* returns the result of the dot product */
    template<typename T>
    inline T Dot(const vec4<T>& a, const vec4<T>& b)
    {
        T dot = static_cast<T>(0.0);
        for(unsigned int i = 0; i < a.Size(); ++i)
            dot += a.data[i] * b.data[i];
        return dot;
    }

    /* returns the result of the cross product */
    template<typename T>
    inline vec3<T> Cross(const vec3<T>& a, const vec3<T>& b)
    {
        vec3<T> result;
        result[0] = a[1] * b[2] - a[2] * b[1];
        result[1] = a[2] * b[0] - a[0] * b[2];
        result[2] = a[0] * b[1] - a[1] * b[0];
        return result;
    }

    /* returns the linear interpolation of two vectors */
    template<typename T>
    inline vec2<T> Lerp(const vec2<T>& start, const vec2<T>& end, T percent)
    {
        return (start + percent * (end - start));
    }

    /* returns the linear interpolation of two vectors */
    template<typename T>
    inline vec3<T> Lerp(const vec3<T>& start, const vec3<T>& end, T percent)
    {
        return (start + percent * (end - start));
    }

    /* returns the linear interpolation of two vectors */
    template<typename T>
    inline vec4<T> Lerp(const vec4<T>& start, const vec4<T>& end, T percent)
    {
        return (start + percent * (end - start));
    }

    /* returns the linear interpolation of three vectors */
    template<typename T>
    inline vec2<T> Lerp(const vec2<T>& v0, const vec2<T>& v1, const vec3<T>& v2, T u, T v)
    {
        return ((static_cast<T>(1.0) - u - v) * v0) + (u * v1) + (v * v2);
    }

    /* returns the linear interpolation of three vectors */
    template<typename T>
    inline vec3<T> Lerp(const vec3<T>& v0, const vec3<T>& v1, const vec3<T>& v2, T u, T v)
    {
        return ((static_cast<T>(1.0) - u - v) * v0) + (u * v1) + (v * v2);
    }

    /* returns the linear interpolation of three vectors */
    template<typename T>
    inline vec4<T> Lerp(const vec4<T>& v0, const vec4<T>& v1, const vec4<T>& v2, T u, T v)
    {
        return ((static_cast<T>(1.0) - u - v) * v0) + (u * v1) + (v * v2);
    }

    /* returns the spherical linear interpolation of two vectors */
    template<typename T>
    inline vec2<T> Slerp(const vec2<T>& start, const vec2<T>& end, T percent)
    {
        T dot = std::clamp(Dot(start, end), static_cast<T>(-1.0), static_cast<T>(1.0));
        T theta = std::acos(dot) * percent;
        vec2<T> relative_vec = Normalize(end - start * dot);
        return ((start * std::cos(theta)) + (relative_vec * std::sin(theta)));
    }

    /* returns the spherical linear interpolation of two vectors */
    template<typename T>
    inline vec3<T> Slerp(const vec3<T>& start, const vec3<T>& end, T percent)
    {
        T dot = std::clamp(Dot(start, end), static_cast<T>(-1.0), static_cast<T>(1.0));
        T theta = std::acos(dot) * percent;
        vec3<T> relative_vec = Normalize(end - start * dot);
        return ((start * std::cos(theta)) + (relative_vec * std::sin(theta)));
    }

    /* returns the spherical linear interpolation of two vectors */
    template<typename T>
    inline vec4<T> Slerp(const vec4<T>& start, const vec4<T>& end, T percent)
    {
        T dot = std::clamp(Dot(start, end), static_cast<T>(-1.0), static_cast<T>(1.0));
        T theta = std::acos(dot) * percent;
        vec4<T> relative_vec = Normalize(end - start * dot);
        return ((start * std::cos(theta)) + (relative_vec * std::sin(theta)));
    }

} // namespace orca
#endif // !_ORCA_VECTOR_FUNCTIONS_HPP_