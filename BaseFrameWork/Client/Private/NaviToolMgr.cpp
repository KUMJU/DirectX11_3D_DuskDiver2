#include "pch.h"
#include "NaviToolMgr.h"
#include "Cell.h"
#include "GameInstance.h"

#include "Shader.h"
#include "Transform.h"

#include <fstream>
#include <iostream>

IMPLEMENT_SINGLETON(CNaviToolMgr)

CNaviToolMgr::CNaviToolMgr()
{
}

CNaviToolMgr::~CNaviToolMgr()
{
}

void CNaviToolMgr::Initialize()
{
	m_pDevice = CGameInstance::GetInstance()->GetDeviceInfo();
	m_pContext = CGameInstance::GetInstance()->GetDeviceContextInfo();
	m_pShader = CGameInstance::GetInstance()->GetShader(TEXT("Shader_Cell"));

}

void CNaviToolMgr::AddNewCell()
{
	//점과 점의 비교로 1차 sort
	SortPoint();
	//외적을 통한 면의 방향벡터로 2차 sort
	SortCross();
	 
	shared_ptr<CCell> pCell = CCell::Create(CGameInstance::GetInstance()->GetDeviceInfo(), CGameInstance::GetInstance()->GetDeviceContextInfo(), m_pPoints, (_uint)m_Cells.size());
	m_Cells.push_back(pCell);

	m_eCurrentOrder = EORDER::ORDER_FIRST;

}

void CNaviToolMgr::PickPoint(_float3 _fNewPickPoint)
{
	//점 보정

	//XMStoreFloat3(&_fNewPickPoint, XMVector3TransformCoord(XMLoadFloat3(&_fNewPickPoint), m_WorldMatirx));
	_float3 fNearPoint = FindNearestPoint(_fNewPickPoint);


	if (m_fMinDistance > 1.f) {
		fNearPoint = _fNewPickPoint;
	}

	switch (m_eCurrentOrder) 
	{ 
	case Client::CNaviToolMgr::ORDER_FIRST:

		m_pPoints[0] = fNearPoint;
		m_eCurrentOrder = EORDER::ORDER_SECOND;
		break;
	case Client::CNaviToolMgr::ORDER_SECOND:
		m_pPoints[1] = fNearPoint;
		m_eCurrentOrder = EORDER::ORDER_THIRD;
		break;
	case Client::CNaviToolMgr::ORDER_THIRD:
		m_pPoints[2] = fNearPoint;
		AddNewCell();
		break;
	case Client::CNaviToolMgr::ORDER_END:
		break;
	default:
		break;
	}

}

_float3 CNaviToolMgr::FindNearestPoint(_float3 _fPickPoint)
{
	//아직 셀 리스트가 비어있으면 그대로의 값을 반환해준다
	if (m_Cells.empty()) {
		return _fPickPoint;
	}

	_float fDistance = 9999999.f;
	_float3 vNearestPoint = _float3();
	_vector vPickPt = XMLoadFloat3(&_fPickPoint);

	for (auto& iter : m_Cells) {
		_float3* pCellPoints = iter->GetPoints();

		for (_int i = 0; i < 3; ++i) {

			_vector vCellPt = XMLoadFloat3(&pCellPoints[i]);
			vCellPt = vCellPt - vPickPt;
			
			_float fCurrentMin = XMVector3Length(vCellPt).m128_f32[0];

			if (fDistance > fCurrentMin) {
				vNearestPoint = pCellPoints[i];
				fDistance = fCurrentMin;
				m_fMinDistance = fCurrentMin;
			}
		}
	}

	return vNearestPoint;
}

