#include "FileSystem.h"
#include "Core/Logger.h"
#include <filesystem>
#include <fstream>

namespace Core
{
    std::string FileSystem::ReadFile(const std::string &name)
    {
        std::ifstream file(name, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            CE_WARN("Unable to read file '%s'.", name.c_str());
            return std::string("");
        }

        std::string content;
        file.seekg(0, std::ios::end);
        content.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&content[0], content.size());
        file.close();

        return content;
    }

    void FileSystem::WriteFile(const std::string &name, const std::string &content)
    {
        std::ofstream file(name, std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            CE_WARN("Unable to write to file '%s'.", name.c_str());
            return;
        }

        file.write(content.c_str(), content.size());
        file.close();
    }

    void FileSystem::CreateFile(const std::string &name)
    {
        std::ofstream file(name);
        if (!file.is_open())
        {
            CE_WARN("Unable to create file '%s'.", name.c_str());
            return;
        }

        file.close();
    }

    bool FileSystem::ExistsFile(const std::string &name)
    {
        FILE *file = std::fopen(name.c_str(), "rb");
        if (file)
        {
            std::fclose(file);
            return true;
        }
        return false;
    }

    void FileSystem::RemoveFile(const std::string &name)
    {
        if (std::remove(name.c_str()) != 0)
        {
            CE_WARN("Unable to remove file '%s'.", name.c_str());
        }
    }

    void FileSystem::CopyFile(const std::string &source, const std::string &dest)
    {
        std::ifstream src(source, std::ios::binary);
        if (!src.is_open())
        {
            CE_WARN("Unable to open source file '%s' for copying.", source.c_str());
            return;
        }

        std::ofstream dst(dest, std::ios::binary);
        if (!dst.is_open())
        {
            CE_WARN("Unable to open destination file '%s' for writing.", dest.c_str());
            return;
        }

        dst << src.rdbuf();
    }
} // namespace Core
