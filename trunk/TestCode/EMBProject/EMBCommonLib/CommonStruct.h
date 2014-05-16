//任务信息结构体
struct CommonRequestType
{
	char cTaskID[256];     //任务ID
	char cUserCode[256]    //用户编码
	char cUserName[256]    //用户名称
	char cSourceID[512]    //原始系统ID
	long nType;            //任务类型：1.迁移，2.MD5，3.转码，4.技审
	long nTaskOp;          //任务操作：1.提交，2.删除，3.修改，4.重置，5.查询
	long nRetry;           //重试次数
	long nPRI;             //优先级
};

//文件路径结构体
struct FileAddressType
{
	char cFileName[512];   //文件名称
	char cFilePath[512];   //在FTP模式下，为文件相对路径；磁盘拷贝模式下，为文件绝对路径
	char cFtpIP[20];       //FTP模式下IP地址
	char cFtpUser[512];    //FTP模式下用户名
	char cFtpPwd[512];     //FTP模式下密码
	long cFtpPort;         //FTP模式下端口号
	long nType;            //地址类型：1.FTP，2.磁盘拷贝
};

//任务信息结构体
struct MediaTaskInfoType
{
	CommonRequestType taskInfo;
	char taskData[4096];
};

//设备信息结构体
struct ActorDevInfoType
{

};