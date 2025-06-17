#pragma once

#if defined(_WIN32)
  #ifdef QRCODE_FINDER_BUILD
    #define QRCODE_FINDER_EXPORT __declspec(dllexport)
  #else
    #define QRCODE_FINDER_EXPORT __declspec(dllimport)
  #endif
#else
  #define QRCODE_FINDER_EXPORT
#endif
