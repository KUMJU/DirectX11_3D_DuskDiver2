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
		STATE_SKILL_DASH,
		STATE_COLLECT,
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

	//�⺻
	shared_ptr<CModel> m_pModelCom = nullptr;
	//����Ʈ���
	shared_ptr<CModel> m_pBurstModelCom = nullptr;
	//�������
	shared_ptr<CModel> m_pBattleModelCom = nullptr;

private:
	HRESULT AddComponent();
	HRESULT BindShaderResources();

	void ChangeAnim(_uint _iAnimNum, _bool _isLoop);

	void FinishCombo();

private:
	HEROSTATE m_eCurrentState = HEROSTATE::STATE_END;

	_uint m_iCurrentAnimIdx = 0;
	_bool m_isAnimLoop = true;


	_bool m_ComboArr[5] = {false, false, false, false, false};
	_uint m_ComboAnimKeyArr[5] = { 0, 1, 3, 5, 7 };
	_int m_CurrentCombo = -1;
private:
	//�ִϸ��̼� root �̵��Ÿ��� �����ؼ� �÷��̾� �����ӿ� ���� �����ش� 
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

	_int m_iCurrentAnim = 0;

	_int m_iMaxHp = 100;
	_int m_iHp = 100;

	_int m_iBurstGage = 100;
	_int m_iSkillGage = 3;


public:
	static shared_ptr<CPlayer> Create();

};

END