#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Value.h"
#include "Camera.h"

class CDynamicCamera : public CCamera
{
private:
	explicit CDynamicCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicCamera(const CDynamicCamera& rhs);
	virtual ~CDynamicCamera(void);

public:
	HRESULT Ready_GameObject(const _vec3* pEye,
							const _vec3* pAt,
							const _vec3* pUp,
							const float& fFovY,
							const float& fAspect,
							const float& fNear,
							const float& fFar) ;
	virtual _int Update_GameObject(const _double& _dTimeDelta) override;

private:
	void		Key_Input(const _double& _dTimeDelta);
	void		Mouse_Move(const _double& _dTimeDelta);
	void		Mouse_Fix(void);

public:
	static CDynamicCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _vec3* pEye, 
										const _vec3* pAt, 
										const _vec3* pUp,
										const float& fFovY, 
										const float& fAspect, 
										const float& fNear,
										const float& fFar);

public:
	virtual void Free(void) override;

private:
	_bool		m_bClick = false;
	_bool		m_bFix = true;
};

#endif // DynamicCamera_h__
