#include "pch.h"
#include "UI_SequenceTex.h"

#include "GameInstance.h"

#include "Shader.h"
#include "Texture.h"
#include "VIBuffer_UI.h"

CUI_SequenceTex::CUI_SequenceTex()
{
}

HRESULT CUI_SequenceTex::Initialize(CUI::tagUIInfo* _info, const wstring& _strTexKey, _int _iPriorityNum, SequenceTexInfo* _TexInfo)
{

	__super::Initialize(*_info, _iPriorityNum);
	m_SequenceInfo = *_TexInfo;

	CUI::AddBaseComponent();

	m_pTextureCom = CGameInstance::GetInstance()->GetTexture(_strTexKey);
	m_Components.emplace(TEXT("Com_Texture"), m_pTextureCom);

	m_IsEnabled = false;

	m_vRowUVRatio.x = 0.f;
	m_vRowUVRatio.y = (_float)(1.f) / (_float)m_SequenceInfo.iRow;

	m_vColUVRatio.x = 0.f;
	m_vColUVRatio.y = (_float)(1.f) / (_float)m_SequenceInfo.iCol;

	return S_OK;
}

void CUI_SequenceTex::PriorityTick(_float _fTimeDelta)
{
}

void CUI_SequenceTex::Tick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	if (m_SequenceInfo.fScrollTime != 0) {

		m_fAccTime += _fTimeDelta;

		if (m_fAccTime >= m_SequenceInfo.fScrollTime) {

			SlideNextTexIdx();
			m_fAccTime = 0.f;

		}

	}
}

void CUI_SequenceTex::LateTick(_float _fTimeDelta)
{
	if (!m_IsEnabled)
		return;

	__super::LateTick(_fTimeDelta);
}

HRESULT CUI_SequenceTex::Render()
{

	if (FAILED(m_pTransformCom->BindShaderResource(m_pShader, "g_WorldMatrix")))
		return E_FAIL;

	_float4x4 ViewMatrix, ProjMatrix;

	XMStoreFloat4x4(&ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	if(FAILED(m_pShader->BindMatrix("g_ViewMatrix", &ViewMatrix)))
		return E_FAIL;

	if(FAILED(m_pShader->BindMatrix("g_ProjMatrix", &ProjMatrix)))
		return E_FAIL;


 	if(FAILED(m_pShader->BindRawValue("g_fStartRowUV", &m_vRowUVRatio.x, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->BindRawValue("g_fEndRowUV", &m_vRowUVRatio.y, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->BindRawValue("g_fStartColUV", &m_vColUVRatio.x, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShader->BindRawValue("g_fEndColUV", &m_vColUVRatio.y, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->BindShaderResource(m_pShader, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShader->Begin(4)))
		return E_FAIL;

	if (FAILED(m_VIRectCom->BindBuffers()))
		return E_FAIL;

	if (FAILED(m_VIRectCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CUI_SequenceTex::SlideNextTexIdx()
{
	++m_iCurrentRow;

	//가로 끝까지 갔을 때 
	if (m_SequenceInfo.iRow == m_iCurrentRow) {
		m_iCurrentRow = 0;
		++m_iCurrentCol;

		if (m_SequenceInfo.iCol == m_iCurrentCol) {
			m_iCurrentCol = 0;
		}
	}

	m_vRowUVRatio.x = (_float)m_iCurrentRow / (_float)m_SequenceInfo.iRow;
	m_vRowUVRatio.y = (_float)(m_iCurrentRow + 1.f) / (_float)m_SequenceInfo.iRow;

	m_vColUVRatio.x = (_float)(m_iCurrentCol) / (_float)m_SequenceInfo.iCol;
	m_vColUVRatio.y = (_float)(m_iCurrentCol + 1.f) / (_float)m_SequenceInfo.iCol;


}

shared_ptr<CUI_SequenceTex> CUI_SequenceTex::Create(CUI::tagUIInfo* _info, const wstring& _strTexKey, _int _iPriorityNum, SequenceTexInfo* _TexInfo)
{
	shared_ptr<CUI_SequenceTex> pInstance = make_shared<CUI_SequenceTex>();

	if (pInstance->Initialize(_info, _strTexKey, _iPriorityNum, _TexInfo))
		MSG_BOX("Failed to Create : CUI_SequenceTex");

	return pInstance;
}
