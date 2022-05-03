#include "STLWriter.h"

#include <ostream>

#include "stlio.hpp"


void STLWriter::Write(std::string file, const STLModelPtr model)
{
	tyti::stl::basic_solid<float> solid;

	size_t tri = model->normals.size() / 3;

	solid.header = "";
	solid.vertices.resize(3 * tri);
	solid.normals.resize(tri);

	std::memcpy(solid.vertices.data(), model->vertices.data(), 9 * tri * sizeof(float));
	std::memcpy(solid.normals.data(), model->normals.data(), 3 * tri * sizeof(float));

	auto out = std::ofstream(file);
	tyti::stl::write(out, solid, binary);
}

