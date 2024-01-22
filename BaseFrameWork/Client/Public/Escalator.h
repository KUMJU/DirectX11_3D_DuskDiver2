#pragma once

#include "Client_Defines.h"
#include "MapObject.h"

BEGIN(Client)

class CEscalator : public CMapObject
{
public:
	CEscalator();
	virtual ~CEscalator() = default;

public:
	virtual HRESULT Initialize(_uint _ModelType);
	virtual void PriorityTick(_float _fTimeDelta) override;
	virtual void Tick(_float _fTimeDelta) override;
	virtual void LateTick(_float _fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	shared_ptr<CModel> m_pStepModel = { nullptr };
	shared_ptr<CShader> m_pAnimShader = { nullptr };

	HRESULT BindAnimShader();

private:
	void StepOn();

public:
	static shared_ptr<CEscalator> Create(_uint _ModelType);

};

END