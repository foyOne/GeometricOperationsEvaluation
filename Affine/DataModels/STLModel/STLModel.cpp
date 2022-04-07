#include "STLModel.h"
#include "Readers/STLReader/STLReader.h"
#include "Writers/STLWriter/STLWriter.h"

STLModel::STLModel(std::string file): STLModel()
{
	Read(file);
}

void STLModel::Read(std::string file)
{
	STLReader reader;
	bool isOk = reader.Read(file, _vertices, _normals, _triangles);
	if (!isOk)
	{
		Reset();
	}
}

void STLModel::Write(std::string file)
{
	STLWriter writer;
	writer.Write(file, _vertices, _normals, _triangles, false);
}

void STLModel::Transform(const AffineMap& map)
{
	if (_transformationExecutor)
		_transformationExecutor->Transform(_vertices, _normals, _triangles, map);
}
