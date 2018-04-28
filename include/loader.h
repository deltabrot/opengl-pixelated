#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <string.h>
#include <vector>

struct Vertex{
	double x, y, z;
};

class Model{
	public:
		float scale;
		unsigned int textures[5];
		std::vector<Vertex> out_vertices, out_uvs, out_normals;
		Model(const char *filepath, float scale);
		Model(const char *filepath);
		void draw();
		bool loadModelData(const char *filepath);
};

#endif
