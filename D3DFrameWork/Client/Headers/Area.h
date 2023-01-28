#pragma once
#ifndef __AREA_H__
#define __AREA_H__

#include "Value.h"
#include "Unit.h"

BEGIN(Engine)
class CStaticMesh;
END

enum AREASTATE {
	AREASTATE_ACTIVE,	//플레이어가 영역 내에 있으므로 더이상 스폰하지 않고 공룡들 업데이트시킴
	AREASTATE_READY,	//플레이어의 가시거리 안쪽이므로 공룡들 스폰하되 공룡 업데이트는 생략
	AREASTATE_WAIT,		//플레이어의 가시거리 바깥이므로 모든 동작을 멈추고 대기
	AREASTATE_DISABLE,	//외부에서 동작시킬 때까지 기능 정지
	AREASTATE_END
};

class CArea : public CUnit
{
private:
	explicit CArea(LPDIRECT3DDEVICE9 _pDevice);
	explicit CArea(const CArea& rhs);
	virtual ~CArea() = default;

public:
	virtual HRESULT			Ready_GameObject_Prototype() override;
	virtual HRESULT			Ready_GameObject(void * _pArg = nullptr) override;
	virtual _int			Update_GameObject(const _double & _dTimeDelta) override;
	virtual _int			LateUpdate_GameObject(const _double& _dTimeDelta) override;
	virtual void			Render_GameObject() override;

public:
	static CArea*			Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual CGameObject*	Clone(void * _pArg = nullptr) override;
	virtual void			Free() override;

private:
	HRESULT					Add_Component();
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// 쉐이더 상수 테이블 세팅함수
	virtual _int			Check_Arg(void * _pArg) override;

private:
	//플레이어 포인터 얻어옴. 실패시 리턴
	HRESULT					Update_PlayerPointer();
	//플레이어와의 2D거리 확인해 갱신
	void					Update_DistanceToPlayer();
	//조건 확인해 State 갱신
	_int					Update_State();
	//State에 따른 액션 실시
	HRESULT					Update_Actions(const _double & dTimeDelta);

	HRESULT					Action_Ready(const _double& dTimeDelta);
	HRESULT					Action_Active(const _double& dTimeDelta);
	//HRESULT					Action_Wait(const _double& dTimeDelta);		//<-리턴하는거뿐
	
	HRESULT					Spawn_Dino();

public:
	//외부에서 State를 강제 변경시키는 함수
	HRESULT					Set_State(const _int& eState);

private:
	CStaticMesh*			m_pMeshCom = nullptr;
	/*CShader*				m_pShaderCom = nullptr;
	CCalculator*			m_pCalculatorCom = nullptr;	
	CTransform*				m_pTransformCom = nullptr;*/

	_float					m_fRadius = 0.f;
	_float					m_fRadiusReady = 500.f;
	_float					m_fRadiusWait = 800.f;

	//_vec3					CGameObject::m_vColor;
	_float					m_fAlpha = 0.f;

	CUnit*					m_pPlayer = nullptr;
	_float					m_fDistance = 0.f;
	_bool					m_bPlayerSteppedIn = false;

	AREASTATE				m_eState = AREASTATE_END;
	AREASTATE				m_eStatePast = AREASTATE_END;

	//DINOTAG					m_eDinoTag = DINOTAG_END;
	wstring					m_strDinoTag;
	list<class CDino*>		m_listDino;
	_int					m_iDinoCountMax = 0;

	_double					m_dTimeSpawn = 0;
};

#endif // !__PLAYER_H__