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


	// ����Ÿ��
	typedef struct tagVertex_Screen
	{
		_vec4		vPosition;
		_vec2		vTexUV;
	}VTX_SCREEN;

	const _ulong FVF_SCREEN = D3DFVF_XYZRHW | D3DFVF_TEX1;

	/// ���̳��� �Ž�
	typedef	struct D3DXFRAME_DERIVED : public D3DXFRAME
	{
		_mat			CombinedTransformMatrix;		//  = ���� ���� ���� ��� * �θ� ���
	}D3DXFRAME_DERIVED;

	typedef struct D3DXMESHCONTAINER_DERIVED : public D3DXMESHCONTAINER
	{
		LPDIRECT3DTEXTURE9*			ppTexture[TEXTURE_END];

		LPD3DXMESH					pOriMesh;		// ������ ���¸� ������ �ִ� �޽�(���� ������ ������ �ʴ� �޽� ����)

		_ulong						dwNumBones;		// ���� ����

		_mat*						pFrameOffsetMatrix;		// �ִϸ��̼��� ������ ���·� ������ �ִϸ��̼� ������ ���� ��� ����
		_mat**						ppFrameCombinedMatrix;  // ������ ���� CombinedTransformMatrix���� �ּҰ��� �迭�� ���·� �����ϱ� ���� ������ ������

		_mat*						pRenderingMatrix;		// ��� ��ȯ�� ���� ��, ���������� ����� �ϱ� ���� ���(��� �� = pFrameOffsetMatrix * *ppFrameCombinedMatrix)

		//////////////////////////////////////////////////////////////////
		_ulong						dwVtxNum;
		_ulong						dwVtxSize;		// �޽��� ���� ������ ũ�⸦ �����ϴ� ����
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
		LPD3DXBUFFER				pBoneCombiantionBuf;		// LPD3DXBONECOMBINATION ���� ĳ�����ؼ� ����� ��

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
		_vec3		vMin;		// �ø���
		_vec3		vMax;		// �ø���
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
		_int				iInvenNum = NULL;			//������	||  ����		||   �����  ||	   ������	  ||    ����
		_int				iStat = NULL;				//������	||	������		||   ���ݷ�  ||	   ���ݷ�	  ||    ����
		_int				iPosition = NULL;			//�񸶸�	||  ü�� ȸ��	||   ���Ϸ�  ||	  �߰� ä���� ||   ���� ��ġ
		_double				dLife = 0.0;				//���� �ð�	||  ���� �ð�	||   ������  ||	   ������	  ||    ������
		_double				dMaxLife = 0.0;				//								   ��          ��     (Max)
		_float				fWeight = FNULL;			//����	    ||    ����		||    ����	 ||	  	����	  ||     ����
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
		_vec3		vLightPos;							// ShadowMap ���� ��ġ
		_vec3		vLightAt;							// ShadowMap ������ �ٶ󺸴� ��ġ
		_vec3		vLightDir = vLightAt - vLightPos;	// ShadowMap ������ ���� ����
	}SHADOWLIGHT;

}

#endif // !__ENGINE_STRUCT_H__
