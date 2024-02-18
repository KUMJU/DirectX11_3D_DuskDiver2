#pragma once

#include "UI.h"

BEGIN(Client)

class CUI_SequenceTex : public CUI
{
public:
	struct SequenceTexInfo {
		_uint iCol;
		_uint iRow;
		_float fScrollTime;
		_bool bLoop;
	};


public:
	CUI_SequenceTex();
	virtual ~CUI_SequenceTex() = default;

public:
	virtual HRESULT Initialize(CUI::tagUIInfo* _info, const wstring& _strTexKey, _int _iPriorityNum, SequenceTexInfo* _TexInfo);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void SetCurrentTexIdx(_uint _iRow, _uint _iCol){ 
		m_iCurrentRow = _iRow;
		m_iCurrentCol = _iCol;
	}


public:

	void SlideNextTexIdx();

private:
	SequenceTexInfo m_SequenceInfo = {};

	_uint m_iCurrentRow = 0;
	_uint m_iCurrentCol = 0;

	// x : StartUV , y : EndUV
	_float2 m_vColUVRatio = _float2();
	_float2 m_vRowUVRatio = _float2();

	_float m_fAccTime = 0.f;

public:
	static shared_ptr<CUI_SequenceTex> Create(CUI::tagUIInfo* _info, const wstring& _strTexKey, _int _iPriorityNum, SequenceTexInfo* _TexInfo);

};

END