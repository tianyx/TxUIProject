/* ************************************************************** *
 *	MD5.cpp
 *  CMD5 的实现
 *
 *  Created on:      2008-03-31 16:25:00
 *  Original author: 张鲁平
 *
 *	功能：MD5加密算法.
 *
 * *************************************************************** */

#include "StdAfx.h"
#include <memory.h>		 /* for memcpy() */
#include "MD5.h"


// -------- STRART ---------------------------------------------------------------------------
CNEWMD5::CNEWMD5(void)
{
	// -------- 启动初始化 ------------- 
}

CNEWMD5::~CNEWMD5(void)
{
	// -------- 退出  释放 -------------
}

/* ------------------------------------------------------------------------
 *
 * Start MD5 accumulation. 
 * Set bit count to 0 and buffer to mysterious initialization constants.
 *
 * ------------------------------------------------------------------------ */
void CNEWMD5::MD5Init( MD5_CTX *pstruCtx )
{
	pstruCtx->uBuf[0] = 0x67452301;
	pstruCtx->uBuf[1] = 0xefcdab89;
	pstruCtx->uBuf[2] = 0x98badcfe;
	pstruCtx->uBuf[3] = 0x10325476;

	pstruCtx->uBits[0] = 0;
	pstruCtx->uBits[1] = 0;
}

/* --------------------------------------------------------------------------
 *
 * Update context to reflect the concatenation of another buffer fullof bytes.
 *
 * -------------------------------------------------------------------------- */
void CNEWMD5::MD5Update( MD5_CTX *pstruCtx, unsigned char *pBuf, unsigned int nLen )
{
	uint32 t;

	/* Update bitcount */
	t = pstruCtx->uBits[0];
	if ( ( pstruCtx->uBits[0] = t + ( ( uint32 ) nLen << 3 ) ) < t )
	{
		pstruCtx->uBits[1]++; 	/* Carry from low to high */
	}
	pstruCtx->uBits[1] += nLen >> 29;

	t = ( t >> 3 ) & 0x3f;	/* Bytes already in shsInfo->data */

	/* Handle any leading odd-sized chunks */
	if ( t ) 
	{
		unsigned char *p = (unsigned char *) pstruCtx->ucIn + t;

		t = 64 - t;
		if ( nLen < t ) 
		{
			memcpy( p, pBuf, nLen );
			return;
		}

		memcpy( p, pBuf, t );
		ByteReverse( pstruCtx->ucIn, 16 );
		MD5Transform( pstruCtx->uBuf, (uint32 *) pstruCtx->ucIn );
		pBuf += t;
		nLen -= t;
	}

	/* Process data in 64-byte chunks */
	while ( nLen >= 64 ) 
	{
		memcpy( pstruCtx->ucIn, pBuf, 64 );
		ByteReverse( pstruCtx->ucIn, 16 );
		MD5Transform( pstruCtx->uBuf, ( uint32 * ) pstruCtx->ucIn );
		pBuf += 64;
		nLen -= 64;
	}

	/* Handle any remaining bytes of data. */
	memcpy( pstruCtx->ucIn, pBuf, nLen );
}

/* --------------------------------------------------------------------------
*
* Final wrap up - pad to 64-byte boundary with the bit pattern 
* 1 0* (64-bit count of bits processed, MSB-first)
*
* -------------------------------------------------------------------------- */
void CNEWMD5::MD5Final( unsigned char ucDigest[16], MD5_CTX *pstruCtx )
{
	
	unsigned int nCount;
	unsigned char *pChar;

	/* Compute number of bytes mod 64 */
	nCount = ( pstruCtx->uBits[0] >> 3 ) & 0x3F;

	/* Set the first char of padding to 0x80.  This is safe since there is
	always at least one byte free */
	pChar = pstruCtx->ucIn + nCount;
	*pChar++ = 0x80;

	/* Bytes of padding needed to make 64 bytes */
	nCount = 64 - 1 - nCount;

	/* Pad out to 56 mod 64 */
	if ( nCount < 8 )
	{
		/* Two lots of padding:  Pad the first block to 64 bytes */
		memset( pChar, 0, nCount );
		ByteReverse( pstruCtx->ucIn, 16 );
		MD5Transform( pstruCtx->uBuf, ( uint32 * )pstruCtx->ucIn );

		/* Now fill the next block with 56 bytes */
		memset( pstruCtx->ucIn, 0, 56 );
	} 
	else
	{
		/* Pad block to 56 bytes */
		memset( pChar, 0, nCount - 8 );
	}

	ByteReverse( pstruCtx->ucIn, 14 );

	/* Append length in bits and transform */
	( (uint32 *) pstruCtx->ucIn )[14] = pstruCtx->uBits[0];
	( (uint32 *) pstruCtx->ucIn )[15] = pstruCtx->uBits[1];

	MD5Transform( pstruCtx->uBuf, (uint32 *) pstruCtx->ucIn );
	ByteReverse( (unsigned char *) pstruCtx->uBuf, 4 );
	memcpy( ucDigest, pstruCtx->uBuf, 16 );
	memset( pstruCtx, 0, sizeof( pstruCtx ) );        /* In case it's sensitive */
}

