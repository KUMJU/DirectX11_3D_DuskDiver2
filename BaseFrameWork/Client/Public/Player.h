#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
END


BEGIN(Client)

class CPlayer : public CGameObject
{
public:
	enum class HEROSTATE {
		IDLE, 
		WALK, 
		ATTACK, 
		ENUM_END
	};


public:
	CPlayer();
	~CPlayer() = default;

public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CShader> m_pShader = nullptr;
	shared_ptr<CModel> m_pModelCom = nullptr;

private:
	HRESULT AddComponent();
	HRESULT BindShaderResources();

	void ChangeAnim(_uint _iAnimNum, _bool _isLoop);

private:
	HEROSTATE m_eCurrentState = HEROSTATE::ENUM_END;

	_uint m_iCurrentAnimIdx = 1;
	_bool m_isAnimLoop = true;

private:
	void KeyInput(_float _fTimeDelta);
	void MouseInput(_float _fTimeDelta);

	_bool m_IsMouseDeb = false;
	_bool m_IsKeyDeb = false;
	
	_float m_fDebTime = 0.f;

private:
	_float3 m_vCurretnAnimPos = { 0.f, 0.f , 0.f };
	_float3 m_vPrevAnimPos = { 0.f, 0.f, 0.f };

private:

	_int m_iMaxHp = 100;
	_int m_iHp = 100;

	_int m_iBurstGage = 100;
	_int m_iSkillGage = 3;


public:
	static shared_ptr<CPlayer> Create();

};

END