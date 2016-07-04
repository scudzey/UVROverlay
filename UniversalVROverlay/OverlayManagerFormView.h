#pragma once
#define _AFXDLL
#include "stdafx.h"
#include "resource.h"

// OverlayManagerFormView form view

class OverlayManagerFormView : public CFormView
{
	DECLARE_DYNCREATE(OverlayManagerFormView)

protected:
	OverlayManagerFormView();           // protected constructor used by dynamic creation
	virtual ~OverlayManagerFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = OverlayMangerMainView };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