/* --------------------------------------------------------------------------
*
* The core of the MD5 algorithm, this alters an existing MD5 hash to
* reflect the addition of 16 longwords of new data.  MD5Update blocks
* the data and converts bytes into longwords for this routine.
*
* -------------------------------------------------------------------------- */
void CNEWMD5::MD5Transform( uint32 uBuf[4], uint32 uIn[16] )
{
	register uint32 a, b, c, d;

	a = uBuf[0];
	b = uBuf[1];
	c = uBuf[2];
	d = uBuf[3];

	MD5STEP( F1, a, b, c, d, uIn[0]  + 0xd76aa478, 7  );
	MD5STEP( F1, d, a, b, c, uIn[1]  + 0xe8c7b756, 12 );
	MD5STEP( F1, c, d, a, b, uIn[2]  + 0x242070db, 17 );
	MD5STEP( F1, b, c, d, a, uIn[3]  + 0xc1bdceee, 22 );
	MD5STEP( F1, a, b, c, d, uIn[4]  + 0xf57c0faf, 7  );
	MD5STEP( F1, d, a, b, c, uIn[5]  + 0x4787c62a, 12 );
	MD5STEP( F1, c, d, a, b, uIn[6]  + 0xa8304613, 17 );
	MD5STEP( F1, b, c, d, a, uIn[7]  + 0xfd469501, 22 );
	MD5STEP( F1, a, b, c, d, uIn[8]  + 0x698098d8, 7  );
	MD5STEP( F1, d, a, b, c, uIn[9]  + 0x8b44f7af, 12 );
	MD5STEP( F1, c, d, a, b, uIn[10] + 0xffff5bb1, 17 );
	MD5STEP( F1, b, c, d, a, uIn[11] + 0x895cd7be, 22 );
	MD5STEP( F1, a, b, c, d, uIn[12] + 0x6b901122, 7  );
	MD5STEP( F1, d, a, b, c, uIn[13] + 0xfd987193, 12 );
	MD5STEP( F1, c, d, a, b, uIn[14] + 0xa679438e, 17 );
	MD5STEP( F1, b, c, d, a, uIn[15] + 0x49b40821, 22 );

	MD5STEP( F2, a, b, c, d, uIn[1]  + 0xf61e2562, 5  );
	MD5STEP( F2, d, a, b, c, uIn[6]  + 0xc040b340, 9  );
	MD5STEP( F2, c, d, a, b, uIn[11] + 0x265e5a51, 14 );
	MD5STEP( F2, b, c, d, a, uIn[0]  + 0xe9b6c7aa, 20 );
	MD5STEP( F2, a, b, c, d, uIn[5]  + 0xd62f105d, 5  );
	MD5STEP( F2, d, a, b, c, uIn[10] + 0x02441453, 9  );
	MD5STEP( F2, c, d, a, b, uIn[15] + 0xd8a1e681, 14 );
	MD5STEP( F2, b, c, d, a, uIn[4]  + 0xe7d3fbc8, 20 );
	MD5STEP( F2, a, b, c, d, uIn[9]  + 0x21e1cde6, 5  );
	MD5STEP( F2, d, a, b, c, uIn[14] + 0xc33707d6, 9  );
	MD5STEP( F2, c, d, a, b, uIn[3]  + 0xf4d50d87, 14 );
	MD5STEP( F2, b, c, d, a, uIn[8]  + 0x455a14ed, 20 );
	MD5STEP( F2, a, b, c, d, uIn[13] + 0xa9e3e905, 5  );
	MD5STEP( F2, d, a, b, c, uIn[2]  + 0xfcefa3f8, 9  );
	MD5STEP( F2, c, d, a, b, uIn[7]  + 0x676f02d9, 14 );
	MD5STEP( F2, b, c, d, a, uIn[12] + 0x8d2a4c8a, 20 );

	MD5STEP( F3, a, b, c, d, uIn[5]  + 0xfffa3942, 4  );
	MD5STEP( F3, d, a, b, c, uIn[8]  + 0x8771f681, 11 );
	MD5STEP( F3, c, d, a, b, uIn[11] + 0x6d9d6122, 16 );
	MD5STEP( F3, b, c, d, a, uIn[14] + 0xfde5380c, 23 );
	MD5STEP( F3, a, b, c, d, uIn[1]  + 0xa4beea44, 4  );
	MD5STEP( F3, d, a, b, c, uIn[4]  + 0x4bdecfa9, 11 );
	MD5STEP( F3, c, d, a, b, uIn[7]  + 0xf6bb4b60, 16 );
	MD5STEP( F3, b, c, d, a, uIn[10] + 0xbebfbc70, 23 );
	MD5STEP( F3, a, b, c, d, uIn[13] + 0x289b7ec6, 4  );
	MD5STEP( F3, d, a, b, c, uIn[0]  + 0xeaa127fa, 11 );
	MD5STEP( F3, c, d, a, b, uIn[3]  + 0xd4ef3085, 16 );
	MD5STEP( F3, b, c, d, a, uIn[6]  + 0x04881d05, 23 );
	MD5STEP( F3, a, b, c, d, uIn[9]  + 0xd9d4d039, 4  );
	MD5STEP( F3, d, a, b, c, uIn[12] + 0xe6db99e5, 11 );
	MD5STEP( F3, c, d, a, b, uIn[15] + 0x1fa27cf8, 16 );
	MD5STEP( F3, b, c, d, a, uIn[2]  + 0xc4ac5665, 23 );

	MD5STEP( F4, a, b, c, d, uIn[0]  + 0xf4292244, 6  );
	MD5STEP( F4, d, a, b, c, uIn[7]  + 0x432aff97, 10 );
	MD5STEP( F4, c, d, a, b, uIn[14] + 0xab9423a7, 15 );
	MD5STEP( F4, b, c, d, a, uIn[5]  + 0xfc93a039, 21 );
	MD5STEP( F4, a, b, c, d, uIn[12] + 0x655b59c3, 6  );
	MD5STEP( F4, d, a, b, c, uIn[3]  + 0x8f0ccc92, 10 );
	MD5STEP( F4, c, d, a, b, uIn[10] + 0xffeff47d, 15 );
	MD5STEP( F4, b, c, d, a, uIn[1]  + 0x85845dd1, 21 );
	MD5STEP( F4, a, b, c, d, uIn[8]  + 0x6fa87e4f, 6  );
	MD5STEP( F4, d, a, b, c, uIn[15] + 0xfe2ce6e0, 10 );
	MD5STEP( F4, c, d, a, b, uIn[6]  + 0xa3014314, 15 );
	MD5STEP( F4, b, c, d, a, uIn[13] + 0x4e0811a1, 21 );
	MD5STEP( F4, a, b, c, d, uIn[4]  + 0xf7537e82, 6  );
	MD5STEP( F4, d, a, b, c, uIn[11] + 0xbd3af235, 10 );
	MD5STEP( F4, c, d, a, b, uIn[2]  + 0x2ad7d2bb, 15 );
	MD5STEP( F4, b, c, d, a, uIn[9]  + 0xeb86d391, 21 );

	uBuf[0] += a;
	uBuf[1] += b;
	uBuf[2] += c;
	uBuf[3] += d;
}
// -------- END ---------------------------------------------------------------------------