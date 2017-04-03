#include "ShaderProgram.h"
#include "MyFiles.h"
#include "MyGL.h"
#include "GameLogger.h"
#include <vector>


bool ShaderProgram::Initialize()
{
	m_id = glCreateProgram();
	if (MyGL::TestForError(MsgType::cFatalError, "ShaderProgram::Initialize() - An error occured when attempting to create a Shader Program. m_id = [%d]\n", m_id) || m_id == 0) return false;
	return true;
}

bool ShaderProgram::Shutdown()
{
	if (DeleteAll())
	{
		GameLogger::Log(MsgType::Info, "ShaderProgram::Shutdown() - Successfully deleted all shaders and shader programs in use.\n");
		return true;
	}
	return false;
}

bool ShaderProgram::AddVertexShader(const char * const filename)
{
	AddShader(filename, GL_VERTEX_SHADER);
	return true;
}

bool ShaderProgram::AddGeometryShader(const char * const filename)
{
	AddShader(filename, GL_GEOMETRY_SHADER);
	return true;
}

bool ShaderProgram::AddFragmentShader(const char * const filename)
{
	AddShader(filename, GL_FRAGMENT_SHADER);
	return true;
}

bool ShaderProgram::AddShader(const char * const filename, GLenum shaderType)
{
	GLuint shaderID = glCreateShader(shaderType);
	if (MyGL::TestForError(MsgType::Error, "ShaderProgram::AddShader() - An error occured when attempting to create a shader [ID: %d].\n", shaderID) || shaderID == 0) return false;
	
	const char* const source = MyFiles::ReadFileIntoString(filename);
	glShaderSource(shaderID, 1, &source, nullptr);
	if (MyGL::TestForError(MsgType::Error, "ShaderProgram::AddShader() - An error occured when attempting to associate source code to the shader [ID: %d].\n", shaderID)) return false;
	
	glCompileShader(shaderID);
	if (MyGL::TestForError(MsgType::Error, "ShaderProgram::AddShader() - An error occured when attempting to compile the source code for the shader [ID: %d].\n", shaderID) || !CheckShaderCompileStatus(shaderID))
	{
		glDeleteShader(shaderID);
		return false;
	}

	SaveShaderInfo(filename, shaderType, shaderID);

	glAttachShader(m_id, shaderID);
	if (MyGL::TestForError(MsgType::Error, "ShaderProgram::AddShader() - An error occured when attempting to attach the shader [ID: %d] to the program [ID: %d].\n", shaderID, m_id)) return false;
	return true;
}

bool ShaderProgram::CheckShaderCompileStatus(GLuint shaderID)
{
	GLint hasCompiled; 
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &hasCompiled);
	if (MyGL::TestForError(MsgType::Error, "ShaderProgram::CheckShaderCompileStatus() - An error occured when attempting to determine the GL_COMPILE_STATUS of the shader [ID: %d].\n", shaderID)) return false;
	
	if (hasCompiled != GL_TRUE)
	{
		GLint errorLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &errorLength);
		if (MyGL::TestForError(MsgType::Error, "ShaderProgram::CheckShaderCompileStatus() - An error occured when attempting to determine the GL_INFO_LOG_LENGTH for the shader [ID: %d].\n", shaderID)) return false;
		GLchar* buffer = new GLchar[errorLength];
		ShowShaderLogInfo(shaderID, buffer, errorLength);
		delete[] buffer;
		return false;
	}
	GameLogger::Log(MsgType::Info, "ShaderProgram::CheckShaderCompileStatus() - The shader [ID: %d] has successfully compiled.\n", shaderID);
	return true;
}

bool ShaderProgram::CheckProgramStatus(GLenum pname)
{
	GLint status;
	glGetProgramiv(m_id, pname, &status);
	if (MyGL::TestForError(MsgType::Error, "ShaderProgram::CheckProgramStatus() - An error occured when attempting to determine the %d for the program [ID: %d].\n", pname, m_id)) return false;
	
	if (status == GL_FALSE)
	{
		GLint errorLength;
		glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &errorLength);
		if (MyGL::TestForError(MsgType::Error, "ShaderProgram::CheckProgramStatus() - An error occured when attempting to determine the GL_INFO_LOG_LENGTH for the program [ID: %d].\n", m_id)) return false;
		GLchar* buffer = new GLchar[errorLength];
		ShowProgramLogInfo(buffer, errorLength, pname);
		delete[] buffer;
		return false;
	}
	return true;
}

