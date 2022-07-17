#pragma once

#ifdef _WIN32
#if defined TESTLIBRARY_PUBLIC_DLL
#define TESTLIBRARY_PUBLIC_EXPORT __declspec(dllexport)
#else
#define TESTLIBRARY_PUBLIC_EXPORT __declspec(dllimport)
#endif
#else
#define TESTLIBRARY_PUBLIC_EXPORT
#endif // _WIN32
