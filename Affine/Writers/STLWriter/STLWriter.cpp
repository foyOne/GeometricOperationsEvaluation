#include "STLWriter.h"

#include <ostream>

#include "ThirdPartyLibraries/stlio.hpp"


void STLWriter::Write(std::string file, const std::vector<float>& vertices, const std::vector<float>& normals, const size_t triangles, bool binary)
{
	tyti::stl::basic_solid<float> solid;

	size_t floatVertices = 9 * triangles;
	size_t floatNormals = 3 * triangles;
	size_t numVertices = 3 * triangles;

	solid.header = "";
	solid.vertices.resize(numVertices);
	solid.normals.resize(triangles);

	size_t i = 0;
	for (auto it = solid.vertices.begin(); it != solid.vertices.end(); ++it, i += 3)
	{
		it->data[0] = vertices[i + 0];
		it->data[1] = vertices[i + 1];
		it->data[2] = vertices[i + 2];
	}

	i = 0;
	for (auto it = solid.normals.begin(); it != solid.normals.end(); ++it, i += 3)
	{
		it->data[0] = normals[i + 0];
		it->data[1] = normals[i + 1];
		it->data[2] = normals[i + 2];
	}

	auto out = std::ofstream(file);
	tyti::stl::write(out, solid, binary);
}