#include "CeDataSet.h"

namespace Core
{
    CeDataSet::CeDataSet() {}

    CeDataSet::~CeDataSet()
    {
        for (auto d : data)
            delete d.second;

        data.clear();
    }

    void CeDataSet::Rename(const std::string &oldName, const std::string &newName)
    {
        if (!Exists(oldName))
        {
            return;
        }

        CeData *d = data[oldName];
        data.erase(oldName);
        data[newName] = d;
        d->SetName(newName);
    }

    void CeDataSet::Add(const std::string &name, CeDataType dt, void *dataPtr)
    {
        if (Exists(name))
        {
            // todo: Make it so that the data changes to the new data if wanted
            // todo: Think lol
            return;
        }

        CeData *ceData = new CeData(name, dt, dataPtr);
        data[name] = ceData;
    }

    void CeDataSet::Add(CeData *ceData, const std::string &name)
    {
        if (Exists(name))
            return;

        data[name] = ceData;
    }

    void CeDataSet::Remove(const std::string &name)
    {
        if (!Exists(name))
            return;

        delete data[name];
        data.erase(name);
    }

    CeData *CeDataSet::Get(const std::string &name)
    {
        if (!Exists(name))
            return nullptr;
        return data[name];
    }

    bool CeDataSet::Exists(const std::string &name) { return data.find(name) != data.end(); }

} // namespace Core
