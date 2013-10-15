#pragma  once

#include <vector>
#include "IMSerialize.h"

typedef ULONG USERID;
typedef ULONG GROUPID;
typedef ULONG64 NODEID;
typedef ULONG64 USERNODEID;
typedef	ULONG64 GROUPNODEID;
typedef ULONG PrivilegeID;

typedef ULONG64 ULONG64_PTR, *PULONG64_PTR;
#define MAKELONG64KEY(a, b)      ((ULONG64)(((ULONG)((ULONG_PTR)(a<b? a:b) & 0xffffffff)) | ((ULONG64)((ULONG)((ULONG_PTR)(a<b? b:a) & 0xffffffff))) << 32))
#define MAKELONG64(a, b)      ((ULONG64)(((ULONG)((ULONG_PTR)(a) & 0xffffffff)) | ((ULONG64)((ULONG)((ULONG_PTR)(b) & 0xffffffff))) << 32))
#define LOWLONG64(l)           ((ULONG64)((ULONG64_PTR)(l) & 0xffffffff))
#define HILONG64(l)           ((ULONG64)((ULONG64_PTR)(l) >> 32))



#define  IM_NODE_VER  (int)1
#define CUSTOM_GROUP_IDBASE 100000  //custom group id;

#define NODETYPE_NONE	0
#define NODETYPE_USER	1
#define NODETYPE_GROUP	2

#define FILETRANS_TYPE_FORGROUND	0
#define FILETRANS_TYPE_CHAT_HIDE	1
#define FILETRANS_TYPE_AVMSG_HIDE	2
#define FILETRANS_TYPE_AVMSG_RECV	3

#define NODE_KEY_DEPART			NODE_KEY_SYS_BASE + 1
#define NODE_KEY_FAVORITE		NODE_KEY_SYS_BASE + 2
#define NODE_KEY_DISCUSSGROUP	NODE_KEY_SYS_BASE + 3
#define NODE_KEY_RECENT			NODE_KEY_SYS_BASE + 4
#define NODE_KEY_MEETING		NODE_KEY_SYS_BASE + 5

#define NODE_KEY_BLACK_LIST		NODE_KEY_SYS_BASE +6
#define NODE_KEY_CLOSEDDISGROP	NODE_KEY_SYS_BASE +7

#define NODE_KEY_AUDIO_MEETING	NODE_KEY_SYS_BASE +8

#define NODE_KEY_VALID_BASE		NODE_KEY_SYS_BASE + 50

//user state
#define IM_USER_STATUS_OFFLINE	0x00
#define IM_USER_STATUS_ONLINE	0x01
#define IM_USER_STATUS_BUSY		0x02
#define IM_USER_STATUS_LEAVE	0x03
#define IM_USER_STATUS_HIDE		0x04


#define GROUP_STATE_ACTIVE		0
#define GROUP_STATE_UNKNOW		1
#define GROUP_STATE_DEACTIVE	2
#define GROUP_STATE_BEKICKED	3
#define GROUP_STATE_CLOSED		4

#define GROUP_ITEM_ROW_OFFSET			5
#define USER_ITEM_ROW_OFFSET			5

#define USER_UPDATE_TYPE_ALL			0
#define USER_UPDATE_TYPE_ONLINE_STATE	1

#define USER_PRIVILEGE_TYPE_NORMAL		1
#define USER_PRIVILEGE_TYPE_POWER		2
#define USER_PRIVILEGE_TYPE_ADMIN		3

//for search key
#define SEARCH_MASK_USERID	0x1
#define SEARCH_MASK_NICK	0x2
#define SEARCH_MASK_NAME	0x4
#define SEARCH_MASK_KEYWORD 0x8
#define SEARCH_MASK_SEX		0X10
#define SEARCH_MASK_AGE		0x20
#define SEARCH_MASK_COUNTRY 0x40
#define SEARCH_MASK_PHONE	0x80



//for treelistpic 
#define TREE_ICON_OFFSET 5

typedef std::pair<ULONG64, int> PAIR_NODE_ID; // key1 = key, key2= node type, user node = 1, or group node = 2

//////////////////////////////////////////////////////////////////////////
struct NODE_KEY 
{
	NODEID nId; //node id
	int nType; //1 for user node ,2 for group node
	NODE_KEY()
	{
		nId = 0;
		nType = 1; 
	}

