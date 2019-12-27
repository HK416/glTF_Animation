/*************************************/
/*  FILE NAME: matrix_functions.hpp  */
/*************************************/
#ifndef _ORCA_MATRIX_FUNCTIONS_HPP_
#define _ORCA_MATRIX_FUNCTIONS_HPP_

/**************/
/*  INCLUDES  */
/**************/
#include <cmath>
#include <limits>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include "vector.hpp"
#include "vector_functions.hpp"
#include "matrix.hpp"

namespace orca
{
	/* print mat2<T> data */
	template<typename T>
	inline void PrintMatrix(const mat2<T>& mat)
	{
		for(unsigned int i = 0; i < mat.RowSize(); ++i)
		{
			for(unsigned int j = 0; j < mat.ColSize(); ++j)
				std::cout << mat[i][j] << " ";
			std::cout << std::endl;
		}
	}
	
	/* print mat3<T> data */
	template<typename T>
	inline void PrintMatrix(const mat3<T>& mat)
	{
		for(unsigned int i = 0; i < mat.RowSize(); ++i)
		{
			for(unsigned int j = 0; j < mat.ColSize(); ++j)
				std::cout << mat[i][j] << " ";
			std::cout << std::endl;
		}
	}
	
	/* print mat4<T> data */
	template<typename T>
	inline void PrintMatrix(const mat4<T>& mat)
	{
		for(unsigned int i = 0; i < mat.RowSize(); ++i)
		{
			for(unsigned int j = 0; j < mat.ColSize(); ++j)
				std::cout << mat[i][j] << " ";
			std::cout << std::endl;
		}
	}

	/* make mat2<T> type */
	template<typename T>
	inline orca::mat2<T> MakeMatrix2X2(const T* data)
	{
		orca::mat2<T> result;
		for(unsigned int i = 0; i < result.RowSize(); ++i)
			for(unsigned int j = 0; j < result.ColSize(); ++j)
				result.data[i][j] = data[2 * i + j];
		return result;
	}

	/* make mat2<T> type */
	template<typename Out, typename In>
	inline orca::mat2<Out> MakeMatrix2X2(const In* data)
	{
		orca::mat2<Out> result;
		for(unsigned int i = 0; i < result.RowSize(); ++i)
			for(unsigned int j = 0; j < result.ColSize(); ++j)
				result.data[i][j] = static_cast<Out>(data[2 * i + j]);
		return result;
	}

	/* make mat3<T> type */
	template<typename T>
	inline orca::mat3<T> MakeMatrix3X3(const T* data)
	{
		orca::mat3<T> result;
		for(unsigned int i = 0; i < result.RowSize(); ++i)
			for(unsigned int j = 0; j < result.ColSize(); ++j)
				result.data[i][j] = data[3 * i + j];
		return result;
	}
	
	/* make mat3<T> type */
	template<typename Out, typename In>
	inline orca::mat3<Out> MakeMatrix3X3(const In* data)
	{
		orca::mat3<Out> result;
		for(unsigned int i = 0; i < result.RowSize(); ++i)
			for(unsigned int j = 0; j < result.ColSize(); ++j)
				result.data[i][j] = static_cast<Out>(data[3 * i + j]);
		return result;
	}
	
	/* make mat4<T> type */
	template<typename T>
	inline orca::mat4<T> MakeMatrix4X4(const T* data)
	{
		orca::mat4<T> result;
		for(unsigned int i = 0; i < result.RowSize(); ++i)
			for(unsigned int j = 0; j < result.ColSize(); ++j)
				result.data[i][j] = data[4 * i + j];
		return result;
	}
	
	/* make mat4<T> type */
	template<typename Out, typename In>
	inline orca::mat4<Out> MakeMatrix4X4(const In* data)
	{
		orca::mat4<Out> result;
		for(unsigned int i = 0; i < result.RowSize(); ++i)
			for(unsigned int j = 0; j < result.ColSize(); ++j)
				result.data[i][j] = static_cast<Out>(data[4 * i + j]);
		return result;
	}

