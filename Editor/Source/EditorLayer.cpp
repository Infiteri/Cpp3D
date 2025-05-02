#include "EditorLayer.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Renderer/Camera/CameraSystem.h"
#include "Renderer/Renderer.h"
#include "Scene/Components/Components.h"
#include "Scene/Scene.h"
#include "Scene/World.h"

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

    void EditorLayer::OnAttach()
    {
        CE_TRACE("Hello world from layer");

        Scene *scene = World::Create("Test");
        World::Activate("Test");

        auto test1 = scene->CreateActor("TTV");
        auto mesh = test1->AddComponent<MeshComponent>();
    }

    void EditorLayer::OnDetach() {}

    void EditorLayer::OnUpdate()
    {
        if (Input::GetMouseMode() == MouseModes::Locked)
            UpdateCameraMovement();

        if (Input::GetKey(Keys::G))
            Input::SetMouseMode(MouseModes::Locked);

        if (Input::GetKey(Keys::H))
            Input::SetMouseMode(MouseModes::Visible);
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

        ImGui::End();
    }
} // namespace Core
