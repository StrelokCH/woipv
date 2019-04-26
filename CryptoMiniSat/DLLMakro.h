#pragma once

#ifdef CRYPTOMINISAT_EXPORTS
/*Enabled as "export" while compiling the dll project*/
#define CMS_API __declspec(dllexport)
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define CMS_API __declspec(dllimport)
#endif
