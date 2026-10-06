#pragma once
#include"UI_Parent.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CVIBuffer_Rect;
class CShader;
class CTexture;
END
BEGIN(Client)
class CUI_Dialogue final :public CUI_Parent
{

public:
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		//1280 720 -> 100, 100, -- 1180 620
	}UI_DIALOGUE_DESC;

private:
	explicit  CUI_Dialogue(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_Dialogue(const CUI_Dialogue& Prototype);
	virtual ~CUI_Dialogue() = default;
public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	
	void Clear_Text();
	_bool Get_TextEnd() { return m_isTextEnd; };
	void  Reset_Dialogue();

	void Set_Dialogues(vector<DialogueInfo>& Dialogues ,_uint iNowNpcNo);
	void Set_NpcName(_tchar* pName) { memcpy(m_szNpcName, pName,sizeof(m_szNpcName)); }
	_uint Get_NpcNowNo() { return m_iNowNpcNo; };
	virtual void Set_Active()override;
	virtual void Set_InActive()override;
private:
	HRESULT  Ready_Components();
	HRESULT  Ready_ChildUIs();
	HRESULT  Bind_ShaderResources();


private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom_BackGround = { nullptr };
	vector<DialogueInfo> m_Dialogues;
	_uint m_iDialogueIndex = {};
	_bool m_isTextEnd = { false };

	_uint m_iNowNpcNo = {};
	_tchar m_szNpcName[MAX_PATH] = {};

public:
	static   CUI_Dialogue* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};END

