#pragma once

#ifndef InputDev_h__
#define InputDev_h__

#include"Engine_Defines.h"
#include"Base.h"

enum class KEY_STATE
{
	NONE,
	PRESSING,
	DOWN,
	UP,
	KEY_STATE_END
};
typedef struct
{
	KEY_STATE eState;
	bool PrevPressed;
}tKeyInfo;

enum class KEY
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	Q,
	W,
	E,
	R,
	T,
	Y,
	U,
	I,
	O,
	P,
	A,
	S,
	D,
	F,
	G,
	H,
	J,
	K,
	L,
	Z,
	X,
	C,
	V,
	B,
	N,
	M,
	SPACE,
	ENTER,
	TAB,
	SHIFT,
	ESC,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	NINE,
	ZERO,
	KEY_END,
};
enum class MOUSE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
enum class MOUSE_MOVE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

BEGIN(Engine)
class ENGINE_DLL CKey_Manager final : public CBase
{
	
private:
	explicit CKey_Manager(void);
	virtual ~CKey_Manager(void)=default;

	HRESULT Initialize_DInputDev(HINSTANCE hInst, HWND hWnd);

public:
	void Update_KeyManager(void);

public:
	
	KEY_STATE Get_DIKeySatate(KEY KeyID)
	{
		return m_vecKeyState[(int)KeyID].eState;
	}


	KEY_STATE Get_DIMouseState(MOUSE eMouse)
	{
		return m_vecMouseState[(int)eMouse].eState; 	//상태는 마우스 좌클릭 우클릭 마우스 휠 마지막 1개는 X 버튼 (옛날에 있던 버튼)
	}
	_long	Get_DIMouseMove(MOUSE_MOVE eMouseState)
	{
		return *(((_long*)&m_MouseState) + (int)eMouseState);	//마우스 상태인 구조체가 LONG타입의 멤버로 이루어저 있고 XYZ 타입에 접근하여 반환한다는 의미
	}

private:
	IDirectInput8* m_pDirectInput = { nullptr };
	IDirectInputDevice8* m_pKeyboard = { nullptr };
	IDirectInputDevice8* m_pMouse = { nullptr };

	unsigned char m_KeyboardState[256] = { 0 };
	DIMOUSESTATE m_MouseState;


	vector<tKeyInfo> m_vecKeyState;
	vector<tKeyInfo> m_vecMouseState;

public:
	static CKey_Manager* Create(HINSTANCE hInst, HWND hWnd);
	virtual void Free(void);
};
END
#endif