    /* return identity mat2<T> */
    template<typename T>
    inline mat2<T> Identity(const mat2<T>& mat)
    {
        mat2<T> result(0);
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            result[i][i] = static_cast<T>(1.0);
        return result;
    }
    
    /* return identity mat3<T> */
    template<typename T>
    inline mat3<T> Identity(const mat3<T>& mat)
    {
        mat3<T> result(0);
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            result[i][i] = static_cast<T>(1.0);
        return result;
    }
    
    /* return identity mat4<T> */
    template<typename T>
    inline mat4<T> Identity(const mat4<T>& mat)
    {
        mat4<T> result(0);
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            result[i][i] = static_cast<T>(1.0);
        return result;
    }

    /* return transpose mat2<T> */
    template<typename T>
    inline mat2<T> Transpose(const mat2<T>& mat)
    {
        mat2<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = mat[j][i];
        return result;
    }

    /* return transpose ma32<T> */
    template<typename T>
    inline mat3<T> Transpose(const mat3<T>& mat)
    {
        mat3<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = mat[j][i];
        return result;
    }
    
    /* return transpose mat4<T> */
    template<typename T>
    inline mat4<T> Transpose(const mat4<T>& mat)
    {
        mat4<T> result;
        for(unsigned int i = 0; i < result.RowSize(); ++i)
            for(unsigned int j = 0; j < result.ColSize(); ++j)
                result[i][j] = mat[j][i];
        return result;
    }

    /* return translate matrix */
    template<typename T>
    inline mat4<T> Translate(T x, T y, T z)
    {
        mat4<T> result;
		result[0][0] = static_cast<T>(1.0);
		result[0][1] = static_cast<T>(0.0);
		result[0][2] = static_cast<T>(0.0);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(0.0);
		result[1][1] = static_cast<T>(1.0);
		result[1][2] = static_cast<T>(0.0);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(0.0);
		result[2][1] = static_cast<T>(0.0);
		result[2][2] = static_cast<T>(1.0);
		result[2][3] = static_cast<T>(0.0);

		result[3][0] = x;
		result[3][1] = y;
		result[3][2] = z;
		result[3][3] = static_cast<T>(1.0);
        return result;
    }
    
    /* return translate matrix */
    template<typename T>
    inline mat4<T> Translate(const vec3<T>& vec)
    {
        mat4<T> result;
		result[0][0] = static_cast<T>(1.0);
		result[0][1] = static_cast<T>(0.0);
		result[0][2] = static_cast<T>(0.0);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(0.0);
		result[1][1] = static_cast<T>(1.0);
		result[1][2] = static_cast<T>(0.0);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(0.0);
		result[2][1] = static_cast<T>(0.0);
		result[2][2] = static_cast<T>(1.0);
		result[2][3] = static_cast<T>(0.0);

		result[3][0] = vec[0];
		result[3][1] = vec[1];
		result[3][2] = vec[2];
		result[3][3] = static_cast<T>(1.0);
        return result;
    }

    /* return scale matrix */
    template<typename T>
    inline mat4<T> Scale(T x, T y, T z)
    {
        mat4<T> result;
		result[0][0] = x;
		result[0][1] = static_cast<T>(0.0);
		result[0][2] = static_cast<T>(0.0);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(0.0);
		result[1][1] = y;
		result[1][2] = static_cast<T>(0.0);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(0.0);
		result[2][1] = static_cast<T>(0.0);
		result[2][2] = z;
		result[2][3] = static_cast<T>(0.0);

		result[3][0] = static_cast<T>(0.0);
		result[3][1] = static_cast<T>(0.0);
		result[3][2] = static_cast<T>(0.0);
		result[3][3] = static_cast<T>(1.0);
        return result;
    }
    
