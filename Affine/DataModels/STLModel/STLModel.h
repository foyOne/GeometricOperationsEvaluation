#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <memory>

#include "TransformationExecutors/ITtransformationExecutor.h"

#include "AffineMap.h"

class STLModel
{
	using ITtransformationExecutorPtr = std::shared_ptr<ITtransformationExecutor>;

public:
	STLModel() : _triangles(0), _transformationExecutor(nullptr) { }
	STLModel(std::string file);

	void Read(std::string file);
	void Write(std::string file);
	void Transform(const AffineMap& map);

	void SetTtransformationExecutor(ITtransformationExecutorPtr transformationExecutor)
	{
		_transformationExecutor.reset();
		_transformationExecutor = transformationExecutor;
	}

	void Reset()
	{
		_vertices.clear();
		_normals.clear();
		_triangles = 0;
	}

	~STLModel()
	{
		Reset();
	}

private:
	std::vector<float> _vertices; // 9 * _triangles
	std::vector<float> _normals; // 3 * _triangles
	size_t _triangles;

	ITtransformationExecutorPtr _transformationExecutor;
};

