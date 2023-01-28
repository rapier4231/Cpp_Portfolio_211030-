#include "stdafx.h"
#include "..\Headers\DinoList_UI.h"

#include "Export_Function.h"
#include "Unit.h"

#define RING_ALPHA 0.6f
#define RING_COMMAND_COLOR {0.1f,0.7f,0.9f,0.2f}
#define RING_FONT_COLOR {1.f,1.f,1.f,1.f}
#define RING_Dis_Near 0.2f
#define RING_Dis_Far 0.48f

USING(Engine)

HRESULT CDinoList_UI::Set_Screen_UI()
{
	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	return S_OK;
}

CDinoList_UI::CDinoList_UI(LPDIRECT3DDEVICE9 _pDevice)
	: COrthoUI(_pDevice)
{
}

CDinoList_UI::CDinoList_UI(const CDinoList_UI & rhs)
	: COrthoUI(rhs)
{
}

HRESULT CDinoList_UI::Ready_GameObject_Prototype()
{
	COrthoUI::Ready_GameObject_Prototype();

	return S_OK;
}

HRESULT CDinoList_UI::Ready_GameObject(void * _pArg/* = nullptr*/)
{
	COrthoUI::Ready_GameObject(_pArg);

	m_tORTHOUI_INFO.fSizeX = stURI.fSizeX[EScrnResolution];
	m_tORTHOUI_INFO.fSizeY = stURI.fSizeY[EScrnResolution];
	m_tORTHOUI_INFO.fPosX = stURI.fPosX[EScrnResolution];
	m_tORTHOUI_INFO.fPosY = stURI.fPosY[EScrnResolution];
	m_tORTHOUI_INFO.fAlpha = RING_ALPHA;

	//vector<STRCD> Temp;
	//STRCD Teemp;
	//Teemp.pwchCommand_Text = L"인벤토리 열기";
	//Teemp.bCharging_Click = true;
	//Temp.emplace_back(Teemp);
	//Teemp.pwchCommand_Text = L"이름 다시 짓기";
	//Teemp.bCharging_Click = true;
	//Temp.emplace_back(Teemp);
	//Teemp.pwchCommand_Text = L"건물 철거";
	//Teemp.bCharging_Click = true;
	//Temp.emplace_back(Teemp);
	//Teemp.pwchCommand_Text = L"나를 따라와";
	//Temp.emplace_back(Teemp);
	//Teemp.pwchCommand_Text = L"가만히 멈춰 서";
	//Temp.emplace_back(Teemp);
	//Teemp.pwchCommand_Text = L"너는 아무것도 하지마";
	//Temp.emplace_back(Teemp);
	//Teemp.pwchCommand_Text = L"앞으로 달려가";
	//Temp.emplace_back(Teemp);
	//Open_DinoList(Temp);
	return S_OK;
}

_int CDinoList_UI::Update_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
	{
		m_fOpen = 0.f;
		return NO_EVENT;
	}
	COrthoUI::Update_GameObject(_dTimeDelta);
	m_fOpen += (_float)_dTimeDelta*1.5f;
	if (m_fOpen > 1.f)
		m_fOpen = 1.f;
	Ef_Func(_dTimeDelta);

	Select_Func(Mouse_Func(_dTimeDelta));

	return NO_EVENT;
}

_int CDinoList_UI::LateUpdate_GameObject(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
		return NO_EVENT;
	COrthoUI::LateUpdate_GameObject(_dTimeDelta);

	Engine::Add_RenderGroup(RENDER_LASTUI, this);
	return NO_EVENT;
}

