#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	struct NAVI_DESC{
		_int iStartCellIndex;
	};

public:
	CNavigation(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext);
	virtual ~CNavigation() = default;

public:
	HRESULT Initialize(const wstring& _strNavigationDataFilePath);
	void Tick(_fmatrix _TerrainWorldMatrix);
	_bool IsMove(_fvector vPosition);


#ifdef _DEBUG
public:
	HRESULT Render();
#endif

private:
	_int m_iCurrentIndex = { -1 };
	vector<shared_ptr<class CCell>> m_Cells;
	static _float4x4 m_WorldMatrix;

#ifdef _DEBUG
private:
	shared_ptr<class CShader> m_pShader = { nullptr };
#endif

private:
	HRESULT MakeNeighbors();

public:
	static shared_ptr<CNavigation> Create(wrl::ComPtr<ID3D11Device> _pDevice, wrl::ComPtr<ID3D11DeviceContext> _pContext, const wstring& _strNavigationDataFilePath);

};

END