#include "stdafx.h"
#include "..\Headers\MiniMap_UI.h"

#include "Export_Function.h"
#include "Player.h"
#include "SleepingBag.h"

#define MiniMapAlpha 0.8f
USING(Engine)

HRESULT CMiniMap_UI::Set_Screen_UI()
{
	STMiniMap_UIURI stURI;

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	return S_OK;
}

CMiniMap_UI::CMiniMap_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CMiniMap_UI::CMiniMap_UI(const CMiniMap_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CMiniMap_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CMiniMap_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = MiniMapAlpha;

	///////////�׽�Ʈ
	//MAP_BED_POS tBedPos;
	//tBedPos.vRealPos = _vec3{ 300.f, 10.f, 2000.f };
	//tBedPos.v2MapPos = Calculate_Bed_Pos(tBedPos.vRealPos);
	//m_mapBedPos.emplace(1, tBedPos);
	return S_OK;
}

_int CMiniMap_UI::Update_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::Update_GameObject(_dTimeDelta);
	//Calculate_Compass_World();
	//Calculate_Player_Pos();
	Select_Bed_Erase();
	MiniMap_Center_Move(_dTimeDelta);

	//if (Engine::KeyDown(KeyCheck_E))
	//	MiniMap_Move(true);
	//if(Engine::KeyDown(KeyCheck_Lbutton))
	//	MiniMap_Move(false);
	return NO_EVENT;
}

_int CMiniMap_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_ORTHOBACKUI, this);
	return NO_EVENT;
}

void CMiniMap_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();

	SetUp_ConstantTable(pEffect, _matOrtho);

	_uint iPassMax = 6;

	pEffect->Begin(&iPassMax, NULL);

	//pEffect->CommitChanges();

	//pEffect->BeginPass(3); ///// �� �׶���

	//m_pBufferCom->Render_VIBuffer();

	//pEffect->EndPass();

	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho,1);
	pEffect->BeginPass(2);    //// ����
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 3);
	pEffect->BeginPass(2);    //// ��ħ��
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////
	//////////////////////////	
	pEffect->CommitChanges();
	SetUp_ConstantTable(pEffect, _matOrtho, 2);
	pEffect->BeginPass(2);    //// �÷��̾� ��ġ
	m_pBufferCom->Render_VIBuffer();
	pEffect->EndPass();
	//////////////////////////

	//ħ�� ����
	if (0 != m_mapBedPos.size())
	{
		for (auto& iter : m_mapBedPos)
		{
			pEffect->CommitChanges();
			SetUp_ConstantTable(pEffect, _matOrtho, 4, iter.second.v2MapPos, iter.first);
			pEffect->BeginPass(2);    //// ħ�� ��ġ
			m_pBufferCom->Render_VIBuffer();
			pEffect->EndPass();
		}
	}
	//////////////////////////////////////////

	pEffect->End();

	Safe_Release(pEffect);

	//������
#ifdef _DEBUG
	wstring wstrPos = to_wstring(m_fa);
	Engine::Render_Font(L"Font_Debug_Bold", wstrPos, &_vec2(400.f, 100.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
#endif
}

HRESULT CMiniMap_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pMiniMapTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_MiniMap"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture2", pComponent);

	pComponent = m_pPingTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Ping"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture3", pComponent);

	pComponent = m_pCompassTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_Compass"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture4", pComponent);

	pComponent = m_pBedSelecTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_BedSelect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture5", pComponent);

	pComponent = m_pBedUnSelecTex_Com = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_BedUnSelect"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_BaseTexture6", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_Loading"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CMiniMap_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo, _vec2 _BedMapPos, _int _iBedKey)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);
	
	if (1 == _bTwo) // ����
	{
		Calculate_Player_Pos();
		//matWorld = m_matMiniMapWorld;
		matWorld._11 = m_tORTHOUI_INFO.fSizeX;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;

		m_pMiniMapTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha-0.1f);
	}
	else if (2 == _bTwo) // �÷��̾� ��ġ
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX*0.1f;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY*0.1f;
		matWorld._33 = FTRUE;

		matWorld._41 = m_v2PlayerPos.x - WINCX * FHALF;
		matWorld._42 = -m_v2PlayerPos.y + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pPingTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha);
	}
	else if (3 == _bTwo) // ��ħ��
	{
		Calculate_Compass_World();
		matWorld = m_matCompassWorld;
		m_pCompassTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", 1.f); 
	}
	else if (4 == _bTwo) //ħ��
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX*0.05f;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY*0.05f;
		matWorld._33 = FTRUE;

		matWorld._41 = _BedMapPos.x - WINCX * FHALF;
		matWorld._42 = -_BedMapPos.y + WINCY * FHALF;
		matWorld._43 = FTRUE;
		if (Now_Mouse_On_BedIcon(_BedMapPos, _vec2{ matWorld._11 , matWorld._22 })&& m_bCenter)
		{
			m_pBedSelecTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
			if (Engine::KeyDown(KeyCheck_Lbutton))
				/*if(m_bCenter)*/
					Bed_Select(_iBedKey);
		}
		else
			m_pBedUnSelecTex_Com->Set_Texture(_pEffect, "g_Bg_Tex");
		_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha);
	}
	else
	{
		matWorld._11 = m_tORTHOUI_INFO.fSizeX;
		matWorld._22 = m_tORTHOUI_INFO.fSizeY;
		matWorld._33 = FTRUE;

		matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
		matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
		matWorld._43 = FTRUE;
		m_pMiniMapTex_Com->Set_Texture(_pEffect, "g_Bg2_Tex");
		_pEffect->SetFloat("g_fAlpha", m_tORTHOUI_INFO.fAlpha);
	}


	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

