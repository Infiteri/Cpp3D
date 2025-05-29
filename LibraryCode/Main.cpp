#include "Script/ActorScript.h"

#include <iostream>

class Test : public Core::ActorScript
{
public:
    void OnStart() { std::cout << "Hello world"; }
};

CE_DEFINE_ACTOR_SCRIPT(Test);
