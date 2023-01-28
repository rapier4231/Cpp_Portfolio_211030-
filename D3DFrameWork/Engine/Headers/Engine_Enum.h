#ifndef __ENGINE_ENUM_H__
#define __ENGINE_ENUM_H__

//
namespace Engine
{
	enum WINMODE { MODE_FULL, MODE_WIN };				// 윈도우 모드
	enum COMPONENTID { ID_DYNAMIC, ID_STATIC, ID_END };	// 컴포넌트 타입 (업데이트 여부)

	// trasnform
	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POS, INFO_END };
	enum ROTATE { ROT_X, ROT_Y, ROT_Z, ROT_END };
	enum TRANSINFO { TRANS_SCALE, TRANS_ROTATE, TRANS_POS, TRANS_END};

	// Direct Input Key
	enum MOUSEKEYSTATE { DIM_LB, DIM_RB, DIM_MB, DIM_END };
	enum MOUSEMOVESTATE { DIMS_X, DIMS_Y, DIMS_Z, DIMS_END };

	//  Texture
	enum TEXTURETYPE { TEX_NORMAL, TEX_CUBE, TEX_END };

	// Render Tpye
	enum RENDERID { RENDER_PRIORITY, RENDER_NONALPHA, RENDER_ALPHA, RENDER_EFFECT
		,RENDER_WolrdPosUI, RENDER_BACKUI, RENDER_ORTHOBACKUI, RENDER_ORTHOUI, RENDER_ITEMBORDER, RENDER_ITEMUI,RENDER_LASTUI
		, RENDER_AFTEREFFECT, RENDER_SHADOWMAP, RENDER_END };
	
	// NaviMesh
	enum COMPARE { COMPARE_LEFT, COMPARE_RIGHT }; // 플레이어 위치를 기준으로 왼쪽에 선분이 있으면 이동한 것

	// Rotate
	enum ROTATE_QUATERNION { ROT_PITCH, ROT_YAW, ROT_ROLL, ROTQUATERNION_END };

	// Textrue Type
	enum TEXTURE_TYPE { TEXTURE_DIFFUSE, TEXTURE_NORMAL, /*TEXTURE_SPECULAR,*/ TEXTURE_MASK, /*TEXTURE_ALPHA,*/ TEXTURE_END };

	/// 오브젝트
	enum MESHID
	{
		NAVIMESH,
		BUILDING_STATIC,
		STRUCTURE_STATIC,
		ETC_STATIC,
		END_OBJECTID
	};

	enum MESHTYPE
	{
		STATIC_MESH,
		DYNAMIC_MESH,
		NAVI_MESH,
		END_MESH
	};

	enum SHADER_PASS_MESH
	{
		SHADER_PASS_NORMAL,
		SHADER_PASS_ALPHATEST,
		SHADER_PASS_DISSOLVE,
		SHADER_PASS_DISSOLVE_A,
		SHADER_PASS_EFFECT,
		SHADER_PASS_NORMAL_CUL = 6,
		SHADER_PASS_NORMAL_END
	};

	// 콜리더
	enum COLLIDER_TYPE
	{
		COLLIDER_BOX,
		COLLIDER_SPHERE,
		COLLIDER_END
	};

	enum COLTYPE
	{
		COL_STATIC,			// 고정
		COL_WALL,			// 벽
		COL_OVER,			// 장애물
		COL_HIGHOVER,		// 높은 장애물
		COL_HANG,			// 매달리기 (벽 없음)
		COL_HANGWALL,		// 매달리기 (벽딛고)
		COL_HANGFREE,		// 매달리기 (봉)
		COL_BALANCE,		// 균형잡기
		COL_LADDER,			// 사다리
		COL_EDGEWORK,		// 가장자리 걷기 (게걸음)
		COL_ZIPLINE,		// 짚라인
		COL_SAVEPOINT,		// 세이브포인트
		COL_BLACKHAWK,			// 블랙호크
		COL_UP_MIDDLE,		// 중간 올라가기
		COL_END
	};

	enum COLTYPEID { COLTYPE_STATIC, COLTYPE_INTERACT, COLTYPE_END };
	enum COLDIR { COLDIR_TOP, COLDIR_RIGHT,  COLDIR_BOTTOM, COLDIR_LEFT, COLDIR_END };
	enum COLLISIONSTATE { COL_FALSE, COL_TRUE, COLLISIONSTATE_END };

	enum CULL_TYPE
	{
		CULL_VIEW,
		CULL_LIGHT,
		CULL_END
	};

	enum InvenUserUnitTypeID {
		InvenUserUnitTypeID_Player, InvenUserUnitTypeID_Monster, InvenUserUnitTypeID_Campfire
		, InvenUserUnitTypeID_Smithy, InvenUserUnitTypeID_Mortar, 
		InvenUserUnitTypeID_Locker, InvenUserUnitTypeID_Brazier, InvenUserUnitTypeID_Harvestable,	InvenUserUnitTypeID_END};

	
	enum CENTER_UIID { CENTER_STAT_UI,CENTER_MONSTER_UI, CENTER_MYMONSTER_UI,CENTER_CAMPFIRE_UI, 
		CENTER_SMITHY_UI, CENTER_MORTAR_UI, CENTER_LOCKER_UI, CENTER_BRAZIER_UI,CENTER_UI_END };

