/*************************/
/*  FILE NAME: mat2.hpp  */
/*************************/
#ifndef _ORCA_MAT2_HPP_
#define _ORCA_MAT2_HPP_

/**************/
/*  INCLUDES  */
/**************/
#include <cmath>
#include <limits>
#include <stdexcept>
#include "matrix.hpp"

namespace orca
{
    template<typename T> class mat3;
    template<typename T> class mat4;
}

namespace orca
{
    /**********************/
    /*  CLASS NAME: mat2  */
    /**********************/
    template<typename T>
    class mat2
    {
    private:
        using element_type = T;
        using this_type = mat2<element_type>;
        using vector_type = vec2<element_type>;
        static constexpr unsigned int max_row_size = 2;
        static constexpr unsigned int max_col_size = 2;

    public:
        mat2() { /* empty */ }

        mat2(const vector_type& v0, const vector_type& v1)
        {
            data[0] = v0;
            data[1] = v1;
        }

        mat2(const this_type& other)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = other.data[i][j];
        }

        explicit mat2(element_type num)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = num;
        }

        explicit mat2(const vector_type& vec)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for(unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = vec[j];
        }

        explicit mat2(const mat3<element_type>& mat)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for (unsigned int j = 0; j < max_col_size; ++j)
                    data[i][j] = mat.data[i][j];
        }

        explicit mat2(const mat4<element_type>& mat)
        {
            for(unsigned int i = 0; i < max_row_size; ++i)
                for (unsigned int j = 0; j < max_col_size; ++j)
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
    }; // class mat2<T>


    template<typename T>
    inline mat2<T> operator+(T lhs, const mat2<T>& rhs)
    {
        mat2<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = lhs + rhs[i][j];
        return result;
    }

    template<typename T>
    inline mat2<T> operator-(T lhs, const mat2<T>& rhs)
    {
        mat2<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = lhs - rhs[i][j];
        return result;
    }
    
    template<typename T>
    inline mat2<T> operator*(T lhs, const mat2<T>& rhs)
    {
        mat2<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = lhs * rhs[i][j];
        return result;
    }

    template<typename T>
    inline vec2<T> operator*(const vec2<T>& lhs, const mat2<T>& rhs)
    {
        T sum;
        vec2<T> result;
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
#endif // !_ORCA_MAT2_HPP_