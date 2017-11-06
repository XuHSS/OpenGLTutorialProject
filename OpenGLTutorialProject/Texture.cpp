#include "texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& fileName)
{
	//�����������������ͨ�� stbi_load ���õ�
	int width, height, numComponents;
	//����������stbi_image_free��Ӧ��ʹ��stbi_load��������ؽ���   ʹ��stbi_image_freeɾ��������
	unsigned char* data = stbi_load((fileName).c_str(), &width, &height, &numComponents, 4);

	//����ʧ�ܣ������������Ϣ
	if (data == NULL)
		std::cerr << "Unable to load texture: " << fileName << std::endl;

	//����������Ҫ����de
	glGenTextures(1, &m_texture);
	//Ȼ����������ͣ�Opengl�ṩ�ܶ������ͣ���������ʹ�ö�ά��������ʹ��GL_TEXTURE_2D
	glBindTexture(GL_TEXTURE_2D, m_texture);

	//�����������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//�������͵�GPU 
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);
}

Texture::~Texture()
{
	//ɾ������
	glDeleteTextures(1, &m_texture);
}

void Texture::Bind()
{
	//ʹ��ʱ����
	glBindTexture(GL_TEXTURE_2D, m_texture);
}
