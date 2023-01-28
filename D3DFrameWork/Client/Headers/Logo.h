#pragma once
#ifndef __LOGO_H__
#define __LOGO_H__

#include "Value.h"
#include "Scene.h"

class CLoading;
class CStage;
class CLoadingTex_UI;
class CLogo : public CScene
{
private:
	explicit CLogo(LPDIRECT3DDEVICE9 _pDevice);
	virtual ~CLogo() = default;

public:
	virtual HRESULT		Ready_Scene(void) override;
	virtual _int		Update_Scene(const _double & dTimeDelta) override;
	virtual void		Render_Scene(void) override;

private:
	HRESULT				Ready_Environment_Layer();

public:
	static CLogo*		Create(LPDIRECT3DDEVICE9 _pDevice);
	virtual void		Free() override;

private:
	CLoading*			m_pLoading;
	CStage*				m_pStage;
	CLoadingTex_UI*		m_pLoadingTex_UI;
};

#endif // !__LOGO_H__
