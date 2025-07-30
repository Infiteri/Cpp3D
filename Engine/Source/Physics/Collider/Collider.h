#pragma once

namespace Core
{
    enum class ColliderType
    {
        None,
        Box
    };

    struct ColliderConfig
    {
        ColliderType Type = ColliderType::None;
        union
        {
            struct
            {
                float NONE[3];
            } NoneType;

            struct
            {
                float Width = 1, Height = 1, Depth = 1;
            } BoxType;
        };

        ColliderConfig()
        {
            Type = ColliderType::None;
            BoxType.Width = BoxType.Height = BoxType.Depth = 0;
        };
    };
} // namespace Core
