#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

// Trevor Sifton
// 6/25/2016
// ShaderProgram.h
// Wrapper class for an OpenGL Shader Program

#include <GL\glew.h>

const int maxShaderNameLen = 256;  // relative path to shader
const int maxShaders       = 2;    // only need vertex and frag shaders for now

struct ShaderInfo
    {
		char   name[maxShaderNameLen] { 0 }; // name[0] == '\0' if this shader info is empty
		GLuint id   { 0 };
		GLenum type { 0 };      // shader type, eg, GL_VERTEX_SHADER
    };

class ShaderProgram
    {
    public:
        bool Initialize();
        bool Shutdown();

        // Shader methods
        bool AddVertexShader (const char* const filename);
		bool AddGeometryShader(const char* const filename);
        bool AddFragmentShader (const char* const filename);
        bool AddShader (const char* const filename, GLenum shaderType); // prefer adding specific shaders
        bool CheckShaderCompileStatus (GLuint shaderID);

        // Program methods
        bool CheckProgramStatus(GLenum pname);
        bool CheckProgramStatus();
		bool LinkProgram();
		bool UseProgram();
        bool LinkAndUseProgram();

        GLint GetUniformLocation (const GLchar* name);
        GLint GetAttribLocation  (const GLchar* name);

        GLint GetProgramId() const { return m_id; }

		bool IsLinked() const { return m_isLinked; }

    private:
        void ShowShaderLogInfo (GLuint shaderID, char* infoBuffer, GLint bufferLen);
        void ShowProgramLogInfo(char* infoBuffer, GLint bufferLen, GLenum pname);
        bool SaveShaderInfo(const char* const filename, GLenum shaderType, GLuint shaderId);

		void ParseShader();
		void ShowActiveAttributes();
		void ShowActiveUniforms(bool showUniformBlocks = false);
		void ShowActiveUniformBlocks();
		const char* const GetTypeString(GLenum type);

        bool DeleteShaders ();
        bool DeleteProgram ();
		bool DeleteAll();

    private:
        GLint m_id; // Set to -1 if program is deleted
        ShaderInfo m_shaders[maxShaders];
		bool m_isLinked = false;
    };

#endif // ndef SHADERPROGRAM_H_
