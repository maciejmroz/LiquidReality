

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Apr 23 10:47:44 2006
 */
/* Compiler settings for .\LREnvelopeEditor.odl:
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

MIDL_DEFINE_GUID(IID, LIBID_LREnvelopeEditor2Lib,0xA01F028C,0xC6B1,0x4c37,0x88,0x0B,0xD3,0xA6,0x13,0xC2,0x3E,0x7C);


MIDL_DEFINE_GUID(IID, DIID__DLREnvelopeEditor2,0xE9D668E1,0x49C3,0x4538,0xB8,0x6C,0xA7,0xDA,0xB0,0xB3,0x95,0x84);


MIDL_DEFINE_GUID(IID, DIID__DLREnvelopeEditor2Events,0x6E1200B0,0xEE62,0x4acb,0x90,0xD0,0x1E,0xD1,0xE1,0xE9,0x31,0xA0);


MIDL_DEFINE_GUID(CLSID, CLSID_LREnvelopeEditor2,0xDEBE34DF,0xB53A,0x4576,0xBB,0x0F,0x9A,0x34,0xB6,0x38,0x99,0xFB);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



