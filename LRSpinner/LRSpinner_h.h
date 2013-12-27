

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __LRSpinner_h_h__
#define __LRSpinner_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DLRSpinner2_FWD_DEFINED__
#define ___DLRSpinner2_FWD_DEFINED__
typedef interface _DLRSpinner2 _DLRSpinner2;
#endif 	/* ___DLRSpinner2_FWD_DEFINED__ */


#ifndef ___DLRSpinner2Events_FWD_DEFINED__
#define ___DLRSpinner2Events_FWD_DEFINED__
typedef interface _DLRSpinner2Events _DLRSpinner2Events;
#endif 	/* ___DLRSpinner2Events_FWD_DEFINED__ */


#ifndef __LRSpinner2_FWD_DEFINED__
#define __LRSpinner2_FWD_DEFINED__

#ifdef __cplusplus
typedef class LRSpinner2 LRSpinner2;
#else
typedef struct LRSpinner2 LRSpinner2;
#endif /* __cplusplus */

#endif 	/* __LRSpinner2_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __LRSpinner2Lib_LIBRARY_DEFINED__
#define __LRSpinner2Lib_LIBRARY_DEFINED__

/* library LRSpinner2Lib */
/* [control][helpstring][helpfile][version][uuid] */ 


DEFINE_GUID(LIBID_LRSpinner2Lib,0x8830AB1C,0xA61C,0x43c1,0xA8,0xFB,0x57,0x7C,0x33,0x03,0x3F,0xA7);

#ifndef ___DLRSpinner2_DISPINTERFACE_DEFINED__
#define ___DLRSpinner2_DISPINTERFACE_DEFINED__

/* dispinterface _DLRSpinner2 */
/* [hidden][helpstring][uuid] */ 


DEFINE_GUID(DIID__DLRSpinner2,0x96967794,0xB5C3,0x4da4,0x87,0xC4,0x85,0xE4,0xF0,0x1D,0x20,0xAA);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("96967794-B5C3-4da4-87C4-85E4F01D20AA")
    _DLRSpinner2 : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DLRSpinner2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DLRSpinner2 * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DLRSpinner2 * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DLRSpinner2 * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DLRSpinner2 * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DLRSpinner2 * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DLRSpinner2 * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DLRSpinner2 * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DLRSpinner2Vtbl;

    interface _DLRSpinner2
    {
        CONST_VTBL struct _DLRSpinner2Vtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DLRSpinner2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DLRSpinner2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DLRSpinner2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DLRSpinner2_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DLRSpinner2_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DLRSpinner2_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DLRSpinner2_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DLRSpinner2_DISPINTERFACE_DEFINED__ */


#ifndef ___DLRSpinner2Events_DISPINTERFACE_DEFINED__
#define ___DLRSpinner2Events_DISPINTERFACE_DEFINED__

/* dispinterface _DLRSpinner2Events */
/* [helpstring][uuid] */ 


DEFINE_GUID(DIID__DLRSpinner2Events,0xA311F008,0x7103,0x4ac8,0xB5,0xC9,0xE5,0xD8,0xAE,0xE4,0x9E,0xF3);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("A311F008-7103-4ac8-B5C9-E5D8AEE49EF3")
    _DLRSpinner2Events : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DLRSpinner2EventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DLRSpinner2Events * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DLRSpinner2Events * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DLRSpinner2Events * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DLRSpinner2Events * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DLRSpinner2Events * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DLRSpinner2Events * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DLRSpinner2Events * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DLRSpinner2EventsVtbl;

    interface _DLRSpinner2Events
    {
        CONST_VTBL struct _DLRSpinner2EventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DLRSpinner2Events_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _DLRSpinner2Events_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _DLRSpinner2Events_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _DLRSpinner2Events_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _DLRSpinner2Events_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _DLRSpinner2Events_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _DLRSpinner2Events_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DLRSpinner2Events_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_LRSpinner2,0xA9313B55,0xBB94,0x4804,0xB0,0x80,0x05,0xE5,0x8C,0xE1,0xA6,0x2C);

#ifdef __cplusplus

class DECLSPEC_UUID("A9313B55-BB94-4804-B080-05E58CE1A62C")
LRSpinner2;
#endif
#endif /* __LRSpinner2Lib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


