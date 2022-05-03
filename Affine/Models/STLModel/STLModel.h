#pragma once

#include <vector>
#include <memory>

class STLModel;
using STLModelPtr = std::shared_ptr<STLModel>;

class STLModel
{
public:
	STLModel() { }

	void Reset()
	{
		vertices.clear();
		normals.clear();
	}

	bool Compare(const STLModelPtr other, bool fast) const;

	~STLModel()
	{
		Reset();
	}

public:
	std::vector<float> vertices; // 9 * triangles count
	std::vector<float> normals; // 3 * triangles count
};

