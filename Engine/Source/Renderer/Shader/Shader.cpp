#include "Shader.h"
#include "Core/FileSystem/FileSystem.h"
#include "Core/Logger.h"

#include <fstream>
#include <glad/glad.h>

namespace Core
{
    static u32 LoadShader(const char *src, u32 type)
    {
        u32 shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, 0);
        glCompileShader(shader);

        int compileStatus = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

        if (compileStatus == GL_FALSE)
        {
            int maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            char *buffer = new char[maxLength];
            glGetShaderInfoLog(shader, maxLength, &maxLength, &buffer[0]);

            CE_LOG("CE_RENDER", Error, "Shader compilation error: ");
            CE_LOG("CE_RENDER", Error, "%s", buffer);

            delete[] buffer;
            return 0;
        }

        return shader;
    }

    static int ShaderComposeSources(const std::string &filename, std::string &vs, std::string &fs)
    {
        if (filename.empty())
            return -1;

        std::string fileContent = FileSystem::ReadFile(filename);
        if (fileContent.empty())
            return -2;

        auto vertexCommentPosition = fileContent.find("// VERTEX");
        auto fragmentCommentPosition = fileContent.find("// FRAGMENT");
        if (vertexCommentPosition == std::string::npos ||
            fragmentCommentPosition == std::string::npos)
        {
            return -3;
        }
        vs = fileContent.substr(vertexCommentPosition,
                                fragmentCommentPosition - vertexCommentPosition);
        fs = fileContent.substr(fragmentCommentPosition);
        return 0;
    }

    u32 Shader::GetUniLoc(const char *n)
    {
        Use();
        auto ret = glGetUniformLocation(id, n);
        return ret;
    }

    void Shader::CompileWithSources(const std::string &vs, const std::string &fs)
    {
        u32 vertexShader = LoadShader(vs.c_str(), GL_VERTEX_SHADER);
        u32 fragmentShader = LoadShader(fs.c_str(), GL_FRAGMENT_SHADER);

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        GLint isLinked = 0;
        glGetProgramiv(id, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            char *infoLog = new char[maxLength];
            glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);

            glDeleteProgram(id);
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            CE_LOG("CE_RENDER", Error, "Shader program creation error: \n %s ", infoLog);

            delete[] infoLog;
        }

        // Delete shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    Shader::Shader(const std::string &filename) { Load(filename); }

    Shader::~Shader() { Destroy(); }

    void Shader::Load(const std::string &filename)
    {
        if (loaded)
            return;

        id = 0;
        std::string vertexSource, fragmentSource;
        int composeResult = ShaderComposeSources(filename, vertexSource, fragmentSource);

        if (composeResult != 0)
        {
            CE_LOG("CE_RENDER", Error, "Shader cannot get sources from file '%s', error code: %i",
                   filename.c_str(), composeResult);
            return;
        }

        CompileWithSources(vertexSource, fragmentSource);
        loaded = true;
    }

    void Shader::Vec4(const Color &color, const char *name)
    {
        GLint location = GetUniLoc(name);
        if (location == -1)
        {
            CE_WARN("Shader uniform '%s' not found or optimized out.", name);
            return;
        }

        Color normal = color.Normalized();
        glUniform4f(location, normal.r, normal.g, normal.b, normal.a);
    }

    void Shader::Vec3(const Vector3 &vec, const char *name)
    {
        GLint location = GetUniLoc(name);
        if (location == -1)
        {
            CE_WARN("Shader uniform '%s' not found or optimized out.", name);
            return;
        }

        glUniform3f(location, vec.x, vec.y, vec.z);
    }
    void Shader::Mat4(const Matrix4 &matrix, const char *name)
    {
        glUniformMatrix4fv(GetUniLoc(name), 1, false, matrix.data);
    }

    void Shader::Int(int i, const char *name) { glUniform1i(GetUniLoc(name), i); }

    void Shader::Float(float f, const char *name) { glUniform1f(GetUniLoc(name), f); }

    void Shader::Use() { glUseProgram(id); }

    void Shader::Destroy() { glDeleteProgram(id); }
} // namespace Core
