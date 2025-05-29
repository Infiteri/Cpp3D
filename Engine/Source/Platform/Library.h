#pragma once

#include "Base.h"

#include <memory>
#include <string>
#include <unordered_map>

#ifdef CE_WIN32
#include <Windows.h>
#endif

namespace Core
{
    // todo: Think about API
    class CE_ED_API Library
    {
    public:
        struct Function
        {
            std::string Name;
            void *PFN;
        };

    private:
        bool valid = false;
        std::string name;
        std::unordered_map<std::string, std::unique_ptr<Function>> functions;

#ifdef CE_WIN32
        HMODULE mod;
#endif

    public:
        Library();
        Library(const std::string &name);
        ~Library();

        void LoadFunction(const std::string &name);

        template <typename T> T GetFunction(const std::string &name)
        {
            if (!valid)
                return NULL;

            if (functions.find(name) == functions.end())
            {
                LoadFunction(name);
                if (functions.find(name) == functions.end())
                    return nullptr;
            }

            return (T)functions[name]->PFN;
        };
        //
        void Create(const std::string &name);
        void Destroy();

        inline bool IsValid() { return valid; }
    };
} // namespace Core
