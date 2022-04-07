#pragma once

#include <vector>

#include "AffineMap.h"

class ITtransformationExecutor
{
public:
	virtual void Transform(std::vector<float>& vertices, std::vector<float>& normals, size_t trianglesCount, const AffineMap& map) = 0;

	virtual ~ITtransformationExecutor() = default;
};