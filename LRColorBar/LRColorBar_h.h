

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __LRColorBar_h_h__
#define __LRColorBar_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DLRColorBar2_FWD_DEFINED__
#define ___DLRColorBar2_FWD_DEFINED__
typedef interface _DLRColorBar2 _DLRColorBar2;
#endif 	/* ___DLRColorBar2_FWD_DEFINED__ */


#ifndef ___DLRColorBar2Events_FWD_DEFINED__
#define ___DLRColorBar2Events_FWD_DEFINED__
typedef interface _DLRColorBar2Events _DLRColorBar2Events;
#endif 	/* ___DLRColorBar2Events_FWD_DEFINED__ */


#ifndef __LRColorBar_FWD_DEFINED__
#define __LRColorBar_FWD_DEFINED__

#ifdef __cplusplus
typedef class LRColorBar LRColorBar;
#else
typedef struct LRColorBar LRColorBar;
#endif /* __cplusplus */

#endif 	/* __LRColorBar_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __LRColorBar2Lib_LIBRARY_DEFINED__
#define __LRColorBar2Lib_LIBRARY_DEFINED__

/* library LRColorBar2Lib */
/* [control][helpstring][helpfile][version][uuid] */ 


DEFINE_GUID(LIBID_LRColorBar2Lib,0x1B162BF9,0x2F19,0x4125,0x8B,0xB0,0xC9,0x0B,0x02,0xFC,0x05,0x53);

#ifndef ___DLRColorBar2_DISPINTERFACE_DEFINED__
#define ___DLRColorBar2_DISPINTERFACE_DEFINED__

/* dispinterface _DLRColorBar2 */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DLRColorBar2,0x5334F68B,0xC6EB,0x41bd,0x9E,0xC7,0x4A,0x33,0x18,0x49,0x3F,0x5B);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5334F68B-C6EB-41bd-9EC7-4A3318493F5B")
    _DLRColorBar2 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DLRColorBar2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DLRColorBar2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DLRColorBar2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DLRColorBar2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DLRColorBar2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DLRColorBar2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DLRColorBar2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DLRColorBar2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DLRColorBar2Vtbl;

    interface _DLRColorBar2
    {
        CONST_VTBL struct _DLRColorBar2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DLRColorBar2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DLRColorBar2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DLRColorBar2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DLRColorBar2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DLRColorBar2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DLRColorBar2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DLRColorBar2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DLRColorBar2_DISPINTERFACE_DEFINED__ */


#ifndef ___DLRColorBar2Events_DISPINTERFACE_DEFINED__
#define ___DLRColorBar2Events_DISPINTERFACE_DEFINED__

/* dispinterface _DLRColorBar2Events */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DLRColorBar2Events,0x5320B318,0xAF6C,0x4dd1,0xA4,0x52,0xA4,0x25,0xF9,0x27,0x06,0x98);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("5320B318-AF6C-4dd1-A452-A425F9270698")
    _DLRColorBar2Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DLRColorBar2EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DLRColorBar2Events * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DLRColorBar2Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DLRColorBar2Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DLRColorBar2Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DLRColorBar2Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DLRColorBar2Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DLRColorBar2Events * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DLRColorBar2EventsVtbl;

    interface _DLRColorBar2Events
    {
        CONST_VTBL struct _DLRColorBar2EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DLRColorBar2Events_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DLRColorBar2Events_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DLRColorBar2Events_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DLRColorBar2Events_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DLRColorBar2Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DLRColorBar2Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DLRColorBar2Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DLRColorBar2Events_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_LRColorBar,0xAEB9981E,0x773A,0x4ca2,0x83,0xCB,0x8B,0x74,0xB3,0x30,0x54,0x0B);

#ifdef __cplusplus

class DECLSPEC_UUID("AEB9981E-773A-4ca2-83CB-8B74B330540B")
LRColorBar;
#endif
#endif /* __LRColorBar2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


