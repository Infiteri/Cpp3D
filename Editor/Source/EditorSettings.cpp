#include "EditorSettings.h"
#include "Base.h"
#include "Core/Engine.h"
#include "Core/Layer/ImGuiLayer.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializer.h"
#include "EditorTextureSystem.h"
#include "EditorUtils.h"

#include "Platform/Platform.h"
#include "imgui.h"
#include "yaml-cpp/emitter.h"
#include "yaml-cpp/emittermanip.h"

#include <yaml-cpp/yaml.h>

#define CE_DEFINE_COLOR_EDITABLE(name, color) Theme.Colors.push_back({name, color})

namespace Core
{
    void EditorSettings::RegisterThemeColors()
    {
        Theme.Colors.clear();

        // Text & Background
        CE_DEFINE_COLOR_EDITABLE("Text", ImGuiCol_Text);
        CE_DEFINE_COLOR_EDITABLE("TextDisabled", ImGuiCol_TextDisabled);
        CE_DEFINE_COLOR_EDITABLE("WindowBg", ImGuiCol_WindowBg);
        CE_DEFINE_COLOR_EDITABLE("ChildBg", ImGuiCol_ChildBg);
        CE_DEFINE_COLOR_EDITABLE("PopupBg", ImGuiCol_PopupBg);
        CE_DEFINE_COLOR_EDITABLE("Border", ImGuiCol_Border);
        CE_DEFINE_COLOR_EDITABLE("BorderShadow", ImGuiCol_BorderShadow);

        // Frame & Widgets
        CE_DEFINE_COLOR_EDITABLE("FrameBg", ImGuiCol_FrameBg);
        CE_DEFINE_COLOR_EDITABLE("FrameBgHovered", ImGuiCol_FrameBgHovered);
        CE_DEFINE_COLOR_EDITABLE("FrameBgActive", ImGuiCol_FrameBgActive);

        // Title Bar
        CE_DEFINE_COLOR_EDITABLE("TitleBg", ImGuiCol_TitleBg);
        CE_DEFINE_COLOR_EDITABLE("TitleBgActive", ImGuiCol_TitleBgActive);
        CE_DEFINE_COLOR_EDITABLE("TitleBgCollapsed", ImGuiCol_TitleBgCollapsed);

        // Menu Bar
        CE_DEFINE_COLOR_EDITABLE("MenuBarBg", ImGuiCol_MenuBarBg);

        // Scrollbar
        CE_DEFINE_COLOR_EDITABLE("ScrollbarBg", ImGuiCol_ScrollbarBg);
        CE_DEFINE_COLOR_EDITABLE("ScrollbarGrab", ImGuiCol_ScrollbarGrab);
        CE_DEFINE_COLOR_EDITABLE("ScrollbarGrabHovered", ImGuiCol_ScrollbarGrabHovered);
        CE_DEFINE_COLOR_EDITABLE("ScrollbarGrabActive", ImGuiCol_ScrollbarGrabActive);

        // Checkboxes, RadioButtons
        CE_DEFINE_COLOR_EDITABLE("CheckMark", ImGuiCol_CheckMark);

        // Sliders
        CE_DEFINE_COLOR_EDITABLE("SliderGrab", ImGuiCol_SliderGrab);
        CE_DEFINE_COLOR_EDITABLE("SliderGrabActive", ImGuiCol_SliderGrabActive);

        // Buttons
        CE_DEFINE_COLOR_EDITABLE("Button", ImGuiCol_Button);
        CE_DEFINE_COLOR_EDITABLE("ButtonHovered", ImGuiCol_ButtonHovered);
        CE_DEFINE_COLOR_EDITABLE("ButtonActive", ImGuiCol_ButtonActive);

        // Headers (Collapsing headers, etc.)
        CE_DEFINE_COLOR_EDITABLE("Header", ImGuiCol_Header);
        CE_DEFINE_COLOR_EDITABLE("HeaderHovered", ImGuiCol_HeaderHovered);
        CE_DEFINE_COLOR_EDITABLE("HeaderActive", ImGuiCol_HeaderActive);

        // Separators
        CE_DEFINE_COLOR_EDITABLE("Separator", ImGuiCol_Separator);
        CE_DEFINE_COLOR_EDITABLE("SeparatorHovered", ImGuiCol_SeparatorHovered);
        CE_DEFINE_COLOR_EDITABLE("SeparatorActive", ImGuiCol_SeparatorActive);

        // Resize Grip
        CE_DEFINE_COLOR_EDITABLE("ResizeGrip", ImGuiCol_ResizeGrip);
        CE_DEFINE_COLOR_EDITABLE("ResizeGripHovered", ImGuiCol_ResizeGripHovered);
        CE_DEFINE_COLOR_EDITABLE("ResizeGripActive", ImGuiCol_ResizeGripActive);

        // Tabs
        CE_DEFINE_COLOR_EDITABLE("Tab", ImGuiCol_Tab);
        CE_DEFINE_COLOR_EDITABLE("TabHovered", ImGuiCol_TabHovered);
        CE_DEFINE_COLOR_EDITABLE("TabActive", ImGuiCol_TabActive);
        CE_DEFINE_COLOR_EDITABLE("TabUnfocused", ImGuiCol_TabUnfocused);
        CE_DEFINE_COLOR_EDITABLE("TabUnfocusedActive", ImGuiCol_TabUnfocusedActive);

        // Docking (optional)
        CE_DEFINE_COLOR_EDITABLE("DockingPreview", ImGuiCol_DockingPreview);
        CE_DEFINE_COLOR_EDITABLE("DockingEmptyBg", ImGuiCol_DockingEmptyBg);

        // Plotting
        CE_DEFINE_COLOR_EDITABLE("PlotLines", ImGuiCol_PlotLines);
        CE_DEFINE_COLOR_EDITABLE("PlotLinesHovered", ImGuiCol_PlotLinesHovered);
        CE_DEFINE_COLOR_EDITABLE("PlotHistogram", ImGuiCol_PlotHistogram);
        CE_DEFINE_COLOR_EDITABLE("PlotHistogramHovered", ImGuiCol_PlotHistogramHovered);

        // Table
        CE_DEFINE_COLOR_EDITABLE("TableHeaderBg", ImGuiCol_TableHeaderBg);
        CE_DEFINE_COLOR_EDITABLE("TableBorderStrong", ImGuiCol_TableBorderStrong);
        CE_DEFINE_COLOR_EDITABLE("TableBorderLight", ImGuiCol_TableBorderLight);
        CE_DEFINE_COLOR_EDITABLE("TableRowBg", ImGuiCol_TableRowBg);
        CE_DEFINE_COLOR_EDITABLE("TableRowBgAlt", ImGuiCol_TableRowBgAlt);

        // Drag & Drop
        CE_DEFINE_COLOR_EDITABLE("DragDropTarget", ImGuiCol_DragDropTarget);

        // Navigation Highlight
        CE_DEFINE_COLOR_EDITABLE("NavHighlight", ImGuiCol_NavHighlight);
        CE_DEFINE_COLOR_EDITABLE("NavWindowingHighlight", ImGuiCol_NavWindowingHighlight);
        CE_DEFINE_COLOR_EDITABLE("NavWindowingDimBg", ImGuiCol_NavWindowingDimBg);

        // Modal
        CE_DEFINE_COLOR_EDITABLE("ModalWindowDimBg", ImGuiCol_ModalWindowDimBg);
    }

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

