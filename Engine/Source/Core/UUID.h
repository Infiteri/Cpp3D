#pragma once

#include "Base.h"

namespace Core
{
    class CE_API UUID
    {
    private:
        u64 id;

    public:
        UUID();
        UUID(u64 id);
        UUID(const UUID &other);
        ~UUID() = default;

        operator u64() { return id; };
        inline u64 Get() { return id; };
        bool operator==(const UUID &other) { return id == other.id; };
        bool operator!=(const UUID &other) { return id != other.id; };
    };
} // namespace Core