	NODE_KEY(NODEID id, int type)
	{
		nId = id;
		nType = type; 
	}

	bool operator == (const NODE_KEY& other) const
	{
		if (this == &other)
		{
			return true;
		}
		return other.nId == this->nId && other.nType == this->nType;
	}

	bool operator < (const NODE_KEY& other) const
	{	
		if (nType != other.nType)
		{
			return nType < other.nType;
		}
		else if (nId != other.nId)
		{
			return nId < other.nId;
		}
		else 
		{
			return false;
		}
	}


	BOOL operator << (CIMSerialize& ar) const
	{
		ar << IM_NODE_VER;
		ar << nId;
		ar << nType;
		return TRUE;
	}

	BOOL operator >> (CIMSerialize& ar) 
	{

		BOOL bRet = TRUE;
		long nVer = 0;
		ar >> nVer;
		switch (nVer)
		{
		case 1:
			{
				ar >> nId;
				ar >> nType;
			}break;

		default:
			{
				ASSERT(FALSE);
				bRet = FALSE;
			}
			break;
		}

		return bRet;
	}
};



typedef std::vector<NODE_KEY> VEC_NODE_IDS;



//
struct UserLocationInfo
{
	DWORD dwLocationId;
	UserLocationInfo()
	{
		dwLocationId = 0;
	}
};

//store user info
struct IMUserInfo 
{
	USERID nId;
	CString szNick;
	CString szSign;
	int nSex;
	int nPicIdx;
	ULONG nLastModifiedDate;
	BOOL bFriend;
	UserLocationInfo locationInfo;
	//extern use for disgroup sessionkey
	ULONG nExternData1;
	ULONG nExternData2;
	ULONG nExternData3;

	//new add 1101
	CString szName;
	CString szCountry;
	CString szPhone;
	CString szAddress;
	ULONG64	lBornDate;
	CString szKeyword;

	IMUserInfo()
	{
		nId = 0;
		nSex = 0;
		nPicIdx = 0;
		nLastModifiedDate = 0;
		bFriend = FALSE;
		nExternData1 = 0;
		nExternData2 = 0;
		nExternData3 = 0;
		lBornDate = 0;
	}

	BOOL operator << (CIMSerialize& ar) const
	{
		ar << IM_NODE_VER;
		ar << nId;
		ar << szNick;
		ar << szSign;
		ar << nSex;
		ar << nPicIdx;
		ar << nExternData1;
		ar << nExternData2;
		ar << nExternData3;
		ar << szName;
		ar << szCountry;
		ar << szPhone;
		ar << szAddress;
		ar << lBornDate;
		ar << szKeyword;
		
		return TRUE;
	}

	BOOL operator >> (CIMSerialize& ar) 
	{
		
		BOOL bRet = TRUE;
		long nVer = 0;
		ar >> nVer;
		switch (nVer)
		{
		case 1:
			{
				ar >> nId;
				ar >> szNick;
				ar >> szSign;
				ar >> nSex;
				ar >> nPicIdx;
				ar >> nExternData1;
				ar >> nExternData2;
				ar >> nExternData3;
				ar >> szName;
				ar >> szCountry;
				ar >> szPhone;
				ar >> szAddress;
				ar >> lBornDate;
				ar >> szKeyword;
			}break;

		default:
			{
				ASSERT(FALSE);
				bRet = FALSE;
			}
			break;
		}

		return bRet;
	}

};

//////////////////////////////////////////////////////////////////////////
//store group info, 
struct IMGroupInfo 
{
	GROUPID nId;		//group id  
	CString szName;
	CString szDesc;
	int nPicIdx;
	IMGroupInfo()
	{
		nId = 0;
		nPicIdx = 0;
	}

	BOOL operator << (CIMSerialize& ar) const
	{
		
		ar << IM_NODE_VER;
		ar << nId;
		ar << szName;
		ar << szDesc;
		return TRUE;
	}

	BOOL operator >> (CIMSerialize& ar)
	{
		BOOL bRet = TRUE;
		long nVer = 0;
		ar >> nVer;
		switch (nVer)
		{
		case 1:
			{
				ar >> nId;
				ar >> szName;
				ar >> szDesc;
			}break;

		default:
			{
				ASSERT(FALSE);
				bRet = FALSE;
			}
			break;
		}

		return bRet;
	}
};

