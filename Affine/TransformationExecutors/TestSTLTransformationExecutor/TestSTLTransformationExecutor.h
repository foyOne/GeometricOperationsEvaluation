#pragma once

#pragma once

#include "../TransformationExecutor.h"

#include "../TransformationBuilder.h"

#include <array>

class TestSTLTransformationExecutor : public TransformationExecutor
{
public:

    ~TestSTLTransformationExecutor() = default;

    TestSTLTransformationExecutor(const TransformationBuilder& builder)
    {
        SetBuilder(builder);
    }

    void SetBuilder(const TransformationBuilder& builder)
    {
        this->_builder = builder;
    }

    Result Transform(const STLModelPtr model) override;

private:

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