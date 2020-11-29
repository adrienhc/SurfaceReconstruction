#include "terrain.h"


Terrain::Terrain(glm::vec3 position, char orientation, std::string heightMapPath) //'N' 'S' 'E' 'W'
{
	
	float heightScale = 10.0f;
	float lengthScale = 30.0f;

	//Load HeightMap Texture
	heightMapTexture = new Texture(heightMapPath);
	GLubyte* textureData = heightMapTexture->GetData();
	glm::vec3 textureDimensions = heightMapTexture->GetDimensions();

	//Store HeightMap Data
	int width = (int)textureDimensions.x;
	int height = (int)textureDimensions.y;
	int nrChannels = (int)textureDimensions.z;

	heightMap = new int[width * height];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			heightMap[x + (width * y)] = (int) textureData[x * nrChannels + (width * y * nrChannels)];
		}
	}

	

	//Initialize Model Mesh
	std::vector<mVertex> terrainVertices;
	std::vector<unsigned int> terrainIndices;
	std::vector<mTexture> terrainTextures;

	//VERTICES
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			mVertex v;
			v.Position = glm::vec3(lengthScale * (float)x/width, heightScale * heightMap[x + (width * y)] / 255.0f, lengthScale * (float)y/height);
			//std::cout << heightMap[x + (width * y)] << " ";
			glm::vec3 normal;
			if (x == 0 || x == width - 1 || y == 0 || y == height - 1)
				normal = glm::vec3(0.0f, 1.0f, 0.0f);
			else
				normal = GetNormal(heightMap[x + (width * (y - 1))], heightMap[(x + 1) + (width * y)], heightMap[x + (width * (y + 1))], heightMap[(x - 1) + (width * y)], heightMap[x + (width * y)], heightScale/255.0f);

			v.Normal = normal;
			v.TexCoords = glm::vec2((float)x / (float)width, (float)y / (float)height);
			terrainVertices.push_back(v);
		}
	}

	//INDICES
	for (int y = 0; y < height-1; y++)
	{
		for (int x = 0; x < width-1; x++)
		{
			terrainIndices.push_back((y * height) + x);
			terrainIndices.push_back((y * height) + (x + 1));
			terrainIndices.push_back((y * height) + (x + width));

			terrainIndices.push_back((y * height) + (x + 1));
			terrainIndices.push_back((y * height) + (x + width));
			terrainIndices.push_back((y * height) + (x + 1 + width));
		}
	}

//	for (int i = 0; i < 21; i++)
//		std::cout << terrainIndices[i] << " ";

	//TEXTURE
	mTexture terrainTexture;
	terrainTexture.id = heightMapTexture->GetID();
	terrainTexture.type = "texture_diffuse";
	terrainTexture.path = heightMapPath;


	//Terrain Model
	model = new Model();
	model->gammaCorrection = false;
	model->cullingOFF();
	model->meshes.push_back(Mesh(terrainVertices, terrainIndices, terrainTextures));
	model->textures_loaded.push_back(terrainTexture);

	//Terrain Model Node
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	
	switch (orientation) // LOADED ALIGNED WITH N
	{
		case 'E':
			transform = glm::rotate(transform, glm::radians(-90.0f), Up);
		break;
		case 'W':
			transform = glm::rotate(transform, glm::radians(90.0f), Up);
		break;
		case 'S':
			transform = glm::rotate(transform, glm::radians(180.0f), Up);
		break;
		default:
		break;
	}

	nodeModel = new nModel(model, eModelshdr);
	nodeModel->SetTransform(transform);
}


Terrain::~Terrain()
{
	delete heightMapTexture;
	delete heightMap;
	delete nodeModel;
}

nModel* Terrain::GetNodeModel()
{
	return nodeModel;
}

Model* Terrain::GetModel()
{
	return model;
}

void Terrain::addLayer(AbstractLayer* layer)
{
	nNode* Terrain = new nNode();
	Terrain->AddChildren(nodeModel);

	Group* g_terrain = new Group(Terrain);
	layer->Add(g_terrain);
}

glm::vec3 Terrain::GetNormal(float a, float b, float c, float d, float n, float heightScale)
{
	glm::vec3 va = glm::vec3(0.0f, (a - n) * heightScale, -1.0f);
	glm::vec3 vb = glm::vec3(1.0f, (b - n) * heightScale, 0.0f);
	glm::vec3 vc = glm::vec3(0.0f, (c - n) * heightScale, 1.0f);
	glm::vec3 vd = glm::vec3(-1.0f, (d - n) * heightScale, 0.0f);
	//cross products of each vector yields the normal of each tri - return the average normal of all 4 tris
	glm::vec3 avg = (glm::cross(va, vb) + glm::cross(vb, vc) + glm::cross(vc, vd) + glm::cross(vd, va)) / -4.0f;

	return glm::normalize(avg);
}

