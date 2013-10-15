
#ifndef __IASIDEVPRECONFIG__
#define __IASIDEVPRECONFIG__

#define DEVVALUENOTUSED 0xffffffff
#pragma pack(4)
struct ST_DEVPARAM
{
	int nDevType; //2144
	int nPort; //0-3
	int bAutoAdjustOutTsBps; //only enable when blockmode = 0;
	int nOutTsBps; //out bps ,can set when running
	int nFifoSize; //byte
	int nTxMode; //188 byte DTAPI_TXMODE_188
	int bInputBlockMode; // 1= block , 0= nonblock
	void* pExInfo;

	ST_DEVPARAM()
	{
		nDevType = DEVVALUENOTUSED;
		nPort = DEVVALUENOTUSED;
		bAutoAdjustOutTsBps = DEVVALUENOTUSED;
		nOutTsBps = DEVVALUENOTUSED;
		nFifoSize = DEVVALUENOTUSED;
		nTxMode = DEVVALUENOTUSED;
		bInputBlockMode = DEVVALUENOTUSED;
		pExInfo = NULL;
	}
};

#pragma pack()

#ifdef __cplusplus
extern "C" {
#endif

	//input TS bitrate, VT_INT, readonly
	// {4969544D-6C82-4952-90E5-1E1F83680156}
	static const GUID CLSID_ASIInputBitrate =
	{0x4969544d, 0x6c82, 0x4952, {0x90, 0xe5, 0x1e, 0x1f, 0x83, 0x68, 0x1, 0x56}};
	//output TS bitrate,VT_INT
	// {153411D2-641D-4b8f-922C-DD0F1E3FF25A}
	static const GUID CLSID_ASIOutputBitrate = 
	{0x153411d2, 0x641d, 0x4b8f, {0x92, 0x2c, 0xdd, 0xf, 0x1e, 0x3f, 0xf2, 0x5a}};

	// {61CAF378-D0F5-4861-B35C-C738BF7D6BF7}
	static const GUID CLSID_AutoAdjustOutputBitrate = 
	{0x61caf378, 0xd0f5, 0x4861, {0xb3, 0x5c, 0xc7, 0x38, 0xbf, 0x7d, 0x6b, 0xf7}};



	// {A12665E7-A873-4b18-818E-CFBDEDAAA7F9}
	DEFINE_GUID(IID_IASIDevPreConfig,
		0xa12665e7, 0xa873, 0x4b18, 0x81, 0x8e, 0xcf, 0xbd, 0xed, 0xaa, 0xa7, 0xf9);

	DECLARE_INTERFACE_(IASIDevPreConfig, IUnknown)
	{
		STDMETHOD(init_DevPreConfig) (THIS_
			ST_DEVPARAM *paramIn      //
			) PURE;

		STDMETHOD(get_DevPreConfig) (THIS_
			ST_DEVPARAM *paramOut      // 
			) PURE;

		STDMETHOD(get_value) ( /* [in] */ GUID *pParamID,
			/* [in] */ const VARIANT *pValue
			) PURE;

		STDMETHOD(set_value) ( /* [in] */ const GUID *pParamID,
			/* [in] */ const VARIANT *pValue 
			) PURE;
// 		STDMETHOD(put_Default) (THIS) PURE;
	};

#ifdef __cplusplus
}
#endif

#endif // __IASIDEVPRECONFIG__

