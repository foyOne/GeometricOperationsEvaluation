#include "MKLSTLTransformationExecutor.h"

#include "../TransformationView.h"
#include "../Result.h"
#include "../Math.h"

#include <cmath>
#include <mkl.h>
#include <omp.h>
#include <algorithm>

void MKLSTLTransformationExecutor::Multiply(std::vector<float>& vectorSet, std::pair<size_t, size_t> shape, float affineTransformation[9])
{
    std::vector<float> transformedSet(shape.first * shape.second);

    cblas_sgemm(
        CBLAS_LAYOUT::CblasRowMajor,
        CBLAS_TRANSPOSE::CblasNoTrans,
        CBLAS_TRANSPOSE::CblasNoTrans,
        shape.first, shape.second, 3,
        1.0f,
        affineTransformation, 3,
        vectorSet.data(), shape.second,
        0.0f,
        transformedSet.data(), shape.second
    );
    vectorSet = transformedSet;
}

void MKLSTLTransformationExecutor::ReflectVectorSet(std::vector<float>& vectors, std::pair<size_t, size_t> shape, ReflectionPtr reflection)
{
    auto L_slarfx = LAPACKE_slarfx(
        LAPACK_ROW_MAJOR,
        'L',
        shape.first,
        shape.second,
        reflection->normal.data(),
        2,
        vectors.data(),
        shape.second,
        nullptr);
}

void MKLSTLTransformationExecutor::Reflect(Result result, ReflectionPtr reflection)
{
    auto vertexShape = result.model->GetVertexShape();
    auto normalShape = result.model->GetNormalShape();

    auto time = omp_get_wtime();
    ReflectVectorSet(result.model->vertices, vertexShape, reflection);
    ReflectVectorSet(result.model->normals, normalShape, reflection);
    result.time += omp_get_wtime() - time;
}


Result MKLSTLTransformationExecutor::Transform(const STLModelPtr model)
{
    STLModelPtr copy = std::make_shared<STLModel>(*model);
    copy->SetStorageType(StorageType::Width);

    Result result(copy, 0.0);

    auto& vertices = copy->vertices;
    auto& normals = copy->normals;

    size_t vertexCount = vertices.size();
    size_t normalCount = normals.size();

    TransformationView viewer;

    double time = 0.0;

    auto vertexShape = result.model->GetVertexShape();
    auto normalShape = result.model->GetNormalShape();

    for (const auto& t : _builder.GetTransformations())
    {
        auto m = viewer.AsMatrix(t);

        switch (t->GetType())
        {
            case TransformationType::Translation:
            {
                time = omp_get_wtime();

                std::for_each(
                    vertices.begin() + 0 * vertexShape.second,
                    vertices.begin() + 1 * vertexShape.second,
                    [m](float& value) { value += m[0]; }
                );

                std::for_each(
                    vertices.begin() + 1 * vertexShape.second,
                    vertices.begin() + 2 * vertexShape.second,
                    [m](float& value) { value += m[1]; }
                );

                std::for_each(
                    vertices.begin() + 2 * vertexShape.second,
                    vertices.begin() + 3 * vertexShape.second,
                    [m](float& value) { value += m[2]; }
                );

                result.time += omp_get_wtime() - time;
            }
            break;

            case TransformationType::Reflection:
            {
                Reflect(result, std::dynamic_pointer_cast<Reflection>(t));
            }
            break;

            case TransformationType::Scale:
            {
                time = omp_get_wtime();
                Multiply(result.model->vertices, vertexShape, m.data());
                result.time += omp_get_wtime() - time;
            }
            break;

            default:
            {
                time = omp_get_wtime();
                Multiply(result.model->vertices, vertexShape, m.data());
                Multiply(result.model->normals, normalShape, m.data());
                result.time += omp_get_wtime() - time;
            }
        }
    }

    result.model->SetStorageType(StorageType::Height);

    if (NeedToCorrectAfterReflection())
    {
        for (size_t i = 0; i < vertexCount; i += 9)
        {
            std::swap(vertices[i + 0], vertices[i + 3]);
            std::swap(vertices[i + 1], vertices[i + 4]);
            std::swap(vertices[i + 2], vertices[i + 5]);
        }
    }

    return result;
}
