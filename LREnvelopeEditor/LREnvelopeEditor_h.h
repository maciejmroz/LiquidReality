

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __LREnvelopeEditor_h_h__
#define __LREnvelopeEditor_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DLREnvelopeEditor2_FWD_DEFINED__
#define ___DLREnvelopeEditor2_FWD_DEFINED__
typedef interface _DLREnvelopeEditor2 _DLREnvelopeEditor2;
#endif 	/* ___DLREnvelopeEditor2_FWD_DEFINED__ */


#ifndef ___DLREnvelopeEditor2Events_FWD_DEFINED__
#define ___DLREnvelopeEditor2Events_FWD_DEFINED__
typedef interface _DLREnvelopeEditor2Events _DLREnvelopeEditor2Events;
#endif 	/* ___DLREnvelopeEditor2Events_FWD_DEFINED__ */


#ifndef __LREnvelopeEditor2_FWD_DEFINED__
#define __LREnvelopeEditor2_FWD_DEFINED__

#ifdef __cplusplus
typedef class LREnvelopeEditor2 LREnvelopeEditor2;
#else
typedef struct LREnvelopeEditor2 LREnvelopeEditor2;
#endif /* __cplusplus */

#endif 	/* __LREnvelopeEditor2_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __LREnvelopeEditor2Lib_LIBRARY_DEFINED__
#define __LREnvelopeEditor2Lib_LIBRARY_DEFINED__

/* library LREnvelopeEditor2Lib */
/* [control][helpstring][helpfile][version][uuid] */ 


DEFINE_GUID(LIBID_LREnvelopeEditor2Lib,0xA01F028C,0xC6B1,0x4c37,0x88,0x0B,0xD3,0xA6,0x13,0xC2,0x3E,0x7C);

#ifndef ___DLREnvelopeEditor2_DISPINTERFACE_DEFINED__
#define ___DLREnvelopeEditor2_DISPINTERFACE_DEFINED__

/* dispinterface _DLREnvelopeEditor2 */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DLREnvelopeEditor2,0xE9D668E1,0x49C3,0x4538,0xB8,0x6C,0xA7,0xDA,0xB0,0xB3,0x95,0x84);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E9D668E1-49C3-4538-B86C-A7DAB0B39584")
    _DLREnvelopeEditor2 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DLREnvelopeEditor2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DLREnvelopeEditor2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DLREnvelopeEditor2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DLREnvelopeEditor2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DLREnvelopeEditor2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DLREnvelopeEditor2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DLREnvelopeEditor2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DLREnvelopeEditor2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DLREnvelopeEditor2Vtbl;

    interface _DLREnvelopeEditor2
    {
        CONST_VTBL struct _DLREnvelopeEditor2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DLREnvelopeEditor2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DLREnvelopeEditor2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DLREnvelopeEditor2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DLREnvelopeEditor2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DLREnvelopeEditor2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DLREnvelopeEditor2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DLREnvelopeEditor2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DLREnvelopeEditor2_DISPINTERFACE_DEFINED__ */


#ifndef ___DLREnvelopeEditor2Events_DISPINTERFACE_DEFINED__
#define ___DLREnvelopeEditor2Events_DISPINTERFACE_DEFINED__

/* dispinterface _DLREnvelopeEditor2Events */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DLREnvelopeEditor2Events,0x6E1200B0,0xEE62,0x4acb,0x90,0xD0,0x1E,0xD1,0xE1,0xE9,0x31,0xA0);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("6E1200B0-EE62-4acb-90D0-1ED1E1E931A0")
    _DLREnvelopeEditor2Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DLREnvelopeEditor2EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DLREnvelopeEditor2Events * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DLREnvelopeEditor2Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DLREnvelopeEditor2Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DLREnvelopeEditor2Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DLREnvelopeEditor2Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DLREnvelopeEditor2Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DLREnvelopeEditor2Events * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DLREnvelopeEditor2EventsVtbl;

    interface _DLREnvelopeEditor2Events
    {
        CONST_VTBL struct _DLREnvelopeEditor2EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DLREnvelopeEditor2Events_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DLREnvelopeEditor2Events_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DLREnvelopeEditor2Events_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DLREnvelopeEditor2Events_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DLREnvelopeEditor2Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DLREnvelopeEditor2Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DLREnvelopeEditor2Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DLREnvelopeEditor2Events_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_LREnvelopeEditor2,0xDEBE34DF,0xB53A,0x4576,0xBB,0x0F,0x9A,0x34,0xB6,0x38,0x99,0xFB);

#ifdef __cplusplus

class DECLSPEC_UUID("DEBE34DF-B53A-4576-BB0F-9A34B63899FB")
LREnvelopeEditor2;
#endif
#endif /* __LREnvelopeEditor2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


