#ifndef __ENGINE_STRUCT_H__
#define __ENGINE_STRUCT_H__

namespace Engine
{
	typedef struct tagIndex16
	{
		WORD _1, _2, _3;
	}INDEX16;
	
	typedef struct tagIndex32
	{
		DWORD _1, _2, _3;
	}INDEX32;
	
	typedef struct tagVertexColor
	{
		_vec3 vPosition;
		_uint iColor;
	}VTX_COLOR;

	const _uint FVF_VTX_COLOR = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagVertexRectTex
	{
		_vec3		vPosition;
		_vec2		vTexUV;
	}VTX_TEX;

	const _ulong	FVF_VTX_TEX = D3DFVF_XYZ | D3DFVF_TEX1;

	typedef struct tagVertexTex
	{
		_vec3		vPosition;
		_vec3		vNormal;
		_vec2		vTexUV;
	}VTXTEX;

	const _ulong	FVF_TEX = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	typedef struct tagVertexCube
	{
		_vec3		vPosition;
		_vec3		vTexUV;

	}VTX_CUBE;

	const _ulong	FVF_CUBE = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);


	// 랜더타겟
	typedef struct tagVertex_Screen
	{
		_vec4		vPosition;
		_vec2		vTexUV;
	}VTX_SCREEN;

	const _ulong FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	/// 다이나믹 매쉬
	typedef	struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_mat			CombinedTransformMatrix;		//  = 뼈가 지닌 원본 행렬 * 부모 행렬
	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*			ppTexture[TEXTURE_END];

		LPD3DXMESH					pOriMesh;		// 최초의 상태를 가지고 있는 메시(정점 정보가 변하지 않는 메쉬 상태)

		_ulong						dwNumBones;		// 뼈의 개수

		_mat*						pFrameOffsetMatrix;		// 애니메이션을 포함한 형태로 최초의 애니메이션 정보를 지닌 행렬 상태
		_mat**						ppFrameCombinedMatrix;  // 뼈마다 지닌 CombinedTransformMatrix들의 주소값을 배열의 형태로 보관하기 위해 선언한 포인터

		_mat*						pRenderingMatrix;		// 행렬 변환이 끝난 뒤, 최종적으로 출력을 하기 위한 행렬(결과 값 = pFrameOffsetMatrix * *ppFrameCombinedMatrix)

		//////////////////////////////////////////////////////////////////
		_ulong						dwVtxNum;
		_ulong						dwVtxSize;		// 메쉬가 지닌 정점의 크기를 저장하는 변수
		_vec3*						pVtxPos;

		LPBYTE						pIndex;
		_ulong						dwIndexNum;
		_ulong						dwStride;
		//////////////////////////////////////////////////////////////////////////
		LPD3DXBUFFER				pBoneCombiantionBuf;
		_ulong						dwNumAttributeGroups;
		LPD3DXATTRIBUTERANGE		pAttributeTable;
		_ulong						dwMaxVertInfluences;
		///
		// boneMatrixptr		== ppFrameCombinedMatrix
		// boneOffsetMatrices	== pFrameOffsetMatrix
		// currentBoneMatrices	== pRenderingMatrix
		///
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct SKINNEDMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*			ppTexture[TEXTURE_END];
		//D3DMATERIAL9*				pMaterials;
		

		LPD3DXMESH					pOrigMesh;

		_ulong						dwNumAttributeGroups;
		LPD3DXATTRIBUTERANGE		pAttributeTable;
		_ulong						dwNumInfl;
		LPD3DXBUFFER				pBoneCombiantionBuf;		// LPD3DXBONECOMBINATION 으로 캐스팅해서 사용할 것

		_mat**						ppBoneMatrixPtrs;
		_mat*						pBoneOffsetMatrices;
		_mat*						pCurrentBoneMatrices;
		_ulong						dwNumPalatteEntries;

		_bool						bUseSoftwareVP;
		_ulong						dwAttributeSW;
	}SKINNEDMESHCONTAINER_DERIVED;



	// NaviMesh 
	typedef struct tagNaviMesh
	{
		_vec3 _1;
		_vec3 _2;
		_vec3 _3;
	}NAVIMESH_DESC;

	typedef struct tagNaviMeshInfo
	{
		_ulong			dwNumNaviMesh;
		NAVIMESH_DESC*	pNaviMesh_Desc;
	}NAVUMESHINFO;

	// ObjectInfo
	typedef struct tagObjectInfo
	{
		std::wstring strName;
		MESHTYPE	eMeshType;
		MESHID		eMeshID;
		_vec3		vScale;
		_vec3		vRotate;
		_vec3		vPos;
		_vec3		vMin;		// 컬링용
		_vec3		vMax;		// 컬링용
	}OBJECT_DESC;

	// Collider
	typedef struct tagColInfo
	{
		_vec3			vScale;
		_vec3			vPos;
		COLLIDER_TYPE	eColliderType;
		COLTYPE			eColType;
		COLDIR			eColDir;
		_ubyte			ubyArea;
	}COL_INFO;

	//UI
	typedef struct tagOrthoUIInfo
	{
		_bool				bUpdate = TRUE;
		_bool				bRender = FALSE;
		_int				iIdentifyNumber = NULL;
		_int				iMasterdentifyNumber = NULL;
		_float				fAlpha = FTRUE;
		_float				fPosX  = FNULL;
		_float				fPosY  = FNULL;
		_float				fSizeX = BASICUISIZE;
		_float				fSizeY = BASICUISIZE;
		_float				fEdgeAlpha = FTRUE;
		D3DXVECTOR4			v4EdgeSize;
		_int				iOrderEdge = NULL;
		_float				fOrderContents = 0.0;
		POINT				tMousePos;
		unsigned char		uchSelectUI = NULL;
	}ORTHOUI_INFO;

	typedef struct tagItemInfo
	{
		_tchar*				tchName = nullptr;
		_tchar*				tchKoreaName = nullptr;
		_tchar*				tchToolTip = nullptr;
		_int				iItemID = ITEMID_END;
		_int				iItemType = ITEMID_END;
		_int				iAmount = NULL;
		_int				iMaxAmount = 1;
		_int				iInvenNum = NULL;			//베리류	||  고기류		||   무기류  ||	   도구류	  ||    장비류
		_int				iStat = NULL;				//포만감	||	포만감		||   공격력  ||	   공격력	  ||    방어력
		_int				iPosition = NULL;			//목마름	||  체력 회복	||   스턴력  ||	  추가 채집력 ||   장착 위치
		_double				dLife = 0.0;				//부패 시간	||  부패 시간	||   내구도  ||	   내구도	  ||    내구도
		_double				dMaxLife = 0.0;				//								   상          동     (Max)
		_float				fWeight = FNULL;			//무게	    ||    무게		||    무게	 ||	  	무게	  ||     무게
	}ITEM_INFO;

	typedef struct tagMakingInfo
	{
		_tchar*				tchName = nullptr;
		_tchar*				tchKoreaName = nullptr;
		_int				iMaxMakingAmount = NULL;
		_int				iMakingItemID = ITEMID_END;
		_int				iIngredientItemID_1 = ITEMID_END;
		_int				iIngredientItemID_2 = ITEMID_END;
		_int				iIngredientItemID_3 = ITEMID_END;
		_int				iIngredientItemID_4 = ITEMID_END;
		_int				iNumber_of_Required_ItemID_1 = NULL;
		_int				iNumber_of_Required_ItemID_2 = NULL;
		_int				iNumber_of_Required_ItemID_3 = NULL;
		_int				iNumber_of_Required_ItemID_4 = NULL;
		_double				dMakingTime = 0.0;
		_int				iMakingPossibleLv = NULL;
	}MAKING_INFO;

	typedef struct tagLeftGuideFontInfo
	{
		_tchar*				tchTransText = nullptr;
		_tchar*				tchAmountText = nullptr;
		_tchar*				tchNameText = nullptr;
		_vec2				v2TransPos = {};
		_vec2				v2AmountPos = {};
		_vec2				v2NamePos = {};
		D3DXCOLOR			tTransColor = {};
		D3DXCOLOR			tAmountColor = {};
		D3DXCOLOR			tNameColor = {};
		_int				iItemID = ITEMID_END;
		_int				iAmount = NULL;
		_int				iTransType = NULL;
		_double				dTime = 0.0;
	}LEFTGUIDEFONT_INFO;

	typedef struct tagTopGuideFontInfo
	{
		_tchar*				tchText = nullptr;
		_vec2				v2Pos = {};
		D3DXCOLOR			tColor = {};
		_int				eMessageEnum = NULL;
		_double				dTime = 0.0;
	}TOPGUIDEFONT_INFO;

	typedef struct
	{
		_vec3		vLightPos;							// ShadowMap 광원 위치
		_vec3		vLightAt;							// ShadowMap 광원이 바라보는 위치
		_vec3		vLightDir = vLightAt - vLightPos;	// ShadowMap 광원이 보는 방향
	}SHADOWLIGHT;

}

#endif // !__ENGINE_STRUCT_H__
