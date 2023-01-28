#pragma once
#ifndef __Value_H__
#define __Value_H__

#include "Export_System.h"
#include "Sound_Mgr.h"

#define WINCX					Engine::Get_WinCX()
#define WINCY					Engine::Get_WinCY()
#define ScrnRatio				Engine::Get_Screen_Ratio()
#define EScrnResolution			Engine::Get_Screen_Resolution()
#define ESNRT_END				Engine::CGraphicDevice::ESCREEN_RESOULTION::ESCREEN_RESOULTION_END

#define pSMgr					CSound_Mgr::GetInstance()

#define VTXCNTX		129
#define VTXCNTZ		129
#define VTXITV		1

#define GRAVITY		-9.81f


#define MINCAMDIS	0.5f
#define MAXCAMDIS	3.f
#define MAXCAMRIGHT 1.f
#define MAXCAMFOV	60.f

//////////////스텟 얼마나 올라갈지 디파인
#define IncreasePlayerMaxHp			10
#define IncreasePlayerMaxStamina	2
#define IncreasePlayerMaxFood		10
#define IncreasePlayerMaxWater		5
#define IncreasePlayerMaxWeight		50
#define IncreasePlayerMaxAtk		2
#define IncreasePlayerMaxSpeed		2
#define IncreasePlayerMaxEngram		1
#define IncreasePlayerMaxEndure		1

#define SKIP_NOTHING	0x0000
#define SKIP_UPDATE		0x0001
#define SKIP_RENDER		0x0002

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;}

#define FAILED_CHECK_RETURN_AND_COUNT(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )					\
		{MessageBox(NULL, L"Failed", L"System Error",MB_OK);return _return;}					\
		else																					\
		{++m_stLoadingCount.iLoadingBarCount;}

#define FAILED_CHECK_RETURN_AND_COUNT_Stage(_hr, _return)	if( ((HRESULT)(_hr)) < 0 )			\
		{MessageBox(NULL, L"Failed", L"System Error",MB_OK);return _return;}					\
		else																					\
		{++m_pCount->iLoadingBarCount;}

extern	HWND			g_hWnd;
extern  HINSTANCE		g_hInst;
extern	_double			g_GlobalTime;
extern	_bool			g_bInstancing;

enum LOADINGID { LOADING_STAGE, LOADING_END};

typedef struct tMapToolParsingInfomation
{
	_ulong	dwID;
	_vec3	vScale;
	_vec3	vRot;
	_vec3	vPos;
	_ulong	dwLevel;
	_vec3	vColor;
	_ulong	dwOption;

	void Set_Scale(const _vec3& _vScale)
	{
		vScale.x = _vScale.x;
		vScale.y = _vScale.y;
		vScale.z = _vScale.z;
	}
	void Set_Scale(const _float& _fScale)
	{
		vScale.x = vScale.y = vScale.z = _fScale;
	}
	void Set_Scale(const _float& _fScaleX, const _float& _fScaleY, const _float& _fScaleZ)
	{
		vScale.x = _fScaleX;
		vScale.y = _fScaleY;
		vScale.z = _fScaleZ;
	}

	void Set_Rot(const _vec3& _vRot)
	{
		vRot.x = _vRot.x;
		vRot.y = _vRot.y;
		vRot.z = _vRot.z;
	}
	void Set_Rot(const _float& _fRotX, const _float& _fRotY, const _float& _fRotZ)
	{
		vRot.x = _fRotX;
		vRot.y = _fRotY;
		vRot.z = _fRotZ;
	}

	void Set_Pos(const _vec3& _vPos)
	{
		vPos.x = _vPos.x;
		vPos.y = _vPos.y;
		vPos.z = _vPos.z;
	}
	void Set_Pos(const _float& _fPosX, const _float& _fPosY, const _float& _fPosZ)
	{
		vPos.x = _fPosX;
		vPos.y = _fPosY;
		vPos.z = _fPosZ;
	}

}OBJINFO;

typedef	struct Making_IngredientItemInfo
{
	_int	iIngredientItemID[4] = {0,0,0,0};
}MII_INFO;

enum BULLET_TYPE
{
	BULLET_BOLA,
	BULLET_SPEAR,
	BULLET_ARROW_TORPID,
	BULLET_ARROW,
	BULLET_END
};

