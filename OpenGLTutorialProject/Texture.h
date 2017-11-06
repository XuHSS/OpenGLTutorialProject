#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>

class Texture
{
public:
	//���ļ��м���һ������
	Texture(const std::string& fileName);

	//OpenGLʹ��������÷ǳ�easy�����Ե����ǵ���Bind����ʱ���ͻ��Զ���ʹ������������������
	void Bind();

	virtual ~Texture();
protected:
private:

	//�������ָ����OpenGL���ƶ�������ı�ʶ
	GLuint m_texture;
};

#endif
