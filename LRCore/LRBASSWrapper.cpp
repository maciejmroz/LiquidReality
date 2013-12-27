
#include "stdafx.h"

using namespace LR;

LRBASSWrapper::LRBASSWrapper() :
	m_hBASS(NULL),
	m_pErrorGetCode(NULL),
	m_pInit(NULL),
	m_pFree(NULL),
	m_pStart(NULL),
	m_pSetVolume(NULL),
	m_pMusicLoad(NULL),
	m_pMusicFree(NULL),
	m_pMusicPlayEx(NULL),
	m_pStreamCreateFile(NULL),
	m_pStreamFree(NULL),
	m_pStreamPlay(NULL),
	m_pChannelSeconds2Bytes(NULL),
	m_pChannelStop(NULL),
	m_pChannelPause(NULL),
	m_pChannelResume(NULL),
	m_pChannelSetPosition(NULL),
	m_pChannelGetLevel(NULL),
	m_pChannelGetData(NULL),
	m_bLoaded(NULL),
	m_bInitialized(NULL),
	m_bPlaying(NULL),
	m_bPaused(NULL),
	m_dwMusicHandle(0),
	m_pModuleData(NULL),
	m_dwFrameHash(0xffffffff)
{
}

LRBASSWrapper::~LRBASSWrapper() {
	if(m_bInitialized) {
		cleanup();
	}
}

void LRBASSWrapper::setFrameHash(DWORD dwHash) {
	m_dwFrameHash=dwHash;
}

void LRBASSWrapper::cleanup() {
	if(!m_bInitialized) {
		return;
	}
	m_pFree();
	::FreeLibrary(m_hBASS);
	delete [] m_pModuleData;
	m_bLoaded=false;
	m_bInitialized=false;
}

void LRBASSWrapper::getFFT1024Samples(float **ppfFFT) {
	//TODO
}

float LRBASSWrapper::getLevel() {
	//TODO
	return 0.0f;
}

#define BASS_WRAPPER_GET_FUNC(func_var,func_name) {\
	void **tmp_void=(void**)&func_var;\
	*tmp_void=::GetProcAddress(m_hBASS,func_name);\
	if(!func_var) {\
		char szMsg[128];\
		sprintf(szMsg,"LRBASSWrapper (ERROR): failed GetProcAddress on function %s\n",func_name);\
		DEBUG_STRING(szMsg);\
		return false;\
	}\
}

bool LRBASSWrapper::init(HWND hwnd) {
	if(m_bInitialized) {
		return true;
	}
	m_hBASS=::LoadLibrary("bass.dll");
	if(!m_hBASS) {
		DEBUG_STRING("LRBASSWrapper (ERROR): LoadLibrary failed\n");
		return false;
	}
	BASS_WRAPPER_GET_FUNC(m_pErrorGetCode,"BASS_ErrorGetCode");
	BASS_WRAPPER_GET_FUNC(m_pInit,"BASS_Init");
	BASS_WRAPPER_GET_FUNC(m_pFree,"BASS_Free");
	BASS_WRAPPER_GET_FUNC(m_pStart,"BASS_Start");
	BASS_WRAPPER_GET_FUNC(m_pSetVolume,"BASS_SetVolume");
	BASS_WRAPPER_GET_FUNC(m_pMusicLoad,"BASS_MusicLoad");
	BASS_WRAPPER_GET_FUNC(m_pMusicFree,"BASS_MusicFree");
	BASS_WRAPPER_GET_FUNC(m_pMusicPlayEx,"BASS_MusicPlayEx");
	BASS_WRAPPER_GET_FUNC(m_pStreamCreateFile,"BASS_StreamCreateFile");
	BASS_WRAPPER_GET_FUNC(m_pStreamFree,"BASS_StreamFree");
	BASS_WRAPPER_GET_FUNC(m_pStreamPlay,"BASS_StreamPlay");
	BASS_WRAPPER_GET_FUNC(m_pChannelSeconds2Bytes,"BASS_ChannelSeconds2Bytes");
	BASS_WRAPPER_GET_FUNC(m_pChannelStop,"BASS_ChannelStop");
	BASS_WRAPPER_GET_FUNC(m_pChannelPause,"BASS_ChannelPause");
	BASS_WRAPPER_GET_FUNC(m_pChannelResume,"BASS_ChannelResume");
	BASS_WRAPPER_GET_FUNC(m_pChannelSetPosition,"BASS_ChannelSetPosition");
	BASS_WRAPPER_GET_FUNC(m_pChannelGetLevel,"BASS_ChannelGetLevel");
	BASS_WRAPPER_GET_FUNC(m_pChannelGetData,"BASS_ChannelGetData");
	if(!m_pInit(1,44100,0,hwnd,NULL)) {
		::FreeLibrary(m_hBASS);
		DEBUG_STRING("LRBASSWrapper (ERROR): Init failed\n");
		return false;
	}
	m_pStart();
	m_bInitialized=true;
	return true;
}

