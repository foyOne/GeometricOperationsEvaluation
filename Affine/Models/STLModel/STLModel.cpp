#include "STLModel.h"

#include <cmath>
#include <algorithm>
#include <numeric>

#include <mkl.h>


void STLModel::SetStorageType(StorageType newStorageType)
{
	if (storageType == newStorageType)
		return;

	auto vertexShape = GetVertexShape();
	MKL_Simatcopy(
		'R',
		'T',
		vertexShape.first,
		vertexShape.second,
		1.0f,
		vertices.data(),
		vertexShape.second,
		vertexShape.first);

	auto normalShape = GetNormalShape();
	MKL_Simatcopy(
		'R',
		'T',
		normalShape.first,
		normalShape.second,
		1.0f,
		normals.data(),
		normalShape.second,
		normalShape.first);

	storageType = newStorageType;
}

std::pair<size_t, size_t> STLModel::GetVertexShape() const
{
	switch (storageType)
	{
		case StorageType::Height:
		{
			return std::pair<size_t, size_t>(vertices.size() / 3, 3);
		}
		case StorageType::Width:
		{
			return std::pair<size_t, size_t>(3, vertices.size() / 3);
		}
	}
}

std::pair<size_t, size_t> STLModel::GetNormalShape() const
{
	switch (storageType)
	{
		case StorageType::Height:
		{
			return std::pair<size_t, size_t>(normals.size() / 3, 3);
		}
		case StorageType::Width:
		{
			return std::pair<size_t, size_t>(3, normals.size() / 3);
		}
	}
}

bool STLModel::Compare(const STLModelPtr other, bool fast) const
{

	if (vertices.size() != other->vertices.size() || normals.size() != other->normals.size())
		return false;

	float tol = 1e-5f;

	bool checkVertex, checkNormal;
	checkVertex = false;
	checkNormal = false;

	// Сравнение по внутреннему кубу с ребром 2 * tol / sqrt(3)
	// O(n)
	fast = true;
	if (fast)
	{
		float vertexLimit = vertices.size() * tol;
		float normalLimit = normals.size() * tol;

		float a, b;
		a = std::accumulate(vertices.begin(), vertices.end(), 0.0f);
		b = std::accumulate(other->vertices.begin(), other->vertices.end(), 0.0f);
		float sumVertex = std::sqrt(3.0f) * (a - b);
		checkVertex = -vertexLimit < sumVertex&& sumVertex < vertexLimit;

		a = std::accumulate(normals.begin(), normals.end(), 0.0f);
		b = std::accumulate(other->normals.begin(), other->normals.end(), 0.0f);
		float sumNormal = std::sqrt(3.0f) * (a - b);
		checkNormal = -normalLimit < sumNormal&& sumNormal < normalLimit;
	}

	if (checkVertex && checkNormal)
		return true;


	//// Стандартное сравнение (предполагается, что точки сравниваемых моделей взаимосвязаны)
	//// O(n)
	//std::vector<float> squaredMagnitude(vertices.size());
	//std::transform(
	//	vertices.begin(),
	//	vertices.end(),
	//	other->vertices.begin(),
	//	squaredMagnitude.begin(),
	//	[](const float& coord1, const float& coord2)
	//	{
	//		return (coord1 - coord2) * (coord1 - coord2);
	//	});

	//float squaredMagnitudeSum = std::accumulate(squaredMagnitude.begin(), squaredMagnitude.end(), 0.0f);

	//float compared = std::sqrt(squaredMagnitudeSum / (vertices.size() / 3));
	//checkVertex = compared < tol;

	//squaredMagnitude.resize(normals.size());
	//std::transform(
	//	normals.begin(),
	//	normals.end(),
	//	other->normals.begin(),
	//	squaredMagnitude.begin(),
	//	[](const float& coord1, const float& coord2)
	//	{
	//		return (coord1 - coord2) * (coord1 - coord2);
	//	});

	//squaredMagnitudeSum = std::accumulate(squaredMagnitude.begin(), squaredMagnitude.end(), 0.0f);

	//compared = std::sqrt(squaredMagnitudeSum / (normals.size() / 3));
	//checkNormal = compared < tol;

	//if (checkVertex && checkNormal)
	//	return true;


	// Сравнение с поиском пар подходящих точек (для моделей с невзамосвязанными векторами)

}