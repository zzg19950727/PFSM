// MyOverlayIcon.h : CMyOverlayIcon µÄÉùÃ÷

#pragma once
#include "OverlayIcon_i.h"
#include "resource.h"       // Ö÷·ûºÅ
#include <comsvcs.h>

// You can put these includes in "stdafx.h" if you want
#include <shlobj.h>
#include <comdef.h>

using namespace ATL;



// CMyOverlayIcon

class ATL_NO_VTABLE CMyOverlayIcon :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyOverlayIcon, &CLSID_MyOverlayIcon>,
	public IShellIconOverlayIdentifier,
	public IDispatchImpl<IMyOverlayIcon, &IID_IMyOverlayIcon, &LIBID_OverlayIconLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMyOverlayIcon()
	{
	}

	// IShellIconOverlayIdentifier Methods
  STDMETHOD(GetOverlayInfo)(LPWSTR pwszIconFile, 
           int cchMax,int *pIndex,DWORD* pdwFlags);
  STDMETHOD(GetPriority)(int* pPriority);
  STDMETHOD(IsMemberOf)(LPCWSTR pwszPath,DWORD dwAttrib);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYOVERLAYICON)

DECLARE_NOT_AGGREGATABLE(CMyOverlayIcon)

BEGIN_COM_MAP(CMyOverlayIcon)
	COM_INTERFACE_ENTRY(IMyOverlayIcon)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IShellIconOverlayIdentifier)
END_COM_MAP()




// IMyOverlayIcon
public:
};

OBJECT_ENTRY_AUTO(__uuidof(MyOverlayIcon), CMyOverlayIcon)
