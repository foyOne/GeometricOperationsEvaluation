#pragma once

#include <vector>

class Math
{
public:

	template<class T = float>
	static void VectorMatrix3x3(std::vector<T>& vector, const std::vector<T>& matrix)
	{
		Math::VectorMatrix3x3(vector.data(), matrix);
	}

	template<class T = float>
	static void VectorMatrix3x3(T* vector, const std::vector<T>& matrix)
	{
		T x = 0, y = 0, z = 0;

		x = vector[0] * matrix[0] + vector[1] * matrix[3] + vector[2] * matrix[6];
		y = vector[0] * matrix[1] + vector[1] * matrix[4] + vector[2] * matrix[7];
		z = vector[0] * matrix[2] + vector[1] * matrix[5] + vector[2] * matrix[8];

		vector[0] = x;
		vector[1] = y;
		vector[2] = z;
	}

	template<class T = float>
	static void Transpose3x3(std::vector<T>& matrix)
	{
		std::swap(matrix[1], matrix[3]);
		std::swap(matrix[2], matrix[6]);
		std::swap(matrix[5], matrix[7]);
	}

	template<class T = float>
	static void Transpose3x3(T* matrix)
	{
		std::swap(matrix[1], matrix[3]);
		std::swap(matrix[2], matrix[6]);
		std::swap(matrix[5], matrix[7]);
	}
};