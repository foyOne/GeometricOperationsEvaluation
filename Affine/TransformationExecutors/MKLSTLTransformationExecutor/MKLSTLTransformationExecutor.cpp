#include "MKLSTLTransformationExecutor.h"

#include <cmath>

#include "AffineMap.h"

void MKLSTLTransformationExecutor::Transform(std::vector<float>& vertices, std::vector<float>& normals, size_t trianglesCount, const AffineMap& map)
{
	float M[9];
	float T[3];
	map.Get(M, T);
	AffineMap::Transpose(M);

	size_t numVertices = 9 * trianglesCount;
	size_t numNormals = 3 * trianglesCount;
}