#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	//从文件中加载一个纹理
	Texture(const std::string& fileName);

	//OpenGL使得纹理调用非常easy，所以当我们调用Bind函数时，就会自动的使用我们所创建的纹理
	void Bind();

	virtual ~Texture();
protected:
private:

	//这个变量指的是OpenGL所制定的纹理的标识
	GLuint m_texture;
};

#endif