void CMiniMap_UI::MiniMap_Center_Move(const _double& _dTimeDelta)
{
	if (2 == m_iCenter)
		return;

	if (1 == m_iCenter)
	{
		_vec3 vMapPos = { m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY  , 0.f};
		_vec3 vGoalPos = { WINCX*0.5f, WINCY*0.5f,0.f };
		_vec3 vDir = vGoalPos - vMapPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vMapPos += vDir*(_float)_dTimeDelta * WINCX;

		m_tORTHOUI_INFO.fSizeX += (_float)_dTimeDelta * WINCX;
		m_tORTHOUI_INFO.fSizeY += (_float)_dTimeDelta * WINCX;
		m_tORTHOUI_INFO.fPosX = vMapPos.x;
		m_tORTHOUI_INFO.fPosY = vMapPos.y;
		m_tORTHOUI_INFO.fAlpha += (_float)_dTimeDelta;

		if (WINCY*0.8f <= m_tORTHOUI_INFO.fSizeX)
			m_tORTHOUI_INFO.fSizeX = WINCY*0.8f;
		if (WINCY*0.8f <= m_tORTHOUI_INFO.fSizeY)
			m_tORTHOUI_INFO.fSizeY = WINCY*0.8f;
		if (WINCX*0.5f <= m_tORTHOUI_INFO.fPosX)
			m_tORTHOUI_INFO.fPosX = WINCX*0.5f;
		if (WINCY*0.5f >= m_tORTHOUI_INFO.fPosY)
			m_tORTHOUI_INFO.fPosY = WINCY*0.5f;

		if (0.9f <= m_tORTHOUI_INFO.fAlpha)
			m_tORTHOUI_INFO.fAlpha = 0.9f;

		if ((WINCX*0.5f <= m_tORTHOUI_INFO.fSizeX) && (WINCX*0.5f <= m_tORTHOUI_INFO.fSizeY) && (WINCX*0.5f <= m_tORTHOUI_INFO.fPosX)
			&& (WINCY*0.5f <= m_tORTHOUI_INFO.fPosY) && (0.9f <= m_tORTHOUI_INFO.fAlpha))
		{
			m_iCenter = 2;
			m_bCenter = true;
		}
	}
	else if(0 == m_iCenter)
	{
		_vec3 vMapPos = { m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY  , 0.f };
		_vec3 vGoalPos = { stURI.fPosX[EScrnResolution], stURI.fPosY[EScrnResolution],0.f };
		_vec3 vDir = vGoalPos - vMapPos;
		D3DXVec3Normalize(&vDir, &vDir);
		vMapPos += vDir*(_float)_dTimeDelta * WINCX;

		m_tORTHOUI_INFO.fSizeX -= (_float)_dTimeDelta * WINCX;
		m_tORTHOUI_INFO.fSizeY -= (_float)_dTimeDelta * WINCX;
		m_tORTHOUI_INFO.fPosX = vMapPos.x;
		m_tORTHOUI_INFO.fPosY = vMapPos.y;
		m_tORTHOUI_INFO.fAlpha -= (_float)_dTimeDelta;

		if (stURI.fSizeXY[EScrnResolution] >= m_tORTHOUI_INFO.fSizeX)
			m_tORTHOUI_INFO.fSizeX = stURI.fSizeXY[EScrnResolution];
		if (stURI.fSizeXY[EScrnResolution] >= m_tORTHOUI_INFO.fSizeY)
			m_tORTHOUI_INFO.fSizeY = stURI.fSizeXY[EScrnResolution];
		if (stURI.fPosX[EScrnResolution] >= m_tORTHOUI_INFO.fPosX)
			m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
		if (stURI.fPosY[EScrnResolution] <= m_tORTHOUI_INFO.fPosY)
			m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];

		if (0.7f >= m_tORTHOUI_INFO.fAlpha)
			m_tORTHOUI_INFO.fAlpha = 0.7f;

		if((stURI.fSizeXY[EScrnResolution] >= m_tORTHOUI_INFO.fSizeX) && (stURI.fSizeXY[EScrnResolution] >= m_tORTHOUI_INFO.fSizeY)&& (stURI.fPosX[EScrnResolution] >= m_tORTHOUI_INFO.fPosX)&&
			(stURI.fPosY[EScrnResolution] >= m_tORTHOUI_INFO.fPosY)&& (0.6f >= m_tORTHOUI_INFO.fAlpha))
		{
			m_iCenter = 2;
			m_bCenter = false;
		}
	}

	//2�� �ƴ϶� �������� ������ ħ�� ��ġ ��� ����
	for (auto& iter : m_mapBedPos)
	{
		iter.second.v2MapPos = Calculate_Bed_Pos(iter.second.vRealPos);
	}
}