    /* return scale matrix */
    template<typename T>
    inline mat4<T> Scale(const vec3<T>& vec)
    {
        mat4<T> result;
		result[0][0] = vec[0];
		result[0][1] = static_cast<T>(0.0);
		result[0][2] = static_cast<T>(0.0);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(0.0);
		result[1][1] = vec[1];
		result[1][2] = static_cast<T>(0.0);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(0.0);
		result[2][1] = static_cast<T>(0.0);
		result[2][2] = vec[2];
		result[2][3] = static_cast<T>(0.0);

		result[3][0] = static_cast<T>(0.0);
		result[3][1] = static_cast<T>(0.0);
		result[3][2] = static_cast<T>(0.0);
		result[3][3] = static_cast<T>(1.0);
        return result;
    }

    /* return look at matrix */
    template<typename T>
    inline mat4<T> LookAtLH(const vec3<T>& pos, const vec3<T>& target, const vec3<T>& up)
    {
        const vec3<T> n = Normalize(target - pos);
        const vec3<T> u = Normalize(Cross(up, n));
        const vec3<T> v = Cross(n, u);

        mat4<T> result;
		result[0][0] = u[0];
		result[0][1] = v[0];
		result[0][2] = n[0];
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = u[1];
		result[1][1] = v[1];
		result[1][2] = n[1];
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = u[2];
		result[2][1] = v[2];
		result[2][2] = n[2];
		result[2][3] = static_cast<T>(0.0);

		result[3][0] = -Dot(pos, u);
		result[3][1] = -Dot(pos, v);
		result[3][2] = -Dot(pos, n);
		result[3][3] = static_cast<T>(1.0);
        return result;
    }

    /* return look at matrix */
    template<typename T>
    inline mat4<T> LookAtRH(const vec3<T>& pos, const vec3<T>& target, const vec3<T>& up)
    {
        const vec3<T> n = Normalize(target - pos);
        const vec3<T> u = Normalize(Cross(up, n));
        const vec3<T> v = Cross(n, u);

        mat4<T> result;
		result[0][0] = u[0];
		result[0][1] = v[0];
		result[0][2] = -n[0];
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = u[1];
		result[1][1] = v[1];
		result[1][2] = -n[1];
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = u[2];
		result[2][1] = v[2];
		result[2][2] = -n[2];
		result[2][3] = static_cast<T>(0.0);

		result[3][0] = -Dot(pos, u);
		result[3][1] = -Dot(pos, v);
		result[3][2] = Dot(pos, n);
		result[3][3] = static_cast<T>(1.0);
        return result;
    }

    /* return perspective matrix */
    template<typename T>
    inline mat4<T> PerspectiveLH(T fovy_rad, T aspect, T n, T f)
    {
        if(std::abs(aspect) <= std::numeric_limits<T>::epsilon())
            throw std::invalid_argument("function \'PerspectiveLH\' error: screen aspect is zero.");

        const T tan_half_fovy = std::tan(fovy_rad / static_cast<T>(2.0));

        mat4<T> result;
		result[0][0] = static_cast<T>(1.0) / (aspect * tan_half_fovy);
		result[0][1] = static_cast<T>(0.0);
		result[0][2] = static_cast<T>(0.0);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(0.0);
		result[1][1] = static_cast<T>(1.0) / tan_half_fovy;
		result[1][2] = static_cast<T>(0.0);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(0.0);
		result[2][1] = static_cast<T>(0.0);
		result[2][2] = f / (f - n);
		result[2][3] = static_cast<T>(1.0);

		result[3][0] = static_cast<T>(0.0);
		result[3][1] = static_cast<T>(0.0);
		result[3][2] = - (f * n) / (f - n);
		result[3][3] = static_cast<T>(0.0);
        return result;
    }
    
