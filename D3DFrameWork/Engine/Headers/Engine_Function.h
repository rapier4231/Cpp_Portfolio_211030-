#ifndef __ENGINE_FUNCTION_H__
#define __ENGINE_FUNCTION_H__

#include "Engine_Typedef.h"

namespace Engine
{
	template <typename T>
	void Safe_Delete(T& ptr)
	{
		if (NULL != ptr)
		{
			delete ptr;
			ptr = NULL;
		}
	}

	template <typename T>
	void Safe_Delete_Array(T& ptr)
	{
		if (NULL != ptr)
		{
			delete[] ptr;
			ptr = NULL;
		}
	}

	template <typename T>
	DWORD Safe_Release(T& ptr)
	{
		DWORD	dwRefCnt = 0;
		if (NULL != ptr)
		{
			dwRefCnt = ptr->Release();
			if (dwRefCnt == 0)
				ptr = NULL;
		}
		return dwRefCnt;
	}
}

class CDeleteObj
{
public:
	explicit CDeleteObj(void) {}
	~CDeleteObj(void) {}
public: // operator
	template <typename T>
	void operator () (T& pInstance)
	{
		_ulong dwRefCnt = 0;

		dwRefCnt = pInstance->Release();

		if (0 == dwRefCnt)
			pInstance = nullptr;
	}
};

// 연관컨테이너 삭제용
class CDeleteMap
{
public:
	explicit CDeleteMap(void) {}
	~CDeleteMap(void) {}
public: // operator	
	template <typename T>
	void operator () (T& Pair)
	{
		_ulong dwRefCnt = 0;

		dwRefCnt = Pair.second->Release();

		if (0 == dwRefCnt)
			Pair.second = NULL;
	}
};

#endif // !__ENGINE_FUNCTION_H__
