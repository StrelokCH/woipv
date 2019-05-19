#pragma once

#ifdef SOLVERPORTFOLIO_EXPORTS
/*Enabled as "export" while compiling the dll project*/
#define PORTFOLIO_API __declspec(dllexport)
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define PORTFOLIO_API __declspec(dllimport)
#endif