void CNaviToolMgr::SortPoint()
{

	_float3 fTempPosition[3];

	memcpy(fTempPosition, m_pPoints, sizeof(_float3) * 3);

	// 1. X가 작은 순으로 우선 정렬
	for (_uint i = 0; i < 2; i++) {

		_uint iMinIdx = i;

		for (_uint j = i + 1; j < 3; ++j) {
			if (fTempPosition[i].x > fTempPosition[j].x) {
				iMinIdx = j;
			}
		}
	
		_float3 fTemp;
		fTemp = fTempPosition[i];
		fTempPosition[i] = fTempPosition[iMinIdx];
		fTempPosition[iMinIdx] = fTemp;
	}


	//2. 두번째와 세번째의 z를 비교해서 두번째의 z값이 더 작으면 순서를 바꿔준다

	if (fTempPosition[1].z < fTempPosition[2].z) {

		m_pPoints[0] = fTempPosition[0];
		m_pPoints[1] = fTempPosition[2];
		m_pPoints[2] = fTempPosition[1];

	}
	else {

		m_pPoints[0] = fTempPosition[0];
		m_pPoints[1] = fTempPosition[1];
		m_pPoints[2] = fTempPosition[2];

	}

}

void CNaviToolMgr::SortCross()
{
	_float3 fTempPosition[3];
	memcpy(fTempPosition, m_pPoints, sizeof(_float3) * 3);

	//첫번째 두번째
	_vector vLineAB = XMVectorSetY(XMLoadFloat3(&fTempPosition[1]),0.f) - XMVectorSetY(XMLoadFloat3(&fTempPosition[0]), 0.f);
	//세번째 첫번째
	_vector vLineAC = XMVectorSetY(XMLoadFloat3(&fTempPosition[2]), 0.f) - XMVectorSetY(XMLoadFloat3(&fTempPosition[0]), 0.f);
	_vector Cross = XMVector3Cross(vLineAB, vLineAC);

	//음수가 나올 경우 바꿔준다 
	if (Cross.m128_f32[1] <  0) {
		m_pPoints[1] = fTempPosition[2];
		m_pPoints[2] = fTempPosition[1];
	}
}

void CNaviToolMgr::CancleLast()
{
	shared_ptr<CCell> LastCell =  m_Cells.back();
	m_Cells.pop_back();
	m_eCurrentOrder = EORDER::ORDER_FIRST;
}

void CNaviToolMgr::Render()
{

	if (!m_pTerrainTransCom)
		return;

	_float4x4 identity;

	_matrix temp = XMMatrixIdentity();
	

	XMStoreFloat4x4(&identity, m_WorldMatirx);

	if (FAILED(m_pShader->BindMatrix("g_WorldMatrix", &identity)))
		return;

	_float4x4 ViewMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_VIEW);

	if (FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMat)))
		return;

	_float4x4 ProjMat = CGameInstance::GetInstance()->GetTransformFloat4x4(CPipeLine::D3DTS_PROJ);

	if (FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMat)))
		return;

	_float4 CamPos = CGameInstance::GetInstance()->GetCamPosition();
	_float4 vColor = _float4(0.f, 1.f, 0.f, 1.f);

	if (FAILED(m_pShader->BindRawValue("g_vColor", &vColor, sizeof(_float4))))
		return;

	m_pShader->Begin(0);


	for (auto& iter : m_Cells)
		iter->Render();
}

void CNaviToolMgr::SetTerrainTransform(shared_ptr<CTransform> _pTransform)
{
	
	m_pTerrainTransCom = _pTransform; 
	m_WorldMatirx = m_pTerrainTransCom->GetWorldMatrix();

}

HRESULT CNaviToolMgr::ParsingCell(const wstring& _strPath)
{

	ofstream fp(_strPath, ios::binary);

	//셀 포지션 
	for (auto& iter : m_Cells) {

 		_float3* pPoints =  iter->GetPoints();
		fp.write((char*)pPoints, sizeof(_float3) * 3);

	}

	fp.close();
	return S_OK;
}

HRESULT CNaviToolMgr::LoadData(const wstring& _strPath)
{
	ifstream fp(_strPath, ios::binary);


	for (_int i = 1; i < 165; ++i) {
		_float3 pPoint[3];
		fp.read((char*)pPoint, sizeof(_float3) * 3);

		
		shared_ptr<CCell> pCell = CCell::Create(m_pDevice, m_pContext, pPoint, m_Cells.size());
		m_Cells.push_back(pCell);
		
		 
		if (fp.eof())
			break;
	}

	fp.close();
	return S_OK;
}
