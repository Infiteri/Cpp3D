#include "EditorKeybind.h"
#include "Base.h"
#include "Core/Logger.h"
#include "EditorLayer.h"

namespace Core
{
    void EditorKeybind::Update() {}

    void EditorKeybind::OnKeyDown(Keys key)
    {
        auto inst = EditorLayer::GetInstance();
        CE_VERIFY(inst);

        bool ctrl = Input::GetKey(Keys::LeftControl);
        bool shift = Input::GetKey(Keys::LeftShift);
        bool alt = Input::GetKey(Keys::LeftAlt);

        switch (key)
        {
        case Keys::S:
        {
            if (ctrl)
            {
                if (shift)
                    inst->SceneSaveAs();
                else
                    inst->SceneSave();
            }
        }
        break;

        case Keys::O:
        {
            if (ctrl)
                inst->SceneOpen();
        }
        break;

        case Keys::N:
        {
            if (ctrl)
                inst->SceneNew();
        }
        break;

        default:
            break;
        }
    }
} // namespace Core
