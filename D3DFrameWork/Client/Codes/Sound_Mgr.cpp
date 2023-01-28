#include "stdafx.h"
#include "Sound_Mgr.h"

IMPLEMENT_SINGLETON(CSound_Mgr)

CSound_Mgr::CSound_Mgr()
{
}

CSound_Mgr::~CSound_Mgr()
{
	Free();
}

HRESULT CSound_Mgr::Ready_Sound_Mgr()
{
	FMOD_System_Create(&m_pSystem);

	//m_vecSound_ID.resize(SoundIDFlagNum); // 32비트, 채널은 총 256개. 256/32 = 8 ; 비트 플래그면 8개면 됨 (0번은 BGM꺼)
	//for (_int i = NULL; i < SoundIDFlagNum; ++i)
	//	m_vecSound_ID[i] = NULL;	//전부 0으로 초기화
	//m_vecSound_ID[NULL] = TRUE; // BGM 채널 비트 1 켜주기
	
	// 1. 시스템 포인터, 2. 사용할 가상채널 수 , 초기화 방식) 
	FMOD_System_Init(m_pSystem, ELimited_Sound_END, FMOD_INIT_NORMAL, NULL);
	
	LoadSoundFile();

	FMOD_System_Update(m_pSystem);

	return S_OK;
}

HRESULT CSound_Mgr::Update_Sound_Mgr()
{
	FMOD_System_Update(m_pSystem);
	return S_OK;
}

//_int CSound_Mgr::Granted_Sound_ID(_vec3* _pGameObjectPos)
//{
//	_int	iSoundID = -1;
//	_vec3*	pGOPos = nullptr;
//	if (!_pGameObjectPos) // 안 넣어 줬으면
//	{
//		if (!m_pPlayerPos) //플레이어도 NULL이면
//			pGOPos = &m_vOneJum;
//		else
//			pGOPos = m_pPlayerPos;
//	}
//	else
//		pGOPos = _pGameObjectPos;
// 
//	for (_int i = NULL; i < SoundIDFlagNum; ++i)
//	{
//		for (_int j = NULL; j < sizeof(_ulong); ++j)
//		{
//			if (!((m_vecSound_ID[i / SoundIDFlagNum] >> j) & FirstBit)) // 앞부터 해서 비트 0인 부분이 있으면!
//			{
//				m_vecSound_ID[i / SoundIDFlagNum] |= (FirstBit << j); //비트 켜주고
//				iSoundID = i*(SoundIDFlagNum)+j;				 //몇번째 인지 넘기고
//				m_mapGameObjectsPos.emplace(iSoundID, pGOPos);	//포스 값 넣고
//				return iSoundID;								//탈!출!
//			}
//		}
//	}
//
//	return iSoundID;
//}
//
//void CSound_Mgr::Delete_Sound_ID(_int _iSoundID)
//{
//	if (SoundMaxChanel < _iSoundID)
//		return;
//	if (NULL > _iSoundID)
//		return;
//
//	m_vecSound_ID[_iSoundID / SoundIDFlagNum] &= ~(FirstBit << (_iSoundID%SoundIDFlagNum)); //비트 꺼주고
//	m_mapGameObjectsPos.erase(_iSoundID);	//키 값 넣어서 지우고
//	return; //탈 출~!
//}

void CSound_Mgr::PlaySound(TCHAR* pSoundKey, EChannel _eChannel, _float _fVolume, _vec3* _pPos, _bool _bMustPlay)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;
	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});
	if (iter == m_mapSound.end())
		return;
	////////////////////////////////////////////////////////////////////////예외 처리

	_int iFist, iLast;
	EChannel eChannel = _eChannel;
	iFist = PreChannel(eChannel);
	iLast = (_int)_eChannel;

	_int iSoundID = Channel_Check(iFist, iLast);
	if (-1 == iSoundID)
		if (_bMustPlay)
		{
			FMOD_Channel_Stop(m_pChannelArr[iLast]);
			iSoundID = iLast;
		}
		else
			return;

	//FMOD_BOOL bPlay = FALSE;
	//if (FMOD_Channel_IsPlaying(m_pChannelArr[iSoundID], &bPlay))
	//{
		//FMOD_CHANNELGROUP asd;
		//FMOD_CHANNEL_FREE,
		ESVT eSVT = ESVT_End;
		if (_eChannel < EChannel_Player_Speech) // BGM은 여기 안온당
			eSVT = ESVT_Effect;
		else
			eSVT = ESVT_Speech;

		_float fVolume;
		if(_pPos)
			fVolume = (Volume_To_Dis(*_pPos, _fVolume));
		else
		{
			if(m_pPlayerPos)
				fVolume = (Volume_To_Dis(*m_pPlayerPos, _fVolume));
			else
				fVolume = (Volume_To_Dis(m_vOneJum, _fVolume));
		}
		if (FNULL == fVolume)
			return;

		fVolume  *= (m_fVolume[eSVT] * m_fVolume[ESVT_All]);
		FMOD_System_PlaySound(m_pSystem, iter->second, NULL, FALSE, &m_pChannelArr[iSoundID]);
		FMOD_Channel_SetVolume(m_pChannelArr[iSoundID], fVolume);
	//}
}

