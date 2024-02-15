#pragma once

#include "GameObject.h"
#include "Tool_Defines.h"

BEGIN(Engine)
class CModel;
class CShader;
END


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

public:
	
	shared_ptr<CModel> GetModel() { return m_pModel; }

	void ChangeAnimation(_uint _iAnimNum);


private:

	shared_ptr<class CEffectPreset> m_pEffectPreset = nullptr;


	shared_ptr<CModel> m_pModel = nullptr;
	shared_ptr<CShader> m_pShader = nullptr;

public:
	static shared_ptr<CEffectTestModel> Create();


};

END