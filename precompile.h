#ifndef PRECOMPILE_H_
#define PRECOMPILE_H_

// 基本
#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <tchar.h>

// STL
#include <vector>
#include <string>
#include <map>

// DirectX9
#include <d3dx9.h>

// imgui
#include "imgui.h"

// json
#include "nlohmann/json.hpp"

// デバッグ用
#include "debug_proc.h"

// input
#define	 DIRECTINPUT_VERSION (0x0800)
#include <dinput.h>
#include <xinput.h>

#endif // !PRECOMPILE_H_
