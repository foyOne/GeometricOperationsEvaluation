#pragma once

#include <string>
#include <vector>

class STLReader
{
public:
	STLReader() = default;
	bool Read(std::string filename, std::vector<float>& vertices, std::vector<float>& normals, size_t& triangles);
};

