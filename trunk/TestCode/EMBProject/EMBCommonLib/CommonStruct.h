//������Ϣ�ṹ��
struct CommonRequestType
{
	char cTaskID[256];     //����ID
	char cUserCode[256]    //�û�����
	char cUserName[256]    //�û�����
	char cSourceID[512]    //ԭʼϵͳID
	long nType;            //�������ͣ�1.Ǩ�ƣ�2.MD5��3.ת�룬4.����
	long nTaskOp;          //���������1.�ύ��2.ɾ����3.�޸ģ�4.���ã�5.��ѯ
	long nRetry;           //���Դ���
	long nPRI;             //���ȼ�
};

//�ļ�·���ṹ��
struct FileAddressType
{
	char cFileName[512];   //�ļ�����
	char cFilePath[512];   //��FTPģʽ�£�Ϊ�ļ����·�������̿���ģʽ�£�Ϊ�ļ�����·��
	char cFtpIP[20];       //FTPģʽ��IP��ַ
	char cFtpUser[512];    //FTPģʽ���û���
	char cFtpPwd[512];     //FTPģʽ������
	long cFtpPort;         //FTPģʽ�¶˿ں�
	long nType;            //��ַ���ͣ�1.FTP��2.���̿���
};

//������Ϣ�ṹ��
struct MediaTaskInfoType
{
	CommonRequestType taskInfo;
	char taskData[4096];
};

//�豸��Ϣ�ṹ��
struct ActorDevInfoType
{

};