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

        operator u64() const { return id; };
        inline u64 Get() const { return id; };
        bool operator==(const UUID &other) const { return id == other.id; };
        bool operator!=(const UUID &other) const { return id != other.id; };
    };
} // namespace Core
