#pragma once

#include "Engine_Defines.h"
#include <concurrent_queue.h>
#include <thread>
#include <functional>

BEGIN(Engine)

class ENGINE_DLL CThreadPool
{
	DECLARE_SINGLETON(CThreadPool)

private:
	CThreadPool();
	~CThreadPool();

public:
	void Initialize(size_t _iThreadNum);
	void AddNewTask(std::function<void()> _func);
	void WorkerThread();

private:

	size_t m_iThreadNum = { 0 };
	concurrency::concurrent_queue<std::function<void()>> m_pTaskQueue;
	vector<std::thread> m_Workers;

	std::atomic<_int> m_iTaskNum = { 0 };
};

END