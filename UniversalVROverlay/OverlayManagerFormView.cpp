// OverlayManagerFormView.cpp : implementation file
//


#include "OverlayManagerFormView.h"


// OverlayManagerFormView

IMPLEMENT_DYNCREATE(OverlayManagerFormView, CFormView)

OverlayManagerFormView::OverlayManagerFormView()
	: CFormView(OverlayMangerMainView)
{

}

OverlayManagerFormView::~OverlayManagerFormView()
{
}

void OverlayManagerFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(OverlayManagerFormView, CFormView)
END_MESSAGE_MAP()


// OverlayManagerFormView diagnostics

#ifdef _DEBUG
void OverlayManagerFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void OverlayManagerFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// OverlayManagerFormView message handlers
