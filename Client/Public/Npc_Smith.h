#pragma once
#include"Client_Defines.h"
#include"Npc.h"
BEGIN(Client)
class CNpc_Smith  final :public CNpc
{


private:
	explicit CNpc_Smith(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CNpc_Smith(const CNpc_Smith& Prototype);
	virtual ~CNpc_Smith() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


public:
	virtual void Activate_Npc()override;

public:
	void Set_Dialogues(vector<DialogueInfo>& Dialogues) { m_Dialogues = Dialogues; };
	_bool GoToTheNextStage() { return m_GotoTheNextStage; };

private:
	_bool m_isAnimEnd = { false };
	_bool m_isChangingMotion = { false };

	vector<DialogueInfo> m_Dialogues;
	_bool m_GotoTheNextStage = { false };

	_tchar m_szNpcName[MAX_PATH] = L"Smith";
private:
	HRESULT Ready_Components();
	HRESULT Ready_Particle();

public:
	static   CNpc_Smith* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};
END
