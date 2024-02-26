#pragma once

#include "UI.h"

class CUIMiniquest : public CUI
{
public:
	CUIMiniquest();
	virtual ~CUIMiniquest() = default;


public:
	virtual HRESULT Initialize();
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void StartCoinQuest();
	void StartMoleGame();


	void SetSuccessNum(_int _iSucNum) { m_iCurrentSuccessNum = _iSucNum; }
	void AddSuccessNum() { ++m_iCurrentSuccessNum; }
	void ResetUI();

private:
	_int m_iCurrentIConNum = 0;
	//목표점수
	_int m_iGoalNum = 0;
	//현재 달성점수 
	_int m_iCurrentSuccessNum = 0;


private:
	shared_ptr<CTexture> m_pIconTexture[2];
	shared_ptr<CTransform> m_pIconTransform;

public:
	static shared_ptr<CUIMiniquest> Create();
};

