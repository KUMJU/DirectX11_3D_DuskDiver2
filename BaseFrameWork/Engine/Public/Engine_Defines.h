#pragma once

#pragma warning (disable : 4251)

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <DirectXCollision.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Effects11/d3dx11effect.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK\ScreenGrab.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
using namespace DirectX;

#include <Fmod/fmod.h>

#define MAX_BONE 512

namespace Engine
{
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };
}

#include "Engine_Function.h"
#include "Engine_Macro.h"
#include "Engine_Struct.h"
#include "Engine_Typedef.h"
using namespace Engine;

//ComPtr
#include <wrl.h>
namespace wrl = Microsoft::WRL;

//SmartPointer
#include <memory>
#include <iostream>
//STL
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <algorithm>
using namespace std;