enum ATTACK_OPTION
{
	ATTACK_OPTION_BULLET_BOLA,
	ATTACK_OPTION_BULLET_SPEAR,
	ATTACK_OPTION_BULLET_ARROW_TORPID,
	ATTACK_OPTION_BULLET_ARROW,
	ATTACK_OPTION_DINO_ATTACK,
	ATTACK_OPTION_DINO_GRAB,
	ATTACK_OPTION_END
};

typedef struct tagBullet_Desc
{
	BULLET_TYPE		eBulletType;
	_vec3			vPos;
	_float			fAngleX;
	_float			fAngleY;
	_float			fGravity;
	_float			fSpeed;
	//CUnit*			pShooter;
}BULLET_DESC;

enum INDEX_INVENLIST
{
	INVENTORY_INVENTORY, INVENTORY_BORDER, INVENTORY_INVENTAP	//0, 1, 2
	, INVENTORY_INVENICON_TRANSFER, TRANSFER_BORDER				//3, 4
	, INVENTORY_INVENICON_DROPALL, DROPALL_BORDER				//5, 6
	, INVENTORY_INVENICON_ITEMSORT, ITEMSORT_BORDER				//7, 8
	, INVENTORY_INVENICON_FOLDER, FOLDER_BORDER					//9, 10
	, INVENTORY_INVENICON_CATEGORY, CATEGORY_BORDER				//11, 12
	, INVENTORY_INVENICON_TOOLTIP, TOOLTIP_BORDER				//13, 14
	, INVENTORY_INVENICON_SEARCHBAR, SEARCHBAR_BORDER			//15, 16
	, INDEX_INVENLIST_END
};

enum INDEX_STATLIST
{
	BACK_STAT_UI, BACK_STAT_UI_BORDER	//0, 1
	, CENTER_TAP						//2
	, INDEX_STATLIST_END
};

enum INDEX_SUMMARYSTATLIST
{
	SUMMARYSTAT_UI_HEALTH, SUMMARYSTAT_UI_HEALTH_BORDER
	, SUMMARYSTAT_UI_STAMINA, SUMMARYSTAT_UI_STAMINA_BORDER
	, SUMMARYSTAT_UI_FOOD, SUMMARYSTAT_UI_FOOD_BORDER
	, SUMMARYSTAT_UI_WATER, SUMMARYSTAT_UI_WATER_BORDER
	, SUMMARYSTAT_UI_WEIGHT, SUMMARYSTAT_UI_WEIGHT_BORDER
	, SUMMARYSTAT_UI_XP, SUMMARYSTAT_UI_XP_BORDER
	, INDEX_SUMMARYSTATLIST_END
};

enum WEAPON_TYPE
{
	WEAPON_DINO,
	WEAPON_HAND,
	WEAPON_HAND_HARVEST,
	WEAPON_HATCHET,
	WEAPON_STONEPICK,
	WEAPON_WOODCLUB,
	WEAPON_SPEAR,
	WEAPON_BOLA,
	WEAPON_BOW,
	WEAPON_END
};

enum STRUCTURE_TYPE
{
	STRUCTURE_CAMPFIRE,					// 모닥불
	STRUCTURE_SLEEPINGBAG,				// 1회용 침낭
	STRUCTURE_FORGE,					// 화로
	STRUCTURE_MORTARPESTAL,				// 절구와 공이
	STRUCTURE_SMITHY,					// 대장간
	STRUCTURE_THATCHBED,				// 침대
	STRUCTURE_WALLTORCH,				// 횃불
	STRUCTURE_WATERWELL,				// 우물
	STRUCTURE_BARRICADE,				// 바리게이트
	STRUCTURE_COMPOSTBIN,				// 퇴비통
	STRUCTURE_CROPPLOT,					// 작물밭
	STRUCTURE_FEEDINGTROUGH,			// 여물통
	STRUCTURE_FIREPLACE,				// 벽난로
	STRUCTURE_S_STORAGE,				// 작은 보관함
	STRUCTURE_L_STORAGE,				// 큰 보관함
	STRUCTURE_COOKINGCAMPFIRE,			// 요리용 냄비

	STRUCTURE_THATCH_BED,				// 침대
	STRUCTURE_WOOD_SLOPE,				// 나무 경사로

