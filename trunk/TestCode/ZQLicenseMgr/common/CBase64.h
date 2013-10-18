// ***************************************************************
//  CBase64.h   create date: 05/02/2009
//  -------------------------------------------------------------
//  Author: Song Haifeng
//  $LastChangedDate: 2009-05-19 15:36:27 +0800 (ÐÇÆÚ¶þ, 2009-05-19) $
//  $Rev: 352 $
//  Version history: 
//	 	  
//  -------------------------------------------------------------
//  Copyright (C) 2009  Hooyou tech. - All Rights Reserved
// ***************************************************************
// 
// ***************************************************************

#if ! defined( BASE_64_CODING_CLASS_HEADER )
class CBase64
{
public:
	static void base64(unsigned char *out, const unsigned char *in, int len);
	static int unbase64(unsigned char *out, const unsigned char *in, int maxlen);
};


#endif // BASE_64_CODING_CLASS_HEADER
