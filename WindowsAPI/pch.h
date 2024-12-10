#pragma once

#include <windows.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <format>
#include <filesystem>

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