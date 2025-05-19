#include "EditorSettings.h"
#include "Base.h"
#include "Core/Engine.h"
#include "Core/Layer/ImGuiLayer.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "imgui.h"
#include "yaml-cpp/emitter.h"
#include "yaml-cpp/emittermanip.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    void ThemeSettings::Render(bool &wantsSave)
    {
        wantsSave = false;
        if (!Active)
            return;

        float margin = 50.0f;

        ImGui::SetNextWindowPos({margin, margin});
        ImGui::SetNextWindowSize({Engine::GetWindow()->GetWidth() - (margin + 5),
                                  Engine::GetWindow()->GetHeight() - (margin + 5)});

        ImGui::Begin("Theme Settings");

        for (auto &it : Colors)
        {
            auto &colors = ImGui::GetStyle().Colors;
            int target = it.Color;

            float data[4] = {colors[target].x, colors[target].y, colors[target].z,
                             colors[target].w};
            if (ImGui::ColorEdit4(it.Name.c_str(), data))
            {
                colors[target].x = data[0];
                colors[target].y = data[1];
                colors[target].z = data[2];
                colors[target].w = data[3];
            }
        }
        if (ImGui::Button("Reset"))
        {
            wantsSave = true;
            ImGuiLayer::SetDefaultTheme();
        }
        ImGui::Dummy(ImVec2(0, 0));
        ImVec2 buttonSize = ImVec2(80, ImGui::GetFrameHeightWithSpacing());
        ImVec2 buttonPos =
            ImVec2(ImGui::GetWindowSize().x - buttonSize.x - ImGui::GetStyle().FramePadding.x * 2,
                   ImGui::GetWindowSize().y - buttonSize.y - ImGui::GetStyle().FramePadding.y * 2);

        ImGui::SetCursorPos(buttonPos);

        if (ImGui::Button("Ok", buttonSize))
        {
            Active = false;
            wantsSave = true;
        }
        ImGui::End();
    }

    EditorSettingsSerializer::EditorSettingsSerializer(EditorSettings *target)
    {
        settings = target;
    }

    // static void SerializeImGuiColor(YAML::Emitter& out, )

    void EditorSettingsSerializer::Serialize(const std::string &path)
    {
        CE_VERIFY(settings);

        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Key << "EditorSettings" << YAML::Value << YAML::BeginMap;

        out << YAML::Key << "Color" << YAML::Value << YAML::BeginMap;

        for (auto &it : settings->Theme.Colors)
        {
            auto &color = ImGui::GetStyle().Colors[it.Color];

            out << YAML::Key << it.Name.c_str() << YAML::Value << YAML::Flow;
            out << YAML::BeginSeq << color.x << color.y << color.z << color.w << YAML::EndSeq;
        }

        out << YAML::EndMap;
        out << YAML::EndMap;
        out << YAML::EndMap;

        _SaveEmitter(out, path);
    }

    void EditorSettingsSerializer::Deserialize(const std::string &path)
    {
        CE_VERIFY(settings);

        auto data = _LoadEmitterData(path);

        if (!data)
        {
            CE_LOG("CE_EDITOR", Error,
                   "Unable to deserialize editor settings, '%s' file format is wrong",
                   path.c_str());
            return;
        }

        if (!data["EditorSettings"])
        {
            CE_LOG("CE_EDITOR", Error, "Unable to deserialize editor settings, no root node.");
            return;
        }

        auto colors = data["EditorSettings"]["Color"];
        if (!colors)
        {
            CE_LOG("CE_EDITOR", Error, "Unable to deserialize editor settings, no color node.");
            return;
        }

        for (auto it : settings->Theme.Colors)
        {
            const std::string &name = it.Name;
            if (!colors[name])
                continue;

            auto colorNode = colors[name];
            if (colorNode.IsSequence() && colorNode.size() == 4)
            {
                auto &color = ImGui::GetStyle().Colors[it.Color];
                color.x = colorNode[0].as<float>();
                color.y = colorNode[1].as<float>();
                color.z = colorNode[2].as<float>();
                color.w = colorNode[3].as<float>();
            }
        }
    }
} // namespace Core
