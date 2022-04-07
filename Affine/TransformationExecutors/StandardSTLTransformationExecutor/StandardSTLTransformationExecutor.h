#pragma once

#include "../ITtransformationExecutor.h"

class StandardSTLTransformationExecutor : public ITtransformationExecutor
{
public:

	StandardSTLTransformationExecutor() = default;

	~StandardSTLTransformationExecutor() = default;

	void Transform(std::vector<float>& vertices, std::vector<float>& normals, size_t trianglesCount, const AffineMap& map) override;
};