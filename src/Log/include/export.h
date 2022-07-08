#pragma once

#ifdef _WIN32
#if defined ANT_BATTLE_LOG_PUBLIC_DLL
#define ANT_BATTLE_LOG_EXPORT __declspec(dllexport)
#else
#define ANT_BATTLE_LOG_EXPORT __declspec(dllimport)
#endif
#else
#define ANT_BATTLE_LOG_PUBLIC_EXPORT
#endif // _WIN32
