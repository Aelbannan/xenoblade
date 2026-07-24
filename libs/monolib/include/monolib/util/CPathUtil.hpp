#pragma once

#include <types.h>
#include "monolib/util/FixStr.hpp"

namespace ml{

    /// Utility class for path and filename string manipulation.
    class CPathUtil {
    public:
        /// Returns a pointer to the filename portion (past the last path separator) of the given path.
        static const char* getFilePtrFromPath(const char* pPath);

        /// Returns a pointer to the file extension portion (past the last '.') of the given filename.
        static const char* getFileExtPtr(const char* pFilename);

        /// Strips the extension from the filename in the given path and copies the result to outStr.
        static void getNoPathExtName(FixStr<64>& outStr, const char* pPath);

        /// Converts an integer to a left-padded zero-digit string, stored in outStr.
        static void itoa(FixStr<16>& outStr, int num, int digits);

        /// Removes the file extension from a fixed string in-place.
        static inline void removeExt(FixStr<32>& str){
            int length = str.rfind(".", -1);

            if((u32)length + 1 > 1){
                str = str.substr(0, length);
            }
        }
    };

}
