#include "Shader.h"
#include <iostream>
#include <fstream> 

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
	//����һ��program
	m_program = glCreateProgram();

	//��ȡ��ɫ���ļ��������ش��������ɫ��
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		//����ɫ�����ص�program��
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");

	//����program
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error:Program Linking failed");

	//��֤program
	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error:Program Linking invalid");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		//��glAttachShader��Ӧ����shader��program��ж��
		glDetachShader(m_program, m_shaders[i]);
		//ɾ����shader
		glDeleteShader(m_shaders[i]);
	}
	//ɾ��program
	glDeleteProgram(m_program);
}

void Shader::Bind()
{
	//ʹ��programe
	glUseProgram(m_program);
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

//����һ��shader,��һ�������Ǽ��ص���ɫ���������ַ������ڶ��������Ǽ��ص���ɫ������
static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	//���ȴ���һ��shader
	GLuint shader = glCreateShader(shaderType);

	//�������ʧ�ܣ������ʾ��Ϣ
	if (shader == 0)
	{
		std::cerr << "Error: Shader creation failed!" << std::endl;
	}

	//��ȡ��ɫ�����ַ���
	const GLchar* shaderSourceString[1];
	//��ȡ��ɫ�����ַ����ĳ���
	GLint shaderSourceStringLengths[1];

	shaderSourceString[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	//��shader�е��������Ѿ�д�õ���ɫ��
	glShaderSource(shader, 1, shaderSourceString, shaderSourceStringLengths);
	//����shader
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error:Shader compilation failed");

	return shader;
}

//������ɫ���ļ�����������ɫ������������
static std::string LoadShader(const std::string& fileName)
{
	std::ifstream file;
	file.open((fileName).c_str());

	std::string output;
	std::string line;

	if (file.is_open())
	{
		while (file.good())
		{
			getline(file, line);
			output.append(line + "\n");
		}
	}
	else
	{
		std::cerr << "Unable to load shader:" << fileName << std::endl;
	}

	return output;
}

//�����ɫ���Ƿ���ڴ��󣬲����������Ϣ
static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
	GLint success = 0;
	GLchar error[1024] = { 0 };

	if (isProgram)
	{
		glGetProgramiv(shader, flag, &success);
	}
	else
	{
		glGetShaderiv(shader, flag, &success);
	}

	if (success == GL_FALSE)
	{
		if (isProgram)
		{
			glGetProgramInfoLog(shader, sizeof(error), NULL, error);
		}
		else
		{
			glGetShaderInfoLog(shader, sizeof(error), NULL, error);
		}

		std::cerr << errorMessage << ":'" << error << "'" << std::endl;
	}
}