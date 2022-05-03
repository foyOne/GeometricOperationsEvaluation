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

    void Convolve() {};

    TransformationBuilder& AddTranslation(const Translation& translation)
    {
        TransformationPtr toAdd(new Translation(translation));
        _transformations.push_back(toAdd);
        return *this;
    }

    TransformationBuilder& AddRotation(const Rotation& rotation)
    {
        TransformationPtr toAdd(new Rotation(rotation));
        _transformations.push_back(toAdd);
        return *this;
    }

    TransformationBuilder& AddReflection(const Reflection& reflection)
    {
        TransformationPtr toAdd(new Reflection(reflection));
        _transformations.push_back(toAdd);
        return *this;
    }

    TransformationBuilder& AddTransformation(const TransformationPtr& t)
    {
        _transformations.push_back(t);
        return *this;
    }
};