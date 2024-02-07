#pragma once

#include "GameObject.h"
#include "Tool_Defines.h"

BEGIN(Tool)

class CEffectTestModel : public CGameObject
{
public:
	CEffectTestModel();
	virtual ~CEffectTestModel() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

private:

	shared_ptr<class CEffectPreset> m_pEffectPreset = nullptr;

public:
	static shared_ptr<CEffectTestModel> Create(shared_ptr<class CEffectPreset> _pPreset, const wstring& _strModelName);


};

END