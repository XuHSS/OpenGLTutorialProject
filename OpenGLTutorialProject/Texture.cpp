#include "texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& fileName)
{
	//这里的三个变量都是通过 stbi_load 后获得的
	int width, height, numComponents;
	//这里和下面的stbi_image_free对应，使用stbi_load把纹理加载进来   使用stbi_image_free删除掉纹理
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	//加载失败，则输出错误信息
	if (data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;

	//首先设置需要保留de
	glGenTextures(1, &m_texture);
	//然后绑定它的类型，Opengl提供很多种类型，这里我们使用二维纹理，所以使用GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//设置纹理参数
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//将纹理传送到GPU 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

Texture::~Texture()
{
	//删除纹理
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
	//使用时绑定它
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
