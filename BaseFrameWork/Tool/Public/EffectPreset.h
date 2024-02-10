#pragma once

#include "GameObject.h"
#include "Tool_Defines.h"


#include "Json/json.h"
#include "Json/json-forwards.h"

BEGIN(Tool)
 
//Tool¿ë

class CEffectPreset : public CGameObject
{
public:
	CEffectPreset();
	CEffectPreset(const CEffectPreset& _rhs);
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

	void SetLoop(_bool _bLoop) { m_bLoop = _bLoop; }
	void SetDuration(_float _vDuration) { m_vTotalDuration = _vDuration;}

public:
	void PlayEffect();
	void StopEffect();

public:
	void ParsingEffect(Json::Value& _root);
	void ResetEffect();

public:
	_bool m_bLoop = false;
	 
	_float m_vTotalDuration = 0.f;
	_float m_fAccTime = 0.f;


private:
	vector<shared_ptr<class CEffect>> m_Effects; 


public:
	static shared_ptr<CEffectPreset> Create();
	static shared_ptr<CEffectPreset> Clone(shared_ptr<CEffectPreset> _rhs);

};

END