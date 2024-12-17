#pragma once

#include <iostream>
#include <vector>  // & Stack
#include <list>
#include <queue>   // replace to deque
#include <unordered_map>  // & map
#include <unordered_set>  // & set

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <windows.h>

using namespace std;

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"

#pragma comment(lib, "ws2_32.lib")