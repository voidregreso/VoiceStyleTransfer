
// VoiceStyleTransferDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "VoiceStyleTransfer.h"
#include "VoiceStyleTransferDlg.h"
#include "afxdialogex.h"

// CVoiceStyleTransferDlg dialog


CVoiceStyleTransferDlg::CVoiceStyleTransferDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VOICESTYLETRANSFER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVoiceStyleTransferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVoiceStyleTransferDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CVoiceStyleTransferDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_EXPORT_SOUND_THREAD_FINISHED, &CVoiceStyleTransferDlg::OnExportSoundThreadFinished)
END_MESSAGE_MAP()


// CVoiceStyleTransferDlg message handlers

BOOL CVoiceStyleTransferDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	m_EditBrowse = (CMFCEditBrowseCtrl*)GetDlgItem(IDC_BROWSEWAV);
	m_EditBrowse->EnableFileBrowseButton(_T(""), _T("Wave Files(*.wav)|*.wav|All Files (*.*)|*.*||"));
	m_comboBox = (CComboBox*)GetDlgItem(IDC_COMBOSTYLE);
	m_comboBox->InsertString(0, L"Lolita");
	m_comboBox->InsertString(1, L"Uncle");
	m_comboBox->InsertString(2, L"Thriller");
	m_comboBox->InsertString(3, L"Funny");
	m_comboBox->InsertString(4, L"Ethereal");
	m_comboBox->InsertString(5, L"Chorus");
	m_comboBox->InsertString(6, L"Tremolo");
	m_comboBox->SetCurSel(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVoiceStyleTransferDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVoiceStyleTransferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

typedef struct {
	char chunkID[4]; // Identificador del bloque, debería ser "RIFF"
	DWORD chunkSize; // Tamaño del archivo
	char format[4]; // Tipo de formato, debería ser "WAVE"
	char subChunk1ID[4]; // Identificador del sub-bloque 1, debería ser "fmt"
	DWORD subChunk1Size; // Tamaño del sub-bloque 1
	WORD audioFormat; // Formato de audio, por ejemplo PCM es 1
	WORD numChannels; // Número de canales, por ejemplo 1 para mono, 2 para estéreo
	DWORD sampleRate; // Frecuencia de muestreo, por ejemplo 44100Hz
	DWORD byteRate; // Tasa de bits, cantidad de datos por segundo
	WORD blockAlign; // Alineación de bloques de datos
	WORD bitsPerSample; // Ancho de bits, por ejemplo 16 bits
	char subChunk2ID[4]; // Identificador del sub-bloque 2, debería ser "data"
	DWORD subChunk2Size; // Tamaño del sub-bloque 2
} WAVE_HEADER;


void CVoiceStyleTransferDlg::OnBnClickedButton1()
{
	CString selectedPath, newPath;
	GetDlgItemText(IDC_BROWSEWAV, selectedPath);
	newPath = selectedPath + L"_output.wav";
	CFile file;
	int sampleRate = 16000;
	if (file.Open(selectedPath, CFile::modeRead)) {
		WAVE_HEADER header;
		file.Read(&header, sizeof(header));
		file.Close();
		if (header.numChannels != 1) {
			AfxMessageBox(L"Only mono channel is supported!");
			return;
		}
		sampleRate = header.sampleRate;
	}
	CExportSoundThread* pThread = new CExportSoundThread(this, selectedPath, newPath, m_comboBox->GetCurSel(), sampleRate);
	pThread->CreateThread();
	m_progressDlg.Create(IDD_PROGRESS_DIALOG, this);
	m_progressDlg.SetTitle(L"Working");
	m_progressDlg.SetProgress(50);
	m_progressDlg.ShowWindow(SW_SHOW);
}

LRESULT CVoiceStyleTransferDlg::OnExportSoundThreadFinished(WPARAM wParam, LPARAM lParam)
{
	m_progressDlg.DestroyWindow();
	AfxMessageBox(L"Completed!");
	return 0;
}