//////////////////////////////////////////////////////////////////////////
//user tree struct
//
//user tree node, unique in tree.
struct UserNodeInfo
{
	USERNODEID	nId;			//node id
	USERID	nInfoId;			//user info id
	NODEID	nParentId;			//parent id, 0 for no parent
	NODEID	nTopParentId;		//for top group ID;
	int		nPrivilege;			//Privilege
	NODEID	nExtenData1;		///in depart for save company id 
	CString strName;		//node Name

	UserNodeInfo()
	{
		nId = 0;
		nInfoId		= 0;
		nPrivilege	= USER_PRIVILEGE_TYPE_NORMAL; //3 for admin
		nParentId	= 0;
		nTopParentId = 0;
		nExtenData1 = 0;
	}

	BOOL operator << (CIMSerialize& ar) const
	{

		ar << IM_NODE_VER;
		ar << nId;
		ar << nInfoId;
		ar << nPrivilege;
		ar << nParentId;
		ar << nTopParentId;
		ar << nExtenData1;
		ar << strName;
		return TRUE;
	}

	BOOL operator >> (CIMSerialize& ar)
	{
		BOOL bRet = TRUE;
		long nVer = 0;
		ar >> nVer;
		switch (nVer)
		{
		case 1:
			{
				ar >> nId;
				ar >> nInfoId;
				ar >> nPrivilege;
				ar >> nParentId;
				ar >> nTopParentId;
				ar >> nExtenData1;
				ar >> strName;
			}break;

		default:
			{
				ASSERT(FALSE);
				bRet = FALSE;
			}
			break;
		}

		return bRet;
	}

};

//group tree node, unique in tree
struct GroupNodeInfo
{
	GROUPNODEID	nId;			//node info id
	GROUPID	nInfoId;		//group info id
	NODEID	nParentId;		//parent id
	NODEID	nTopParentId;	//top parent id
	NODEID	nExtenData1;	//in depart:save company id ;
	int		nPrivilege;
	VEC_NODE_IDS vNIds;		//child Node ids, may child group or user node
	GroupNodeInfo()
	{
		nId = 0;
		nInfoId		= 0;
		nPrivilege	= 1;
		nParentId	= 0;
		nTopParentId = 0;
		nExtenData1 = 0;
	}

	BOOL operator << (CIMSerialize& ar) const
	{

		ar << IM_NODE_VER;
		ar << nId;
		ar << nInfoId;
		ar << nPrivilege;
		ar << nParentId;
		ar << vNIds;
		ar << nTopParentId;
		ar << nExtenData1;
		return TRUE;
	}

	BOOL operator >> (CIMSerialize& ar)
	{
		BOOL bRet = TRUE;
		long nVer = 0;
		ar >> nVer;
		switch (nVer)
		{
		case 1:
			{
				ar >> nId;
				ar >> nInfoId;
				ar >> nPrivilege;
				ar >> nParentId;
				ar >> vNIds;
				ar >> nTopParentId;
				ar >> nExtenData1;
			}break;

		default:
			{
				ASSERT(FALSE);
				bRet = FALSE;
			}
			break;
		}

		return bRet;
	}
};


//////////////////////////////////////////////////////////////////////////
//privilege user int code to identify
//1: default
//2:...
struct NodePrivilege
{
	BOOL bCanMove;
	BOOL bCanDel;
	BOOL bCanEdit;
	BOOL bCanAddUser;
	BOOL bCanAddGroup;

	NodePrivilege()
	{
		//default Privilege
		bCanMove = FALSE;
		bCanDel = FALSE;
		bCanEdit = FALSE;
		bCanAddUser = TRUE;
		bCanAddGroup = TRUE;
	}
};

//////////////////////////////////////////////////////////////////////////
//maybe used?
struct UserPrivildege
{
	BOOL bFullPower;
	BOOL bSuperNodePrivilege;
	NodePrivilege nodePrivilege;

	UserPrivildege()
	{
		bFullPower = FALSE;
		bSuperNodePrivilege = FALSE;
	}

};