    /* return perspective matrix */
    template<typename T>
    inline mat4<T> PerspectiveRH(T fovy_rad, T aspect, T n, T f)
    {
        if(std::abs(aspect) <= std::numeric_limits<T>::epsilon())
            throw std::invalid_argument("function \'PerspectiveRH\' error: screen aspect is zero.");

        const T tan_half_fovy = std::tan(fovy_rad / static_cast<T>(2.0));

        mat4<T> result;
		result[0][0] = static_cast<T>(1.0) / (aspect * tan_half_fovy);
		result[0][1] = static_cast<T>(0.0);
		result[0][2] = static_cast<T>(0.0);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(0.0);
		result[1][1] = static_cast<T>(1.0) / tan_half_fovy;
		result[1][2] = static_cast<T>(0.0);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(0.0);
		result[2][1] = static_cast<T>(0.0);
		result[2][2] = f / (n - f);
		result[2][3] = - static_cast<T>(1.0);

		result[3][0] = static_cast<T>(0.0);
		result[3][1] = static_cast<T>(0.0);
		result[3][2] = - (f * n) / (f - n);
		result[3][3] = static_cast<T>(0.0);
        return result;
    }

    /* return ortho matrix */
    template<typename T>
    inline mat4<T> OrthoLH(T left, T right, T bottom, T top, T n, T f)
    {
        mat4<T> result;
		result[0][0] = static_cast<T>(2.0) / (right - left);
		result[0][1] = static_cast<T>(0.0);
		result[0][2] = static_cast<T>(0.0);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(0.0);
		result[1][1] = static_cast<T>(2.0) / (top - bottom);
		result[1][2] = static_cast<T>(0.0);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(0.0);
		result[2][1] = static_cast<T>(0.0);
		result[2][2] = static_cast<T>(1.0) / (f - n);
		result[2][3] = static_cast<T>(0.0);

		result[3][0] = - (right + left) / (right - left);
		result[3][1] = - (top + bottom) / (top - bottom);
		result[3][2] = - n / (f - n);
		result[3][3] = static_cast<T>(1.0);
        return result;
    }
    
    /* return ortho matrix */
    template<typename T>
    inline mat4<T> OrthoRH(T left, T right, T bottom, T top, T n, T f)
    {
        mat4<T> result;
		result[0][0] = static_cast<T>(2.0) / (right - left);
		result[0][1] = static_cast<T>(0.0);
		result[0][2] = static_cast<T>(0.0);
		result[0][3] = static_cast<T>(0.0);

		result[1][0] = static_cast<T>(0.0);
		result[1][1] = static_cast<T>(2.0) / (top - bottom);
		result[1][2] = static_cast<T>(0.0);
		result[1][3] = static_cast<T>(0.0);

		result[2][0] = static_cast<T>(0.0);
		result[2][1] = static_cast<T>(0.0);
		result[2][2] = - static_cast<T>(1.0) / (f - n);
		result[2][3] = static_cast<T>(0.0);

		result[3][0] = - (right + left) / (right - left);
		result[3][1] = - (top + bottom) / (top - bottom);
		result[3][2] = - n / (f - n);
		result[3][3] = static_cast<T>(1.0);
        return result;
    }

	/* return cofactor matrix */
	template<typename T>
	inline mat2<T> Cofactor(const mat3<T>& mat, unsigned int p, unsigned int q)
	{
		if(!(0 <= p && p < mat.RowSize()) || !(0 <= q && q < mat.ColSize()))
			throw std::out_of_range("function \'Cofactor\' error: out of range.");

		orca::mat2<T> result;
		unsigned int i = 0, j = 0;

		for(unsigned int row = 0; row < mat.RowSize(); ++row)
		{
			for(unsigned int col = 0; col < mat.ColSize(); ++col)
			{
				if(row != p && col != q)
				{
					result[i][j++] = mat[row][col];

					if(j == mat.ColSize() - 1)
					{
						j = 0;
						++i;
					}
				}
			}
		}	
		return result;
	}

	/* return cofactor matrix */
	template<typename T>
	inline mat3<T> Cofactor(const mat4<T>& mat, unsigned int p, unsigned int q)
	{
		if(!(0 <= p && p < mat.RowSize()) || !(0 <= q && q < mat.ColSize()))
			throw std::out_of_range("function \'Cofactor\' error: out of range.");

		orca::mat3<T> result;
		unsigned int i = 0, j = 0;

		for(unsigned int row = 0; row < mat.RowSize(); ++row)
		{
			for(unsigned int col = 0; col < mat.ColSize(); ++col)
			{
				if(row != p && col != q)
				{
					result[i][j++] = mat[row][col];

					if(j == mat.ColSize() - 1)
					{
						j = 0;
						++i;
					}
				}
			}
		}	
		return result;
	}