bool ShaderProgram::CheckProgramStatus()
{
	if (!CheckProgramStatus(GL_LINK_STATUS)) return false;
	/*GameLogger::Log(MsgType::Info, "ShaderProgram::CheckProgramStatus() - The program [ID: %d] has been successfully linked.\n", m_id);*/

	glValidateProgram(m_id);
	if (MyGL::TestForError(MsgType::Error, "An error occured when attempting to validate the program [ID: %d].\n", m_id)) return false;
	if (!CheckProgramStatus(GL_VALIDATE_STATUS)) return false;
	/*GameLogger::Log(MsgType::Info, "ShaderProgram::CheckProgramStatus() - The program [ID: %d] has been successfully validated.\n", m_id);*/
	return true;
}

bool ShaderProgram::LinkProgram()
{
	glLinkProgram(m_id);
	if (MyGL::TestForError(MsgType::cFatalError, "ShaderProgram::LinkProgram() - An error occured when attempting to link the program [ID: %d].\n", m_id) || !CheckProgramStatus())
	{
		DeleteAll();
		return m_isLinked = false;
	}
	ParseShader();
	return m_isLinked = true;
}

bool ShaderProgram::UseProgram()
{
	glUseProgram(m_id);
	if (MyGL::TestForError(MsgType::cFatalError, "ShaderProgram::UseProgram() - An error occured when attempting to use the program [ID: %d].\n", m_id)) return false;
	return true;
}

bool ShaderProgram::LinkAndUseProgram()
{
	return LinkProgram() && UseProgram();
}

void ShaderProgram::ParseShader()
{
	printf("\n --- Item --- -- Loc -- -------- Name --------    ---- Type ----   %2d %s\n", m_id, m_shaders[0].name);
	ShowActiveAttributes();
	ShowActiveUniforms();
	ShowActiveUniformBlocks();

	printf("\n");
}

