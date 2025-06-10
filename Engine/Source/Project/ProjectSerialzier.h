#pragma once

#include "Base.h"
#include "Core/Serializer/CeSerializer.h"
#include "Project.h"

namespace Core
{
    //  todo: Change serializer to bool functions instead of void functions
    class CE_API ProjectSerializer : public CeSerializer
    {
    private:
        Project *project;

    public:
        ProjectSerializer(Project *target);
        ~ProjectSerializer() {};

        void Serialize(const std::string &name);
        void Deserialize(const std::string &name);
    };
} // namespace Core
