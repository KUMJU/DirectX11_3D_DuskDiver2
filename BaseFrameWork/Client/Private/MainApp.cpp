#include "pch.h"
#include "MainApp.h"

#include "GameInstance.h"
#include "LevelLoading.h"

#include "VIRect.h"
#include "Shader.h"

#include "ImguiMgr.h"
#include "Engine_Defines.h"

#include <fstream>

#include "Json/json.h"

CMainApp::CMainApp()
{
}

CMainApp::~CMainApp()
{
	Free();
}

void CMainApp::PreInit()
{
}

HRESULT CMainApp::Initialize()
{
	srand((unsigned)time(NULL));

	GRAPHIC_DESC GraphicDesc = {};

	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.isWindowed = true;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	if (FAILED(CGameInstance::GetInstance()->InitializeEngine(g_hInst, LEVEL_END, g_iWinSizeX, g_iWinSizeY, GraphicDesc)))
		return E_FAIL;

	CGameInstance::GetInstance()->LoadLevelResource(LEVEL_LOGO);

	if (FAILED(OpenLevel(LEVEL_LOGO)))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Tick(_float _fTimeDelta)
{
	CGameInstance::GetInstance()->TickEngine(_fTimeDelta);
}


HRESULT CMainApp::Render()
{

	if (FAILED(CGameInstance::GetInstance()->ClearBackBufferView(_float4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->ClearDepthStencilView()))
		return E_FAIL;

	if (FAILED(CGameInstance::GetInstance()->Draw()))
		return E_FAIL;

	if (LEVEL_EDIT == CGameInstance::GetInstance()->GetCurrentLevel()) {
		if (FAILED(CImguiMgr::GetInstance()->Render()))
			return E_FAIL;
	}

	if (FAILED(CGameInstance::GetInstance()->Present()))
		return E_FAIL;


	return S_OK;
}

HRESULT CMainApp::OpenLevel(LEVEL _eStartLevel)
{
	if (LEVEL_LOADING == _eStartLevel)
		return E_FAIL;

	return CGameInstance::GetInstance()->OpenLevel(LEVEL_LOADING, CLevelLoading::Create(_eStartLevel));
}

void CMainApp::TestFileWrite()
{
	const _tchar* szName = TEXT("Test.dat");

	vector<_float3> pVector = {};
	pVector.push_back(_float3(1.f, 2.f, 3.f));
	pVector.push_back(_float3(4.f, 5.f, 6.f));


	_ulong dwByte = 0;
	HANDLE hFile = CreateFile(szName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile) {
		MSG_BOX("Failed to Save");
		return;
	}

	for (_int i = 0; i < 2; ++i) {

		WriteFile(hFile, &pVector[i].x, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &pVector[i].y, sizeof(_float), &dwByte, nullptr);
		WriteFile(hFile, &pVector[i].z, sizeof(_float), &dwByte, nullptr);

	}

	CloseHandle(hFile);
}

void CMainApp::TestFileRead()
{
	const _tchar* szName = TEXT("Test.dat");

	_ulong dwByte = 0;
	HANDLE hFile = CreateFile(szName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	//파일x
	if (0 == hFile)
		return;

	//테스트용으로 벡터 2개 저장해보기

	_float fX, fY, fZ;

	vector<_float3> pVector = {};

	for (_int i = 0; i < 2; i++) {

		ReadFile(hFile, &fX, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &fY, sizeof(_float), &dwByte, nullptr);
		ReadFile(hFile, &fZ, sizeof(_float), &dwByte, nullptr);

		_float3 newFloat = _float3(fX, fY, fZ);
		pVector.push_back(newFloat);
	}

	CloseHandle(hFile);

}


void CMainApp::Free()
{
	CImguiMgr::DestroyInstance();
	CGameInstance::DestroyInstance();
}

//텍스쳐 생성, 공부용으로 넣어둔 코드고 앞으로 안 쓸것 같으니 대충 시간 지나면 지워둘 것
HRESULT CMainApp::ReadyGara()
{
	wrl::ComPtr<ID3D11DeviceContext> pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();
	wrl::ComPtr<ID3D11Device> pDevice = CGameInstance::GetInstance()->GetDeviceInfo();

	ID3D11Texture2D* pTexture2D = { nullptr };

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 256;
	TextureDesc.Height = 256;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	//텍스쳐 생성시에는 꼭 동적 생성을 해야함
	TextureDesc.Usage = D3D11_USAGE_DYNAMIC;
	TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TextureDesc.MiscFlags = 0;

	_ulong* pPixel = new _ulong[TextureDesc.Width * TextureDesc.Height];

	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint		iIndex = (_uint)i * TextureDesc.Width + (_uint)j;

			pPixel[iIndex] = D3DCOLOR_ARGB(255, 255, 0, 0);
		}
	}

	pPixel[0] = D3DCOLOR_ARGB(255, 255, 255, 0);

	D3D11_SUBRESOURCE_DATA		InitialData = {};

	InitialData.pSysMem = pPixel;
	InitialData.SysMemPitch = TextureDesc.Width * 4;

	if (FAILED(pDevice->CreateTexture2D(&TextureDesc, &InitialData, &pTexture2D)))
		return E_FAIL;

	for (size_t i = 0; i < TextureDesc.Height; i++)
	{
		for (size_t j = 0; j < TextureDesc.Width; j++)
		{
			_uint		iIndex = (_uint)i * TextureDesc.Width + (_uint)j;

			if (j < 128)
				pPixel[iIndex] = D3DCOLOR_ARGB(255, 255, 255, 255);
			else
				pPixel[iIndex] = D3DCOLOR_ARGB(255, 0, 0, 0);

		}
	}


	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	//Lock Unlock의 기능을 똑같이 수행하는 함수 
	//D3D11_MAP_WRITE_DISCARD : 해당 주소에 있던 기존에 있던 픽셀을 모두 초기화 시키고 채워주는 플래그
	//이미지(텍스쳐)를 생성할 때는 보통 이 플래그를 준다->속도가 빨라지기 때문에
	pContext->Map(pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	memcpy(SubResource.pData, pPixel, sizeof(_ulong) * TextureDesc.Width * TextureDesc.Height);

	pContext->Unmap(pTexture2D, 0);

	//if (FAILED(DirectX::SaveDDSTextureToFile(pContext.Get(), pTexture2D, TEXT("../Bin/Resources/Textures/Terrain/GaraMask.dds"))))
	//	return E_FAIL;

	delete[] pPixel;
	delete pTexture2D;

	return S_OK;
}