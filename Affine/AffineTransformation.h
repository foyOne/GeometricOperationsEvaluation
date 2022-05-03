#pragma once

#include <array>
#include <memory>

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>



class Transformation;
class Translation;
class Rotation;
class Reflection;
class GeneralTransformation;

using TransformationPtr = std::shared_ptr<Transformation>;
using TranslationPtr = std::shared_ptr<Translation>;
using RotationPtr = std::shared_ptr<Rotation>;
using ReflectionPtr = std::shared_ptr<Reflection>;
using GeneralTransformationPtr = std::shared_ptr<GeneralTransformation>;

enum class TransformationType;

class Transformation
{
public:
    virtual ~Transformation() {}

    virtual TransformationType GetType() const = 0;
};

enum class TransformationType
{
    Translation,
    Scale,
    Rotation,
    Reflection,
    General,
};

class Translation : public Transformation
{
public:
    Translation(float x, float y, float z) : vector({ x, y, z }) {}

    Translation(const std::array<float, 3>& vector) : vector(vector) {}

    Translation(const Translation& other)
    {
        vector = other.vector;
    }

    TransformationType GetType() const override
    {
        return TransformationType::Translation;
    }

public:
    std::array<float, 3> vector;
};


class Scale : public Transformation
{
public:
    Scale(float x, float y, float z) : scale({ x, y, z }) {}

    Scale(const std::array<float, 3>& scale) : scale(scale) {}

    Scale(const Scale& other)
    {
        scale = other.scale;
    }

    TransformationType GetType() const override
    {
        return TransformationType::Scale;
    }

public:
    std::array<float, 3> scale;
};

class Rotation : public Transformation
{
public:

    Rotation(const std::array<float, 3>& axis, float angleInDegrees)
    {
        angle = static_cast<float>(M_PI) * angleInDegrees / 180.0f;

        float x = axis[0];
        float y = axis[1];
        float z = axis[2];

        float m = std::sqrt(x * x + y * y + z * z);

        if (m > 1e-5f)
        {
            x /= m;
            y /= m;
            z /= m;

            this->axis = { x, y, z };
        }
    }

    Rotation(const Rotation& other)
    {
        axis = other.axis;
        angle = other.angle;
    }

    TransformationType GetType() const override
    {
        return TransformationType::Rotation;
    }

public:
    std::array<float, 3> axis;
    float angle;
};

class Reflection : public Transformation
{
public:

    Reflection(const std::array<float, 3>& normal)
    {
        Reflection(normal[0], normal[1], normal[2]);
    }

    Reflection(float x, float y, float z)
    {
        float m = std::sqrt(x * x + y * y + z * z);

        if (m > 1e-5f)
        {
            x /= m;
            y /= m;
            z /= m;

            this->normal = { x, y, z };
        }
    }

    Reflection(const Reflection& other)
    {
        normal = other.normal;
    }

    TransformationType GetType() const override
    {
        return TransformationType::Reflection;
    }

public:
    std::array<float, 3> normal;
};

class GeneralTransformation : public Transformation
{
public:

    GeneralTransformation()
    {
        data =
        {
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
        };
    }

    GeneralTransformation(const std::array<float, 9>& data) : data(data) {}

    GeneralTransformation(const GeneralTransformation& other)
    {
        data = other.data;
    }

    TransformationType GetType() const override
    {
        return TransformationType::General;
    }

public:
    std::array<float, 9> data;
};