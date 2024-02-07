#pragma once

#include "GameObject.h"
#include "Tool_Defines.h"

BEGIN(Tool)
 
//Tool¿ë

class CEffectPreset : public CGameObject
{
public:
	CEffectPreset();
	virtual ~CEffectPreset() = default;


public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

public:
	void AddEffect(shared_ptr<class CEffect> _pEffect);
	void DeleteEffect(_uint _iEffectIdx);

	shared_ptr<CEffect> GetEffect(_uint _iEffectIdx) { return m_Effects[_iEffectIdx]; }
	vector<shared_ptr<class CEffect>>* GetEffectList() {return &m_Effects;}

public:
	void ParsingEffect();
	void ResetEffect();

public:
	_bool m_bLoop = false;
	_float2 m_vTotalDuration = _float2(0.f, 0.f);

private:
	vector<shared_ptr<class CEffect>> m_Effects; 


public:
	static shared_ptr<CEffectPreset> Create();

};

END