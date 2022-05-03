#include "STLReader.h"

#include "stlio.hpp"

STLModelPtr STLReader::Read(std::string file)
{
	STLModelPtr model(new STLModel());

	try
	{
		auto solid = tyti::stl::read(file);

		size_t verticesCount = 3 * solid.first.vertices.size();
		size_t normalsCount = 3 * solid.first.normals.size();

		model->vertices.resize(3 * solid.first.vertices.size());
		model->normals.resize(3 * solid.first.normals.size());

		std::memcpy(model->vertices.data(), solid.first.vertices.data(), verticesCount * sizeof(float));
		std::memcpy(model->normals.data(), solid.first.normals.data(), normalsCount * sizeof(float));
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		model.reset();
	}

	return model;
}