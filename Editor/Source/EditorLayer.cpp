#include "EditorLayer.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Material/MaterialSystem.h"
#include "Renderer/Renderer.h"

#include <imgui.h>

namespace Core
{
    static float speed = 3.0f;
    static float sensitivity = 0.005;

    static void UpdateCameraMovement()
    {
        auto cam = CameraSystem::GetActivePerspective();
        CE_VERIFY(cam);

        float speedActual = speed;
        Vector3 rotation = cam->GetRotation();

        if (Input::GetKey(Keys::W))
        {
            Matrix4 rotation = Matrix4::RotateZYX(cam->GetRotation());
            Vector3 way = Matrix4::Forward(rotation);
            cam->GetPosition() += way * speedActual;
            cam->UpdateView();
        }

        if (Input::GetKey(Keys::S))
        {
            Matrix4 rotation = Matrix4::RotateZYX(cam->GetRotation());
            Vector3 way = Matrix4::Forward(rotation);
            cam->GetPosition() -= way * speedActual;
            cam->UpdateView();
        }

        if (Input::GetKey(Keys::A))
        {
            Matrix4 rotation = Matrix4::RotateZYX(cam->GetRotation());
            Vector3 way = Matrix4::Right(rotation);
            cam->GetPosition() -= way * speedActual;
            cam->UpdateView();
        }

        if (Input::GetKey(Keys::D))
        {
            Matrix4 rotation = Matrix4::RotateZYX(cam->GetRotation());
            Vector3 way = Matrix4::Right(rotation);
            cam->GetPosition() += way * speedActual;
            cam->UpdateView();
        }
        if (Input::GetKey(Keys::J))
        {
            cam->GetRotation().y -= speed;
            cam->UpdateView();
        }
        if (Input::GetKey(Keys::L))
        {
            cam->GetRotation().y += speed;
            cam->UpdateView();
        }
        if (Input::GetKey(Keys::I))
        {
            cam->GetRotation().x += speed;
            cam->UpdateView();
        }
        if (Input::GetKey(Keys::K))
        {
            cam->GetRotation().x -= speed;
            cam->UpdateView();
        }

        Vector2 delta = Input::GetMouseDelta();
        if (delta.x != 0 || delta.y != 0)
        {
            cam->GetRotation().y += delta.x * sensitivity;
            cam->GetRotation().x -= delta.y * sensitivity;

            float clamp = 90 * (3.14 / 180);
            if (cam->GetRotation().x < -clamp)
                cam->GetRotation().x = -clamp;
            if (cam->GetRotation().x > clamp)
                cam->GetRotation().x = clamp;

            cam->UpdateView();
        }
    }

    void EditorLayer::OnInit() { CE_TRACE("Hello world from layer"); }

    void EditorLayer::OnDestroy() {}

    void EditorLayer::OnUpdate()
    {
        if (Input::GetMouseMode() == MouseModes::Locked)
            UpdateCameraMovement();

        if (Input::GetKey(Keys::G))
            Input::SetMouseMode(MouseModes::Locked);

        if (Input::GetKey(Keys::H))
            Input::SetMouseMode(MouseModes::Visible);
    }

    static bool Vec3(Vector3 *v, const char *label)
    {
        float data[3] = {v->x, v->y, v->z};
        if (ImGui::DragFloat3(label, data, 0.05))
        {
            v->Set(data[0], data[1], data[2]);
            return true;
        }
        return false;
    }

    void EditorLayer::OnImGuiRender()
    {
        ImGui::Begin("Hello world");
        Color *c = &Renderer::GetColor();
        float data[3] = {c->r / 255, c->g / 255, c->b / 255};
        if (ImGui::ColorEdit3("BG", data))
        {
            c->r = data[0] * 255;
            c->g = data[1] * 255;
            c->b = data[2] * 255;
        }

        c = &Renderer::GetMesh()->GetMaterial()->GetColor();
        data[0] = c->r / 255;
        data[1] = c->g / 255;
        data[2] = c->b / 255;
        if (ImGui::ColorEdit3("Mesh", data))
        {
            c->r = data[0] * 255;
            c->g = data[1] * 255;
            c->b = data[2] * 255;
        }

        auto camera = CameraSystem::GetActivePerspective();
        if (Vec3(&camera->GetPosition(), "Position") || Vec3(&camera->GetRotation(), "Rotation"))
            camera->UpdateView();
        ImGui::End();
    }
} // namespace Core
