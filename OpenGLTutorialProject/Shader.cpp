#include "Shader.h"
#include <iostream>
#include <fstream> 

static void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static std::string LoadShader(const std::string& fileName);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName)
{
	//创建一个program
	m_program = glCreateProgram();

	//读取着色器文件，并返回创建后的着色器
	m_shaders[0] = CreateShader(LoadShader(fileName + ".vs"), GL_VERTEX_SHADER);
	m_shaders[1] = CreateShader(LoadShader(fileName + ".fs"), GL_FRAGMENT_SHADER);

	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		//将着色器加载到program中
		glAttachShader(m_program, m_shaders[i]);
	}

	glBindAttribLocation(m_program, 0, "position");
	glBindAttribLocation(m_program, 1, "texCoord");
	glBindAttribLocation(m_program, 2, "normal");

	//连接program
	glLinkProgram(m_program);
	CheckShaderError(m_program, GL_LINK_STATUS, true, "Error:Program Linking failed");

	//验证program
	glValidateProgram(m_program);
	CheckShaderError(m_program, GL_VALIDATE_STATUS, true, "Error:Program Linking invalid");

	m_uniforms[TRANSFORM_U] = glGetUniformLocation(m_program, "transform");
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < NUM_SHADERS; i++)
	{
		//和glAttachShader对应，将shader从program中卸载
		glDetachShader(m_program, m_shaders[i]);
		//删除掉shader
		glDeleteShader(m_shaders[i]);
	}
	//删除program
	glDeleteProgram(m_program);
}

void Shader::Bind()
{
	//使用programe
	glUseProgram(m_program);
}

void Shader::Update(const Transform& transform, const Camera& camera)
{
	glm::mat4 model = camera.GetViewProjection() * transform.GetModel();
	glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
}

//返回一个shader,第一个参数是加载的着色器的所有字符串，第二个参数是加载的着色器类型
static GLuint CreateShader(const std::string& text, GLenum shaderType)
{
	//首先创建一个shader
	GLuint shader = glCreateShader(shaderType);

	//如果创建失败，输出提示信息
	if (shader == 0)
	{
		std::cerr << "Error: Shader creation failed!" << std::endl;
	}

	//获取着色器的字符串
	const GLchar* shaderSourceString[1];
	//获取着色器的字符串的长度
	GLint shaderSourceStringLengths[1];

	shaderSourceString[0] = text.c_str();
	shaderSourceStringLengths[0] = text.length();

	//向shader中导入我们已经写好的着色器
	glShaderSource(shader, 1, shaderSourceString, shaderSourceStringLengths);
	//编译shader
	glCompileShader(shader);

	CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error:Shader compilation failed");

	return shader;
}

//加载着色器文件，并返回着色器的所有内容
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

//检测着色器是否存在错误，并输出错误信息
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