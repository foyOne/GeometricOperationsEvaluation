#pragma once

#include "../ITtransformationExecutor.h"

class MKLSTLTransformationExecutor : public ITtransformationExecutor
{
public:

	MKLSTLTransformationExecutor() = default;

	~MKLSTLTransformationExecutor() = default;

	void Transform(std::vector<float>& vertices, std::vector<float>& normals, size_t trianglesCount, const AffineMap& map) override;
};