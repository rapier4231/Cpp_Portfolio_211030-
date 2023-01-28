#pragma once
#ifndef __SOUND_MGR_H__
#define __SOUND_MGR_H__

#include "Engine_Define.h"
#include "Base.h"

//enum CHANNELID 
//{ 
//	BGM, 
//	PLAYER_SOUND,
//	PLAYER_EFFECT,
//	PLAYER_CLOTH,
//	WIND,
//	HELICOPTER,
//	EFFECT,
//	BODYFALL,
//	UI, 
//	MAXCHANNEL 
//};

#define FirstBit 0x01
#define MinimunSound 0.05f

enum ELimited_Sound
{
	BGM,			// 0 같거나 이하 -> BGM 볼륨
	///////////////Eff///////////////////////////////////////
	Player_Eff1,
	Player_Eff2,
	Player_Eff3,
	Player_Eff4,

	Dino_Eff1,
	Dino_Eff2,
	Dino_Eff3,
	Dino_Eff4,
	Dino_Eff5,
	Dino_Eff6,

	Map_Eff1,
	Map_Eff2,
	Map_Eff3,
	Map_Eff4,
	Map_Eff5,
	Map_Eff6,

	Etc_Eff1,
	Etc_Eff2,
	Etc_Eff3,
	Etc_Eff4,
	Etc_Eff5,
	Etc_Eff6,
	Etc_Eff7,
	/////////////////Speach/////////////////////////////////////////////////
	Player_Speech1,//플레이어 성대는 하나 뿐일테니 1개, 여기부턴 스피치

	Dino_Speech1,
	Dino_Speech2,
	Dino_Speech3,

	Map_Speech1,

	Etc_Speech1,
	Etc_Speech2,
	Etc_Speech3,
	////////////////////////////////End
	ELimited_Sound_END};
enum ESVT {ESVT_Bgm, ESVT_Effect, ESVT_Speech, ESVT_All, ESVT_End}; //ESound_Volume_Type 약자
enum EChannel { 
	EChannel_BGM,
	EChannel_Player_Eff = 4,
	EChannel_Dino_Eff = 10, 
	EChannel_Map_Eff = 16, 
	EChannel_Etc_Eff = 23, 
	EChannel_Player_Speech = 24, 
	EChannel_Dino_Speech = 27, 
	EChannel_Map_Speech = 28, 
	EChannel_Etc_Speech = 31, 
	EChannel_End};

class CSound_Mgr : public CBase
{
	DECLARE_SINGLETON(CSound_Mgr)
private:
	explicit CSound_Mgr();
	virtual	~CSound_Mgr();

public:
	HRESULT Ready_Sound_Mgr();
	HRESULT	Update_Sound_Mgr();

public:
	//_int Granted_Sound_ID(_vec3* _pGameObjectPos = nullptr);//객체별로 부여받을 사운드 아이디 생성
	//void Delete_Sound_ID(_int _iSoundID);//부여해준 사운드 아이디 삭제 -둘 다 유닛에서 넣어줘도 될 듯
	////비트 플래그로 관리하자 ㅇㅇ

	void PlayBGM(TCHAR* pSoundKey);

	void PlaySound(TCHAR* pSoundKey, //파일 이름
				   EChannel _eChannel, //채널
				   _float _fVolume, //볼륨 절댓값-알아서 넣으시길
				   _vec3* _pPos = nullptr, //소리 나는 위치 안 넣으면 플레이어 위치
				   _bool _bMustPlay = false //반드시 나와야 하는 소리인지, 기본은 ㄴㄴ
					);
	void StopChannelSound(EChannel _eChannel);	//관련 채널 싹 다 멈춤

	void StopAll();	//부를 일 거의 없을 듯?

private:
	void LoadSoundFile();

private:
	virtual void Free() override;

private:
	// 사운드 리소스 정보를 갖는 객체 
	map<TCHAR*, FMOD_SOUND*> m_mapSound;
	// FMOD_CHANNEL : 재생하고 있는 사운드를 관리할 객체 
	FMOD_CHANNEL* m_pChannelArr[ELimited_Sound_END];
	//FMOD_CHANNEL* m_pChannelArr[MAXCHANNEL];
	// 사운드 ,채널 객체 및 장치를 관리하는 객체 
	FMOD_SYSTEM* m_pSystem = nullptr;

public:
	void	Set_PlayerPos(_vec3* _pPlsyerPos) { m_pPlayerPos = _pPlsyerPos; }
	void	Set_Volume(_float _fVolume , ESVT _eESVT) { m_fVolume[_eESVT] = _fVolume; }
private:
	_float	Volume_To_Dis(_vec3 _vGameObjectsPos, _float _fVolume);
	_int	Channel_Check(_int _iFirst, _int _iLast);
	_int	PreChannel(EChannel _eChannel);
private:
	_vec3*				m_pPlayerPos = nullptr;
	_ulong				m_ulSound_Bit = 0;
	map<_int, _vec3*>	m_mapPos;
	_float				m_fVolume[ESVT_End] = { FHALF, FTRUE, FTRUE, FTRUE };
	_vec3				m_vOneJum = {FNULL, FNULL, FNULL};
};

#endif // !__SOUND_MGR_H__
