#pragma once

#include <vector>
#include <memory>
#include <string>

class STLModel;
using STLModelPtr = std::shared_ptr<STLModel>;

enum class StorageType
{
	Height, // N by 3 matrix
	Width, // 3 by N matrix
};

class STLModel
{
public:

	STLModel() 
	{
		storageType = StorageType::Height;
	}

	STLModel(const STLModel& other)
	{
		header = other.header;
		vertices = other.vertices;
		normals = other.normals;
		storageType = other.storageType;
	}

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

	std::pair<size_t, size_t> GetVertexShape() const;

	std::pair<size_t, size_t> GetNormalShape() const;

	// Change storage type
	void SetStorageType(StorageType storageType);



public:

	// Header of stl file
	std::string header;

	// Represented by coordinates
	// Number of coordinates is 9 * triangles count
	std::vector<float> vertices;

	// Represented by coordinates
	// Number of coordinates is 3 * triangles count
	std::vector<float> normals;

	// first - row count of vertex representation
	// second - row count of normal representation
	//std::pair<size_t, size_t> shape;

	StorageType storageType;
};

