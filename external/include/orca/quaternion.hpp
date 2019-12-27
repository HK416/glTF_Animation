/*******************************/
/*  FILE NAME: quaternion.hpp  */
/*******************************/
#ifndef _ORCA_QUATERNION_HPP_
#define _ORCA_QUATERNION_HPP_

/**************/
/*  INCLUDES  */
/**************/
#include <cmath>
#include <limits>
#include <algorithm>
#include <stdexcept>

#include "vector.hpp"
#include "vector_functions.hpp"

namespace orca
{
    template<typename T> class vec2;
    template<typename T> class vec3;
    template<typename T> class vec4;
}

namespace orca
{
    /****************************/
    /*  CLASS NAME: quaternion  */
    /****************************/
    template<typename T>
    class quaternion
    {
    private:
        using element_type = T;
        using this_type = quaternion<element_type>;
        static constexpr unsigned int max_data_size = 4;

    public:
        quaternion() : r(static_cast<element_type>(0.0)), x(static_cast<element_type>(0.0)), y(static_cast<element_type>(0.0)), z(static_cast<element_type>(0.0)) {}
        quaternion(element_type x, element_type y, element_type z) : r(static_cast<element_type>(0.0)), x(x), y(y), z(z) {}
        quaternion(element_type r, element_type x, element_type y, element_type z) : r(r), x(x), y(y), z(z) {}
        quaternion(const this_type& other) : r(other.r), x(other.x), y(other.y), z(other.z) {}
        explicit quaternion(const vec3<element_type>& vec) : r(static_cast<element_type>(0.0)), x(vec.x), y(vec.y), z(vec.z) {}
        explicit quaternion(const vec4<element_type>& vec) : r(vec.w), x(vec.x), y(vec.y), z(vec.z) {}

        quaternion(element_type angle_rad, const vec3<element_type>& axis) : r(static_cast<element_type>(0.0)), x(static_cast<element_type>(0.0)), y(static_cast<element_type>(0.0)), z(static_cast<element_type>(0.0))
        {
            const vec3<element_type> axis_norm = orca::Normalize(axis);
            r = static_cast<element_type>(std::cos(static_cast<double>(angle_rad) / 2.0));
            x = static_cast<element_type>(std::sin(static_cast<double>(angle_rad) / 2.0)) * axis_norm.x;
            y = static_cast<element_type>(std::sin(static_cast<double>(angle_rad) / 2.0)) * axis_norm.y;
            z = static_cast<element_type>(std::sin(static_cast<double>(angle_rad) / 2.0)) * axis_norm.z;
        }

    public:
        this_type& operator=(const this_type& rhs)
        {
            for(unsigned int i = 0; i < max_data_size; ++i)
                data[i] = rhs.data[i];
            return *this;
        }
        
        this_type& operator=(const vec3<element_type>& rhs)
        {
            r = static_cast<element_type>(0.0);
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }

        this_type& operator=(const vec4<element_type>& rhs)
        {
            r = rhs.w;
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            return *this;
        }

        this_type operator+(const this_type& rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] + rhs.data[i];
            return result;
        }

        this_type& operator+=(const this_type& rhs)
        {
            return (*this = *this + rhs);
        }

        this_type operator-() const
        {
            this_type result;
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] * -1;
            return result;
        }

        this_type operator-(const this_type& rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] - rhs.data[i];
            return result;
        }

        this_type& operator-=(const this_type& rhs)
        {
            return (*this = *this - rhs);
        }

        this_type operator*(element_type rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] * rhs;
            return result;
        }

        this_type& operator*=(element_type rhs) 
        {
            return (*this = *this * rhs);
        }

        this_type operator*(const this_type& rhs) const
        {
            this_type result;
            result.r = r * rhs.r - x * rhs.x - y * rhs.y - z * rhs.z;
            result.x = r * rhs.x + x * rhs.r + y * rhs.z - z * rhs.y;
            result.y = r * rhs.y + y * rhs.r + z * rhs.x - x * rhs.z;
            result.z = r * rhs.z + z * rhs.r + x * rhs.y - y * rhs.x;
            return result;
        }

        this_type& operator*=(const this_type& rhs)
        {
            return (*this = *this * rhs);
        }

        this_type operator/(element_type rhs) const
        {
            if(std::abs(rhs) <= std::numeric_limits<element_type>::epsilon())
                throw std::runtime_error("class \'quaternion<T>\' error: divide by zero.");
            this_type result;
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] / rhs;
            return result;
        }

        this_type& operator/=(element_type rhs)
        {
            return (*this = *this / rhs);
        }

        element_type& operator[](unsigned int index)
        {
            if(!(0 <= index && index < max_data_size))
                throw std::out_of_range("class \'quaternion<T>\' error: out of range.");
            return data[index];
        }

        const element_type& operator[](unsigned int index) const
        {
            if(!(0 <= index && index < max_data_size))
                throw std::out_of_range("class \'quaternion<T>\' error: out of range.");
            return data[index];
        } 

        operator element_type* () { return &data[0]; }
        operator const element_type* () const { return &data[0]; }
        const unsigned int Size() const { return max_data_size; }

    public:
        union
        {
            struct
            {
                element_type r;
                element_type x;
                element_type y;
                element_type z;
            };
            element_type data[max_data_size];
        };
    }; // class quaternion<T>

    template<typename T>
    quaternion<T> operator*(T lhs, const quaternion<T>& rhs)
    {
        quaternion<T> result;
        for(unsigned int i = 0; i < rhs.Size(); ++i)
            result.data[i] = lhs * rhs.data[i];
        return result;
    }
    
} // namespace orca
#endif // !_ORCA_QUATERNION_HPP_