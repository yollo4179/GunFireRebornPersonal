#pragma once

#include "Base.h"

BEGIN(Client)

class CImguiWindow final : public CBase
{
	
private:
	explicit CImguiWindow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CImguiWindow() = default;

public:
	HRESULT	Initialize();
	HRESULT	Start_Render();
	HRESULT	End_Render();
	void	Load_Font();
private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

	
public:
	static CImguiWindow* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free();
};

END