/********************************************************************
	created:	2010/01/26
	created:	26:1:2010   15:12
	filename: 	Serialize.h
	file path:	
	file base:	Serialize
	file ext:	
	author:		tian_yuanxin
	
	purpose:	serialize class, can serialize a struct from/to a buffer 
	usage:		1. create a CTxSerialize instance
				2. use >>, << function to serialize
*********************************************************************/
#pragma once

#include <vector>
#include <map>
#include <list>
class CTxSerialize
{
public:
	
	//*Description: default for serialize a buffer to struct
	//*Input Param: bOut: TRUE for buffer-> struct, FALSE for struct -> buffer
	//*Return Param: 
	//*History: 
	CTxSerialize(void* pDataIn, LONG lLenIn, BOOL bOut = TRUE); //out use this 

	//*Description: default for serialize a  struct to buffer
	//*Input Param: bOut: TRUE for buffer-> struct, FALSE for struct -> buffer
	//*Return Param: 
	//*History: 
	CTxSerialize(); //in use this
public:
	virtual ~CTxSerialize(void);


public:
	BOOL operator <<(const LONG data);
	BOOL operator >>(LONG& data);

	BOOL operator <<(const ULONG data);
	BOOL operator >>(ULONG& data);

	BOOL operator <<(const ULONG64 data);
	BOOL operator >>(ULONG64& data);

	BOOL operator <<(const INT data);
	BOOL operator >>(INT& data);

	BOOL operator <<(const INT64 data);
	BOOL operator >>(INT64& data);


	BOOL operator <<(const CString& data);
	BOOL operator >>(CString& data);

	BOOL operator <<(const bool& data);
	BOOL operator >>(bool& data);

	BOOL operator <<(const SOCKADDR_IN& data);
	BOOL operator >>(SOCKADDR_IN& data);


	BOOL Serialize(char* data, LONG& lenInOut);
	BOOL Serialize(BYTE* data, LONG& lenInOut);

	//write data (at curr pos) to file; 
	LONG WriteToFile(CFile* pFile, const LONG lenIn);
	LONG WriteAllDataToFile(CFile* pFile);

	// return readed
	LONG ReadFromFile(CFile* pFile);
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
	//use friend CTxSerialize;
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


