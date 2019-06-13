// Copyright (C) 2018 Innovation 24, Aix-Marseille University, CNRS. All rights reserved.

#if !defined LIB_LOCAL
#if defined _WIN32
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define LIB_PUBLIC __attribute__ ((dllexport))
      #define LIB_LOCAL
    #else
      #define LIB_PUBLIC __declspec(dllexport) // Note: actually gcc seems to support this syntax too.
      #define LIB_LOCAL
    #endif
  #else
    #ifdef __GNUC__
      #define LIB_PUBLIC
      #define LIB_LOCAL
    #else
      #define LIB_PUBLIC // Note: actually gcc seems to support this syntax too.
      #define LIB_LOCAL
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define LIB_PUBLIC __attribute__ ((visibility ("default")))
    #define LIB_LOCAL __attribute__ ((visibility ("hidden")))
  #else
    #define LIB_PUBLIC
    #define LIB_LOCAL
  #endif
#endif
#endif