void ShaderProgram::ShowActiveAttributes()
{
	GLint numAttribs = 0;
	glGetProgramInterfaceiv(m_id, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

	const int numProps = 3;
	const GLenum props[numProps] = { GL_LOCATION, GL_NAME_LENGTH, GL_TYPE };

	for (int attribIndex = 0; attribIndex < numAttribs; attribIndex++)
	{
		GLint values[numProps];
		glGetProgramResourceiv(m_id, GL_PROGRAM_INPUT, attribIndex, numProps, props, numProps, NULL, values);

		std::vector<char> nameData(values[1]);
		glGetProgramResourceName(m_id, GL_PROGRAM_INPUT, attribIndex, nameData.size(), NULL, &nameData[0]);
		std::string name(nameData.begin(), nameData.end() - 1);
		printf("    Attrib        %d      %-20s          %s\n", values[0], name.c_str(), GetTypeString(values[2]));
	}
}

void ShaderProgram::ShowActiveUniforms(bool showUniformBlocks)
{
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(m_id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

	const int numProps = 4;
	const GLenum props[numProps] = { GL_BLOCK_INDEX, GL_LOCATION, GL_NAME_LENGTH, GL_TYPE };

	for (int uniformIndex = 0; uniformIndex < numUniforms; uniformIndex++)
	{
		GLint values[numProps];
		glGetProgramResourceiv(m_id, GL_UNIFORM, uniformIndex, numProps, props, numProps, NULL, values);

		//Skip any uniforms that are in a block.
		if (values[0] != -1 && !showUniformBlocks || values[0] == -1 && showUniformBlocks)
			continue;

		std::vector<char> nameData(values[2]);
		glGetProgramResourceName(m_id, GL_UNIFORM, uniformIndex, nameData.size(), NULL, &nameData[0]);
		std::string name(nameData.begin(), nameData.end() - 1);
		printf("    Uniform       %d      %-20s          %s\n", values[1], name.c_str(), GetTypeString(values[3]));
	}
}

void ShaderProgram::ShowActiveUniformBlocks()
{
	ShowActiveUniforms(true);
}

const char* const ShaderProgram::GetTypeString(GLenum type)
{
	switch (type)
	{
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_DOUBLE:
		return "double";
	case GL_INT:
		return "int";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_BOOL:
		return "bool";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	default:
		return "Unknown Type";
	}
}

GLint ShaderProgram::GetUniformLocation(const GLchar * name)
{
	GLint location = glGetUniformLocation(m_id, name);
	/*MyGL::TestForError(MsgType::Error, "ShaderProgram::GetUniformLocation() - An error occured when attempting to get the uniform location of [%s] from program [ID: %d].\n", name, m_id);*/
	if (location < 0)
	{
		/*GameLogger::Log(MsgType::Error, "ShaderProgram::GetUniformLocation() - Unexpected uniform location received [%d] for [%s] from program [ID: %d].\n", location, name, m_id);*/
		return -1;
	}
	return location;
}

GLint ShaderProgram::GetAttribLocation(const GLchar * name)
{
	GLint location = glGetAttribLocation(m_id, name);
	MyGL::TestForError(MsgType::Error, "ShaderProgram::GetAttribLocation() - An error occured when attempting to get the uniformIndex location of [%s] from program [ID: %d].\n", name, m_id);
	if (location < 0)
	{
		GameLogger::Log(MsgType::Error, "ShaderProgram::GetAttribLocation() - Unexpected uniformIndex location received [%d] for [%s] from program [ID: %d].\n", location, name, m_id);
		return -1;
	}
	return location;
}

void ShaderProgram::ShowShaderLogInfo(GLuint shaderID, char * infoBuffer, GLint bufferLen)
{
	glGetShaderInfoLog(shaderID, bufferLen, nullptr, infoBuffer);
	if (MyGL::TestForError(MsgType::Error, "ShaderProgram::ShowShaderLogInfo() - An error occured when attempting to acquire the OpenGL generated log for shader [ID: %d].\n", shaderID)) return;
	GameLogger::Log(MsgType::cFatalError, "\n\n	------ Shader [ID: %d] Compile Errors ------\n%s\n", shaderID, infoBuffer);
	GameLogger::Log(MsgType::cInfo, "Aborting shader execution...\n");
}

void ShaderProgram::ShowProgramLogInfo(char * infoBuffer, GLint bufferLen, GLenum pname)
{
	glGetProgramInfoLog(m_id, bufferLen, nullptr, infoBuffer);
	if (MyGL::TestForError(MsgType::Error, "ShaderProgram::ShowProgramLogInfo() - An error occured when attempting to acquire the OpenGL generated log for program [ID: %d].\n", m_id)) return;
	GameLogger::Log(MsgType::cFatalError, "\n\n	------ Program [ID: %d] Compile Errors [%d] ------\n%s\n", m_id, pname, infoBuffer);
	GameLogger::Log(MsgType::cInfo, "Aborting program execution...\n");
}

bool ShaderProgram::SaveShaderInfo(const char * const filename, GLenum shaderType, GLuint shaderId)
{
	ShaderInfo shaderInfo;
	strcpy_s(shaderInfo.name, filename);
	shaderInfo.type = shaderType;
	shaderInfo.id = shaderId;

	for (int i = 0; i < maxShaders; i++)
	{
		if (m_shaders[i].name == '\0')
		{
			m_shaders[i] = shaderInfo;
			return true;
		}
	}
	return false;
}

bool ShaderProgram::DeleteShaders()
{
	for (int i = 0; i < maxShaders; i++)
	{
		ShaderInfo shader = m_shaders[i];
		if (!shader.name[0] == '\0')
		{
			glDeleteShader(shader.id);
			MyGL::TestForError(MsgType::Error, "ShaderProgram::DeleteShaders() - An error occured when attempting to delete a shader [ID: %d].\n", shader.id);
			shader.name[0] = '\0';
		}
	}
	return true;
}

bool ShaderProgram::DeleteProgram()
{
	if (m_id != -1)
	{
		glDeleteProgram(m_id);
		MyGL::TestForError(MsgType::Error, "ShaderProgram::DeleteProgram() - An error occured when attempting to delete a program [ID: %d].\n", m_id);
		m_id = -1;
	}
	return true;
}

bool ShaderProgram::DeleteAll()
{
	return DeleteShaders() && DeleteProgram();
}
