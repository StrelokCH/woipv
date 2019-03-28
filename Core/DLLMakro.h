#pragma once

#ifdef BUILD_CORE
/*Enabled as "export" while compiling the dll project*/
#define CORE_API __declspec(dllexport)
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define CORE_API __declspec(dllimport)
#endif