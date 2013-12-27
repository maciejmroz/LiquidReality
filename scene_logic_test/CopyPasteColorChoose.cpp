// CopyPasteColorChoose.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "CopyPasteColorChoose.h"


// CCopyPasteColorChoose dialog

IMPLEMENT_DYNAMIC(CCopyPasteColorChoose, CDialog)
CCopyPasteColorChoose::CCopyPasteColorChoose(CWnd* pParent /*=NULL*/)
	: CDialog(CCopyPasteColorChoose::IDD, pParent)
{
}

CCopyPasteColorChoose::~CCopyPasteColorChoose()
{
}

void CCopyPasteColorChoose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCopyPasteColorChoose, CDialog)
END_MESSAGE_MAP()


// CCopyPasteColorChoose message handlers