	STRUCTURE_THATCH_FLOOR,				// 짚 토대
	STRUCTURE_THATCH_ROOF,				// 짚 천장
	STRUCTURE_THATCH_WALL,				// 짚 벽
	STRUCTURE_THATCH_DOORFRAME,			// 짚 문 프레임
	STRUCTURE_THATCH_DOOR,				// 짚 문
	STRUCTURE_THATCH_STAIRS,			// 짚 계단
	STRUCTURE_THATCH_SLOPEROOF,			// 짚 경사진 지붕
	STRUCTURE_THATCH_SLOPEROOF_LEFT,	// 짚 경사진 지붕 왼쪽
	STRUCTURE_THATCH_SLOPEROOF_RIGHT,	// 짚 경사진 지붕 오른쪽

	STRUCTURE_STONE_FLOOR,				// 석재 토대
	STRUCTURE_STONE_ROOF,				// 석재 천장
	STRUCTURE_STONE_WALL,				// 석재 벽
	STRUCTURE_STONE_WALLWINDOW,			// 석재 벽 창문
	STRUCTURE_STONE_DOORFRAME,			// 석재 문 프레임
	STRUCTURE_STONE_DOOR,				// 석재 문
	STRUCTURE_STONE_STAIRS,				// 석재 계단
	STRUCTURE_STONE_SLOPEDROOF,			// 석재 경사진 지붕
	STRUCTURE_STONE_SLOPEDROOF_LEFT,	// 석재 경사진 지붕 왼쪽
	STRUCTURE_STONE_SLOPEDROOF_RIGHT,	// 석재 경사진 지붕 오른쪽

	STRUCTURE_STONE_GATEFRAME,			// 석재 공룡게이트
	STRUCTURE_STONE_GATEDOOR,			// 석재 공룡게이트 문

	STRUCTURE_END
};

typedef struct tagStructure_Desc
{
	STRUCTURE_TYPE		eStructureType;
	_bool				bIsPreview;
	_vec3				vPos;
	_float				fAngleY;
	_ulong				dwIndex;
	_float				fInteractRad;
	_mat*				pParentMat;
}STRUCTURE_DESC;

typedef struct stStageCount
{
	_int	iLoadingCount = 0;
	_int	iLoadingBarCount = 0;
}stSC;

enum DINOCOLOR1
{
	REDBROWN,
	BROWN,
	SIENNA,
	PERU,
	GALJO,
	SANDYBROWN,
};

enum DINOCOLOR2
{
	HWANGGAL,
	BURLYWOOD,
	MEAL,
	NAVAHO,
	BISQUE,
	BLANCHEDALMOND,
	CORNSILK,
};

// 공룡 색상 1
#define REDBROWN		{128,  0,   0}
#define BROWN			{165,  42, 42}
#define SIENNA			{160,  82, 45}
#define SITBROWN		{139,  69, 19}
#define PERU			{205, 133, 63}
#define GALJO			{218, 165, 32}
#define SANDYBROWN		{244, 164, 96}

// 공룡 색상 2
#define	HWANGGAL		{210, 180, 140}
#define	BURLYWOOD		{222, 184, 135}
#define	MEAL			{245, 222, 179}
#define	NAVAHO			{255, 222, 173}
#define	BISQUE			{255, 228, 196}
#define	BLANCHEDALMOND  {255, 235, 205}
#define	CORNSILK		{255, 248, 220}

//공룡ID
enum DINOTAG
{
	DINOTAG_DODO,
	DINOTAG_PARA,
	DINOTAG_RAPTOR,
	DINOTAG_THERIZINO,
	DINOTAG_REX,
	DINOTAG_IGUANODON,
	DINOTAG_BRACHIO,
	DINOTAG_ALLO,
	DINOTAG_STEGO,
	DINOTAG_YUTYRANNUS,
	DINOTAG_ANKYLO,
	DINOTAG_QUETZAL,
	DINOTAG_PTERA,
	DINOTAG_DRAGON,
	DINOTAG_EGG,
	DINOTAG_END
};

//육성 관련 상수
#define GROWTHTIME		6.f		// 5분
#define MATINGTIME		0.3f	// 15초
#define PREGTIME		0.3f	// 15초	

#endif // !__Value_H__