void CMiniMap_UI::MiniMap_Move(_bool _bCenter)
{
	if (_bCenter)
		m_iCenter = 1;
	else
		m_iCenter = 0;
	return;
}

_int CMiniMap_UI::Bed_Create(_vec3 _vBedWorldPos)
{
	MAP_BED_POS tBedPos;
	tBedPos.vRealPos = _vBedWorldPos;
	tBedPos.v2MapPos = Calculate_Bed_Pos(tBedPos.vRealPos);
	m_mapBedPos.emplace(m_iBedNum, tBedPos);

	return m_iBedNum++;
}

_bool CMiniMap_UI::Bed_Delete(_int _iBedKeyValue)
{
	if (m_mapBedPos.end() == m_mapBedPos.find(_iBedKeyValue))
		return false;
	m_mapBedPos.erase(_iBedKeyValue);
	return true;
}

size_t CMiniMap_UI::Get_BedCount()
{
	return m_mapBedPos.size();
}

void CMiniMap_UI::Bed_Select(_int _iBedKey)
{
	//���� �׽�Ʈ��
	auto& iter = m_mapBedPos.find(_iBedKey);
	if (iter == m_mapBedPos.end())
		return;
	
	m_iEraseBedNum = _iBedKey; //�̰� �� ������ ������ ������

	_vec3 vPos = iter->second.vRealPos;
	vPos.y += 1.f;	//���ӿ� �� �Ĺ����� �ϱ� ���� ������ <- �������� ������ ������ ��� ����. (yȸ���� �����ؾ�)
	static_cast<CPlayer*>(Engine::Get_GameObject(L"Layer_Player", 0))->Player_Respawn(vPos);	//�ش� ��ġ�� �÷��̾� ����
	MiniMap_Move(false);
	//Bed_Delete(_iBedKey);	//�ش� Ű�� ����
	//m_iEraseBedNum = _iBedKey;	//�ʿ��� ����

	//Ű���� �ش��ϴ� ħ�� ã�Ƽ� �Ҹ�ó��
	list<CGameObject*>* pListBed = Engine::Get_LayerList(L"Layer_Structure");
	for (auto& pObj : *pListBed)
	{
		CSleepingBag* pBag = static_cast<CSleepingBag*>(pObj);
		if (_iBedKey == pBag->Get_BedID())
		{
			pBag->TurnOff_Structure();
			return;
		}
	}

	//Ű���� �ش��ϴ� ħ�� ã�� ���ߴٸ� ����
	ERR_MSG(L" CMiniMap_UI::Bed_Select() : No Matching BedKey");
}

