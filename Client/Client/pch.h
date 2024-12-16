#pragma once

#include <windows.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <format>
#include <filesystem>
#include <functional>  // For Callback Function
#include <cassert>

using namespace std;
namespace fs = std::filesystem;

#include "Types.h"
#include "Defines.h"
#include "Enums.h"
#include "Utils.h"
#include "Constants.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#pragma comment(lib, "msimg32.lib")  // TransparentBlt 함수 사용을 위해 필요한 라이브러리

// 자체 지원 Sound System
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")