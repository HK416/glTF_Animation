/*************************/
/*  FILE NAME: vec4.hpp  */
/*************************/
#ifndef _ORCA_VEC4_HPP_
#define _ORCA_VEC4_HPP_

/**************/
/*  INCLUDES  */
/**************/
#include <stdexcept>
#include <limits>

#include "vector.hpp"

namespace orca
{
    template<typename T> class vec2;
    template<typename T> class vec3;
}

namespace orca
{
    /**********************/
    /*  CLASS NAME: vec4  */
    /**********************/
    template<typename T>
    class vec4
    {
    private:
        using element_type = T;
        using this_type = vec4<element_type>;
        static constexpr unsigned int max_data_size = 4;

    public:
        vec4() : x(static_cast<element_type>(0.0)), y(static_cast<element_type>(0.0)), z(static_cast<element_type>(0.0)), w(static_cast<element_type>(0.0)) {}
        vec4(element_type x, element_type y, element_type z, element_type w) : x(x), y(y), z(z), w(w) {}
        vec4(const vec2<element_type>& vec, element_type z, element_type w) : x(vec.x), y(vec.y), z(z), w(w) {}
        vec4(element_type x, const vec2<element_type>& vec, element_type w) : x(x), y(vec.x), z(vec.y), w(w) {}
        vec4(element_type x, element_type y, const vec2<element_type>& vec) : x(x), y(y), z(vec.x), w(vec.y) {}
        vec4(const vec2<element_type>& v0, const vec2<element_type>& v1) : x(v0.x), y(v0.y), z(v1.x), w(v1.y) {}
        vec4(const vec3<element_type>& vec, element_type w) : x(vec.x), y(vec.y), z(vec.z), w(w) {}
        vec4(element_type x, const vec3<element_type>& vec) : x(x), y(vec.x), z(vec.y), w(vec.z) {}
        vec4(const this_type& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
        explicit vec4(element_type num) : x(num), y(num), z(num), w(num) {}
        explicit vec4(const vec2<element_type>& vec) : x(vec.x), y(vec.y), z(static_cast<element_type>(0.0)), w(static_cast<element_type>(0.0)) {}
        explicit vec4(const vec3<element_type>& vec) : x(vec.x), y(vec.y), z(vec.z), w(static_cast<element_type>(0.0)) {}

    public:
        this_type& operator=(element_type rhs)
        {
            for(unsigned int i = 0; i < max_data_size; ++i)
                data[i] = rhs;
            return *this;
        }

        this_type& operator=(const this_type& rhs)
        {
            for(unsigned int i = 0; i < max_data_size; ++i)
                data[i] = rhs.data[i];
            return *this;
        }

        this_type& operator=(const vec2<element_type>& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = static_cast<element_type>(0.0);
            w = static_cast<element_type>(0.0);
            return *this;
        }

        this_type& operator=(const vec3<element_type>& rhs)
        {
            x = rhs.x;
            y = rhs.y;
            z = rhs.z;
            w = static_cast<element_type>(0.0);
            return *this;
        }

        this_type operator+(element_type rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] + rhs;
            return result;
        }

        this_type& operator+=(element_type rhs)
        {
            return (*this = *this + rhs);
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

        this_type operator-(element_type rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] - rhs;
            return result;
        }

        this_type& operator-=(element_type rhs)
        {
            return (*this = *this - rhs);
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
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] * rhs.data[i];
            return result;
        }

        this_type& operator*=(const this_type& rhs)
        {
            return (*this = *this * rhs);
        }

        this_type operator/(element_type rhs) const
        {
            this_type result;
            if(std::abs(rhs) <= std::numeric_limits<element_type>::epsilon())
                throw std::runtime_error("class \'orca::vec4<T>\' divide by zero.");
            for(unsigned int i = 0; i < max_data_size; ++i)
                result.data[i] = data[i] / rhs;
            return result;
        }

        this_type& operator/=(element_type rhs)
        {
            return (*this = *this / rhs);
        }

        this_type operator/(const this_type& rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_data_size; ++i)
            {   
                if(std::abs(rhs.data[i]) <= std::numeric_limits<element_type>::epsilon())
                    throw std::runtime_error("class \'orca::vec4<T>\' error: divide by zero.");
                result.data[i] = data[i] / rhs.data[i];
            }
            return result;
        }

        this_type& operator/=(const this_type& rhs)
        {
            return (*this = *this / rhs);
        }

        element_type operator[](unsigned int index) const
        {
            if(!(0 <= index && index < max_data_size))
                throw std::out_of_range("class \'orca::vec4<T>\' error: out of range.");
            return data[index];
        }

        element_type& operator[](unsigned int index)
        {
            if(!(0 <= index && index < max_data_size))
                throw std::out_of_range("class \'orca::vec4<T>\' error: out of range.");
            return data[index];
        }

        operator element_type* () { return &data[0]; }
        operator const element_type* () const {return &data[0]; }
        const unsigned int Size() const { return max_data_size; }

    public:
        union 
        {
            struct
            {
                element_type x;
                element_type y;
                element_type z;
                element_type w;
            };
            element_type data[max_data_size];
        };
        
    }; // class vec4<T>



    template<typename T>
    vec4<T> operator+(T lhs, const vec4<T>& rhs)
    {
        vec4<T> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = lhs + rhs.data[i];
        return result;
    }

    template<typename T>
    vec4<T> operator-(T lhs, const vec4<T>& rhs)
    {
        vec4<T> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = lhs - rhs.data[i];
        return result;
    }

    template<typename T>
    vec4<T> operator*(T lhs, const vec4<T>& rhs)
    {
        vec4<T> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
            result.data[i] = lhs * rhs.data[i];
        return result;
    }

    template<typename T>
    vec4<T> operator/(T lhs, const vec4<T>& rhs)
    {
        vec4<T> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
        {
            if(std::abs(rhs.data[i]) <= std::numeric_limits<T>::epsilon())
                throw std::runtime_error("class \'vec4<T>\' error: divide by zero.");

            result.data[i] = lhs / rhs.data[i];
        }
        return result;
    }

} // namespace orca
#endif // !_ORCA_VEC4_HPP_