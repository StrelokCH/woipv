#pragma once

#ifdef GUROBI_EXPORTS
/*Enabled as "export" while compiling the dll project*/
#define GUROBI_API __declspec(dllexport)
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define GUROBI_API __declspec(dllimport)
#endif
