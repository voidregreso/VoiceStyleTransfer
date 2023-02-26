#pragma once

#define WM_EXPORT_SOUND_THREAD_FINISHED 0x3f8c

class CExportSoundThread : public CWinThread
{
	DECLARE_DYNCREATE(CExportSoundThread)

public:
	CExportSoundThread();
	CExportSoundThread(CWnd* pParent, CString selectedPath, CString newPath, int styleIndex, int sampleRate);
	virtual ~CExportSoundThread();

	virtual BOOL InitInstance();
	virtual int ExitInstance();
	bool ExportSound(const char* instr, const char* outstr, int type, int sampleRate);

protected:
	DECLARE_MESSAGE_MAP()

private:
	CWnd* m_pParent;
	CString m_selectedPath;
	CString m_newPath;
	int m_styleIndex;
	int m_sampleRate;
};