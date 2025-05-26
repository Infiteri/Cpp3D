#pragma once

#include "Panel.h"
#include <string>

namespace Core
{
    class LoggerPanel : public Panel
    {
    public:
        int LastLogSize = 0;
        int FilterLevel = 0;
        std::string FilterCategory = "";

        LoggerPanel();

        void OnImGuiRender();
    };
} // namespace Core
