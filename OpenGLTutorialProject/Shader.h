#pragma once

#include <string>
#include <GL/glew.h>
#include "transform.h"
#include "camera.h"

class Shader
{
public:
	//�������ڻ�ȡ��ɫ���ı���·��
	Shader(const std::string& fileName);

	//����ɫ��
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
	//��ɫ������,���ﶨ��2����ɫ�����ֱ��Ƕ�����ɫ����ƬԪ��ɫ��
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
};