void CSound_Mgr::PlayBGM(TCHAR * pSoundKey)
{
	map<TCHAR*, FMOD_SOUND*>::iterator iter;

	iter = find_if(m_mapSound.begin(), m_mapSound.end(), [&](auto& iter)
	{
		return !lstrcmp(pSoundKey, iter.first);
	});
	if (iter == m_mapSound.end())
		return;

	FMOD_Channel_Stop(m_pChannelArr[NULL]);
	FMOD_System_PlaySound(m_pSystem, iter->second, NULL, FALSE, &m_pChannelArr[NULL]);
	FMOD_Channel_SetMode(m_pChannelArr[NULL], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[NULL],m_fVolume[ESVT_Bgm]* m_fVolume[ESVT_All]);
}

void CSound_Mgr::StopChannelSound(EChannel _eChannel)
{
	_int iFist, iLast;
	iFist = PreChannel(_eChannel);
	iLast = (_int)_eChannel;

	for(_int i = iFist; i < (iLast + 1); ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Mgr::StopAll()
{
	for (int i = 0; i < ELimited_Sound_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSound_Mgr::LoadSoundFile()
{
	_finddata_t fd;
	
	intptr_t handle = _findfirst("../Resources/Sound/*.*", &fd);
	
	if (handle == 0)
		return;
	
	_int iResult = 0;
	
	char szCurPath[128] = "../Resources/Sound/";
	char szFullPath[MAX_PATH] = "";
	
	while (iResult != -1)
	{
		strcpy_s(szFullPath, szCurPath);
		strcat_s(szFullPath, fd.name);
		FMOD_SOUND* pSound = nullptr;
	
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);
	
		wstring wstrRemove_DotLater = L"";
		size_t	DotPos = 0;
		if (eRes == FMOD_OK)
		{
			_int iLength = (_int)strlen(fd.name) + 1;
	
			TCHAR* pSoundKey = new TCHAR[iLength];
			ZeroMemory(pSoundKey, sizeof(TCHAR) * iLength);
			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength, pSoundKey, iLength);
			wstrRemove_DotLater = pSoundKey;
			DotPos = wstrRemove_DotLater.rfind(L".");
			wstrRemove_DotLater = wstrRemove_DotLater.substr(0, DotPos);
			iLength = (_int)wcslen(wstrRemove_DotLater.c_str()) + 1;
			wcscpy_s(pSoundKey, (rsize_t)iLength, wstrRemove_DotLater.c_str());
			m_mapSound.emplace(pSoundKey, pSound);
		}
		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);
	_findclose(handle);
}

void CSound_Mgr::Free()
{
	for (auto& Mypair : m_mapSound)
	{
		delete[] Mypair.first;
		FMOD_Sound_Release(Mypair.second);
	}

	m_mapSound.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);
}

_float CSound_Mgr::Volume_To_Dis(_vec3 _vGameObjectsPos , _float _fVolume)
{
	_float fVolume;
	_vec3 vDir;
	if(m_pPlayerPos)
		vDir = *m_pPlayerPos - _vGameObjectsPos;
	else
		vDir = m_vOneJum - _vGameObjectsPos;

	_float fDis = D3DXVec3Length(&vDir);
	if (NULL == fDis)
		fDis = 1.f;
	if(10.f < fDis)
		fDis /= 10.f;

	fDis *= fDis;
	fVolume = _fVolume / fDis;
	if (1.f < fVolume)
		fVolume = 1.f;
	if (MinimunSound > fVolume)
		fVolume = 0.f;

	return fVolume;
}

_int CSound_Mgr::Channel_Check(_int _iFirst, _int _iLast)
{
	FMOD_BOOL bPlay = FALSE;
	for (_int i = _iFirst; i < (_iLast + 1); ++i)
	{
		FMOD_Channel_IsPlaying(m_pChannelArr[i], &bPlay);
		if (!bPlay)
			return i;
	}
	return -1;
}

_int CSound_Mgr::PreChannel(EChannel _eChannel)
{
	switch (_eChannel)
	{
	case EChannel_BGM:
		return -1;
		break;
	case EChannel_Player_Eff:
		return (_int)EChannel_BGM + 1;
		break;
	case EChannel_Dino_Eff:
		return (_int)EChannel_Player_Eff + 1;
		break;
	case EChannel_Map_Eff:
		return (_int)EChannel_Dino_Eff + 1;
		break;
	case EChannel_Etc_Eff:
		return (_int)EChannel_Map_Eff + 1;
		break;
	case EChannel_Player_Speech:
		return (_int)EChannel_Etc_Eff + 1;
		break;
	case EChannel_Dino_Speech:
		return (_int)EChannel_Player_Speech + 1;
		break;
	case EChannel_Map_Speech:
		return (_int)EChannel_Dino_Speech + 1;
		break;
	case EChannel_Etc_Speech:
		return (_int)EChannel_Map_Speech + 1;
		break;
	case EChannel_End:
		return (_int)EChannel_Etc_Speech + 1;
		break;
	default:
		return -1;
		break;
	}
	return -1;
}
