#ifndef Engine_Macro_h__
#define Engine_Macro_h__

namespace Engine
{
#ifndef			MSG_BOX
#define			MSG_BOX(_message)			MessageBox(NULL, TEXT(_message), L"System Message", MB_OK)
#endif

#define			BEGIN(NAMESPACE)		namespace NAMESPACE {
#define			END						}

#define			USING(NAMESPACE)	using namespace NAMESPACE;

#ifdef	ENGINE_EXPORTS
#define ENGINE_DLL		_declspec(dllexport)
#else
#define ENGINE_DLL		_declspec(dllimport)
#endif	

#define NO_COPY(CLASSNAME)											\
			private:												\
			CLASSNAME(const CLASSNAME&) = delete;					\
			CLASSNAME& operator = (const CLASSNAME&) = delete;		

#define DECLARE_SINGLETON(CLASSNAME)								\
			NO_COPY(CLASSNAME)										\
			private:												\
			static CLASSNAME*	m_pInstance;						\
			public:													\
			static CLASSNAME*	GetInstance( void );				\
			static unsigned int DestroyInstance( void );			

#define IMPLEMENT_SINGLETON(CLASSNAME)								\
			CLASSNAME*	CLASSNAME::m_pInstance = nullptr;			\
			CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
			if(nullptr == m_pInstance) {							\
				m_pInstance = new CLASSNAME;						\
				}													\
				return m_pInstance;									\
			}														\
			unsigned int CLASSNAME::DestroyInstance( void ) {		\
				unsigned int iRefCnt = {0};							\
				if (nullptr != m_pInstance) {						\
				iRefCnt = m_pInstance->Release();					\
				if(0 == iRefCnt)									\
					m_pInstance = nullptr;							\
				}													\
				return iRefCnt;										\
			}
}
//Key_Manager
#define KEY_CHECK(Key)			m_pGameInstance->Get_DIKeyState(Key)
#define KEY_NONE(Key)			KEY_CHECK(Key)==KEY_STATE::NONE
#define KEY_UP(Key)				KEY_CHECK(Key)==KEY_STATE::UP
#define KEY_DOWN(Key)			KEY_CHECK(Key)==KEY_STATE::DOWN
#define KEY_PRESSING(Key)		KEY_CHECK(Key)==KEY_STATE::PRESSING


#define MOUSE_CHECK(Key)		m_pGameInstance->Get_DIMouseState(Key)
#define MOUSE_NONE(Key)			MOUSE_CHECK(Key)==KEY_STATE::NONE
#define MOUSE_UP(Key)			MOUSE_CHECK(Key)==KEY_STATE::UP
#define MOUSE_DOWN(Key)			MOUSE_CHECK(Key)==KEY_STATE::DOWN
#define MOUSE_PRESSING(Key)		MOUSE_CHECK(Key)==KEY_STATE::PRESSING

#define GET_MOUSE_MOVE(x)			m_pGameInstance->Get_DIMouseMove(x)
#endif // Engine_Macro_h__
