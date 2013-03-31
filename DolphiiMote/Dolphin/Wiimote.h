
#ifndef _WIIMOTE_H_
#define _WIIMOTE_H_

#include "ChunkFile.h"

#define MAX_WIIMOTES	4

#define WIIMOTE_INI_NAME	"WiimoteNew"

enum
{
	WIIMOTE_SRC_NONE = 0,
	WIIMOTE_SRC_EMU = 1,
	WIIMOTE_SRC_REAL = 2,
	WIIMOTE_SRC_HYBRID = 3,	// emu + real
};

extern unsigned int g_wiimote_sources[MAX_WIIMOTES];

namespace Wiimote
{

void Shutdown();
void Initialize(void* const hwnd);

unsigned int GetAttached();
void DoState(u8 **ptr, PointerWrap::Mode mode);
void EmuStateChange(EMUSTATE_CHANGE newState);

void ControlChannel(int _number, u16 _channelID, const void* _pData, u32 _Size);
void InterruptChannel(int _number, u16 _channelID, const void* _pData, u32 _Size);
void Update(int _number);

}

namespace WiimoteReal
{

void Initialize();
void Shutdown();
void Refresh();

void LoadSettings();

}

#endif
