#include "Base.h"
#include "Core/Input.h"
#include "Core/Logger.h"
#include "Script/ActorScript.h"

class Test : public Core::ActorScript
{
public:
    void OnUpdate()
    {
        CE_VERIFY(Owner);

        auto &t = Owner->GetTransform().Position;
        if (Core::Input::GetKey(Core::Keys::A))
            t.x++;
    }
};

CE_DEFINE_ACTOR_SCRIPT(Test);
