#pragma once

#include "../TransformationExecutor.h"

#include "../TransformationBuilder.h"

class StandardSTLTransformationExecutor;
using StandardSTLTransformationExecutorPtr = std::shared_ptr<StandardSTLTransformationExecutor>;

class StandardSTLTransformationExecutor : public TransformationExecutor
{
public:
    StandardSTLTransformationExecutor(const TransformationBuilder& builder)
    {
        SetBuilder(builder);
    }

    void SetBuilder(const TransformationBuilder& builder)
    {
        this->_builder = builder;
    }

    Result Transform(const STLModelPtr model) override;

private:

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

    STLModelPtr GetCopy(const STLModelPtr model)
    {
        STLModelPtr copied(new STLModel());
        copied->vertices = model->vertices;
        copied->normals = model->normals;

        return copied;
    }

private:
    TransformationBuilder _builder;
};