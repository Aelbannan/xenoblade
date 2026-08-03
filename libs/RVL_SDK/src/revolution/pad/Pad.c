#include <revolution/OS.h>
#include <revolution/SI.h>
#include <revolution/PAD.h>

static u32 AnalogMode = 0x00000300u;
static u32 Spec = 5;
static u32 EnabledBits;
u32 __PADSpec = 0;


void PADControlMotor(s32 chan, u32 command) {
  BOOL enabled;
  u32 chanBit;

  enabled = OSDisableInterrupts();
  chanBit = PAD_CHAN0_BIT >> chan;
  if ((EnabledBits & chanBit) && !(SIGetType(chan) & SI_GC_NOMOTOR)) {
  if (Spec < PAD_SPEC_2 && command == PAD_MOTOR_STOP_HARD) {
    command = PAD_MOTOR_STOP;
  }

  if(OS_PAD_FLAGS & 0x20){
    command = PAD_MOTOR_STOP;
  }

  SISetCommand(chan, (0x40 << 16) | AnalogMode | (command & (0x00000001 | 0x00000002)));
  SITransferCommands();
  }
  OSRestoreInterrupts(enabled);
}


BOOL __PADDisableRecalibration(BOOL disable) {
    BOOL enabled = OSDisableInterrupts();
    BOOL old = (OS_PAD_FLAGS & PAD_FLAG_NO_RECALIBRATE) ? TRUE : FALSE;

    OS_PAD_FLAGS &= ~PAD_FLAG_NO_RECALIBRATE;

    if (disable) {
        OS_PAD_FLAGS |= PAD_FLAG_NO_RECALIBRATE;
    }

    OSRestoreInterrupts(enabled);
    return old;
}