        ImGui::SameLine();

        if (ImGui::Button("Ok"))
        {
            Active = false;
            wantsSave = true;
        }
        ImGui::End();
    }

    static void StringDragDrop(std::string &str)
    {
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("ContentPanelDragDrop"))
            {
                str = (const char *)payload->Data;
            }

            ImGui::EndDragDropTarget();
        }
    }

    static void FontFileButton(std::string &str, int id = 0)
    {
        ImGui::SameLine();

        ImGui::PushID(id);
        const ImVec2 size{10, 10};

        if (ImGui::ImageButton(
                (void *)(u64)EditorTextureSystem::GetTexID("EngineAssets/Icons/folder.png"), size))
        {
            std::string src = Platform::OpenFileDialog(
                "Font Files (*.ttf;*.otf)\0*.ttf;*.otf\0All Files (*.*)\0*.*\0");
            if (!src.empty())
                str = src;
        }

        ImGui::PopID();
    }

    static void FontEditGUI(FontPack &pack, const std::string &type = "")
    {
        // note: INPUT_FIELD BUTTON INPUT_TEXT
        std::string ffs = type + " Font File";
        EditorUtils::ImGuiString(("##" + ffs).c_str(), pack.File);
        ImGui::SameLine();
        FontFileButton(pack.File);
        ImGui::SameLine();
        ImGui::Text(ffs.c_str());
        StringDragDrop(pack.File);
        ImGui::DragFloat((type + " Font Size").c_str(), &pack.Size, 0.01f, 0.01f);
    }

    void GeneralSettings::Render(bool &wantsSave)
    {
        wantsSave = false;
        if (!Active)
            return;

        ImGui::Begin("Settings");

        ImGui::SeparatorText("Fonts");

        FontEditGUI(MainFont, "Main");
        FontEditGUI(ToastFont, "Toast");

        ImGui::SeparatorText("Camera");

        ImGui::DragFloat("FOV", &Camera.FOV, 0.02f, 0.01f);
        ImGui::DragFloat("Normal Speed", &Camera.NormalSpeed, 0.02f, 0.01f);
        ImGui::DragFloat("Fast Speed", &Camera.FastSpeed, 0.02f, 0.01f);
        ImGui::DragFloat("Slow Speed", &Camera.SlowSpeed, 0.02f, 0.01f);
        ImGui::DragFloat("Mouse Sensitivity", &Camera.Sensitivity, 0.02f, 0.01f);

        if (ImGui::Button("Ok"))
        {
            wantsSave = true;
            Active = false;
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            Active = false;

        ImGui::End();
    }

    EditorSettingsSerializer::EditorSettingsSerializer(EditorSettings *target)
    {
        settings = target;
    }

    static void SerializeFontPack(FontPack &pack, const char *label, YAML::Emitter &out)
    {
        out << YAML::Key << label << YAML::BeginMap;
        CE_SERIALIZE_FIELD("Size", pack.Size);
        CE_SERIALIZE_FIELD("File", pack.File);
        out << YAML::EndMap;
    }

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

        out << YAML::Key << "Font" << YAML::Value << YAML::BeginMap;
        SerializeFontPack(settings->General.MainFont, "Main", out);
        SerializeFontPack(settings->General.ToastFont, "Toast", out);
        out << YAML::EndMap;

        out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap;
        CE_SERIALIZE_FIELD("FOV", settings->General.Camera.FOV);
        CE_SERIALIZE_FIELD("NormalSpeed", settings->General.Camera.NormalSpeed);
        CE_SERIALIZE_FIELD("FastSpeed", settings->General.Camera.FastSpeed);
        CE_SERIALIZE_FIELD("SlowSpeed", settings->General.Camera.SlowSpeed);
        CE_SERIALIZE_FIELD("Sensitivity", settings->General.Camera.Sensitivity);
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

        auto camera = data["EditorSettings"]["Camera"];
        if (!camera)
        {
            CE_LOG("CE_EDITOR", Error, "Unable to deserialize editor settings, no camera node.");
            return;
        }

        settings->General.Camera.FOV = camera["FOV"].as<float>();
        settings->General.Camera.NormalSpeed = camera["NormalSpeed"].as<float>();
        settings->General.Camera.FastSpeed = camera["FastSpeed"].as<float>();
        settings->General.Camera.SlowSpeed = camera["SlowSpeed"].as<float>();
        settings->General.Camera.Sensitivity = camera["Sensitivity"].as<float>();

        auto font = data["EditorSettings"]["Font"];
        if (!font)
        {
            CE_LOG("CE_EDITOR", Error, "Unable to deserialize editor settings, no font node.");
            return;
        }

        settings->General.MainFont.Size = font["Main"]["Size"].as<float>();
        settings->General.MainFont.File = font["Main"]["File"].as<std::string>();
        settings->General.ToastFont.Size = font["Toast"]["Size"].as<float>();
        settings->General.ToastFont.File = font["Toast"]["File"].as<std::string>();
    }
} // namespace Core
//
