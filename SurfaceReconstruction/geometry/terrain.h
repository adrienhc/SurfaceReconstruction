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
	Terrain(glm::vec3 position, char orientation, float heightScale, float lengthScale, int downsampleFactor, std::string heightMapPath, bool heightMapAsTexture = true); //'N' 'S' 'E' 'W'  
	~Terrain();
	nModel* GetNodeModel();
	Model* GetModel();
	void AddNoise(float noiseScale, std::string noiseMapPath); //keep same m_Width/m_Length -> recompute different m_DownscaleFactor so that noiseMap matches heightMap
	void addLayer(AbstractLayer* layer);

private: 

	float m_HeightScale;
	float m_LengthScale;
	int m_DownsampleFactor;
	int m_Width;
	int m_Length;
	bool m_HeightMapAsTexture;

	Texture* heightMapTexture = NULL;
	int* heightMap = NULL;

	Texture* noiseMapTexture = NULL;
	int* noiseMap = NULL;
	
	Model* model = NULL;
	nModel* nodeModel = NULL;
	glm::mat4 m_Transform;

	void MakeHeightMap(Texture* &heightMapTexture, int* &heightMap, const std::string heightMapPath, const bool isHeightMap = false);
	void MakeModel(Model* &model, const int* heightMap);
	void MakeNodeModel(nModel* &nodeModel, /*const*/ Model* model, const glm::vec3 position, const char orientation);
	glm::vec3 GetNormal(float a, float b, float c, float d, float n, float heightScale);
};


#endif //TERRAIN_H