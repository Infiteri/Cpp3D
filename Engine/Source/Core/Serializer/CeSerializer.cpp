#include "CeSerializer.h"
#include <fstream>

#include <yaml-cpp/yaml.h>

namespace Core
{
    void CeSerializer::_SaveEmitter(YAML::Emitter &em, const std::string &filepath)
    {
        std::ofstream stream{filepath};
        stream << em.c_str();
    }

    YAML::Node CeSerializer::_LoadEmitterData(const std::string &filepath)
    {
        std::ifstream stream(filepath);
        std::stringstream strStream(filepath);
        strStream << stream.rdbuf();
        return YAML::Load(strStream.str());
    }

} // namespace Core
