#include "../includes/Shader.hpp"

namespace GameBoi { namespace graphics {


	Shader::Shader(const std::string & vertexpath, const std::string & fragpath)
{
	std::string const vertexShader = GetSource(vertexpath);
    std::string const fragmentShader = GetSource(fragpath);
    if(vertexShader.size() == 0 || fragmentShader.size() == 0)
        throw std::logic_error("Empty shader files");
	
    this->_ID = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader.c_str());
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader.c_str());

	CreateShaderProgam(vs,fs);
}

Shader::~Shader()
{}

std::string const Shader::GetSource(const std::string & path)
{
    std::ifstream file(path);

    if (!file.good())
    {
        std::cout << path << " does not exist" << std::endl;
        return NULL;
    }

    std::string line;
    std::stringstream ss;
    while (getline(file, line))
        ss << line << std::endl;
    return ss.str();
}

void Shader::CreateShaderProgam(int vs, int fs)
{
    glAttachShader(this->_ID, vs);
	glAttachShader(this->_ID, fs);
	glLinkProgram(this->_ID);
	glValidateProgram(this->_ID);

	glDeleteShader(vs);
	glDeleteShader(fs);
}

unsigned int Shader::CompileShader(unsigned int type, const char * src)
{
    unsigned int id = glCreateShader(type);
	glShaderSource(id, 1, &src, NULL); 
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	char infoLog[512];
	if (result == GL_FALSE)
	{
		glGetShaderInfoLog(id, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<< (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment")  << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		glDeleteShader(id);
        return 0;
	}
	return id;
}

void Shader::use()
{
	glUseProgram(this->_ID);
}

unsigned int Shader::ID() const
{
	return this->_ID;
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(this->_ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(this->_ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(this->_ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{ 
	glUniform2fv(glGetUniformLocation(this->_ID, name.c_str()), 1, &value[0]); 
}

void Shader::setVec2(const std::string &name, float x, float y) const
{ 
	glUniform2f(glGetUniformLocation(this->_ID, name.c_str()), x, y); 
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{ 
	glUniform3fv(glGetUniformLocation(this->_ID, name.c_str()), 1, &value[0]); 
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{ 
	glUniform3f(glGetUniformLocation(this->_ID, name.c_str()), x, y, z); 
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{ 
	glUniform4fv(glGetUniformLocation(this->_ID, name.c_str()), 1, &value[0]); 
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const
{ 
	glUniform4f(glGetUniformLocation(this->_ID, name.c_str()), x, y, z, w); 
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(this->_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(this->_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4Ptr(const std::string &name, const glm::mat4 &mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(this->_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

}}