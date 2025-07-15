/*==========================================================================
 |               Quectel OpenCPU --  User API
 |
 |              Copyright (c) 2010 Quectel Ltd.
 |
 |--------------------------------------------------------------------------
 | File Description
 | ----------------
 |      Basic data type definition
 |
 |--------------------------------------------------------------------------
 |
 |  Designed by     :   
 |  Coded    by     :   
 |  Tested   by     :   
 |
 \=========================================================================*/

#ifndef __QL_TYPE_H__
#define __QL_TYPE_H__

#ifndef FALSE
#define FALSE    0
#endif

#ifndef TRUE
#define TRUE     1
#endif

#ifndef NULL
#define NULL    ((void *) 0)
#endif
/*========================================================================
 *  Type Definition
 *========================================================================*/
#ifndef BOOLEAN
typedef unsigned char BOOLEAN;
#endif

#ifndef int8
typedef signed char int8;
#endif

#ifndef uint8
typedef unsigned char uint8;
#endif

#ifndef UINT8
typedef unsigned char UINT8;
#endif

#ifndef int16
typedef signed short int16;
#endif

#ifndef uint16
typedef unsigned short uint16;
#endif

#ifndef INT16
typedef signed short              INT16;
#endif

#ifndef UINT16
typedef unsigned short          UINT16;
#endif

#ifndef int32
typedef signed int int32;
#endif

#ifndef INT32
typedef signed int  INT32;
#endif

#ifndef UINT32
typedef unsigned int UINT32;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

#ifndef UINT
typedef unsigned int UINT;
#endif
    
#ifndef uint32
    typedef unsigned int uint32;
#endif

#ifndef PVOID
typedef void *PVOID;
#endif

#ifndef TCHAR
typedef char TCHAR;
#endif

#ifndef CHAR
typedef char     CHAR;
#endif
    
#ifndef int64
typedef long long   int64;
#endif
    
#ifndef uint64
typedef unsigned long long  uint64;
#endif

#ifndef INT64
typedef long long           INT64;
#endif

#ifndef UINT64
typedef unsigned long long     UINT64;
#endif

#ifndef bool
#define bool unsigned char
#endif

#ifndef BOOL
#define BOOL unsigned char

#endif


#ifndef u8
typedef unsigned char       u8;
#endif

#ifndef s8
typedef signed   char       s8;
#endif

#ifndef u16
typedef unsigned short      u16;
#endif

#ifndef s16
typedef          short      s16;
#endif

#ifndef u32
typedef unsigned int        u32;
#endif

#ifndef s32
typedef          int        s32;
#endif

#ifndef u64
typedef unsigned long long  u64;
#endif

#ifndef s64
typedef          long long  s64;
#endif

#ifndef ticks
typedef unsigned int        ticks;
#endif

#ifndef size_t
typedef unsigned int 		size_t;
#endif

#ifndef uint8_t

typedef unsigned char		uint8_t;
#endif

#endif  
// End-of __QL_TYPE_H__