	/* return adjoint matrix of matrix */
	template<typename T>
	inline mat2<T> Adjoint(const mat2<T>& mat)
	{
		orca::mat2<T> result;
		result[0][0] = mat[1][1];
		result[0][1] = -mat[0][1];
		result[1][0] = -mat[1][0];
		result[1][1] = mat[0][0];
		return result;
	}

	/* return adjoint matrix of matrix */
	template<typename T>
	inline mat3<T> Adjoint(const mat3<T>& mat)
	{
		int sign;
		orca::mat3<T> result;
		for(unsigned int i = 0; i < result.RowSize(); ++i)
		{
			for(unsigned int j = 0; j < result.ColSize(); ++j)
			{
				sign = ((i + j) % 2 == 0) ? 1 : -1;
				result[j][i] = sign * Determinant(Cofactor(mat, i, j));
			}
		}
		return result;
	}

	/* return adjoint matrix of matrix */
	template<typename T>
	inline mat4<T> Adjoint(const mat4<T>& mat)
	{
		int sign;
		orca::mat4<T> result;
		for(unsigned int i = 0; i < result.RowSize(); ++i)
		{
			for(unsigned int j = 0; j < result.ColSize(); ++j)
			{
				sign = ((i + j) % 2 == 0) ? 1 : -1;
				result[j][i] = sign * Determinant(Cofactor(mat, i, j));
			}
		}
		return result;
	}

	/* return determinent of matrix */
	template<typename T>
	inline T Determinant(const mat2<T>& mat)
	{
		return (mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]);
	}

	/* return determinant of matrix */
	template<typename T>
	inline T Determinant(const mat3<T>& mat)
	{
		return (mat[0][0] * Determinant(Cofactor(mat, 0, 0))) 
				- (mat[0][1] * Determinant(Cofactor(mat, 0, 1))) 
				+ (mat[0][2] * Determinant(Cofactor(mat, 0, 2)));
	}

	/* return determinant of matrix */
	template<typename T>
	inline T Determinant(const mat4<T>& mat)
	{
		return (mat[0][0] * Determinant(Cofactor(mat, 0, 0))) 
				- (mat[0][1] * Determinant(Cofactor(mat, 0, 1))) 
				+ (mat[0][2] * Determinant(Cofactor(mat, 0, 2)))
				- (mat[0][3] * Determinant(Cofactor(mat, 0, 3)));
	}

	/* return inverse matrix of matrix */
	template<typename T>
	inline mat2<T> Inverse(const mat2<T>& mat)
	{
		T det = Determinant(mat);
		if(std::abs(det) <= std::numeric_limits<T>::epsilon())
			throw std::runtime_error("function \'Inverse\' error: determinant of matrix is zero.");

		return Adjoint(mat) / det;
	} 

	/* return inverse matrix of matrix */
	template<typename T>
	inline mat3<T> Inverse(const mat3<T>& mat)
	{
		T det = Determinant(mat);
		if(std::abs(det) <= std::numeric_limits<T>::epsilon())
			throw std::runtime_error("function \'Inverse\' error: determinant of matrix is zero.");
		
		return Adjoint(mat) / det;
	}

	/* return inverse matrix of matrix */
	template<typename T>
	inline mat4<T> Inverse(const mat4<T>& mat)
	{
		T det = Determinant(mat);
		if(std::abs(det) <= std::numeric_limits<T>::epsilon())
			throw std::runtime_error("function \'Inverse\' error: determinant of matrix is zero.");
		
		return Adjoint(mat) / det;
	}

} // namespace orca
#endif // !_ORCA_MATRIX_FUNCTIONS_HPP_