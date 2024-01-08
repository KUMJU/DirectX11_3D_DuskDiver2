#pragma once

#include "Client_Defines.h"

BEGIN(Client)

class CPlayer 
{
public:
	CPlayer();
	~CPlayer() = default;

public:


private:

	_int m_iMaxHp = 100;
	_int m_iHp = 100;

	_int m_iBurstGage = 100;
	_int m_iSkillGage = 3;

};

END