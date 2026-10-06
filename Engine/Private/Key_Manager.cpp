#include "Key_Manager.h"

unsigned char g_arrKeyMapping[(_uint)KEY::KEY_END] =
{
	DIK_LEFT,
	DIK_RIGHT,
	DIK_UP,
	DIK_DOWN,
	DIK_Q,
	DIK_W,
	DIK_E,
	DIK_R,
	DIK_T,
	DIK_Y,
	DIK_U,
	DIK_I,
	DIK_O,
	DIK_P,
	DIK_A,
	DIK_S,
	DIK_D,
	DIK_F,
	DIK_G,
	DIK_H,
	DIK_J,
	DIK_K,
	DIK_L,
	DIK_Z,
	DIK_X,
	DIK_C,
	DIK_V,
	DIK_B,
	DIK_N,
	DIK_M,
	DIK_SPACE,
	DIK_RETURN,
	DIK_TAB,
	DIK_LSHIFT,
	DIK_ESCAPE,
	DIK_1,
	DIK_2,
	DIK_3,
	DIK_4,
	DIK_5,
	DIK_6,
	DIK_7,
	DIK_8,
	DIK_9,
	DIK_0,
};

CKey_Manager::CKey_Manager(void)
{
}

HRESULT CKey_Manager::Initialize_DInputDev(HINSTANCE hInst, HWND hWnd)
{

	if(FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION, //최신 버전을 사용하겠다는 의미
		IID_IDirectInput8,   //IDirectInput8.0인터페이스를 사용
		(void**)&m_pDirectInput,//LPDIRECTINPUT8 의 객체 포인터를 저장
		NULL)))
		return E_FAIL;

	// 키보드 객체 생성
	if (FAILED(m_pDirectInput->CreateDevice(
		GUID_SysKeyboard,  //globally uniqued identifier   :  생성하려는 장치의 guid로 Enum_Device함수를 호출해 입력 장치드ㅇ의 guid를 얻을수 있음
		&m_pKeyboard,     //장치 인터페이스 포인터 (컴객체를 가리키는 포인터)
		nullptr)))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyboard->SetDataFormat(&c_dfDIKeyboard); //데이터 형식의 포인터(미리 ㄷ정의된 데이터 형식이 존재 ( 헤더 파일 ) (키보드 마우스 조이스틱을 위한 데이터 형식 존재 

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyboard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	//윈도우 핸들과 플래그 flag-> 응용이 활성화이거나 비활성이거나 모두 입력 장치들 사용  and(현재 프로세스에 입력중일때 다른 프로세스는 동작하지 않음 (메모장 입력 등 )

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyboard->Acquire();
	//입력장치를 획득하는 함수 : 입력장치에 대한 제어권을 얻어오고, 데이터를 받아오는 함수 unrequired 함수를 반드시 호출해야함


	// 마우스 객체 생성
	if(FAILED(m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();

	for (size_t i = 0; i < (_uint)KEY::KEY_END; ++i)
	{
		
		m_vecKeyState.push_back(tKeyInfo{KEY_STATE::NONE, false});
	}
	for (size_t i = 0; i < (_uint)MOUSE::DIM_END; ++i)
	{
		
		m_vecMouseState.push_back(tKeyInfo{ KEY_STATE::NONE, false });
	}
	


	return S_OK;
}

void CKey_Manager::Update_KeyManager(void)
{
	m_pKeyboard->GetDeviceState(256, m_KeyboardState);// 키보드 입력상태를 알려준다(저장)
	m_pMouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState); //마우스 입력 상태를 알려준다(저장)

	//keyboard
	for (int i = 0; i < (int)KEY::KEY_END; ++i)
	{
		//현재 키가 눌려있는 경유
		if (m_KeyboardState[g_arrKeyMapping[i]] & 0x80)
		{
			//이전에 키가 눌렸던 경우
			if (true == m_vecKeyState[i].PrevPressed)
			{
				m_vecKeyState[i].eState = KEY_STATE::PRESSING;
			}
			//이전에 키가 눌려있지 낳았던 경우
			else
			{
				m_vecKeyState[i].eState = KEY_STATE::DOWN;
			}

			m_vecKeyState[i].PrevPressed = true;
		}
		else
		{
			//이전에 키가 눌렸던 경우
			if (true == m_vecKeyState[i].PrevPressed)
			{
				m_vecKeyState[i].eState = KEY_STATE::UP;
			}
			//이전에 키가 눌려있지 낳았던 경우
			else
			{
				m_vecKeyState[i].eState = KEY_STATE::NONE;
			}

			m_vecKeyState[i].PrevPressed = false;

		}
	}
	//Mouse

	for (int i = 0; i < (_uint)MOUSE::DIM_END; ++i)
	{
		//현재 마우스의 버튼이 눌렸다
		if (m_MouseState.rgbButtons[i])
		{
			//이전에 마우스가 눌렸다 
			if (true == m_vecMouseState[i].PrevPressed)
			{
				m_vecMouseState[i].eState = KEY_STATE::PRESSING;
			}
			//이전에 마우스가눌린적 없다.
			else
			{
				m_vecMouseState[i].eState = KEY_STATE::DOWN;
			}
			m_vecMouseState[i].PrevPressed = true;
		}
		//현재 마우스의 버튼이 눌리지 않았다
		else
		{
			if (true == m_vecMouseState[i].PrevPressed)
			{
				m_vecMouseState[i].eState = KEY_STATE::UP;
			}
			else
			{
				m_vecMouseState[i].eState = KEY_STATE::NONE;
			}
			m_vecMouseState[i].PrevPressed = false;
		}

	}

 
 }




CKey_Manager* CKey_Manager::Create(HINSTANCE hInst, HWND hWnd)
{
	CKey_Manager* pInstance = new CKey_Manager();
	if (FAILED(pInstance->Initialize_DInputDev(hInst, hWnd)))
	{
		Safe_Release(pInstance);
		MSG_BOX("Create: Failed CKey_Manager");
		return nullptr;
	}
	return pInstance;
}

void CKey_Manager::Free(void)
{
	if (m_pKeyboard) {
		m_pKeyboard->Unacquire();
		Safe_Release(m_pKeyboard);
		m_pKeyboard = nullptr;
	}
	if (m_pMouse) {
		m_pMouse->Unacquire();
		Safe_Release(m_pMouse);
		m_pMouse =nullptr;
	}
	Safe_Release(m_pDirectInput);

	__super::Free();
}
