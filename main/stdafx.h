// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#ifndef ARDUINO
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#pragma comment (lib, "Ws2_32.lib")
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Synchapi.h>
#include <fstream>
#endif
#include <cstring>    // sizeof()
#include <iostream>
#include <string>   
#include <stdlib.h>
#include <stdio.h>
//#include <tchar.h>
#include <cstdint>

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

// TODO: reference additional headers your program requires here
