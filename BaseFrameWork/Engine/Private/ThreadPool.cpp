#include "ThreadPool.h"

IMPLEMENT_SINGLETON(CThreadPool)

CThreadPool::CThreadPool()
{
}

CThreadPool::~CThreadPool()
{
}

void CThreadPool::Initialize(size_t _iThreadNum)
{
	m_Workers.reserve(_iThreadNum);

	for (size_t i = 0; i < _iThreadNum; ++i) {
		m_Workers.emplace_back([this]() { this->WorkerThread(); });
	}
}

void CThreadPool::AddNewTask(std::function<void()> _func)
{
}

void CThreadPool::WorkerThread()
{



}
