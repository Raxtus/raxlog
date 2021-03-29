#include <iostream>
#include <direct.h>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>
#include <time.h>
#include <queue>
#include <stdexcept>

#ifdef _DEBUG
#define CONF "Debug"
#else 
#define CONF "Release"
#endif

#ifdef _M_IX86
#define PLATFORM "x86"
#endif

#ifdef _M_AMD64
#define PLATFORM "x64"
#endif

#ifdef _M_ARM
#define PLATFORM "ARM"
#endif

#ifdef _M_ARM64
#define PLATFORM "ARM64"
#endif

