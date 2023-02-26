#include "pch.h"
#include "ExportSoundThread.h"
#include "fmod.hpp"

#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodL_vc.lib")

IMPLEMENT_DYNCREATE(CExportSoundThread, CWinThread)

using namespace FMOD;

CExportSoundThread::CExportSoundThread() {

}

CExportSoundThread::CExportSoundThread(CWnd* pParent, CString selectedPath, CString newPath, int styleIndex, int sampleRate)
	: m_pParent(pParent)
	, m_selectedPath(selectedPath)
	, m_newPath(newPath)
	, m_styleIndex(styleIndex)
	, m_sampleRate(sampleRate)
{
}

CExportSoundThread::~CExportSoundThread()
{
}


char* CStrW2CStrA(const CStringW& cstrSrcW)
{
    int len = WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cstrSrcW), -1, NULL, 0, NULL, NULL);
    char* str = new char[len];
    memset(str, 0, len);
    WideCharToMultiByte(CP_ACP, 0, LPCWSTR(cstrSrcW), -1, str, len, NULL, NULL);
    return str;
}

bool CExportSoundThread::ExportSound(const char* instr, const char* outstr, int type, int sampleRate) {
    System* system;
    Sound* sound;
    Channel* channel;
    DSP* dsp = nullptr;
    FMOD_RESULT result;
    bool isPlay = true;
    float frequency;
    result = System_Create(&system);
    if (result != FMOD_OK) return false;
    result = system->setSoftwareFormat(sampleRate, FMOD_SPEAKERMODE_MONO, 0);
    if (result != FMOD_OK) return false;
    result = system->setOutput(FMOD_OUTPUTTYPE_WAVWRITER);
    if (result != FMOD_OK) return false;
    result = system->init(32, FMOD_INIT_NORMAL | FMOD_INIT_PROFILE_ENABLE, const_cast<char*>(outstr));
    if (result != FMOD_OK) return false;
    try {
        result = system->createSound(instr, FMOD_DEFAULT, nullptr, &sound);
        if (result != FMOD_OK) return false;
        result = system->playSound(sound, nullptr, false, &channel);
        if (result != FMOD_OK) return false;
        switch (type) {
        case 0: // lolita
            system->createDSPByType(FMOD_DSP_TYPE_NORMALIZE, &dsp);
            channel->getFrequency(&frequency);
            frequency *= 1.6f;
            channel->setFrequency(frequency);
            break;
        case 1: // uncle
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 0.89f);
            channel->addDSP(0, dsp);
            break;
        case 2: // thriller
            system->createDSPByType(FMOD_DSP_TYPE_PITCHSHIFT, &dsp);
            dsp->setParameterFloat(FMOD_DSP_PITCHSHIFT_PITCH, 1.8f);
            channel->addDSP(0, dsp);
            break;
        case 3: // funny
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 50);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 60);
            channel->addDSP(0, dsp);
            break;
        case 4: // ethereal
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 300);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 20);
            channel->addDSP(0, dsp);
            break;
        case 5: // chorus
            system->createDSPByType(FMOD_DSP_TYPE_ECHO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_ECHO_DELAY, 100);
            dsp->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 50);
            channel->addDSP(0, dsp);
            break;
        case 6: // tremolo
            system->createDSPByType(FMOD_DSP_TYPE_TREMOLO, &dsp);
            dsp->setParameterFloat(FMOD_DSP_TREMOLO_SKEW, 0.8f);
            channel->addDSP(0, dsp);
            break;
        default:
            return false;
        }
    }
    catch (...) {
        sound->release();
        system->close();
        system->release();
        return false;
    }
    system->update();
    while (isPlay) {
        Sleep(500);
        channel->isPlaying(&isPlay);
    }
    sound->release();
    system->close();
    system->release();
    ExitInstance();
    return true;
}


BOOL CExportSoundThread::InitInstance()
{
	ExportSound(CStrW2CStrA(m_selectedPath), CStrW2CStrA(m_newPath), m_styleIndex, m_sampleRate);
	return TRUE;
}

int CExportSoundThread::ExitInstance()
{
	if (m_pParent->GetSafeHwnd() && m_pParent->IsWindowVisible())
	{
		m_pParent->PostMessage(WM_EXPORT_SOUND_THREAD_FINISHED);
	}
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CExportSoundThread, CWinThread)
END_MESSAGE_MAP()

