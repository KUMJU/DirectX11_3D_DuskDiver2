#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CModel;
class CMesh;
class CShader;
class CGameInstance;
END

BEGIN(Client)

class CMonster abstract : public CGameObject
{
public:
	struct ANIMINFO {
		_uint iNextAnimNum;
		_bool IsLoop;
	};

public:
	enum class EMONSTER_TYPE {
		MONSTER_NORMAL,
		MONSTER_BOSS,
		MONTER_END
	};

public:
	enum class EMONSTER_STATE {
		STATE_IDLE,
		STATE_WALK,
		STATE_RUN,
		STATE_ATTACK,
		STATE_HIT,
		STATE_DIE,
		STATE_DOWN,
		STATE_STUN,
		STATE_END
	};

public:
	CMonster();
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize(CTransform::TRANSFORM_DESC* _pDesc);
	virtual void PriorityTick(_float _fTimeDelta);
	virtual void Tick(_float _fTimeDelta);
	virtual void LateTick(_float _fTimeDelta);
	virtual HRESULT Render();

protected:
	void ChasePlayer();
	void CalcPlayerDistance();
	void CalcAnimationDistance();
	_float CalcPlayerDistanceReturn();

	virtual void AttackPattern(_uint _iAtkNum) = 0;

protected:

	_bool CheckReserveAnimList();
	void ChangeAnim(_uint _iAnimIdx, _bool _bloop);

	//애니메이션 리스트가 비어있을때 객체마다 다른 제어를 해주기 위해 순수 가상함수로 작성
	virtual void IfEmptyAnimList() = 0;
	virtual void CalcDistanceOption() = 0;

	list<ANIMINFO> m_NextAnimIndex;

protected:

	_bool m_IsNearPlr = false; //공격 가능 여부 확인
	_bool m_IsAtkCool = false;

	_float m_fTotalCoolTime = 0.f;

	_float AttackDistance = 0.f; // 몬스터 사거리
	_float m_bAttackCoolTime = 0.f;

	_uint m_iCurrentAtkNum = 0;
	_uint m_iNextAtkNum = 0;


	_uint m_iTotalAtkNum = 0;
	_uint m_iTestNum = 0; //애니메이션 확인하려고 넣음

	_bool m_IsLinearInterpolation = false;

	_bool m_bStun = false;
	_float m_fStunTime = 0.f;


protected:

	shared_ptr<CModel> m_pModelCom = nullptr;
	shared_ptr<CMesh> m_pMesh = nullptr;
	shared_ptr<CShader> m_pShader = nullptr;

protected:
	HRESULT BindShaderResources();

protected:
	CGameInstance* m_pGameInstance = nullptr;

	_float3 m_vCurrentAnimPos = _float3();
	_float3 m_vPrevAnimPos = { 0.f, 0.f, 0.f };


protected:
	_int m_iHP = 0;
	_uint m_iAtkPattern = 0;

	_bool m_IsSturnState = false;

protected:
	_uint m_iAnimNum = 0;
	_bool m_bLoop = false;

	shared_ptr<class CPlayer> m_pTarget = nullptr;
	shared_ptr<CTransform> m_pTargetTransCom = nullptr;

	EMONSTER_TYPE m_eMonsterType = EMONSTER_TYPE::MONTER_END;
	EMONSTER_STATE m_eCurrentState = EMONSTER_STATE::STATE_IDLE;
};

END