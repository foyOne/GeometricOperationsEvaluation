//#include "MKLSTLTransformationExecutor.h"
//
//#include <cmath>
//#include <mkl.h>
//
//#include "AffineMap.h"
//
//void MKLSTLTransformationExecutor::Transform(std::vector<float>& vertices, std::vector<float>& normals, size_t trianglesCount, const AffineMap& map)
//{
//	float M[9];
//	float T[3];
//	map.Get(M, T);
//	AffineMap::Transpose(M);
//
//	size_t numCoords = 9 * trianglesCount;
//	size_t numVertices = 3 * trianglesCount;
//	size_t numNormals = 3 * trianglesCount;
//
//	std::vector<float> originalVertices = vertices;
//	std::vector<float> originalNormals = normals;
//
//	Multiply(originalVertices, M, vertices, numVertices);
//	Multiply(originalNormals, M, normals, trianglesCount);
//
//	if (map.IsTranslation())
//	{
//		for (size_t i = 0; i < numCoords; i += 3)
//		{
//			vertices[i + 0] += T[0];
//			vertices[i + 1] += T[1];
//			vertices[i + 2] += T[2];
//		}
//	}
//
//	if (map.IsReflection())
//	{
//		for (size_t i = 0; i < numCoords; i += 9)
//		{
//			std::swap(vertices[i + 0], vertices[i + 3]);
//			std::swap(vertices[i + 1], vertices[i + 4]);
//			std::swap(vertices[i + 2], vertices[i + 5]);
//		}
//	}
//
//	if (map.IsScale())
//	{
//		float magnitude = 0;
//		for (size_t i = 0; i < numNormals; i += 3)
//		{
//			magnitude = std::sqrt(normals[i + 0] * normals[i + 0] + normals[i + 1] * normals[i + 1] + normals[i + 2] * normals[i + 2]);
//			normals[i + 0] /= magnitude;
//			normals[i + 1] /= magnitude;
//			normals[i + 2] /= magnitude;
//		}
//	}
//}
//
//void MKLSTLTransformationExecutor::Multiply(std::vector<float>& a, float affineMap[9], std::vector<float>& c, size_t rows)
//{
//	cblas_sgemm(
//		CBLAS_LAYOUT::CblasRowMajor,
//		CBLAS_TRANSPOSE::CblasNoTrans,
//		CBLAS_TRANSPOSE::CblasNoTrans,
//		rows, 3, 3,
//		1.0f,
//		a.data(), 3,
//		affineMap, 3,
//		0.0f,
//		c.data(), 3
//	);
//}
