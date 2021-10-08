#pragma once

#ifdef DLLEXPORT
#define DLLEXPORTCLASS	__declspec(dllexport)
#define DLLEXPORTAPI	__declspec(dllexport)
#else
#define DLLEXPORTCLASS	__declspec(dllimport)
#define DLLEXPORTAPI	__declspec(dllimport)
#endif