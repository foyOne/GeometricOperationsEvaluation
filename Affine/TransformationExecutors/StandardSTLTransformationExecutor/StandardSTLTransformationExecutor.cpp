#include "StandardSTLTransformationExecutor.h"

#include <cmath>

#include "AffineMap.h"

void StandardSTLTransformationExecutor::Transform(std::vector<float>& vertices, std::vector<float>& normals, size_t trianglesCount, const AffineMap& map)
{
	float M[9];
	float T[3];
	map.Get(M, T);
	AffineMap::Transpose(M);

	size_t numVertices = 9 * trianglesCount;
	size_t numNormals = 3 * trianglesCount;

	float x = 0, y = 0, z = 0;
	for (size_t i = 0; i < numVertices; i += 3)
	{
		x = vertices[i] * M[0] + vertices[i + 1] * M[3] + vertices[i + 2] * M[6];
		y = vertices[i] * M[1] + vertices[i + 1] * M[4] + vertices[i + 2] * M[7];
		z = vertices[i] * M[2] + vertices[i + 1] * M[5] + vertices[i + 2] * M[8];

		vertices[i + 0] = x;
		vertices[i + 1] = y;
		vertices[i + 2] = z;
	}


	if (map.IsTranslation())
	{
		for (size_t i = 0; i < numVertices; i += 3)
		{
			vertices[i + 0] += T[0];
			vertices[i + 1] += T[1];
			vertices[i + 2] += T[2];
		}
	}


	if (map.IsReflection())
	{
		for (size_t i = 0; i < numVertices; i += 9)
		{
			std::swap(vertices[i + 0], vertices[i + 3]);
			std::swap(vertices[i + 1], vertices[i + 4]);
			std::swap(vertices[i + 2], vertices[i + 5]);
		}
	}


	for (size_t i = 0; i < numNormals; i += 3)
	{
		x = normals[i] * M[0] + normals[i + 1] * M[3] + normals[i + 2] * M[6];
		y = normals[i] * M[1] + normals[i + 1] * M[4] + normals[i + 2] * M[7];
		z = normals[i] * M[2] + normals[i + 1] * M[5] + normals[i + 2] * M[8];

		normals[i + 0] = x;
		normals[i + 1] = y;
		normals[i + 2] = z;
	}

	if (map.IsScale())
	{
		float magnitude = 0;
		for (size_t i = 0; i < numNormals; i += 3)
		{
			magnitude = std::sqrt(normals[i + 0] * normals[i + 0] + normals[i + 1] * normals[i + 1] + normals[i + 2] * normals[i + 2]);
			normals[i + 0] /= magnitude;
			normals[i + 1] /= magnitude;
			normals[i + 2] /= magnitude;
		}
	}
}