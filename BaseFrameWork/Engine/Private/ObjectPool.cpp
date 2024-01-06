#include "ObjectPool.h"

CObjectPool::CObjectPool()
{
}

void CObjectPool::Initialize()
{
}

shared_ptr<class CGameObejct> CObjectPool::GetEnableObject()
{
	return shared_ptr<class CGameObejct>();
}

void CObjectPool::AddNewObject()
{
}

shared_ptr<CObjectPool> CObjectPool::Create()
{
	return shared_ptr<CObjectPool>();
}