void LRBASSWrapper::load(const char *pszFilename,AutoPtr<fs> &pfs) {
	if(!m_bInitialized) {
		DEBUG_STRING("LRBASSWrapper (ERROR): Not initialized\n");
		return;
	}
	if(m_bLoaded) {
		if(m_eType==BMT_STREAM) {
			m_pStreamFree(m_dwMusicHandle);
		}
		else {
			m_pMusicFree(m_dwMusicHandle);
		}
		m_dwMusicHandle=0;
		m_bLoaded=false;
	}
	//unsigned char *module_data=NULL;
	ULONG tmp_file_handle=0;
	int tmp_file_size=0;
	try {
		pfs->openFile(pszFilename,tmp_file_handle);
		tmp_file_size=pfs->getFileSize(tmp_file_handle);
		delete [] m_pModuleData;
		m_pModuleData=new unsigned char[tmp_file_size];
		pfs->fread(tmp_file_handle,tmp_file_size,m_pModuleData);
		pfs->closeFile(tmp_file_handle);
	}
	catch(IOException&) {
		delete [] m_pModuleData;
		m_pModuleData=NULL;
		pfs->closeFile(tmp_file_handle);
		throw;
	}
	if(strstr(pszFilename,".mp3")) {
		m_dwMusicHandle=m_pStreamCreateFile(TRUE,m_pModuleData,0,
			tmp_file_size,BASS_MP3_SETPOS);
		m_eType=BMT_STREAM;
	}
	else {
		m_dwMusicHandle=m_pMusicLoad(TRUE,m_pModuleData,0,
			tmp_file_size,BASS_MUSIC_CALCLEN,0);
		m_eType=BMT_MODULE;
		delete [] m_pModuleData;
		m_pModuleData=NULL;
	}
	m_bLoaded=true;
	m_bPlaying=false;
	m_bPaused=false;
}

void LRBASSWrapper::pause() {
	if(!m_bInitialized) {
		DEBUG_STRING("LRBASSWrapper (ERROR): Not initialized\n");
		return;
	}
	if(!m_bLoaded) {
		DEBUG_STRING("LRBASSWrapper (ERROR): Music not loaded\n");
		return;
	}
	if(m_bPlaying && (!m_bPaused)) {
		m_pChannelPause(m_dwMusicHandle);
		m_bPaused=true;
	}
}

void LRBASSWrapper::play(DWORD dwOffsetMilliseconds) {
	if(!m_bInitialized) {
		DEBUG_STRING("LRBASSWrapper (ERROR): Not initialized\n");
		return;
	}
	if(!m_bLoaded) {
		DEBUG_STRING("LRBASSWrapper (ERROR): Music not loaded\n");
		return;
	}
	if(m_bPaused) {
		m_pChannelResume(m_dwMusicHandle);
		m_bPaused=false;
		return;
	}
	if(m_eType==BMT_STREAM) {
		m_pStreamPlay(m_dwMusicHandle,TRUE,0);
		setPositionMiliSeconds(dwOffsetMilliseconds);
	}
	else {
		if(!m_pMusicPlayEx(m_dwMusicHandle,(0xffff<<16)+dwOffsetMilliseconds/1000,BASS_MUSIC_POSRESET,TRUE))
		{
			DEBUG_STRING("LRBASSWrapper (ERROR): MusicPlay call failed\n");
			return;
		}
	}
	m_bPlaying=true;
	m_bPaused=false;
}

void LRBASSWrapper::setPositionMiliSeconds(DWORD dwMiliSeconds) {
	if(!m_bInitialized) {
		DEBUG_STRING("LRBASSWrapper (ERROR): Not initialized\n");
		return;
	}
	if(!m_bLoaded) {
		DEBUG_STRING("LRBASSWrapper (ERROR): Music not loaded\n");
		return;
	}
	if(m_eType==BMT_STREAM) {
		QWORD byte_pos=m_pChannelSeconds2Bytes(m_dwMusicHandle,dwMiliSeconds*0.001f);
		m_pChannelSetPosition(m_dwMusicHandle,byte_pos);
	}
	else {
		m_pChannelSetPosition(m_dwMusicHandle,MAKELONG(dwMiliSeconds/1000,0xffff));
	}
}

void LRBASSWrapper::stop() {
	if(!m_bInitialized) {
		DEBUG_STRING("LRBASSWrapper (ERROR): Not initialized\n");
		return;
	}
	if(!m_bLoaded) {
		return;
	}
	if(m_bPlaying) {
		setPositionMiliSeconds(0);
		m_pChannelStop(m_dwMusicHandle);
		m_bPlaying=false;
		m_bPaused=false;
	}
}
