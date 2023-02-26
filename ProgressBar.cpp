#include "pch.h"
#include "ProgressBar.h"

CProgressDlg::CProgressDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PROGRESS_DIALOG, pParent)
	, m_nProgress(0)
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
END_MESSAGE_MAP()


void CProgressDlg::SetProgress(int nProgress)
{
	m_nProgress = nProgress;
	if (GetSafeHwnd())
	{
		CProgressCtrl* pProgressCtrl = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS1);
		pProgressCtrl->SetPos(m_nProgress);
		CString strText;
		strText.Format(_T("%d%%"), m_nProgress);
		GetDlgItem(IDC_PROGRESS_TEXT)->SetWindowText(strText);
	}
}

void CProgressDlg::SetTitle(LPCTSTR lpszTitle) {
	SetWindowText(lpszTitle);
}