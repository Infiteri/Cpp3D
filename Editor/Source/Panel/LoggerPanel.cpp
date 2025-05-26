#include "LoggerPanel.h"
#include "Core/Logger.h"
#include "EditorUtils.h"

#include <imgui.h>
#include <vector>

namespace Core
{
    static const char *LevelToStringStrings[] = {"All", "Info", "Warn", "Error", "Debug", "Trace"};
    static const int selCount = 6;

    LoggerPanel::LoggerPanel()
    {
        LastLogSize = 0;
        FilterLevel = 0;
    }

    void LoggerPanel::OnImGuiRender()
    {
        ImGui::Begin("Logger");

        // -- DROP DOWN -----------
        if (ImGui::BeginCombo("Level", LevelToStringStrings[FilterLevel]))
        {
            for (int i = 0; i < selCount; i++)
            {
                bool isSelected = (FilterLevel == i);
                if (ImGui::Selectable(LevelToStringStrings[i], isSelected))
                    FilterLevel = i;

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        } // ------------------------

        ImGui::SameLine();

        EditorUtils::ImGuiString("Category", FilterCategory);

        for (auto &log : Logger::GetLogMessages())
        {
            ImVec4 color;

            switch (log.Level)
            {
            case Logger::Level::Info:
                color = {0, 1, 0.5, 1};
                break;

            case Logger::Level::Warn:
                color = {1, 1, 0, 1};
                break;

            case Logger::Level::Error:
                color = {1, 0, 0, 1};
                break;

            case Logger::Level::Trace:
                color = {0.9f, 0.9f, 0.9f, 1};
                break;

            case Logger::Level::Debug:
                color = {0, 0.49, 1, 1};
                break;

            default:
                color = {1, 1, 1, 1};
                break;
            }

            bool renderCurrent = true;
            if (FilterLevel != 0)
            {
                Logger::Level level = (Logger::Level)(FilterLevel - 1);
                if (log.Level != level)
                    renderCurrent = false;
            }

            if (!FilterCategory.empty())
            {
                auto ToLower = [](const std::string &str) -> std::string
                {
                    std::string lower;
                    lower.reserve(str.size());
                    for (char c : str)
                        lower += static_cast<char>(::tolower(c));
                    return lower;
                };

                if (ToLower(log.Category).find(ToLower(FilterCategory)) == std::string::npos)
                    renderCurrent = false;
            }

            if (renderCurrent)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::Text(log.Message.c_str());
                ImGui::PopStyleColor();
            }

            if (Logger::GetLogMessages().size() != LastLogSize)
            {
                LastLogSize = Logger::GetLogMessages().size();
                ImGui::SetScrollHereY(1.0f);
            }
        }

        ImGui::End();
    }
} // namespace Core
