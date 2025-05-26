#pragma once

#include "Base.h"
#include "Platform/Platform.h"
#include <string>
#include <unordered_map>

namespace Core
{
    class CE_API Logger
    {
    public:
        enum Level
        {
            Info,
            Warn,
            Error,
            Debug,
            Trace
        };

        struct Category
        {
            std::string Name;
            std::string Prefix;
        };

        struct LogMessage
        {
            std::string Message;
            std::string Category;
            Level Level;
        };

        struct State
        {
            std::unordered_map<std::string, Logger::Category> Categories;
            std::unordered_map<Level, Platform::ConsoleColor> ConsoleColor;
            std::unordered_map<Level, std::string> LevelString;
            std::vector<LogMessage> LogMessages;
        };

    public:
        Logger() {};
        ~Logger() {};

        static void Init();
        static void Shutdown();

        static std::vector<LogMessage> &GetLogMessages();

        static Category &AddCategory(const std::string &name, const std::string &prefix);

        static void Log(const std::string &category, Level level, const char *message, ...);
    };
} // namespace Core

/// MACROS
#define CE_ERROR(msg, ...) Core::Logger::Log("CE_CORE", Core::Logger::Error, msg, ##__VA_ARGS__)

#ifdef CE_NO_LOGGING
#else
#define CE_LOG(category, level, name, ...)                                                         \
    Core::Logger::Log(category, Core::Logger::level, name, ##__VA_ARGS__)
#define CE_INFO(msg, ...) Core::Logger::Log("CE_CORE", Core::Logger::Info, msg, ##__VA_ARGS__)
#define CE_WARN(msg, ...) Core::Logger::Log("CE_CORE", Core::Logger::Warn, msg, ##__VA_ARGS__)
#define CE_TRACE(msg, ...) Core::Logger::Log("CE_CORE", Core::Logger::Trace, msg, ##__VA_ARGS__)
#define CE_DEBUG(msg, ...) Core::Logger::Log("CE_CORE", Core::Logger::Debug, msg, ##__VA_ARGS__)

#define CE_DEFINE_LOG_CATEGORY(name, prefix) Core::Logger::AddCategory(name, prefix)
#define CE_DEFINE_LOG_CATEGORY_SIMPLE(name) Core::Logger::AddCategory(name, name)
#endif
