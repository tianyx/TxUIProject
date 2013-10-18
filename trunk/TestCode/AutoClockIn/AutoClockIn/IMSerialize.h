/********************************************************************
	created:	2010/01/26
	created:	26:1:2010   15:12
	filename: 	Serialize.h
	file path:	
	file base:	Serialize
	file ext:	
	author:		tian_yuanxin
	
	purpose:	serialize
*********************************************************************/
#pragma once

#include <vector>
#include <map>
#include <list>
class CIMSerialize
{
public:
	CIMSerialize(void* pDataIn, LONG lLenIn, BOOL bOut = TRUE); //out use this 
	CIMSerialize(); //in use this
	CIMSerialize(CFile* pFile);
public:
	virtual ~CIMSerialize(void);


public:
	BOOL operator <<(const LONG data);
	BOOL operator >>(LONG& data);

	BOOL operator <<(const ULONG data);
	BOOL operator >>(ULONG& data);

	BOOL operator <<(const ULONG64 data);
	BOOL operator >>(ULONG64& data);

	BOOL operator <<(const INT data);
	BOOL operator >>(INT& data);

	BOOL operator <<(const CString& data);
	BOOL operator >>(CString& data);

	BOOL operator <<(const bool& data);
	BOOL operator >>(bool& data);

	BOOL Serialize(TCHAR* data, LONG& lenInOut);
	BOOL Serialize(BYTE* data, LONG& lenInOut);

	//write data (at curr pos) to file; 
	LONG SerializeFileOut(CFile* pFile, const LONG lenIn);
	LONG SerializeFileOut(CFile* pFile);

	// return readed
	LONG SerializeFileIn(CFile* pFile);
// 	template<typename T> BOOL CanSaveIn(const T& data)
// 	{
// 		if ((m_lmaxlen - m_lUsed -1)  > sizeof(data))
// 		{
// 			return TRUE;
// 		}
// 		return FALSE;
// 	}

// 	template<typename T> BOOL CanWriteOut(const T& data)
// 	{
// 		if ((m_lmaxlen - m_lUsed)  >= sizeof(data))
// 		{
// 			return TRUE;
// 		}
// //		ASSERT(FALSE);
// 		return FALSE;
// 	}


	template<typename T> BOOL operator << (const std::vector<T>& data)
	{
		if (m_bOut)
		{
			return FALSE;
		}
		LONG nLen = (LONG)data.size();
		operator<<(nLen);
		for (size_t i =0; i < data.size(); i++)
		{
			operator<<(data[i]);
		}
		return TRUE;
	}

	template<typename T> BOOL operator >> (std::vector<T>& data)
	{
		if (!m_bOut)
		{
			return FALSE;
		}
		data.clear();
		LONG nLen = 0;
		operator>>(nLen);
		T tmp;
		for (size_t i = 0; i < (size_t)nLen; i++)
		{
			operator>>(tmp);
			data.push_back(tmp);
		}

		return TRUE;
	}

	template<typename T, typename N> BOOL operator << (const std::map<T, N>& data)
	{
		if (m_bOut)
		{
			return FALSE;
		}

		LONG nLen = (LONG)data.size();
		operator <<(nLen);

		if (nLen <= 0)
		{
			return FALSE;
		}

		std::map<T, N>::const_iterator itb = data.begin();
		std::map<T, N>::const_iterator ite = data.end();
		for (; itb != ite; itb++)
		{
			operator<<(itb->first);
			operator<<(itb->second);
		}

		return TRUE;
	}

	template<typename T, typename N> BOOL operator >> (std::map<T, N>& data)
	{
		data.clear();
		if (!m_bOut)
		{
			return FALSE;
		}
		LONG nLen = 0;
		operator >>(nLen);
		if (nLen < 0)
		{
			ASSERT(FALSE);
		}
		T tmpT;
		N tmpN;
		for (int i = 0; i < nLen; i++)
		{
			operator>>(tmpT);
			operator>>(tmpN);
			data[tmpT] = tmpN;
		}
		return TRUE;
	}

	template<typename T, typename N> BOOL operator << (const std::pair<T, N>& data)
	{
		if (m_bOut)
		{
			return FALSE;
		}

		operator<<(data.first);
		operator<<(data.second);
		return TRUE;
	}
	template<typename T, typename N> BOOL operator >> (std::pair<T, N>& data)
	{
		if (!m_bOut)
		{
			return FALSE;
		}
	
		T tmpT;
		N tmpN;

		operator>>(tmpT);
		operator>>(tmpN);
		data.first = tmpT;
		data.second = tmpN;

		return TRUE;
	}

public:
	//template for all struct, if T operator <<() or T::operator >>() not public, 
	//use friend CIMSerialize;
	template<typename T> BOOL operator <<(const T& data)
	{
		if (m_bOut)
		{
			return FALSE;
		}
		return data << (*this);
	}

	template<typename T> BOOL operator >>(T& data)
	{
		if (!m_bOut)
		{
			return FALSE;
		}
		return data >> (*this);
	}



	template<typename T> BOOL operator << (const std::list<T>& data)
	{
		if (m_bOut)
		{
			return FALSE;
		}

		LONG nLen = (LONG)data.size();
		operator <<(nLen);

		if (nLen <= 0)
		{
			return FALSE;
		}

		std::list<T>::const_iterator itb = data.begin();
		std::list<T>::const_iterator ite = data.end();
		for (; itb != ite; itb++)
		{
			operator<<(*itb);
		}

		return TRUE;
	}

	template<typename T> BOOL operator >> (std::list<T>& data)
	{
		data.clear();
		if (!m_bOut)
		{
			return FALSE;
		}
		LONG nLen = 0;
		operator >>(nLen);
		if (nLen < 0)
		{
			ASSERT(FALSE);
		}
		T tmpT;
		for (int i = 0; i < nLen; i++)
		{
			operator>>(tmpT);
			data.push_back(tmpT);
		}
		return TRUE;
	}


public:
	BOOL WriteBuffer(void* pBuffIn, long& lLenInOut);
	LONG GetUsed();
	const BYTE* GetBuffer(long& lSize);
	void Reset();
	const BYTE* GetRemainBuffer(long& lRemainLen);

private:
	BOOL AutoIncreaseBuffer(long nNeeded);

private:
	BOOL m_bOut;
	BYTE* m_pData;
	BYTE* m_pPos;
	long m_lmaxlen;
	long m_lUsed;
	BOOL m_bSelfBuffer;
};


