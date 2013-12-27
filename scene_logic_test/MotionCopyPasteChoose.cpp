// MotionCopyPasteChoose.cpp : implementation file
//

#include "stdafx.h"
#include "scene_logic_test.h"
#include "MotionCopyPasteChoose.h"


// CMotionCopyPasteChoose dialog

IMPLEMENT_DYNAMIC(CMotionCopyPasteChoose, CDialog)
CMotionCopyPasteChoose::CMotionCopyPasteChoose(CWnd* pParent /*=NULL*/)
	: CDialog(CMotionCopyPasteChoose::IDD, pParent)
{
}

CMotionCopyPasteChoose::~CMotionCopyPasteChoose()
{
}

void CMotionCopyPasteChoose::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMotionCopyPasteChoose, CDialog)
END_MESSAGE_MAP()


// CMotionCopyPasteChoose message handlers
