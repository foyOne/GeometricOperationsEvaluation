#include "StandardSTLTransformationExecutor.h"
#include "../TransformationView.h"

#include "../Result.h"

#include "../Math.h"
#include <omp.h>

Result StandardSTLTransformationExecutor::Transform(const STLModelPtr model)
{
    STLModelPtr copy = GetCopy(model);
    Result result(copy, 0.0);

    auto& vertices = copy->vertices;
    auto& normals = copy->normals;

    size_t vertexCount = vertices.size();
    size_t normalCount = normals.size();

    TransformationView viewer;

    double time = 0.0;

    for (const auto& t : _builder.GetTransformations())
    {
        auto m = viewer.AsMatrix(t);

        if (t->GetType() == TransformationType::Translation)
        {
            time = omp_get_wtime();
            for (size_t i = 0; i < vertexCount; i += 3)
            {
                vertices[i + 0] += m[0];
                vertices[i + 1] += m[1];
                vertices[i + 2] += m[2];
            }
            result.time += omp_get_wtime() - time;
        }
        else
        {
            Math::Transpose3x3(m);
            float x = 0, y = 0, z = 0;

            time = omp_get_wtime();
            for (size_t i = 0; i < vertexCount; i += 3)
            {
                Math::VectorMatrix3x3(&vertices[i], m);
                //x = vertices[i + 0] * m[0] + vertices[i + 1] * m[3] + vertices[i + 2] * m[6];
                //y = vertices[i + 0] * m[1] + vertices[i + 1] * m[4] + vertices[i + 2] * m[7];
                //z = vertices[i + 0] * m[2] + vertices[i + 1] * m[5] + vertices[i + 2] * m[8];

                //vertices[i + 0] = x;
                //vertices[i + 1] = y;
                //vertices[i + 2] = z;
            }
            result.time += omp_get_wtime() - time;


            if (t->GetType() != TransformationType::Scale)
            {
                time = omp_get_wtime();
                for (size_t i = 0; i < normalCount; i += 3)
                {
                    Math::VectorMatrix3x3(&normals[i], m);
                    //x = normals[i + 0] * m[0] + normals[i + 1] * m[3] + normals[i + 2] * m[6];
                    //y = normals[i + 0] * m[1] + normals[i + 1] * m[4] + normals[i + 2] * m[7];
                    //z = normals[i + 0] * m[2] + normals[i + 1] * m[5] + normals[i + 2] * m[8];

                    //normals[i + 0] = x;
                    //normals[i + 1] = y;
                    //normals[i + 2] = z;
                }
                result.time += omp_get_wtime() - time;
            }
        }
    }

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