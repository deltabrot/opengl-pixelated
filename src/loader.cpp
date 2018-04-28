#include <iostream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <GL/glew.h>
#include <GL/glut.h>
#include "loader.h"
Model::Model(const char *filepath, float scale){
	this->scale = scale;
	loadModelData(filepath);
}
Model::Model(const char *filepath){
	this->scale = 1.0f;
	loadModelData(filepath);
}

void Model::draw(){
	for(int i=0; i<out_vertices.size()/3; i++){
                glBegin(GL_TRIANGLES);
                for(int j=0;j<3;j++){
                        glNormal3f(out_normals[(i*3)+j].x, out_normals[(i*3)+j].y, out_normals[(i*3)+j].z);
                        glVertex3f(out_vertices[(i*3)+j].x, out_vertices[(i*3)+j].y, out_vertices[(i*3)+j].z);
                }
                glEnd();
        }
}



bool Model::loadModelData(const char *filepath){
	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<Vertex> temp_vertices;
	std::vector<Vertex> temp_uvs;
        std::vector<Vertex> temp_normals;
        FILE * file = fopen(filepath,"r");
        if(file == NULL){
                std::cout<<"File not found."<<std::endl;
                return 0; 
        }  
        while(1){
                char lineHeader[128];

                int res = fscanf(file, "%s", lineHeader);
                if(res == EOF){
                        break;
                }  
                else{
                        if(strcmp(lineHeader, "v") == 0){
				Vertex vertex;
  				fscanf(file, "%lf %lf %lf\n",  &vertex.x, &vertex.y, &vertex.z);
				vertex.x /= scale;
				vertex.y /= scale;
				vertex.z /= scale;
				temp_vertices.push_back(vertex);
                        }
			else if(strcmp(lineHeader, "vt") == 0){
				Vertex uv;
				fscanf(file, "%lf %lf\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}
			else if(strcmp(lineHeader, "vn") == 0){
				Vertex normal;
				fscanf(file, "%lf %lf %lf\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if(strcmp(lineHeader, "f") == 0){
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if(matches != 9){
					std::cout<<"Unable to parse."<<std::endl;
				}
				else{
					for(int i=0;i<3;i++){
						vertexIndices.push_back(vertexIndex[i]);
						uvIndices.push_back(uvIndex[i]);
						normalIndices.push_back(normalIndex[i]);
					}
				}
			}
                }  
        } 
	for(unsigned int i=0; i<vertexIndices.size(); i++){
		unsigned int vertexIndex = vertexIndices[i];
		Vertex vertex = temp_vertices[vertexIndex-1];
		out_vertices.push_back(vertex);
	}
	for(unsigned int i=0; i<uvIndices.size(); i++){
		unsigned int uvIndex = uvIndices[i];
		Vertex uv = temp_uvs[uvIndex-1];
		out_uvs.push_back(uv);
	}
	for(unsigned int i=0; i<normalIndices.size(); i++){
		unsigned int normalIndex = normalIndices[i];
		Vertex normal = temp_normals[normalIndex-1];
		out_normals.push_back(normal);
	}
}
