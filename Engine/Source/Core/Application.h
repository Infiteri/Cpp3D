#pragma once

#include "Base.h"

#include <string>
#include <vector>

namespace Core
{
    struct ApplicationArguments
    {
        int Count;
        std::vector<std::string> Arguments;
    };

    class CE_API Application
    {
    public:
        Application() {};
        virtual ~Application() {};

        virtual void Init() {};
        virtual void Update() {};
        virtual void Destroy() {};
    };

    Application *CreateApplication(ApplicationArguments &arguments);
} // namespace Core
