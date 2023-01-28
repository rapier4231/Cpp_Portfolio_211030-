#pragma once
#ifndef __LOADING_H__
#define __LOADING_H__

#include "Engine_Define.h"
#include "Value.h"
#include "Base.h"
#include "Stage.h"

class CLoading : public CBase
{
public:
	typedef struct
	{
		CStage**		pStage;
		CLoading*		pLoading;
	}EXPORTSCENE;

private:
	explicit CLoading(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CLoading(void) = default;

public:
	HRESULT		Ready_Loading(LOADINGID _eLoadingID, CStage** _eStage);
	_uint		Loading_ForStage(CStage** _eStage);
	
public:
	LOADINGID			Get_LoadingID(void) const { return m_eLoadingID; }
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void) const { return m_bIsFinish; }
	const wstring&		Get_String(void);
	HRESULT				Set_String(const wstring& wstrText);
	stSC*				Get_pSC() { return &m_stLoadingCount; }

	const _int&			Add_Count(const _int& iAdd);

	CScene*				Get_Scene() { return m_pNextScene; }

public:
	static unsigned int __stdcall Thread_Main(void* _pArg);

public:
	static CLoading*	Create(LPDIRECT3DDEVICE9 _pDevice, LOADINGID _eLoadingID, CStage** _eStage);
	virtual void		Free() override;

private:
	HRESULT				Protot_StaticMesh_ForStage();
	HRESULT				Protot_DynamicMesh_ForStage();
	HRESULT				Protot_UI_ForStage();
	HRESULT				Protot_Texture_ForStage();
	HRESULT				Protot_Item_ForStage(_tchar* _tchName, _tchar* _tchKoreaName, _tchar* _tchToolTip, ITEMID _iItemID, _int _iStat, _double _dMaxLife, _int _iPosition , _float _fWeight, _int _iMaxAmount = 1);
	HRESULT				Protot_MakingItem_ForStage(_tchar* _tchName, _tchar* _tchKoreaName, ITEMID _iMakingItemID,_int _iMaxMakingAmount,
		ITEMID iIngredientItemID_1, ITEMID iIngredientItemID_2, ITEMID iIngredientItemID_3, ITEMID iIngredientItemID_4,
		_int _iNumber_of_Required_ItemID_1, _int _iNumber_of_Required_ItemID_2, _int _iNumber_of_Required_ItemID_3, _int _iNumber_of_Required_ItemID_4, 
		_double _dMakingTime,_int _iMakingPossibleLv = 0);

private:
	HANDLE				m_hThread;

	CRITICAL_SECTION	m_Crt;
	LOADINGID			m_eLoadingID;
	LPDIRECT3DDEVICE9	m_pDevice = nullptr;

	_bool				m_bIsFinish = false;
	wstring				m_wstrLoadNote = L"";

	CScene*				m_pNextScene = nullptr;
	EXPORTSCENE			pExportScene = {};

	stSC				m_stLoadingCount;
};

#endif // !__LOADING_H__
