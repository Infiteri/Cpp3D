#include "Library.h"

#ifdef CE_WIN32

#include <Windows.h>

namespace Core
{
    Library::Library() {}

    Library::Library(const std::string &name) { Create(name); }

    Library::~Library() { Destroy(); }

    void Library::Create(const std::string &name)
    {
        if (valid)
            Destroy();

        this->name = name;
        valid = false;
        mod = nullptr;

        mod = LoadLibrary(name.c_str());
        if (mod == nullptr)
        {
            valid = false;
            return;
        }

        valid = true;
    }

    void Library::Destroy()
    {
        if (!valid || !mod)
            return;

        FreeLibrary(mod);
    }

    void Library::LoadFunction(const std::string &name)
    {
        if (this->name.empty() || !valid || !mod)
            return;

        if (functions.find(name) != functions.end())
            return;

        FARPROC f_addr = GetProcAddress(mod, name.c_str());
        if (!f_addr)
            return;

        functions[name] = std::make_unique<Function>();
        functions[name]->Name = name;
        functions[name]->PFN = (void *)f_addr;
    }

} // namespace Core

#endif