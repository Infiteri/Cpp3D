#pragma once

#include "Core/Serializer/CeSerializer.h"
#include "imgui.h"
#include <string>
#include <vector>

namespace Core
{
    struct ThemeSettings
    {
        struct ColorEdit
        {
            std::string Name;
            ImGuiCol Color;

            ColorEdit(const std::string &n, ImGuiCol c) : Name(n), Color(c) {};
        };

        std::vector<ColorEdit> Colors;
        bool Active = false;

        // hack: This is so hacky, will be true if the "Ok" button was pressed, meaning settings
        // were changed and need to be serialized
        void Render(bool &wantsSave);
    };

    struct CameraSettings
    {
        float FOV = 90;
        float NormalSpeed = 1.0f, FastSpeed = 2.0f, SlowSpeed = 0.5f;
    };

    struct GeneralSettings
    {
        CameraSettings Camera;

        bool Active = false;
        void Render(bool &wantsSave);
    };

    struct EditorSettings
    {
        ThemeSettings Theme;
        GeneralSettings General;

        void RegisterThemeColors();
    };

    class EditorSettingsSerializer : public CeSerializer
    {
    private:
        EditorSettings *settings;

    public:
        EditorSettingsSerializer(EditorSettings *target);

        void Serialize(const std::string &path);
        void Deserialize(const std::string &path);
    };

} // namespace Core
