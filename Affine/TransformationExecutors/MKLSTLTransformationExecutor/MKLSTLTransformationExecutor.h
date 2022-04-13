#pragma once

#include "../ITtransformationExecutor.h"

#include <array>

class MKLSTLTransformationExecutor : public ITtransformationExecutor
{
public:

	MKLSTLTransformationExecutor() = default;

	~MKLSTLTransformationExecutor() = default;

	void Transform(std::vector<float>& vertices, std::vector<float>& normals, size_t trianglesCount, const AffineMap& map) override;

private:

	void Multiply(std::vector<float>& a, float affineMap[9], std::vector<float>& c, size_t rows);
};