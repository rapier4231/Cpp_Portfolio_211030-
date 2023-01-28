#ifndef __ENGINE_DEFINE_H__
#define __ENGINE_DEFINE_H__

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <functional>
#include <ctime>

#include "Engine_Macro.h"
#include "Engine_Function.h"
#include "Engine_Enum.h"
#include "Engine_Typedef.h"
#include "Engine_Struct.h"

#pragma warning(disable : 4251)

// 오브젝트 Update 결과
#define NO_EVENT				0
#define OBJECT_DEAD				1
#define DINO_TAMED				2

// 다이렉트 인풋 디바이스
#define DIRECTINPUT_VERSION	0x0800
#include <dinput.h>

// Shadow 관련
#define SHADOW_WIDTH		4096
#define SHADOW_HEIGHT		4096

#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
using namespace Engine;

#endif // !__ENGINE_DEFINE_H__