//////////////////////////////////////////////////////////////////////////
struct DiscussGroupInfo
{
	ULONG nLocationId;
	ULONG nDiscussId;
	ULONG nCreaterId;
	ULONG wparam;  //save createrId
	ULONG lparam;	//save groupId;
	CString strDiscussName;
	ULONG nDisState; //group state,  1 = closed; 2
	DiscussGroupInfo()
	{
		nLocationId = 0;
		nDiscussId = 0;
		nCreaterId = 0;
		wparam = 0;
		lparam = 0;
		nDisState = GROUP_STATE_UNKNOW;
	}


	BOOL operator << (CIMSerialize& ar) const
	{

		ar << IM_NODE_VER;
		ar << nLocationId;
		ar << nDiscussId;
		ar << nCreaterId;
		ar << wparam;
		ar << lparam;
		ar << strDiscussName;
		ar <<  (nDisState == GROUP_STATE_ACTIVE)? GROUP_STATE_UNKNOW:nDisState; //nDisState; default set unknow
		return TRUE;
	}

	BOOL operator >> (CIMSerialize& ar)
	{
		BOOL bRet = TRUE;
		long nVer = 0;
		ar >> nVer;
		switch (nVer)
		{
		case 1:
			{
				ar >> nLocationId;
				ar >> nDiscussId;
				ar >> nCreaterId;
				ar >> wparam;
				ar >> lparam;
				ar >> strDiscussName;
				ar >> nDisState;
			}break;

		default:
			{
				ASSERT(FALSE);
				bRet = FALSE;
			}
			break;
		}

		return bRet;
	}
};



//////////////////////////////////////////////////////////////////////////
struct MeetingJoinInfo {

	BYTE bCommand;				//SM_CLIENT_WEB_LOGINFO，服务器提供给被邀请人
	char szConfName[32];     // 服务器提供给被邀请人
	//CHAR szUserName[65]; 		//用户名称
	//CHAR szUserPassword[32];	//用户密码
	//CHAR szVersion[32];  // Version info, newly added by wei
	unsigned long nConfStartTime; //in second
	unsigned long nConfEndTime; // in second
	//{{Added by wei on 2009/6/2 14:51 for
	unsigned short nConfMaxUser;  
	//	unsigned short nConfCurUser;
	unsigned char confmaxAdminNum;
	unsigned short confMaxPartnerNum;
	unsigned short confMaxListenerNum;
	//}} End on 2009/6/2 14:51
	long nUserID;
	long nUserRight;
	long nConfID;
	long nMcuID;
	//char szMcuIP[16];
	unsigned long dwMcuIP;
	unsigned short iMcuPort;
	long nSessionID;  // 随便给的数字,由服务器分配

	//char szFtpIP[16];
	unsigned long dwFtpIP;
	unsigned short iFtpPort;
	//char szSBCIp[16];		 // screen broadcast server ip
	unsigned long dwSBCIp;
	unsigned short iSBCPort; // port

	//{{ Added by wei on 2011/04/20 15:16: multicast--已经不用，因此客户端可以随便填充一个数值	
	//char szGroupAddr[16];    // multicast 
	//unsigned short iGroupPort; // port
	//}} End added

	MeetingJoinInfo()
	{
		ZeroMemory(this, sizeof(MeetingJoinInfo));
	}

	BOOL operator << (CIMSerialize& ar) const
	{
		ar << 1;
		LONG nLen = sizeof(MeetingJoinInfo);
		ar.Serialize((BYTE*)this, nLen);

		return TRUE;
	}

	BOOL operator >> (CIMSerialize& ar) 
	{

		BOOL bRet = TRUE;
		long nVer = 0;
		ar >> nVer;
		switch (nVer)
		{
		case 1:
			{
				LONG nLen = sizeof(MeetingJoinInfo);
				ar.Serialize((BYTE*)this, nLen);
			}break;
		default:
			{
				ASSERT(FALSE);
				bRet = FALSE;
			}
			break;
		}

		return bRet;
	}

};

//////////////////////////////////////////////////////////////////////////
struct IM_CUSTOM_TREEITEM_DATA 
{
	NODE_KEY nodeKey;
	USERID nInfoId;
	CString strTitle;
	CString strName;
	CString strId;
	int nSessionType;
	int nImgId;
	GROUPID nCompanyId; //only for depart
	HTREEITEM htreeItem; //for position tree form list

	IM_CUSTOM_TREEITEM_DATA()
	{
		nInfoId = 0;
		nSessionType = 0;
		nImgId = 0;
		nCompanyId = 0;
		htreeItem = NULL;
	}

};
