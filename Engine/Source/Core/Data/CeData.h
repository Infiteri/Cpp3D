#pragma once

#include "Base.h"
#include <string>

namespace Core
{
    enum class CeDataType
    {
        None = 0,
        Vector2,
        Vector3,
        Vector4,
        Color,
        Float
    };

    class CE_API CeData
    {
    private:
        std::string name;
        CeDataType type;
        void *data;

    public:
        CeData();
        CeData(const std::string &name, CeDataType dt = CeDataType::None, void *data = nullptr);
        ~CeData();

        // todo :Move some kind of wrapper make these private too unbothered now
        void _DestroyOnType();
        void _SetupDefaultOnType();

        void SetName(const std::string &name);
        void SetType(CeDataType dt);

        inline CeDataType GetType() { return type; };
        inline std::string GetName() { return name; };

        template <typename T> T *As() { return (T *)data; };
    };
} // namespace Core
