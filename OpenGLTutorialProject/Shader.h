#pragma once

#include <string>
#include <GL/glew.h>
#include "transform.h"
#include "camera.h"

class Shader
{
public:
	//参数用于获取着色器文本的路径
	Shader(const std::string& fileName);

	//绑定着色器
	void Bind();

	void Update(const Transform& transform, const Camera& camera);

	virtual ~Shader();
protected:
private:
	static const unsigned int NUM_SHADERS = 2;

	enum 
	{
		TRANSFORM_U,

		NUM_UNIFORMS
	};
	GLuint m_program;
	//着色器数组,这里定义2个着色器，分别是顶点着色器和片元着色器
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
};