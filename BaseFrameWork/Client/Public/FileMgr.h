#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"

BEGIN(Client)

class CFileMgr
{
	DECLARE_SINGLETON(CFileMgr)

private:
	CFileMgr();
	~CFileMgr() = default;

public:

	HRESULT ReadJsonFile();
	HRESULT SaveJsonFile();

private:



};

END