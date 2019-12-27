/*************************/
/*  FILE NAME: mat4.hpp  */
/*************************/
#ifndef _ORCA_MAT4_HPP_
#define _ORCA_MAT4_HPP_

/**************/
/*  INCLUDES  */
/**************/
#include <cmath>
#include <limits>
#include <stdexcept>
#include "matrix.hpp"

namespace orca
{
    template<typename T> class mat2;
    template<typename T> class mat3;
}

namespace orca
{
    /**********************/
    /*  CLASS NAME: mat4  */
    /**********************/
    template<typename T>
    class mat4
    {
    private:
        using element_type = T;
        using this_type = mat4<element_type>;
        using vector_type = vec4<element_type>;
        static constexpr unsigned int max_row_size = 4;
        static constexpr unsigned int max_col_size = 4;

    public:
        mat4() { /* empty */ }

        mat4(const vector_type& v0, const vector_type& v1, const vector_type& v2, const vector_type& v3)
        {
            data[0] = v0;
            data[1] = v1;
            data[2] = v2;
            data[3] = v3;
        }
        
        mat4(const this_type& other)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = other.data[i][j];
        }

        explicit mat4(element_type num)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = num;
        }

        explicit mat4(const vector_type& vec)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = vec[j];
        }

        explicit mat4(const mat2<element_type>& mat)
        {
            for(unsigned int i = 0; i < mat.RowSize(); ++i)
                for(unsigned int j = 0; i < mat.ColSize(); ++j)
                    data[i][j] = mat.data[i][j];
        }

        explicit mat4(const mat3<element_type>& mat)
        {
            for(unsigned int i = 0; i < mat.RowSize(); ++i)
                for(unsigned int j = 0; i < mat.ColSize(); ++j)
                    data[i][j] = mat.data[i][j];
        }

    public:
        this_type& operator=(element_type rhs)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = rhs;
            return *this;
        }

        this_type& operator=(const vector_type& rhs)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = rhs[j];
            return *this;
        }
            
        this_type& operator=(const this_type& rhs)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = rhs.data[i][j];
            return *this;
        }

        this_type operator+(element_type rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] + rhs;
            return result;
        }

        this_type& operator+=(element_type rhs)
        {
            return (*this = *this + rhs);
        }

        this_type operator+(const vector_type& rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] + rhs[j];
            return result;
        }

        this_type& operator+=(const vector_type& rhs)
        {
            return (*this = *this + rhs);
        }

        this_type operator+(const this_type& rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] + rhs.data[i][j];
            return result;
        }

        this_type& operator+=(const this_type& rhs)
        {
            return (*this = *this + rhs);
        }

        this_type operator-() const
        {
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] * -1;
            return result;
        }

        this_type operator-(element_type rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] - rhs;
            return result;
        }

        this_type& operator-=(element_type rhs)
        {
            return (*this = *this - rhs);
        }

        this_type operator-(const vector_type& rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] - rhs[j];
            return result;
        }

        this_type& operator-=(const vector_type& rhs)
        {
            return (*this = *this - rhs);
        }

        this_type operator-(const this_type& rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] - rhs.data[i][j];
            return result;
        }

        this_type operator-=(const this_type& rhs)
        {
            return (*this = *this - rhs);
        }

        this_type operator*(element_type rhs) const
        {
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] * rhs;
            return result;
        }

        this_type& operator*=(element_type rhs)
        {
            return (*this = *this * rhs);
        }
        
        this_type operator*(const this_type& rhs) const
        {
            element_type sum;
            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
            {
                for(unsigned int j = 0; j < max_col_size; ++j)
                {
                    sum = static_cast<element_type>(0.0);
                    for(unsigned int k = 0; k < max_row_size; ++k)
                        sum += data[i][k] * rhs[k][j];
                    result[i][j] = sum;
                }
            }
            return result;
        }

        this_type& operator*=(const this_type& rhs)
        {
            return (*this = *this * rhs);
        }

        this_type operator/(element_type rhs) const
        {
            if(std::abs(rhs) <= std::numeric_limits<element_type>::epsilon())
                throw std::runtime_error("class \'mat2<T>\' error: divide by zero.");

            this_type result;
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    result.data[i][j] = data[i][j] / rhs;
            return result;
        }

        this_type& operator/=(element_type rhs)
        {
            return (*this = *this / rhs);
        }

        vector_type& operator[](unsigned int index)
        {
            if(!(0 <= index && index < max_row_size))
                throw std::out_of_range("class \'mat2<T>\' error: out of range.");
            return data[index];
        }

        const vector_type& operator[](unsigned int index) const
        {
            if(!(0 <= index && index < max_row_size))
                throw std::out_of_range("class \'mat2<T>\' error: out of range.");
            return data[index];
        }

        operator element_type* () { return &data[0][0]; }
        operator const element_type* () const { return &data[0][0]; }
        const unsigned int RowSize() const { return max_row_size; }
        const unsigned int ColSize() const { return max_col_size; }

    public:
        vector_type data[max_row_size];
    }; // class mat4<T>


    template<typename T>
    inline mat4<T> operator+(T lhs, const mat4<T>& rhs)
    {
        mat4<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = lhs + rhs[i][j];
        return result;
    }

    template<typename T>
    inline mat4<T> operator-(T lhs, const mat4<T>& rhs)
    {
        mat4<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = lhs - rhs[i][j];
        return result;
    }
    
    template<typename T>
    inline mat4<T> operator*(T lhs, const mat4<T>& rhs)
    {
        mat4<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = lhs * rhs[i][j];
        return result;
    }

    template<typename T>
    inline vec4<T> operator*(const vec4<T>& lhs, const mat4<T>& rhs)
    {
        T sum;
        vec4<T> result;
        for(unsigned int i = 0; i < result.Size(); ++i)
        {
            sum = static_cast<T>(0.0);
            for(unsigned int j = 0; j < result.Size(); ++j)
                sum += rhs[j][i] * lhs[j];
            result[i] = sum;
        }
        return result;
    }

} // namespace orca
#endif // !_ORCA_MAT4_HPP_