#pragma once

#include "AffineTransformation.h"

#include <vector>

class TransformationBuilder
{
private:
    std::vector<TransformationPtr> _transformations;

public:

    const std::vector<TransformationPtr>& GetTransformations() const { return _transformations; }

    void Reset()
    {
        _transformations.clear();
    }

    TransformationBuilder& AddTranslation(const Translation& translation)
    {
        _transformations.push_back(std::make_shared<Translation>(translation));
        return *this;
    }

    TransformationBuilder& AddRotation(const Rotation& rotation)
    {
        _transformations.push_back(std::make_shared<Rotation>(rotation));
        return *this;
    }

    TransformationBuilder& AddReflection(const Reflection& reflection)
    {
        _transformations.push_back(std::make_shared<Reflection>(reflection));
        return *this;
    }

    TransformationBuilder& AddTransformation(const TransformationPtr& t)
    {
        _transformations.push_back(t);
        return *this;
    }
};