void CDinoList_UI::Render_GameObject(_mat _matOrtho)
{
	LPD3DXEFFECT pEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(pEffect);
	pEffect->AddRef();


	_uint iPassMax = 1;

	pEffect->Begin(&iPassMax, NULL);


	////////////////////////	
	for (_int i = 0; i < m_iCommandAmount; ++i)
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable(pEffect, _matOrtho, i);
		pEffect->BeginPass(0); 
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
	}
	////////////////////////
	
	/////////////////////////효과 UI 
	for (_int i = 0; i < 3; ++i)
	{
		pEffect->CommitChanges();
		SetUp_ConstantTable_To_EffectUI(pEffect, _matOrtho, i);
		pEffect->BeginPass(1);
		m_pBufferCom->Render_VIBuffer();
		pEffect->EndPass();
	}
	///////////////////////

	pEffect->End();

	Safe_Release(pEffect);
	Render_Font();
}

HRESULT CDinoList_UI::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRing_Command_Tex = static_cast<CTexture*>(Engine::Clone_ComponentPrototype(L"Proto_Texture_DinoList"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Texture", pComponent);

	pComponent = m_pShaderCom = static_cast<CShader*>(Engine::Clone_ComponentPrototype(L"Proto_Shader_DinoList_UI"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Shader", pComponent);

	pComponent = m_pBufferCom = static_cast<CVIBuffer_RectTex*>(Engine::Clone_ComponentPrototype(L"Proto_Buffer_RectTex"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[ID_DYNAMIC].emplace(L"Com_Buffer", pComponent);

	return S_OK;
}

HRESULT CDinoList_UI::SetUp_ConstantTable(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matView);

	matWorld._11 = m_tORTHOUI_INFO.fSizeX;
	matWorld._22 = m_tORTHOUI_INFO.fSizeY;
	matWorld._33 = FTRUE;

	matWorld._41 = m_tORTHOUI_INFO.fPosX - WINCX * FHALF;
	matWorld._42 = -m_tORTHOUI_INFO.fPosY + WINCY * FHALF;
	matWorld._43 = FTRUE;

	_pEffect->SetFloat("g_Left_Radian", D3DXToRadian(m_vecSAs[_bTwo].fLeft_Angle));
	_pEffect->SetFloat("g_Right_Radian", D3DXToRadian(m_vecSAs[_bTwo].fRight_Angle));
	_pEffect->SetFloat("g_Dis_Fill", m_vecSAs[_bTwo].fDis_Fill* m_fOpen);
	_pEffect->SetFloat("g_FillAlpha", m_tORTHOUI_INFO.fAlpha);
	_pEffect->SetFloat("g_Dis_Near", RING_Dis_Near * m_fOpen);
	_pEffect->SetFloat("g_Dis_Far", RING_Dis_Far * m_fOpen);
	_pEffect->SetVector("g_vecColor", &_vec4 RING_COMMAND_COLOR);
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matView", &matView);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

HRESULT CDinoList_UI::SetUp_ConstantTable_To_EffectUI(LPD3DXEFFECT & _pEffect, _mat _matOrtho, _int _bTwo)
{
	_mat matWorld, matView;
	D3DXMatrixIdentity(&matWorld);

	m_pRing_Command_Tex->Set_Texture(_pEffect,"g_Bg_Tex",_bTwo);
	_pEffect->SetFloat("g_Left_Radian", m_fRotate[_bTwo]);
	_pEffect->SetFloat("g_FillAlpha", m_fEf_UI_Alpha);
	_pEffect->SetVector("g_vecColor", &_vec4 { m_fEf_UI_Size,m_fEf_UI_Size,stURI.fPosX[EScrnResolution],stURI.fPosY[EScrnResolution] });
	_pEffect->SetFloat("g_Dis_Near", WINCX * FHALF);
	_pEffect->SetFloat("g_Dis_Far", WINCY * FHALF);
	_pEffect->SetMatrix("g_matWorld", &matWorld);
	_pEffect->SetMatrix("g_matProj", &_matOrtho);
	return S_OK;
}

_int CDinoList_UI::Mouse_Func(const _double& _dTimeDelta)
{
	_int iChoice = -1;

	Engine::Get_pMousePos(&m_tORTHOUI_INFO.tMousePos);
	_float MouseX = (_float)m_tORTHOUI_INFO.tMousePos.x;
	_float MouseY = (_float)m_tORTHOUI_INFO.tMousePos.y;

	////////////////////////////거리 체크 부터, 연산이 적으니까 먼저 걸러내는데 쓰자!
	_float fDis = (_float)sqrt((stURI.fPosX[EScrnResolution] - MouseX)* (stURI.fPosX[EScrnResolution] - MouseX) + (stURI.fPosY[EScrnResolution] - MouseY)*(stURI.fPosY[EScrnResolution] - MouseY));
	if (!(fDis > (stURI.fSizeX[EScrnResolution] * RING_Dis_Near) && fDis < (stURI.fSizeX[EScrnResolution] * RING_Dis_Far)))
	{
		for (_int iVecIndex = 0; iVecIndex < m_iCommandAmount; ++iVecIndex)
			m_vecSAs[iVecIndex].fDis_Fill = RING_Dis_Near;
		return iChoice;
	}

	//////////////////////////어느 각도에 있는지!
	_float fPSRadian;
	fPSRadian = atan2(MouseY - WINCY * FHALF, MouseX - WINCX * FHALF);
	if (0 > fPSRadian)
		fPSRadian += D3DXToRadian(360.f);

	fPSRadian *=(180.f / 3.141592f);
	
	for (_int iVecIndex = 0; iVecIndex < m_iCommandAmount; ++iVecIndex)
	{
		if (m_vecSAs[iVecIndex].fLeft_Angle > m_vecSAs[iVecIndex].fRight_Angle)
			if ((fPSRadian < m_vecSAs[iVecIndex].fRight_Angle&&(fPSRadian > 0.f ) )|| (fPSRadian > m_vecSAs[iVecIndex].fLeft_Angle &&(360.f > fPSRadian)))
			{
				m_vecSAs[iVecIndex].fDis_Fill += (_float)_dTimeDelta*0.5f;
				if(KeyDown(KeyCheck_Lbutton))
					if (true == m_vecCommand[iVecIndex].bCharging_Click)
					{
						if (RING_Dis_Far < m_vecSAs[iVecIndex].fDis_Fill)
							iChoice = iVecIndex;
					}
					else
						iChoice = iVecIndex;
			}
			else
				m_vecSAs[iVecIndex].fDis_Fill = RING_Dis_Near;
		else
			if (fPSRadian < m_vecSAs[iVecIndex].fRight_Angle && fPSRadian > m_vecSAs[iVecIndex].fLeft_Angle)
			{
				m_vecSAs[iVecIndex].fDis_Fill += (_float)_dTimeDelta*0.5f;
				if (KeyDown(KeyCheck_Lbutton))
					if (true == m_vecCommand[iVecIndex].bCharging_Click)
					{
						if (RING_Dis_Far < m_vecSAs[iVecIndex].fDis_Fill)
							iChoice = iVecIndex;
					}
					else
						iChoice = iVecIndex;
			}
			else
				m_vecSAs[iVecIndex].fDis_Fill = RING_Dis_Near;
	}

	return iChoice;
}

void CDinoList_UI::Select_Func(_int _iSelectCommand)
{
	if (-1 == _iSelectCommand)
		return;

	m_fOpen = 0.f;
	m_fRotate[0] = 0.f;
	m_fRotate[1] = 0.f;
	m_fRotate[2] = 0.f;
	m_fEf_UI_Alpha = 0.f;
	m_fEf_UI_Size = 0.2f;
	m_iCommandAmount = 0;
	m_tORTHOUI_INFO.bRender = false;
	m_vecSAs.clear();
	vector<STSAs>().swap(m_vecSAs);
	m_vecCommand.clear();
	vector<STRCD>().swap(m_vecCommand);
	//m_pUnit->Set_Ring_Command(_iSelectCommand);
	return;
}

void CDinoList_UI::Split_To_Circle()
{
	//벡터 공간 확보
	m_vecSAs.resize(m_iCommandAmount);
	//한 명령당 가지는 각도
	_float fIncluded_Angle = 360.f / m_iCommandAmount;
	//돌리면서
	STSAs stTemp;

	_float fAngle = -90.f; //아크 탄젠트는 X축부터 0도해서 반시계 방향인데, 게임에선 90도 부터 시계방향으로 커맨드가 뜨는 거 같으므로 이케 한다!

	for (_int iVecIndex = 0; iVecIndex < m_iCommandAmount;++iVecIndex)
	{
		stTemp.fLeft_Angle = fAngle - (fIncluded_Angle * FHALF) + m_fDis_Command_HalfGap;
		if (0 > stTemp.fLeft_Angle)
			stTemp.fLeft_Angle = stTemp.fLeft_Angle += 360.f;
		stTemp.fRight_Angle = fAngle + (fIncluded_Angle * FHALF) - m_fDis_Command_HalfGap;
		if (0 > stTemp.fRight_Angle)
			stTemp.fRight_Angle = stTemp.fRight_Angle += 360.f;
		stTemp.fRadin = D3DXToRadian(fAngle /*+ 90.f*/);
		m_vecSAs[iVecIndex]=(stTemp);
		fAngle += fIncluded_Angle;
	}
}

void CDinoList_UI::Render_Font()
{
	for (_int i = 0; i < m_iCommandAmount; i++)
	{
		_vec3 vPos;
		_float fDis = 280.f;
		vPos.x = fDis * cos(m_vecSAs[i].fRadin) + WINCX * FHALF;
		vPos.y = fDis * sin(m_vecSAs[i].fRadin) + WINCY * FHALF;
	
		Engine::Render_Font(L"Font_TapName", m_vecCommand[i].pwchCommand_Text, &_vec2(vPos.x, vPos.y), RING_FONT_COLOR,2);
	}
}

void CDinoList_UI::Ef_Func(const _double & _dTimeDelta)
{
	if (!m_tORTHOUI_INFO.bRender)
	{
		m_fRotate[0] = 0.f;
		m_fRotate[1] = 0.f;
		m_fRotate[2] = 0.f;
		m_fEf_UI_Alpha = 0.f;
		m_fEf_UI_Size = 0.2f;
	}
	else
	{
		m_fRotate[0] += (_float)_dTimeDelta*1.4f;
		m_fRotate[1] -= (_float)_dTimeDelta*1.8f;
		if(0 == (_int)m_fRotate[0] %2 )
			m_fRotate[2] -= (_float)_dTimeDelta*2.4f;
		else
			m_fRotate[2] += (_float)_dTimeDelta*2.4f;

		m_fEf_UI_Alpha += (_float)_dTimeDelta;
		if (0.1f < m_fEf_UI_Alpha)
			m_fEf_UI_Alpha = 0.1f;
		m_fEf_UI_Size += (_float)_dTimeDelta*1000.f;
		if (7.4f *ScrnRatio < m_fEf_UI_Size)
			m_fEf_UI_Size = 8.88f *ScrnRatio;
	}
}

CDinoList_UI * CDinoList_UI::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	CDinoList_UI*	pInstance = new CDinoList_UI(_pDevice);

	if (FAILED(pInstance->Ready_GameObject_Prototype()))
		Safe_Release(pInstance);

	return pInstance;
}
	
CUI * CDinoList_UI::Clone(void * _pArg)
{
	CDinoList_UI*	pClone = new CDinoList_UI(*this);

	if (FAILED(pClone->Ready_GameObject(_pArg)))
		Safe_Release(pClone);

	return pClone;
}

void CDinoList_UI::Free()
{
	COrthoUI::Free();
}

void CDinoList_UI::Open_DinoList(vector<STRCD> _vecCommand)
{
	m_vecCommand.clear();
	m_vecCommand = _vecCommand;
	m_iCommandAmount = (_int)m_vecCommand.size();
	Split_To_Circle();
	m_tORTHOUI_INFO.bRender = true;
}
