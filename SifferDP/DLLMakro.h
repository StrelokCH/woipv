#pragma once

#ifdef BUILD_SIFFERDP
/*Enabled as "export" while compiling the dll project*/
#define SIFFERDP_API __declspec(dllexport)
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define SIFFERDP_API __declspec(dllimport)
#endif
