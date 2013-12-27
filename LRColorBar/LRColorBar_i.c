

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Apr 23 10:47:59 2006
 */
/* Compiler settings for .\LRColorBar.odl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, LIBID_LRColorBar2Lib,0x1B162BF9,0x2F19,0x4125,0x8B,0xB0,0xC9,0x0B,0x02,0xFC,0x05,0x53);


MIDL_DEFINE_GUID(IID, DIID__DLRColorBar2,0x5334F68B,0xC6EB,0x41bd,0x9E,0xC7,0x4A,0x33,0x18,0x49,0x3F,0x5B);


MIDL_DEFINE_GUID(IID, DIID__DLRColorBar2Events,0x5320B318,0xAF6C,0x4dd1,0xA4,0x52,0xA4,0x25,0xF9,0x27,0x06,0x98);


MIDL_DEFINE_GUID(CLSID, CLSID_LRColorBar,0xAEB9981E,0x773A,0x4ca2,0x83,0xCB,0x8B,0x74,0xB3,0x30,0x54,0x0B);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