#pragma region ItemEnum
	enum ITEMID { 
		ITEMID_Consumable = 0	, ITEMID_AmarBerry, 
		ITEMID_Consumable_Meat = 100, ITEMID_Meat, ITEMID_CookedMeat,
		ITEMID_Craft      = 200	, ITEMID_Wood, ITEMID_Fiber, ITEMID_Thatch, ITEMID_Stone, ITEMID_Flint, ITEMID_Hide, ITEMID_MetalOre, ITEMID_MetalIngot,
		ITEMID_Weapon     = 300	, ITEMID_Club, ITEMID_ForgedCrossbow, ITEMID_Bow,
		ITEMID_CsWeapon	  = 400	, ITEMID_Bolas, ITEMID_Spear, ITEMID_Arrow, ITEMID_TranqArrow,
		ITEMID_Tool		  = 500	, ITEMID_StoneAx, ITEMID_StonePick,
		ITEMID_Equip      = 600	, ITEMID_Leather_Hat, ITEMID_Leather_TOP, ITEMID_Leather_Legs, ITEMID_Leather_Gloves, ITEMID_Leather_Boots
								, ITEMID_Metal_Hat, ITEMID_Metal_TOP, ITEMID_Metal_Legs, ITEMID_Metal_Gloves, ITEMID_Metal_Boots
								, ITEMID_Wood_Sheild,
		ITEMID_Construct  = 700 , ITEMID_Campfire, ITEMID_SleepingBag, ITEMID_Forge, ITEMID_MortarPestle, ITEMID_Smithy, ITEMID_ThatchBed
								, ITEMID_WallTorch, ITEMID_WaterWell, ITEMID_Barricade, ITEMID_CompostBin, ITEMID_CropPlot, ITEMID_FeedingTrough, ITEMID_Fireplace, ITEMID_S_Storage, ITEMID_L_Storage, ITEMID_CookingCampFire
								, ITEMID_ThatchFloor, ITEMID_ThatchRoof, ITEMID_ThatchWall, ITEMID_ThatchDoorFrame, ITEMID_ThatchDoor, ITEMID_ThatchStairs, ITEMID_ThatchSlopedRoof, ITEMID_ThatchSlopedRoofLeft, ITEMID_ThatchSlopedRoofRight
								, ITEMID_StoneFloor, ITEMID_StoneRoof, ITEMID_StoneWall, ITEMID_StoneWindowWall, ITEMID_StoneDoorFrame, ITEMID_StoneDoor, ITEMID_StoneStairs, ITEMID_StoneSlopedRoof, ITEMID_StoneSlopedRoofLeft, ITEMID_StoneSlopedRoofRight
								, ITEMID_StoneGateFrame, ITEMID_StoneGateDoor, ITEMID_WoodSloped,
		ITEMID_Etc        = 800, ITEMID_END};

	enum EQUIP_Position {EQUIP_HAT, EQUIP_TOP, EQUIP_LEGS, EQUIP_GLOVES, EQUIP_SUB, EQUIP_BOOTS, EQUIP_Position_END};
#pragma endregion

#pragma region StatbarEnum
	enum ESTATbar {
		STATbar_HP, STATbar_Stamina, STATbar_Food, STATbar_Water, STATbar_Weight,
		STATbar_Atk, STATbar_Speed, STATbar_Engram, STATbar_Endure, STATbar_Paralysis, STATbar_EXP, STATbar_End
	};

	enum EMonsterSTATbar {
		EMonsterSTATbar_HP, EMonsterSTATbar_Stamina, EMonsterSTATbar_Food, EMonsterSTATbar_Weight,
		EMonsterSTATbar_Atk, EMonsterSTATbar_Speed, EMonsterSTATbar_Paralysis, EMonsterSTATbar_EXP, EMonsterSTATbar_End
	};

	enum EStructureSTATbar { EStructureSTATbar_HP, EStructureSTATbar_Amount, EStructureSTATbar_Weight, EStructureSTATbar_End};
#pragma endregion

	// Light Status
	enum SETLIGHTSTAT { LIGHTPOWERUP, LIGHTPOWERDOWN, LIGHTRANGEUP, LIGHTRANGEDOWN, LIGHTEND };
}
#endif // !__ENGINE_ENUM_H__
