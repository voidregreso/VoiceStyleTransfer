
// VoiceStyleTransferDlg.h : header file
//
#include "ExportSoundThread.h"
#include "ProgressBar.h"

#pragma once


// CVoiceStyleTransferDlg dialog
class CVoiceStyleTransferDlg : public CDialogEx
{
// Construction
public:
	CVoiceStyleTransferDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VOICESTYLETRANSFER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CMFCEditBrowseCtrl *m_EditBrowse;
	CComboBox* m_comboBox;
	CProgressDlg m_progressDlg;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg LRESULT OnExportSoundThreadFinished(WPARAM wParam, LPARAM lParam);
};
