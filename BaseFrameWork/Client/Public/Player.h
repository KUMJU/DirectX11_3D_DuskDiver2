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
		HEAVY_ATTACK,
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

	//기본
	shared_ptr<CModel> m_pModelCom = nullptr;
	//버스트모드
	shared_ptr<CModel> m_pBurstModelCom = nullptr;
	//전투모드
	shared_ptr<CModel> m_pBattleModelCom = nullptr;

private:
	HRESULT AddComponent();
	HRESULT BindShaderResources();

	void ChangeAnim(_uint _iAnimNum, _bool _isLoop);

	void FinishCombo();

private:
	HEROSTATE m_eCurrentState = HEROSTATE::ENUM_END;

	_uint m_iCurrentAnimIdx = 0;
	_bool m_isAnimLoop = true;


	_bool m_ComboArr[5] = {false, false, false, false, false};
	_uint m_ComboAnimKeyArr[5] = { 0, 1, 3, 5, 7 };
	_int m_CurrentCombo = -1;
private:
	//애니메이션 root 이동거리를 측정해서 플레이어 움직임에 직접 더해준다 
	void CalcAnimMoveDistance();

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