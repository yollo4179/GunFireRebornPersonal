#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
/*for collision*/
#include <DirectXMath.h>
#include <DirectXCollision.h>
using namespace DirectX;

/*efffect is for shaderfile*/
/* ToolKit*/
#include "Effects11\d3dx11effect.h"
#include "DirectXTK\DDSTextureLoader.h"
#include "DirectXTK\WICTextureLoader.h"
/*ToolKit ForCollider*/
#include "DirectXTK\ScreenGrab.h"
#include "DirectXTK\PrimitiveBatch.h"
#include "DirectXTK\VertexTypes.h"
#include "DirectXTK\Effects.h"

#include "DirectXTK\SpriteBatch.h"
#include "DirectXTK\SpriteFont.h"


/*for modle*/
#include"assimp\scene.h"
#include"assimp\Importer.hpp"
#include"assimp\postprocess.h"

/*for shaderfimle compile*/
#include <d3dcompiler.h>

namespace Engine
{
	enum class PROTOTYPE { PROTO_GAMEOBJ, PROTO_COMPONENT };

	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
	const unsigned int g_iMaxWidth = 8192;
	const unsigned int g_iMaxHeight = 4608;
}

#include <string>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
#include<queue>
/*For Collision*/
#include<directxcollision.h>
#include<DirectXColors.h>
#include"DirectXTK/PrimitiveBatch.h"
#include"DirectXTK/VertexTypes.h"

using namespace std;

#include "Engine_Typedef.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Function.h"



/*for Event*/
#include<functional>


using namespace Engine;


enum class UI_TYPE
{
	UI_FIXED,//ui매니저가 관리한다.
	UI_MONSTER,//몬스터가 ui를 가진다, ui매니저에서는 관리하지 않고 몬스터가 죽으면 inactive 한다.(몬스터가 잠시 안맞거나)  sorting order는 8(in game) 
	UI_DEMAGEFONT,//몬스터에게 뜨는 UI -> UI매니저에서는 관리하지 않고 Sorting order는 9
	UI_POP, //ui매니저가 관리한다. 
	UI_END
};


#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif

#endif // _DEBUG

