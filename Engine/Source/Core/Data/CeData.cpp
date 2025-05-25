#include "CeData.h"
#include "Core/Logger.h"

#include "Renderer/Color.h"

#include "Math/Vector.h"

#define CE_DATA_CLEAR(dt, cast)                                                                    \
    case CeDataType::dt:                                                                           \
    {                                                                                              \
        cast *data = As<cast>();                                                                   \
        if (!data)                                                                                 \
            CE_WARN("CeData: Destroying type %i but cast is invalid", (int)type);                  \
        else                                                                                       \
            delete data;                                                                           \
    }                                                                                              \
    break

#define CE_DATA_SETUP(dt, cast)                                                                    \
    case CeDataType::dt:                                                                           \
    {                                                                                              \
        data = new cast();                                                                         \
    }                                                                                              \
    break

namespace Core
{
    CeData::CeData() {}

    CeData::CeData(const std::string &name, CeDataType dt, void *data)
        : name(name), type(dt), data(data)
    {
    }

    CeData::~CeData() { _DestroyOnType(); }

    void CeData::SetName(const std::string &name) { this->name = name; }

    void CeData::SetType(CeDataType dt) { type = dt; }

    void CeData::_DestroyOnType()
    {
        if (type == CeDataType::None)
        {
            if (data != nullptr)
            {
                CE_ERROR("CeData: Cannot clear data, data type is None but Data is not empty");
                return;
            }

            return;
        }

        switch (type)
        {
            CE_DATA_CLEAR(Vector2, Vector2);
            CE_DATA_CLEAR(Vector3, Vector3);
            CE_DATA_CLEAR(Vector4, Vector4);
            CE_DATA_CLEAR(Color, Color);
            CE_DATA_CLEAR(Float, float);

        default:
            CE_WARN("CeData: Destroying unhandled data type %i", (int)type);
            break;
        }

        data = nullptr;
    }

    void CeData::_SetupDefaultOnType()
    {
        if (type == CeDataType::None)
            return;

        switch (type)
        {
            CE_DATA_SETUP(Vector2, Vector2);
            CE_DATA_SETUP(Vector3, Vector3);
            CE_DATA_SETUP(Vector4, Vector4);
            CE_DATA_SETUP(Color, Color);
            CE_DATA_SETUP(Float, float);

        default:
            CE_WARN("CeData: Setting up unhandled data type %i", (int)type);
            break;
        }
    }
} // namespace Core
