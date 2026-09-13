#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage; typed KPAD callbacks)
extern "C" void (*WPADIsUsedCallbackByKPAD(void))(void);
extern "C" void WPADSetCallbackByKPAD(void (*callback)(void));
extern "C" u8 WPADGetRadioSensitivity(s32 chan);
extern "C" const float lbl_80518688;
extern "C" const float lbl_8051868C;
