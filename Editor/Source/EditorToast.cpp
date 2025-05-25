#include "EditorToast.h"

namespace Core
{
    static ToastMessage::State state;

    void ToastMessage::Add(const std::string &msg, float life)
    {
        state.Toasts.push_back({msg, life, life});
    }

    void ToastMessage::OnImGuiRender()
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

        ImDrawList *drawList = ImGui::GetForegroundDrawList();
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;

        float padding = 10.0f;
        float extraY = 50.0f;
        float y = padding + extraY;

        for (int i = 0; i < state.Toasts.size();)
        {
            Toast &toast = state.Toasts[i];
            toast.Lifetime -= ImGui::GetIO().DeltaTime;

            if (toast.Lifetime <= 0.0f)
            {
                state.Toasts.erase(state.Toasts.begin() + i);
                continue;
            }

            float alpha = toast.Lifetime / toast.TotalLifetime;

            // Text size
            ImVec2 textSize = ImGui::CalcTextSize(toast.Message.c_str());

            // Align to right side
            ImVec2 pos = ImVec2(screenSize.x - textSize.x - padding * 3, y);

            // Background
            ImU32 bgColor = ImGui::GetColorU32(ImVec4(0.1f, 0.1f, 0.1f, 0.8f * alpha));
            drawList->AddRectFilled(
                pos, ImVec2(pos.x + textSize.x + padding * 2, pos.y + textSize.y + padding),
                bgColor, 5.0f);

            // Text
            ImU32 textColor = ImGui::GetColorU32(ImVec4(1, 1, 1, alpha));
            drawList->AddText(ImVec2(pos.x + padding, pos.y + padding / 2), textColor,
                              toast.Message.c_str());

            y += textSize.y + padding * 1.5f;
            ++i;
        }

        ImGui::PopFont();
    }

} // namespace Core
