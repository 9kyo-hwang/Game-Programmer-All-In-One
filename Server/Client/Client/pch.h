#pragma once

#include "CorePCH.h"  // ������ ��Ʈ��ũ ���̺귯���� "�ֿ켱" ����ž� ��
#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

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
#include <queue>

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

#pragma comment(lib, "msimg32.lib")  // TransparentBlt �Լ� ����� ���� �ʿ��� ���̺귯��

// ��ü ���� Sound System
#include <mmsystem.h>
#include <dsound.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dsound.lib")

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib)
#endif