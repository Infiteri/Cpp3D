#include "EditorCamera.h"
#include "Base.h"

#include "Core/Input.h"

#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Renderer.h"

#define CE_CAM_NAME "CeEditorCamera"

namespace Core
{
    EditorCamera::EditorCamera(PerspectiveCamera *target) : cam(target) {};

    void EditorCamera::SetTarget(PerspectiveCamera *target) { cam = target; }

    void EditorCamera::CreateEditorCamera()
    {
        CameraSystem::AddPerspectiveCamera(
            CE_CAM_NAME, {90, Renderer::GetViewport().GetAspect(), 0.01f, 1000.0f});
        CameraSystem::ActivatePerspectiveCamera(CE_CAM_NAME);
    }

    void EditorCamera::Update()
    {
        CE_VERIFY(cam);
        if (Input::GetButton(Buttons::Right))
        {
            Input::SetMouseMode(MouseModes::Locked);

            float realSpeed = Speed;
            if (Input::GetKey(Keys::LeftShift))
                realSpeed = FastSpeed;
            if (Input::GetKey(Keys::LeftControl))
                realSpeed = SlowSpeed;

            Vector3 rotation = cam->GetRotation();

            if (Input::GetKey(Keys::W))
            {
                Matrix4 rotation = Matrix4::RotateZYX(cam->GetRotation());
                Vector3 way = Matrix4::Forward(rotation);
                cam->GetPosition() += way * realSpeed;
                cam->UpdateView();
            }

            if (Input::GetKey(Keys::S))
            {
                Matrix4 rotation = Matrix4::RotateZYX(cam->GetRotation());
                Vector3 way = Matrix4::Forward(rotation);
                cam->GetPosition() -= way * realSpeed;
                cam->UpdateView();
            }

            if (Input::GetKey(Keys::A))
            {
                Matrix4 rotation = Matrix4::RotateZYX(cam->GetRotation());
                Vector3 way = Matrix4::Right(rotation);
                cam->GetPosition() -= way * realSpeed;
                cam->UpdateView();
            }

            if (Input::GetKey(Keys::D))
            {
                Matrix4 rotation = Matrix4::RotateZYX(cam->GetRotation());
                Vector3 way = Matrix4::Right(rotation);
                cam->GetPosition() += way * realSpeed;
                cam->UpdateView();
            }

            Vector2 delta = Input::GetMouseDelta();
            if (delta.x != 0 || delta.y != 0)
            {
                cam->GetRotation().y += delta.x * Sensitivity;
                cam->GetRotation().x -= delta.y * Sensitivity;

                float clamp = 90 * (3.14 / 180);
                if (cam->GetRotation().x < -clamp)
                    cam->GetRotation().x = -clamp;
                if (cam->GetRotation().x > clamp)
                    cam->GetRotation().x = clamp;

                cam->UpdateView();
            }
        }
        else
            Input::SetMouseMode(MouseModes::Visible);
    }
} // namespace Core
