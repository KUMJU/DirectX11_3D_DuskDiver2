#pragma once

#include "Client_Defines.h"

BEGIN(Engine)

class CCell;
class CShader;
class CTransform;

END


BEGIN(Client)


class CNaviToolMgr
{
	DECLARE_SINGLETON(CNaviToolMgr)

private:
	CNaviToolMgr();
	~CNaviToolMgr();

public:
	void Initialize();


public:
	enum EORDER
	{
		ORDER_FIRST, ORDER_SECOND, ORDER_THIRD, ORDER_END
	};

public:

	//���ο� �� �߰�
	void AddNewCell();
	//��ŷ�� point�� �Ѱ��ش� 
	void PickPoint(_float3 _fNewPickPoint);
	//���� ��ŷ�� ����Ʈ�� ���� ����� ���� ã�� �������ش�
	_float3 FindNearestPoint(_float3 _fPickPoint);
	//�ð���� ����
	
	void SortPoint();
	void SortCross();

	void CancleLast();

public:
	void Render();
	void SetTerrainTransform(shared_ptr<CTransform> _pTransform);

	HRESULT ParsingCell(const wstring& _strPath);

private:
	vector<shared_ptr<class CCell>> m_Cells;
	_float3 m_pPoints[3];
	_float m_fMinDistance = 0.f;

	shared_ptr<CShader> m_pShader = nullptr;
	
	wrl::ComPtr<ID3D11Device> m_pDevice;
	wrl::ComPtr<ID3D11DeviceContext> m_pContext;

	EORDER m_eCurrentOrder = EORDER::ORDER_FIRST;

	shared_ptr<CTransform> m_pTerrainTransCom = nullptr;
	_matrix m_WorldMatirx;

};

END