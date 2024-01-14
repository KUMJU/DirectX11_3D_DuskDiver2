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
	struct ANIMINFO {
		_uint iNextAnimNum;
		_bool IsLoop;
	};

public:
	enum class HEROSTATE {
		STATE_IDLE,
		STATE_WALK,
		STATE_RUN,
		STATE_JUMP,
		STATE_COMBO_ATTACK1,
		STATE_COMBO_ATTACK2,
		STATE_COMBO_ATTACK3,
		STATE_COMBO_ATTACK4,
		STATE_COMBO_ATTACK5,
		STATE_HEAVY_ATTACK,
		STATE_SKILL_A,
		STATE_SKILL_S,
		STATE_SKILL_D,
		STATE_SKILL_F,
		STATE_DASH,
		STATE_HIT,
		STATE_DODGE,
		STATE_SPECIAL_ATTACK,
		STATE_END
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
	void CheckReserveAnimList();
		
	void FinishCombo();

private:
	HEROSTATE m_eCurrentState = HEROSTATE::STATE_END;

	_uint m_iCurrentAnimIdx = 0;
	//진짜 state가 loop
	_bool m_isAnimLoop = true;

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

private:

	_bool m_bAttack = false;
	_bool m_bSuperArmor = false;
	_bool m_bJump = false;
	_bool m_bDash = false;

	_float fDashSpeed = false;
	
	_bool m_bActiveCoolTime = false;

private:

	list<ANIMINFO> m_NextAnimIndex;

	//LoopAnimation
	_float m_fLoopTotalTime = 0.2;
	_bool m_IsLoopMotion = false;
	_float m_fCurLoopTime = 0.f;


private:
	void ResetComboState();

//ComboSystem
private:

	_bool m_ComboArr[5] = { false, false, false, false, false };
	_uint m_ComboAnimKeyArr[5] = { 0, 1, 3, 5, 7 };
	_int m_CurrentCombo = -1;

	_bool m_bReserveCombo = false;
	_bool m_bComboAttackStart = false;

	_float m_fComboTime = 0.f;


//SkillSystem
private:
	_bool m_IsUsingSkill = false;


public:
	static shared_ptr<CPlayer> Create();

};

END