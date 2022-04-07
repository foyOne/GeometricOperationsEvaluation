#include "STLReader.h"

#include "ThirdPartyLibraries/stlio.hpp"

bool STLReader::Read(std::string filename, std::vector<float>& vertices, std::vector<float>& normals, size_t& triangles)
{
	try
	{
		auto solid = tyti::stl::read(filename);

		triangles = solid.first.normals.size();
		vertices.resize(3 * solid.first.vertices.size());
		normals.resize(3 * solid.first.normals.size());

		size_t i = 0;
		for (auto it = solid.first.vertices.begin(); it != solid.first.vertices.end(); ++it, i += 3)
		{
			vertices[i + 0] = it->data[0];
			vertices[i + 1] = it->data[1];
			vertices[i + 2] = it->data[2];
		}

		i = 0;
		for (auto it = solid.first.normals.begin(); it != solid.first.normals.end(); ++it, i += 3)
		{
			normals[i + 0] = it->data[0];
			normals[i + 1] = it->data[1];
			normals[i + 2] = it->data[2];
		}

		return true;
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << std::endl;

		return false;
	}
}

//bool STLReader::ReadAsArray(std::string filename, float*& vertices, float*& normals, size_t& triangles)
//{
//	try
//	{
//		auto solid = tyti::stl::read(filename);
//		triangles = solid.first.normals.size();
//		vertices = new float[3 * solid.first.vertices.size()];
//		normals = new float[3 * solid.first.normals.size()];
//		size_t i = 0;
//		for (auto it = solid.first.vertices.begin(); it != solid.first.vertices.end(); ++it, i += 3)
//		{
//			vertices[i + 0] = it->data[0];
//			vertices[i + 1] = it->data[1];
//			vertices[i + 2] = it->data[2];
//		}
//		i = 0;
//		for (auto it = solid.first.normals.begin(); it != solid.first.normals.end(); ++it, i += 3)
//		{
//			normals[i + 0] = it->data[0];
//			normals[i + 1] = it->data[1];
//			normals[i + 2] = it->data[2];
//		}
//
//		return true;
//	}
//	catch (const std::exception& e)
//	{
//		std::cout << e.what() << std::endl;
//
//		return false;
//	}
//}