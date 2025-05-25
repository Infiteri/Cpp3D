#pragma once

#include "Base.h"
#include "Math/Matrix.h"
#include "Math/Vector.h"
#include "Renderer/Color.h"
#include <string>

namespace Core
{
    class Shader
    {
    private:
        u32 id;
        bool loaded = false;

        void CompileWithSources(const std::string &vs, const std::string &fs);

        u32 GetUniLoc(const char *n);

    public:
        Shader() = default;
        Shader(const std::string &filename);
        ~Shader();

        void Load(const std::string &filename);

        void Vec4(const Color &color, const char *name);

        void Vec3(const Vector3 &vec, const char *name);

        void Vec2(const Vector2 &vec, const char *name);

        void Mat4(const Matrix4 &matrix, const char *name);

        void Int(int i, const char *name);

        void Float(float f, const char *name);

        void Use();
        void Destroy();

        inline u32 GetID() { return id; };
    };
} // namespace Core
