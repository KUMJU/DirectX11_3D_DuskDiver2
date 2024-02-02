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

	//�÷��̾� ����	�ٲ��ֱ� - > battleOn �� lateTick ���� �˻��ؼ� 
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

	//���� ���̾� ����ֱ�
	//


};

END