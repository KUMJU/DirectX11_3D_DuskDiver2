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
	//�� ����Ʈ���� �������̵����� ���� �ʿ��� ������ ������
	virtual void ParsingData(ofstream& _fp) {};
	//���� �ʱ��� ���·� �����ش�
	virtual void ResetEffect() {};

protected:
	shared_ptr<CShader> m_pShader = nullptr;
	shared_ptr<CTexture> m_pTexture = nullptr;

protected:
	EFFECT_TYPE m_eEffectType = EFFECT_TYPE::TYPE_END;

	//Ȱ��ȭ �ð��� ������ false�� ���¸� ���ְ� �ٽ� ����Ʈ Ǯ�� ���������� 
	_float2 vDuration = _float2();
	

	char* m_strEffectName = {};


};

END