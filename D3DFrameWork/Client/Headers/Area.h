#pragma once
#ifndef __AREA_H__
#define __AREA_H__

#include "Value.h"
#include "Unit.h"

BEGIN(Engine)
class CStaticMesh;
END

enum AREASTATE {
	AREASTATE_ACTIVE,	//�÷��̾ ���� ���� �����Ƿ� ���̻� �������� �ʰ� ����� ������Ʈ��Ŵ
	AREASTATE_READY,	//�÷��̾��� ���ðŸ� �����̹Ƿ� ����� �����ϵ� ���� ������Ʈ�� ����
	AREASTATE_WAIT,		//�÷��̾��� ���ðŸ� �ٱ��̹Ƿ� ��� ������ ���߰� ���
	AREASTATE_DISABLE,	//�ܺο��� ���۽�ų ������ ��� ����
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
	HRESULT					SetUp_ConstantTable(LPD3DXEFFECT& _pEffect);		// ���̴� ��� ���̺� �����Լ�
	virtual _int			Check_Arg(void * _pArg) override;

private:
	//�÷��̾� ������ ����. ���н� ����
	HRESULT					Update_PlayerPointer();
	//�÷��̾���� 2D�Ÿ� Ȯ���� ����
	void					Update_DistanceToPlayer();
	//���� Ȯ���� State ����
	_int					Update_State();
	//State�� ���� �׼� �ǽ�
	HRESULT					Update_Actions(const _double & dTimeDelta);

	HRESULT					Action_Ready(const _double& dTimeDelta);
	HRESULT					Action_Active(const _double& dTimeDelta);
	//HRESULT					Action_Wait(const _double& dTimeDelta);		//<-�����ϴ°Ż�
	
	HRESULT					Spawn_Dino();

public:
	//�ܺο��� State�� ���� �����Ű�� �Լ�
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