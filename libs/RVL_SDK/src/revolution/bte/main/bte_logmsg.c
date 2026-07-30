// Decompiled bte_logmsg — RVL_SDK BTE logging
// High-level C reconstruction. No inline asm, no register tricks.

#include <types.h>
#include <stdio.h>
#include <stdarg.h>

extern volatile u8 bte_target_mode;

/* Format string for adding newline to LogMsg output. Name matches retail. */
static char lbl_80665908[8] = "%s\n";
extern void OSReport(const char *msg, ...);

void LogMsg(u32 trace_set_mask, const char *fmt_str, ...)
{
        static char tmp[0x7D0];
    va_list vl;
    va_start(vl, fmt_str);
    vsprintf(tmp, fmt_str, vl);
    va_end(vl);
    sprintf(tmp, lbl_80665908, tmp);
    OSReport(tmp);
}

void LogMsg_0(u32 trace_set_mask, const char *p_str)
{
    if (bte_target_mode != 0)
        return;
    LogMsg(trace_set_mask, p_str);
}

void LogMsg_1(u32 trace_set_mask, const char *fmt_str, u32 p1)
{
    if (bte_target_mode != 0)
        return;
    LogMsg(trace_set_mask, fmt_str, p1);
}

void LogMsg_2(u32 trace_set_mask, const char *fmt_str, u32 p1, u32 p2)
{
    if (bte_target_mode != 0)
        return;
    LogMsg(trace_set_mask, fmt_str, p1, p2);
}

void LogMsg_3(u32 trace_set_mask, const char *fmt_str, u32 p1, u32 p2,
              u32 p3)
{
    if (bte_target_mode != 0)
        return;
    LogMsg(trace_set_mask, fmt_str, p1, p2, p3);
}

void LogMsg_4(u32 trace_set_mask, const char *fmt_str, u32 p1, u32 p2,
              u32 p3, u32 p4)
{
    if (bte_target_mode != 0)
        return;
    LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4);
}

void LogMsg_5(u32 trace_set_mask, const char *fmt_str, u32 p1, u32 p2,
              u32 p3, u32 p4, u32 p5)
{
    if (bte_target_mode != 0)
        return;
    LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4, p5);
}

void LogMsg_6(u32 trace_set_mask, const char *fmt_str, u32 p1, u32 p2,
              u32 p3, u32 p4, u32 p5, u32 p6)
{
    if (bte_target_mode != 0)
        return;
    LogMsg(trace_set_mask, fmt_str, p1, p2, p3, p4, p5, p6);
}
