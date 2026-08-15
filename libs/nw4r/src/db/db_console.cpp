#include <nw4r/db/db_console.h>
#include <revolution/OS.h>

// The retail db_console split holds ONLY Console_Printf and
// Console_GetTotalLines (0xD4 of .text, no data). The remaining console API
// (Console_Create/Draw/VFPrintf/...) lives in other retail TUs, so the stub
// bodies + their statics (sStrBuf[1024], sMutex) must not be emitted here.

namespace nw4r
{
    namespace db
    {
        using namespace detail;

        static u16 GetRingUsedLines_(ConsoleHead* console);

        static u16 GetActiveLines_(ConsoleHead* console)
        {
            u16 lines = GetRingUsedLines_(console);
            if (console->printXPos != 0) lines++;
            return lines;
        }

        static u16 GetRingUsedLines_(ConsoleHead* console)
        {
            long lines = console->printTop - console->ringTop;
            if (lines < 0) lines += console->height;
            return (u16)lines;
        }

        void Console_Printf(ConsoleHead* console, const char* format, ...)
        {
            va_list vlist;
            va_start(vlist, format);
        }

        long Console_GetTotalLines(ConsoleHead* console)
        {
            BOOL enabled = OSDisableInterrupts();

            long count = GetActiveLines_(console) + console->ringTopLineCnt;

            OSRestoreInterrupts(enabled);

            return count;
        }
    }
}
