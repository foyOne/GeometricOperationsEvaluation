#pragma once

#include <vector>
#include "AffineTransformation.h"

namespace RawTransformation
{
	template<class T>
	double A_Nx3_StandardReflection(std::vector<T>& vectors, ReflectionPtr reflection);

	double S_3xN_MKLReflection(std::vector<float>& vectors, ReflectionPtr reflection);

	double S_3xN_HessenbergReflection(std::vector<float>& vectors, ReflectionPtr reflection);

	double D_Nx3_VectorizedStandardReflection(std::vector<double>& vectors, ReflectionPtr reflection);

	double D_Nx3_VectorizedReflection_V1(std::vector<double>& vectors, ReflectionPtr reflection);
}