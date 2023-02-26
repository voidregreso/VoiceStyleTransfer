#pragma once

#include "resource.h"

class CProgressDlg : public CDialogEx
{
public:
	CProgressDlg(CWnd* pParent = nullptr);   // standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PROGRESS_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	int m_nProgress;

public:
	void SetTitle(LPCTSTR lpszTitle);
	void SetProgress(int nProgress);
};
