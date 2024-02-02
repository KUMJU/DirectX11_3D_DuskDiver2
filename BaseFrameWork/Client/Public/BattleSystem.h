#pragma once

#include "Client_Defines.h"

BEGIN(Engine)
class CLayers;
END

BEGIN(Client)

class CBattleSystem
{
	DECLARE_SINGLETON(CBattleSystem)

private:
	CBattleSystem();
	~CBattleSystem() = default;

public:

	void Initialize();

	//플레이어 상태	바꿔주기 - > battleOn 매 lateTick 마다 검사해서 
	void BattleStart();
	void BattleEnd();

	void Tick(_float _fTimeDelta);
	void KillLastOne();

	_bool GetOnBattle() { return m_bOnBattle; }

private:
	_float m_fSlowTIme = 0.f;

	_bool m_bOnBattle = false;
	_bool m_bSlowMotion = false;

	shared_ptr<CLayers> m_pMonsterLayer = nullptr;
	shared_ptr<class CPlayer> m_pPlayer = nullptr;

	//몬스터 레이어 들고있기
	//


};

END