#ifndef TERRAIN_H
#define TERRAIN_H

#include "../pch.h"
#include "model.h"
#include "../material_system/texture.h"
#include "../scene/scenegraph.h"

#include "../rendering/layers/abstractlayer.h"
#include "../rendering/group.h"

class Terrain
{
public:
	//Add Size and Granularity/Samples -- think of a better name
	Terrain(glm::vec3 position, char orientation, std::string heightMapPath); //'N' 'S' 'E' 'W'  
	~Terrain();

	nModel* GetNodeModel();
	Model* GetModel();

	void addLayer(AbstractLayer* layer);

private: 
	Texture* heightMapTexture = NULL;
	int* heightMap = NULL;
	Model* model = NULL;
	nModel* nodeModel = NULL;
	glm::mat4 transform;

	glm::vec3 GetNormal(float a, float b, float c, float d, float n, float heightScale);
};


#endif //TERRAIN_H