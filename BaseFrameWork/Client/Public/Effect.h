#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBufferPoint;
END

BEGIN(Client)

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

protected:
	shared_ptr<CShader> m_pShader = nullptr;
	shared_ptr<CTexture> m_pTexture = nullptr;

protected:
	EFFECT_TYPE m_eEffectType = EFFECT_TYPE::TYPE_END;

	//Ȱ��ȭ �ð��� ������ false�� ���¸� ���ְ� �ٽ� ����Ʈ Ǯ�� ���������� 
	_float m_fLifeTime = 0.f;


};

END