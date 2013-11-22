/* ************************************************************** *
 *	MD5.h
 *  CMD5 的声明
 *
 *  Created on:      2008-03-31 16:25:00
 *  Original author: 张鲁平
 *
 *	功能：MD5加密算法.
 *
 * *************************************************************** */

#ifndef MD5_H
#define MD5_H

#define HIGHFIRST

#ifdef __i386__
#undef HIGHFIRST
#endif

#ifdef __alpha
typedef unsigned int uint32;
#else
typedef unsigned long uint32;
#endif

#define CHECK_HARDWARE_PROPERTIES

// -------- The four core functions - F1 is optimized somewhat --------------------
/* #define F1(x, y, z) (x & y | ~x & z) */
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

// -------- This is the central step in the MD5 algorithm. ------------------------
#define MD5STEP(f, w, x, y, z, data, s) \
	( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )
// --------------------------------------------------------------------------------

// -------- 定义结构体存储 -------------------
struct MD5Context 
{
	uint32 uBuf[4];
	uint32 uBits[2];
	unsigned char ucIn[64];
};
typedef struct MD5Context MD5_CTX;
// -------------------------------------------


// -------- 类CMD5定义 -----------------------
class __declspec(dllexport) CNEWMD5
{
public:
	CNEWMD5(void);
public:
	~CNEWMD5(void);


// -------- MD5加密算法函数 -------------------------------------------------------
public:
	void MD5Init( MD5_CTX *pstruCtx );
	void MD5Update( MD5_CTX *pstruCtx, unsigned char *pBuf, unsigned int nLen );
	void MD5Final( unsigned char ucDigest[16], MD5_CTX *pstruCtx );

protected:
	void MD5Transform( uint32 uBuf[4], uint32 uIn[16] );

	#ifndef HIGHFIRST
	#define ByteReverse( pcBuf, ucLongs )	/* Nothing */
	#else
		void ByteReverse( unsigned char * pcBuf, unsigned long ucLongs )
		{
			uint32 t;
			do 
			{
				t = ( uint32 ) (( unsigned ) pcBuf[3] << 8 | pcBuf[2] ) << 16 |
					(( unsigned ) pcBuf[1] << 8 | pcBuf[0]);

				*( uint32 * ) pcBuf = t;
				
				pcBuf += 4;

			} while( --ucLongs );
		}
	#endif

// -------- END -------------------------------------------------------------------- 
};
#endif /* !MD5_H */