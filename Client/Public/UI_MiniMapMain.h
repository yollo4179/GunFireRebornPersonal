#pragma once
#include"UIObject.h"
#include"Client_Defines.h"
BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END
BEGIN(Client)
class CUI_MiniMapMain final :public CUIObject
{
public:
	/*percent로 나타낼것인지 그냥 숫자로 나타낼것인지  precentage 가 true라면 어떤 숫자가 들어갈 것인지 */
	typedef struct : public CUIObject::UIOBJECT_DESC
	{
		LEVELID eStageLevel;
		_float fRange;

	}UI_MAIN_MAP_DESC;

private:
	explicit CUI_MiniMapMain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	explicit CUI_MiniMapMain(const CUI_MiniMapMain& Prototype);
	virtual ~CUI_MiniMapMain() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Set_Vector(vector<class CMonster*>* ppMonsters);
	vector<class CMonster*>* Get_Vector_For_ChasingMonsters() { return m_ppMonsters; };

	void Set_szLevelName(_tchar* szLevelName) { memcpy(m_szLevelName, szLevelName, sizeof(m_szLevelName)); };
private:
	HRESULT Ready_Components();
	HRESULT BindAndRender_ShaderResources_BG();
	HRESULT BindAndRender_ShaderResources_Player();
	HRESULT BindAndRender_ShaderResources_Monster(_fmatrix MonWorld);
	HRESULT Bind_ShaderResources_Protal();
	HRESULT BindAndRender_ShaderResources_LevelText();
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom_BG = { nullptr };
	CTexture* m_pTextureCom_Player = { nullptr };
	CTexture* m_pTextureCom_Monster = { nullptr };
	CTexture* m_pTextureCom_Portal = { nullptr };
	CTexture* m_pTextureCom_Level = { nullptr };

	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	vector<class CMonster*>* m_ppMonsters;
	_float2 m_vCenter = {};
	_float m_fRadius = {};
	_float m_fRange = {};
	LEVELID m_eStageLevel = {};
	_tchar m_szLevelName[MAX_PATH] = {};

public:
	static  CUI_MiniMapMain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;

};
END
