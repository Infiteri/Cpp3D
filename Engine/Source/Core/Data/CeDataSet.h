#pragma once

#include "Base.h"
#include "Core/Data/CeData.h"
#include <string>
#include <unordered_map>

namespace Core
{
    class CE_API CeDataSet
    {
    private:
        std::unordered_map<std::string, CeData *> data; // todo: Depoint

    public:
        CeDataSet();
        ~CeDataSet();

        void Rename(const std::string &oldName, const std::string &newName);
        void Add(const std::string &name, CeDataType dt = CeDataType::None, void *data = nullptr);
        void Remove(const std::string &name);
        CeData *Get(const std::string &name);
        bool Exists(const std::string &name);

        inline std::unordered_map<std::string, CeData *> &GetDataSet() { return data; };

        template <typename T> T *As(const std::string &name)
        {
            auto data = Get(name);
            if (!data)
                return nullptr;
            else
                return data->As<T>();
        };
    };
} // namespace Core
