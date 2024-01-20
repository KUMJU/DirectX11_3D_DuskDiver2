#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CNavigation;
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
		STATE_SKILL_Q,
		STATE_SKILL_E,
		STATE_SKILL_R,
		STATE_BURST_TRANS,
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

	shared_ptr<CModel> m_pMainModelCom = nullptr;

	//기본
	shared_ptr<CModel> m_pModelCom = nullptr;
	//버스트모드
	shared_ptr<CModel> m_pBurstModelCom = nullptr;
	//전투모드
	shared_ptr<CModel> m_pBattleModelCom = nullptr;

	shared_ptr<CNavigation> m_pNavigationCom = nullptr;


private:
	void SetAnimSpeed();

private:
	HRESULT AddComponent();
	HRESULT BindShaderResources();
	   
	_bool ChangeAnim(_uint _iAnimNum, _bool _isLoop);
	void CheckReserveAnimList();
		
	void FinishCombo();

private:
	HEROSTATE m_eCurrentState = HEROSTATE::STATE_END;

	_uint m_iCurrentAnimIdx = 0;
	//진짜 state가 loop
	_bool m_isAnimLoop = true;

//변신~~~~~~~~~!!!!!
private:
	_float m_fransformTime = 0.f;

private:

	_float m_fLinearTime = 0.06f;

private:
	//애니메이션 root 이동거리를 측정해서 플레이어 움직임에 직접 더해준다 
	void CalcAnimMoveDistance();
	//LookVector가 카메라를 기준으로 갱신
	void CalcLookVector(_float _fDir);

private:
	void KeyInput(_float _fTimeDelta);
	void MouseInput(_float _fTimeDelta);

	void TurnLerp(_vector _vSrc, _vector _vDst, _float _fDeltaTime, _vector _vPos, unsigned char _flag);

	_vector m_vPrevLook = { 0.f, 0.f , 0.f, };

	_bool m_IsMouseDeb = false;
	_bool m_IsKeyDeb = false;
	
	_float m_fDebTime = 0.f;
	_float m_fSlerpFlat = 0.f;

	unsigned char m_MoveFlag = 0x0000;
	unsigned char m_LastMoveFlag = 0x0000;

private:
	_float3 m_vCurretnAnimPos = { 0.f, 0.f , 0.f };
	_float3 m_vPrevAnimPos = { 0.f, 0.f, 0.f };

	_float m_fCurrentDir = 1.f;

	_float m_fSkillRJumpCool = 0.f;
private:

	_int m_iMaxHp = 100;
	_int m_iHp = 100;

	_int m_iBurstGage = 100;
	_int m_iSkillGage = 3;


	_bool m_bBurstMode = false;

private:

	_bool m_bAirBorneAtk = false;

	_bool m_bRunning = false;
	_bool m_bAttack = false;
	_bool m_bSuperArmor = false;

	_bool m_bAtkDeb = false;

	_bool m_bJump = false;
	_bool m_bDrop = false;

	_bool m_bDash = false;
	_bool m_bSprint = false;

	_bool m_bDodge = false;
	 
	_float m_fDashSpeed = 0.f;

	
	_bool m_bActiveCoolTime = false;

	_float m_fCurrentDegree = 0.f;
private:

	list<ANIMINFO> m_NextAnimIndex;

	//LoopAnimation
	_float m_fLoopTotalTime = 0.14f;
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
	_float m_fMinComboAnimTime = 0.5f;

//Jump
private:

	_float m_fJumpSpeed = 4.f;
	_float m_fTotalHeight = 0.f;

private:
	_float AttackCoolTime = 0.f;


//SkillSystem
private:
	_bool m_IsUsingSkill = false;


public:
	static shared_ptr<CPlayer> Create();

};

END