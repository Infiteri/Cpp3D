#include <iostream>

extern "C" __declspec(dllexport) void Try() { std::cout << "try me"; };