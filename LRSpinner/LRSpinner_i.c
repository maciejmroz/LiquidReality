

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Apr 23 10:47:52 2006
 */
/* Compiler settings for .\LRSpinner.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_LRSpinner2Lib,0x8830AB1C,0xA61C,0x43c1,0xA8,0xFB,0x57,0x7C,0x33,0x03,0x3F,0xA7);


MIDL_DEFINE_GUID(IID, DIID__DLRSpinner2,0x96967794,0xB5C3,0x4da4,0x87,0xC4,0x85,0xE4,0xF0,0x1D,0x20,0xAA);


MIDL_DEFINE_GUID(IID, DIID__DLRSpinner2Events,0xA311F008,0x7103,0x4ac8,0xB5,0xC9,0xE5,0xD8,0xAE,0xE4,0x9E,0xF3);


MIDL_DEFINE_GUID(CLSID, CLSID_LRSpinner2,0xA9313B55,0xBB94,0x4804,0xB0,0x80,0x05,0xE5,0x8C,0xE1,0xA6,0x2C);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



