#include "terrain.h"


Terrain::Terrain(glm::vec3 position, char orientation, float heightScale, float lengthScale, int downsampleFactor, std::string heightMapPath, bool heightMapAsTexture) //'N' 'S' 'E' 'W'
	:m_HeightScale(heightScale), m_LengthScale(lengthScale), m_DownsampleFactor(downsampleFactor), m_HeightMapAsTexture(heightMapAsTexture)
{
	
	MakeHeightMap(heightMapTexture, heightMap, heightMapPath, true);
	std::cout << heightMap[0] << std::endl;
	MakeModel(model, heightMap);
	MakeNodeModel(nodeModel, model, position, orientation);
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

void Terrain::AddNoise(float noiseScale, std::string noiseMapPath)
{
	//Noisemap and Height map are both m_Width * m_Height
	MakeHeightMap(noiseMapTexture, noiseMap, noiseMapPath, false);

	//
	std::vector<mVertex> terrainVertices = model->meshes[0].vertices;
	std::vector<unsigned int> terrainIndices = model->meshes[0].indices;
	std::vector<mTexture> terrainTextures = model->meshes[0].textures;

	for (int y = 0; y < m_Length; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			terrainVertices[x + (m_Width * y)].Position = glm::vec3(m_LengthScale * ((float)x / m_Width), (m_HeightScale * ((heightMap[x + (m_Width * y)]) / 255.0f))  + (noiseScale * ((noiseMap[x + (m_Width * y)]) / 255.0f)), m_LengthScale * ((float)y / m_Length));
			
			if (x == 0 || x == m_Width - 1 || y == 0 || y == m_Length - 1)
				terrainVertices[x + (m_Width * y)].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
			else
				terrainVertices[x + (m_Width * y)].Normal = 
					GetNormal((m_HeightScale) * heightMap[x + (m_Width * (y - 1))] + (noiseScale) * noiseMap[x + (m_Width * (y - 1))],
							  (m_HeightScale) * heightMap[(x + 1) + (m_Width * y)] + (noiseScale) * noiseMap[(x + 1) + (m_Width * y)],
						      (m_HeightScale) * heightMap[x + (m_Width * (y + 1))] + (noiseScale) * noiseMap[x + (m_Width * (y + 1))],
						      (m_HeightScale) * heightMap[(x - 1) + (m_Width * y)] + (noiseScale) * noiseMap[(x - 1) + (m_Width * y)],
						      (m_HeightScale) * heightMap[x + (m_Width * y)]       + (noiseScale) * noiseMap[x + (m_Width * y)],
							  1.0f / 255.0f);
		}
	}

	model->meshes.clear();
	model->meshes.push_back(Mesh(terrainVertices, terrainIndices, terrainTextures));
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

void Terrain::MakeHeightMap(Texture* &heightMapTexture, int* &heightMap, const std::string heightMapPath, const bool isHeightMap)
{
	if (!heightMapTexture)
		delete heightMapTexture;

	//Load HeightMap Texture
	heightMapTexture = new Texture(heightMapPath);
	GLubyte* textureData = heightMapTexture->GetData();
	glm::vec3 textureDimensions = heightMapTexture->GetDimensions();

	//Store HeightMap Data
	int tex_width = (int)textureDimensions.x;
	int tex_height = (int)textureDimensions.y;
	int tex_nrChannels = (int)textureDimensions.z;

	if (isHeightMap)
	{
		m_Width = tex_width / m_DownsampleFactor;
		m_Length = tex_height / m_DownsampleFactor;
	}

	//If adding Noise, set downsampleFactor so that match heightMap size
	int downsampleFactor = isHeightMap ? m_DownsampleFactor : (tex_width / m_Width);

	if (!heightMap)
		delete[] heightMap;

	heightMap = new int[m_Width * m_Length];

	for (int y = 0, i = 0; y < tex_height && i < m_Length; y += downsampleFactor, i++)
	{
		for (int x = 0, j = 0; x < tex_width && j < m_Width; x += downsampleFactor, j++)
		{
			heightMap[j + (m_Width * i)] = (int)textureData[x * tex_nrChannels + (tex_width * y * tex_nrChannels)];
		}
	}
}

void Terrain::MakeModel(Model* &model, const int* heightMap)
{
	//Initialize Model Mesh
	std::vector<mVertex> terrainVertices;
	std::vector<unsigned int> terrainIndices;
	std::vector<mTexture> terrainTextures;

	//VERTICES
	for (int y = 0; y < m_Length; y++)
	{
		for (int x = 0; x < m_Width; x++)
		{
			mVertex v;
			v.Position = glm::vec3(m_LengthScale * ((float)x / m_Width), m_HeightScale * ((heightMap[x + (m_Width * y)]) / 255.0f), m_LengthScale * ((float)y / m_Length));

			glm::vec3 normal;
			if (x == 0 || x == m_Width - 1 || y == 0 || y == m_Length - 1)
				normal = glm::vec3(0.0f, 1.0f, 0.0f);
			else
				normal = GetNormal(heightMap[x + (m_Width * (y - 1))], heightMap[(x + 1) + (m_Width * y)], heightMap[x + (m_Width * (y + 1))], heightMap[(x - 1) + (m_Width * y)], heightMap[x + (m_Width * y)], m_HeightScale / 255.0f);

			v.Normal = normal;
			v.TexCoords = glm::vec2((float)x / (float)m_Width, (float)y / (float)m_Length);
			terrainVertices.push_back(v);
		}
	}

	//INDICES
	for (int y = 0; y < m_Length - 1; y++)
	{
		for (int x = 0; x < m_Width - 1; x++)
		{
			terrainIndices.push_back((y * m_Length) + x);
			terrainIndices.push_back((y * m_Length) + (x + 1));
			terrainIndices.push_back((y * m_Length) + (x + m_Width));

			terrainIndices.push_back((y * m_Length) + (x + 1));
			terrainIndices.push_back((y * m_Length) + (x + m_Width));
			terrainIndices.push_back((y * m_Length) + (x + 1 + m_Width));
		}
	}

	//TEXTURE
	mTexture terrainTexture;
	if (m_HeightMapAsTexture)
	{
		terrainTexture.id = heightMapTexture->GetID();
		terrainTexture.type = "texture_diffuse";
		terrainTexture.path = heightMapTexture->GetPath();
	}
	terrainTextures.push_back(terrainTexture);

	//Terrain Model
	if (!model)
		delete model;

	model = new Model();
	model->gammaCorrection = false;
	model->cullingOFF();
	model->meshes.push_back(Mesh(terrainVertices, terrainIndices, terrainTextures));
	model->textures_loaded.push_back(terrainTexture);

}

void Terrain::MakeNodeModel(nModel* &nodeModel, /*const*/ Model* model, const glm::vec3 position, const char orientation)
{
	//Terrain Model Node
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Transform = glm::mat4(1.0f);
	m_Transform = glm::translate(m_Transform, position);

	switch (orientation) // LOADED ALIGNED WITH N
	{
	case 'E':
		m_Transform = glm::rotate(m_Transform, glm::radians(-90.0f), Up);
		break;
	case 'W':
		m_Transform = glm::rotate(m_Transform, glm::radians(90.0f), Up);
		break;
	case 'S':
		m_Transform = glm::rotate(m_Transform, glm::radians(180.0f), Up);
		break;
	default:
		break;
	}

	if (!nodeModel)
		delete nodeModel;

	nodeModel = new nModel(model, eModelshdr);
	nodeModel->SetTransform(m_Transform);
}


