#pragma once

#include "GameObject.h"
#include "Tool_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBufferPoint;
END

BEGIN(Tool)

class CEffect abstract : public CGameObject
{
public:
	enum class EFFECT_TYPE {
		TYPE_TEXTURE,
		TYPE_MESH,
		TYPE_PARTICLE,
		TYPE_END
	};


public:
	CEffect();
	virtual ~CEffect() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();


public:
	char* GetEffectName() {
		return m_strEffectName;
	}


public:
	//각 이펙트마다 오버라이딩에서 각자 필요한 정보를 저장함
	virtual void ParsingData(ofstream& _fp) {};
	//제일 초기의 상태로 돌려준다
	virtual void ResetEffect() {};

protected:
	shared_ptr<CShader> m_pShader = nullptr;
	shared_ptr<CTexture> m_pTexture = nullptr;

protected:
	EFFECT_TYPE m_eEffectType = EFFECT_TYPE::TYPE_END;

	//활성화 시간이 지나면 false로 상태를 꺼주고 다시 이펙트 풀로 돌려보낸다 
	_float2 vDuration = _float2();
	

	char* m_strEffectName = {};


};

END