void CMiniMap_UI::Select_Bed_Erase()
{
	//�̰͵� �׽�Ʈ ��
	if (-1 != m_iEraseBedNum)
	{
		m_mapBedPos.erase(m_iEraseBedNum);
		m_iEraseBedNum = -1;
	}
}

void CMiniMap_UI::Calculate_Compass_World()
{
	STMiniMap_UIURI stURI;

	_mat matPlayerWorld;
	matPlayerWorld = static_cast<CTransform*>(Engine::Get_GameObject(L"Layer_Player", 0)->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_WorldMatrix();
	
	_vec2 vNorth;
	vNorth = { 0.f, 1.f };
	_vec3 vPlayerLook;
	vPlayerLook = { matPlayerWorld._31,0.f,matPlayerWorld._33 };
	D3DXVec3Normalize(&vPlayerLook, &vPlayerLook);

	_float fDot = vNorth.x*vPlayerLook.x + vNorth.y*vPlayerLook.z;
	_float fDet = vNorth.x*vPlayerLook.z - vNorth.y*vPlayerLook.x;
	_float fAngle = atan2(fDet, fDot);
	if (0 > fAngle)
		fAngle += D3DXToRadian(360.f);

#ifdef _DEBUG
	m_fa = fAngle *(180.f / 3.141592f);
#endif
	D3DXMatrixIdentity(&m_matCompassWorld);

	m_matCompassWorld._11 = stURI.fCompassSizeXY[EScrnResolution]; //������
	m_matCompassWorld._22 = stURI.fCompassSizeXY[EScrnResolution];
	m_matCompassWorld._33 = FTRUE;
	 
	_mat matRotZ;
	D3DXMatrixRotationZ(&matRotZ, fAngle); //�����̰��� �� �� ���� ������ �׳� �갡 ����, �䰡 �׵��̶� Z�� ȸ��
	m_matCompassWorld *= matRotZ;
	D3DXMatrixRotationZ(&matRotZ,D3DXToRadian(180.f));
	m_matCompassWorld *= matRotZ;

	m_matCompassWorld._41 = m_v2PlayerPos.x - WINCX * FHALF;
	m_matCompassWorld._42 = -(m_v2PlayerPos.y)+ WINCY * FHALF;
	
}

void CMiniMap_UI::Calculate_Player_Pos()
{
	_vec2 v2MapPos = { m_tORTHOUI_INFO.fPosX ,m_tORTHOUI_INFO.fPosY };
	//_vec2 v2MapSize = { (m_tORTHOUI_INFO.fSizeY*0.75f)*(332.f/ 500.f) ,m_tORTHOUI_INFO.fSizeY*0.75f }; //�׸� ���� �����ֱ�
	_vec2 v2MapSize = { m_tORTHOUI_INFO.fSizeY ,m_tORTHOUI_INFO.fSizeY}; 

	_vec2 v2RealMapSize = { 1260.f ,4350.f };
	_vec2 v2RealMapStaryPos = { -700.f ,-110.f };
	_mat matPlayerWorld;
	matPlayerWorld = static_cast<CTransform*>(Engine::Get_GameObject(L"Layer_Player", 0)->Get_Component(L"Com_Transform", ID_DYNAMIC))->Get_WorldMatrix();
	_vec2 v2PlayerPos = { matPlayerWorld._41,matPlayerWorld._43 }; //�� �÷��̾� ����

	_vec2 v2FixPlayerPos = v2PlayerPos - v2RealMapStaryPos; //���� �� ����� 0,0���� ������ �ɷ� ������
															//��ũ���� �� ������� 0,0�� ����� ���� ����...�ʵ� �� �� �¾Ƽ� ��
	//////////////////���� �� �ȼ��� 332 x 500
	//////////////////y �� ��� 21 �� ���� 471
	//////////////////x ���� �� 23 ���� �� 285

	//////////////////y ��� ����� ���� 4.2%
	//////////////////y �ϴ� ����� ���� 5.8%
	//////////////////x ���� ����� ���� 6.9%
	//////////////////x ���� ����� ���� 14.1%

	////////////////////�ȼ� ��� �սô�.....
	////////////////////y ��� ����� �ȼ�
	//////////////////_float fTopEmpty = (v2MapSize.y / 100.f) * 4.2f;
	////////////////////y �ϴ� ����� �ȼ�
	//////////////////_float fBottomEmpty = (v2MapSize.y / 100.f) * 5.8f;
	////////////////////x ���� ����� �ȼ�
	//////////////////_float fLeftEmpty = (v2MapSize.x / 100.f) * 6.9f;
	////////////////////x ���� ����� �ȼ�
	//////////////////_float fRightEmpty = (v2MapSize.x / 100.f) * 14.1f;

	// ���� �� �ȼ��� 768 x 768
	// y �� ��� 69 �� ���� 637
	// x ���� �� 232 ���� �� 537
	
	// y ��� ����� ���� 9.f%
	// y �ϴ� ����� ���� 17.05%
	// x ���� ����� ���� 30.2%
	// x ���� ����� ���� 30.7%

	//�ȼ� ��� �սô�.....
	//y ��� ����� �ȼ�
	_float fTopEmpty = (v2MapSize.y / 100.f) * 9.f;
	//y �ϴ� ����� �ȼ�
	_float fBottomEmpty = (v2MapSize.y / 100.f) * 17.05f;
	//x ���� ����� �ȼ�
	_float fLeftEmpty = (v2MapSize.x / 100.f) * 30.2f;
	//x ���� ����� �ȼ�
	_float fRightEmpty = (v2MapSize.x / 100.f) * 30.7f;

	//�� ��ũ�� ��ǥ�� ���� �ȼ�
	_vec2 v2FixMapOnePos;
	v2FixMapOnePos.x = v2MapPos.x - (v2MapSize.x * 0.5f);
	v2FixMapOnePos.y = v2MapPos.y + (v2MapSize.y * 0.5f); //Y�� ���ؾߴ�!

	v2FixMapOnePos.x += fLeftEmpty;
	v2FixMapOnePos.y -= fBottomEmpty; //Y�� ���ߴ�!

	//���� ���� ������
	v2MapSize.x = v2MapSize.x - fLeftEmpty - fRightEmpty;
	v2MapSize.y = v2MapSize.y - fTopEmpty - fBottomEmpty;
	//���� �� ũ��� ��ũ�� �� ���� ���� (x , y ����)
	_vec2 v2Scale = { (v2MapSize.x/v2RealMapSize.x) ,(v2MapSize.y / v2RealMapSize.y)};

	//�÷��̾� �Ƚ� �������� ���� �����༭ �� ��ũ�� ��ǥ�� ���� �ȼ����� �����ָ� �űⰡ �÷��̾� ��ġ!
	m_v2PlayerPos.x = v2FixMapOnePos.x + (v2FixPlayerPos.x * v2Scale.x);
	m_v2PlayerPos.y = v2FixMapOnePos.y - (v2FixPlayerPos.y * v2Scale.y); // Y�� �����¡

	////��.. ���� ���� 8.5�� ���ư� �ִ�.... 
	//�÷��̾� ������ ���� (m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY ��������) 170�� ��������
	m_v2PlayerPos.x -= m_tORTHOUI_INFO.fPosX;
	m_v2PlayerPos.y -= m_tORTHOUI_INFO.fPosY;

	_mat matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-8.5f));			//����
	_vec3 vRotZPlayer = { m_v2PlayerPos.x , m_v2PlayerPos.y ,0.f  };
	D3DXVec3TransformNormal(&vRotZPlayer, &vRotZPlayer, &matRotZ);
	m_v2PlayerPos.x = vRotZPlayer.x + m_tORTHOUI_INFO.fPosX;
	m_v2PlayerPos.y = vRotZPlayer.y + m_tORTHOUI_INFO.fPosY;

	////�̹����� ��������..

	//D3DXMatrixIdentity(&m_matMiniMapWorld);

	//m_matMiniMapWorld._11 = m_tORTHOUI_INFO.fSizeX*0.75f;
	//m_matMiniMapWorld._22 = m_tORTHOUI_INFO.fSizeY*0.75f;
	//m_matMiniMapWorld._33 = FTRUE;                             //������

	//m_matMiniMapWorld *= matRotZ;

	//m_matMiniMapWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	//m_matMiniMapWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF; //�̵�
}

