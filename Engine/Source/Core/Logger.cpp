#include "Logger.h"
#include "Platform/Platform.h"

#include <cstdarg>
#include <iostream>
#include <string.h>

#define CE_TRACK_MESSAGE 1

namespace Core
{
    static Logger::State state;

    void Logger::Init()
    {
        CE_DEFINE_LOG_CATEGORY("CE_CORE", "Core");
        CE_DEFINE_LOG_CATEGORY("CE_USER", "User");

        state.ConsoleColor[Info] = Platform::ConsoleColor::Green;
        state.ConsoleColor[Warn] = Platform::ConsoleColor::Yellow;
        state.ConsoleColor[Error] = Platform::ConsoleColor::Red;
        state.ConsoleColor[Trace] = Platform::ConsoleColor::Gray;
        state.ConsoleColor[Debug] = Platform::ConsoleColor::Blue;

        state.LevelString[Info] = "Info";
        state.LevelString[Warn] = "Warn";
        state.LevelString[Error] = "Error";
        state.LevelString[Trace] = "Trace";
        state.LevelString[Debug] = "Debug";
    }

    void Logger::Shutdown() {}

    void Logger::Log(const std::string &category, Level level, const char *message, ...)
    {
        if (state.Categories.find(category) == state.Categories.end())
        {
            if (level == Error)
            {
                Platform::SetConsoleColor(Platform::ConsoleColor::Red);
                Platform::LogMessage(message);
            }
            return;
        }

        Category &cat = state.Categories[category];

        std::string log = state.LevelString[level];
        static const int MESSAGE_SIZE = 32000;

        char OutMessage[MESSAGE_SIZE];
        memset(OutMessage, 0, MESSAGE_SIZE);

        __builtin_va_list argPtr;
        va_start(argPtr, message);
        vsnprintf(OutMessage, MESSAGE_SIZE, message, argPtr);
        va_end(argPtr);

        char OutMessageWithLevels[MESSAGE_SIZE];
        memset(OutMessageWithLevels, 0, MESSAGE_SIZE);
        snprintf(OutMessageWithLevels, MESSAGE_SIZE, "[%s %s]: %s\n", cat.Prefix.c_str(),
                 log.c_str(), OutMessage);

        Platform::SetConsoleColor(state.ConsoleColor[level]);
        Platform::LogMessage(OutMessageWithLevels);

#if CE_TRACK_MESSAGE == 1
        state.LogMessages.push_back({OutMessageWithLevels, cat.Prefix, level});
#endif
    }

    Logger::Category &Logger::AddCategory(const std::string &name, const std::string &prefix)
    {
        Category &c = state.Categories[name];
        c.Name = name;
        c.Prefix = prefix;
        return c;
    }

    std::vector<Logger::LogMessage> &Logger::GetLogMessages() { return state.LogMessages; }

} // namespace Core
