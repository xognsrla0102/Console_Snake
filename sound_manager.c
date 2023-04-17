#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdio.h>
#include <Windows.h>

#include <Digitalv.h>
#include <mmsystem.h>

#include "sound_manager.h"

void load_bgm(const bgm_type_t bgm_type);
void load_sfx(const sfx_type_t sfx_type);

typedef struct {
	MCI_OPEN_PARMS open_parms;
	MCI_PLAY_PARMS play_parms;
	MCI_DGV_SETAUDIO_PARMS audio_parms;
	LPMCI_GENERIC_PARMS generic_parms;
} mci_parms_t;

static mci_parms_t s_bgm_sources[BGM_NUMS];
static mci_parms_t s_sfx_sources[SFX_NUMS];

static const char* bgm_names[BGM_NUMS] = {
	NULL,
	"main",
	"game_over",
};

static const char* sfx_names[SFX_NUMS] = {
	"start",
	"select",
	"get_item",
	"collide",
	"power_up",
};

static const char* DEVICE_TYPE_STR = "mpegvideo";

static bgm_type_t now_play_bgm;

void init_sound_manager(void)
{
	size_t i;
	for (i = 0; i < BGM_NUMS; ++i)
	{
		load_bgm(i);
	}
	for (i = 0; i < SFX_NUMS; ++i)
	{
		load_sfx(i);
	}
}

void release_sound_manager(void)
{
	size_t i;
	for (i = 0; i < BGM_NUMS; ++i)
	{
		assert(s_bgm_sources[i].open_parms.wDeviceID != 0);
		mciSendCommandA(s_bgm_sources[i].open_parms.wDeviceID, MCI_CLOSE, 0, (DWORD)(LPVOID)NULL);
	}
}

static void load_bgm(const bgm_type_t bgm_type)
{
	char bgm_str[256];
	sprintf(bgm_str, "./music/bgm/%s.mp3", bgm_names[bgm_type]);
	bgm_str[255] = '\0';

	s_bgm_sources[bgm_type].open_parms.lpstrDeviceType = DEVICE_TYPE_STR;
	s_bgm_sources[bgm_type].open_parms.lpstrElementName = bgm_str;
	mciSendCommandA(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&s_bgm_sources[bgm_type].open_parms);

	s_bgm_sources[bgm_type].audio_parms.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	s_bgm_sources[bgm_type].audio_parms.dwValue = 300;
	s_bgm_sources[bgm_type].audio_parms.dwOver = 0;
	s_bgm_sources[bgm_type].audio_parms.lpstrAlgorithm = NULL;
	s_bgm_sources[bgm_type].audio_parms.lpstrQuality = NULL;
	mciSendCommandA(s_bgm_sources[bgm_type].open_parms.wDeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&s_bgm_sources[bgm_type].audio_parms);
}

static void load_sfx(const sfx_type_t sfx_type)
{
	char sfx_str[256];
	sprintf(sfx_str, "./music/sfx/%s.mp3", sfx_names[sfx_type]);
	sfx_str[255] = '\0';

	s_sfx_sources[sfx_type].open_parms.lpstrDeviceType = DEVICE_TYPE_STR;
	s_sfx_sources[sfx_type].open_parms.lpstrElementName = sfx_str;
	mciSendCommandA(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE, (DWORD)(LPVOID)&s_sfx_sources[sfx_type].open_parms);

	s_sfx_sources[sfx_type].audio_parms.dwItem = MCI_DGV_SETAUDIO_VOLUME;
	s_sfx_sources[sfx_type].audio_parms.dwValue = 400;
	s_sfx_sources[sfx_type].audio_parms.dwOver = 0;
	s_sfx_sources[sfx_type].audio_parms.lpstrAlgorithm = NULL;
	s_sfx_sources[sfx_type].audio_parms.lpstrQuality = NULL;
	mciSendCommandA(s_sfx_sources[sfx_type].open_parms.wDeviceID, MCI_SETAUDIO, MCI_DGV_SETAUDIO_VALUE | MCI_DGV_SETAUDIO_ITEM, (DWORD)(LPVOID)&s_sfx_sources[sfx_type].audio_parms);
}

void play_bgm(const bgm_type_t bgm_type, const BOOL is_repeat)
{
	assert(s_bgm_sources[bgm_type].open_parms.wDeviceID != 0);

	now_play_bgm = bgm_type;

	mciSendCommandA(s_bgm_sources[bgm_type].open_parms.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommandA(s_bgm_sources[bgm_type].open_parms.wDeviceID, MCI_PLAY, is_repeat ? MCI_DGV_PLAY_REPEAT : 0, (DWORD)(LPVOID)&s_bgm_sources[bgm_type].play_parms);
}

void stop_bgm(void)
{
	assert(s_bgm_sources[now_play_bgm].open_parms.wDeviceID != 0);
	// 이상하게 콘솔에선 PAUSE가 STOP처럼 돎. STOP 사용하면 멈추긴 하지만 갑자기 개 느려짐
	mciSendCommandA(s_bgm_sources[now_play_bgm].open_parms.wDeviceID, MCI_PAUSE, 0, (DWORD)(LPVOID)NULL);

	now_play_bgm = BGM_NONE;
}

void play_sfx(const sfx_type_t sfx_type)
{
	assert(s_sfx_sources[sfx_type].open_parms.wDeviceID != 0);

	mciSendCommandA(s_sfx_sources[sfx_type].open_parms.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START, (DWORD)(LPVOID)NULL);
	mciSendCommandA(s_sfx_sources[sfx_type].open_parms.wDeviceID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&s_sfx_sources[sfx_type].play_parms);
}
