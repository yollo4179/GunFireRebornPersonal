#pragma once

#include <process.h>

namespace Client
{
	const unsigned int			g_iWinSizeX = { 1280 };//1280/720
	const unsigned int			g_iWinSizeY = { 720 };

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_GAMEPLAY2,LEVEL_END };
}

typedef struct
{
	_uint fFireSpeed;
	_uint fBulletSpeed;
	_uint fDemage; 
	_uint fCriticalRate;
	_uint fCriticalDemage;
	_uint fMaxLoadBullets;
	_uint iRange; //for bomb
}HandsInfo;

typedef struct
{

	_int iNumBullets;
	_int iCoins;

	_int iNumGrenades;
	_int iNumMaxGrenades;

}PlayerItemInfo;

typedef struct {
	_tchar sz_text[MAX_PATH];
}DialogueInfo;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;
/* Player Status*/
extern _float4 g_PlayerHPInfo;
extern PlayerItemInfo g_PlayerItemInfo;
static const int g_iNumTools = 6; //0.Ä® 1.¼Õ 2.È£¶ûÀÌÃÑ 3.·¹ÀÌÀú 4.¹°°í±â 5.skill bomb
extern HandsInfo g_HandsInfo[];


extern _bool g_Update_TimeDelta;
using namespace Client;
