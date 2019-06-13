#pragma once

#ifdef BUILD_PARTITIONING
/*Enabled as "export" while compiling the dll project*/
#define PARTITIONINING_API __declspec(dllexport)
#else
/*Enabled as "import" in the Client side for using already created dll file*/
#define PARTITIONINING_API __declspec(dllimport)
#endif