_vec2 CMiniMap_UI::Calculate_Bed_Pos(_vec3 _vRealBedPos)
{
	_vec3 vBedMapPos;

	_vec2 v2MapPos = { m_tORTHOUI_INFO.fPosX ,m_tORTHOUI_INFO.fPosY };
	_vec2 v2MapSize = { m_tORTHOUI_INFO.fSizeY ,m_tORTHOUI_INFO.fSizeY }; //�׸� ���� �����ֱ�

	_vec2 v2RealMapSize = { 1260.f ,4350.f };
	_vec2 v2RealMapStaryPos = { -700.f ,-110.f };
	_vec2 v2BedPos = { _vRealBedPos.x,_vRealBedPos.z }; //�� ħ�� ����

	_vec2 v2FixBedPos = v2BedPos - v2RealMapStaryPos; //���� �� ����� 0,0���� ������ �ɷ� ������
															//�ȼ� ��� �սô�.....
															//y ��� ����� �ȼ�
	_float fTopEmpty = (v2MapSize.y / 100.f) * 9.f;
	//y �ϴ� ����� �ȼ�
	_float fBottomEmpty = (v2MapSize.y / 100.f) * 17.05f;
	//x ���� ����� �ȼ�
	_float fLeftEmpty = (v2MapSize.x / 100.f) * 30.2f;
	//x ���� ����� �ȼ�
	_float fRightEmpty = (v2MapSize.x / 100.f) * 30.7f;

	//�� ��ũ�� ��ǥ�� ���� �ȼ�
	_vec2 v2FixMapOnePos;
	v2FixMapOnePos.x = v2MapPos.x - (v2MapSize.x * 0.5f);
	v2FixMapOnePos.y = v2MapPos.y + (v2MapSize.y * 0.5f); //Y�� ���ؾߴ�!

	v2FixMapOnePos.x += fLeftEmpty;
	v2FixMapOnePos.y -= fBottomEmpty; //Y�� ���ߴ�!

									  //���� ���� ������
	v2MapSize.x = v2MapSize.x - fLeftEmpty - fRightEmpty;
	v2MapSize.y = v2MapSize.y - fTopEmpty - fBottomEmpty;
	//���� �� ũ��� ��ũ�� �� ���� ���� (x , y ����)
	_vec2 v2Scale = { (v2MapSize.x / v2RealMapSize.x) ,(v2MapSize.y / v2RealMapSize.y) };

	//�÷��̾� �Ƚ� �������� ���� �����༭ �� ��ũ�� ��ǥ�� ���� �ȼ����� �����ָ� �űⰡ �÷��̾� ��ġ!
	vBedMapPos.x = v2FixMapOnePos.x + (v2FixBedPos.x * v2Scale.x);
	vBedMapPos.y = v2FixMapOnePos.y - (v2FixBedPos.y * v2Scale.y); // Y�� �����¡

																		 //��.. ���� ���� -8.5�� ���ư� �ִ�.... 
																		 //�÷��̾� ������ ���� (m_tORTHOUI_INFO.fPosX , m_tORTHOUI_INFO.fPosY ��������) 170�� ��������
	vBedMapPos.x -= m_tORTHOUI_INFO.fPosX;
	vBedMapPos.y -= m_tORTHOUI_INFO.fPosY;

	_mat matRotZ;
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-8.5f));			//����
	_vec3 vRotZBed = { vBedMapPos.x , vBedMapPos.y ,0.f };
	D3DXVec3TransformNormal(&vRotZBed, &vRotZBed, &matRotZ);
	vBedMapPos.x = vRotZBed.x + m_tORTHOUI_INFO.fPosX;
	vBedMapPos.y = vRotZBed.y + m_tORTHOUI_INFO.fPosY;

	return _vec2{ vBedMapPos.x,vBedMapPos.y};
}

_bool CMiniMap_UI::Now_Mouse_On_BedIcon(_vec2 _vRealBedPos, _vec2 _vRealSize)
{
	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
	_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

	_float fHalfX = _vRealSize.x * FHALF;
	_float fHalfY = _vRealSize.y * FHALF;
	if (MouseX >= _vRealBedPos.x - fHalfX
		&& MouseX <= _vRealBedPos.x + fHalfX
		&& MouseY >= _vRealBedPos.y - fHalfY
		&& MouseY <= _vRealBedPos.y + fHalfY)
		return true;
	else
		return false;
}

CMiniMap_UI * CMiniMap_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CMiniMap_UI*	pInstance = new CMiniMap_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CMiniMap_UI::Clone(void * _pArg)
{
	CMiniMap_UI*	pClone = new CMiniMap_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CMiniMap_UI::Free()
{
	COrthoUI::Free();
}