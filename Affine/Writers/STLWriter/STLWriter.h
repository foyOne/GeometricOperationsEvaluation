#pragma once

#include <string>
#include <vector>

class STLWriter
{
public:
	STLWriter() = default;
	void Write(std::string file, const std::vector<float>& vertices, const std::vector<float>& normals, const size_t triangles, bool binary);
};

