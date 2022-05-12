#pragma once

#include "../TransformationExecutor.h"

#include "../TransformationBuilder.h"

#include <array>

class MKLSTLTransformationExecutor : public TransformationExecutor
{
public:

	~MKLSTLTransformationExecutor() = default;

    MKLSTLTransformationExecutor(const TransformationBuilder& builder)
    {
        SetBuilder(builder);
    }

    void SetBuilder(const TransformationBuilder& builder)
    {
        this->_builder = builder;
    }

	Result Transform(const STLModelPtr model) override;

private:

    // Only for 3x3 matrix * 3xN matrix, N - vector count
    void Multiply(std::vector<float>& vectorSet, std::pair<size_t, size_t> shape, float affineTransformation[9]);

    void Reflect(Result result, ReflectionPtr reflection);
    void ReflectVectorSet(std::vector<float>& vectors, std::pair<size_t, size_t> shape, ReflectionPtr reflection);

    bool NeedToCorrectAfterReflection()
    {
        size_t reflectionCount = 0;
        for (const auto& t : _builder.GetTransformations())
        {
            if (t->GetType() == TransformationType::Reflection)
                reflectionCount++;
        }

        return reflectionCount % 2 != 0;
    }

private:
    TransformationBuilder _builder;
};