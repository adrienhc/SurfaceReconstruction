#ifndef TEXTURE
#define TEXTURE

#include "../pch.h"


class Texture //Load a texture from an image myself!
{
public:
		Texture(std::string path);
		~Texture();
		void Free();
		void Bind(int offset = 0);
		void Unbind(int offset = 0);
		unsigned int GetID();
		GLubyte* GetData();
		glm::vec3 GetDimensions();
private:
		GLenum format;
		unsigned char *data = NULL;
		int width, height, nrChannels;
		unsigned int texture;

};


#endif //TEXTURE