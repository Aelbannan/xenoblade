#include "monolib/util.hpp"

// Retail .rodata string owned by this TU (monolibdata1 blob dissolve):
//   lbl_eu_80522458 (.rodata 0x2) = "."
// getNoPathExtName's rfind needle and itoa's empty string (retail references
// the pooled "" at lbl_eu_80522458 + 2). The sdata2_threshold 0 pragma keeps
// the 2-byte const string in .rodata — MWCC would otherwise route a <=8B
// const into .sdata2, and consumers address it via lis/@l (matching retail).
#ifdef __MWERKS__
#pragma push
#pragma sdata2_threshold 0
#endif
extern "C" const char lbl_eu_80522458[] = ".";
#ifdef __MWERKS__
#pragma pop
#endif

namespace ml{

    //Returns a pointer to the filename portion of the given path.
    const char* CPathUtil::getFilePtrFromPath(const char* pPath){
        int endIndex = std::strlen(pPath) - 1;

        for(int i = endIndex; i >= 0; i--) {
            char c = pPath[i];

            /* If the current character is a path separator character, return the rest
            of the string */
            if(c == '/' || c == '\\' || c == ':'){
                return pPath + (endIndex + 1);
            }

            endIndex--;
        }

        //If no path separator characters were found, just return the string
        return pPath;
    }

    //Returns a pointer to the file extension portion of the given filename.
    const char* CPathUtil::getFileExtPtr(const char* pFilename){
        int endIndex = std::strlen(pFilename) - 1;

        for(int i = endIndex; i >= 0; i--) {
            char c = pFilename[i];
            if(c == '.') return pFilename + (endIndex + 1);
            endIndex--;
        }

        //If no '.' was found, return null
        return nullptr;
    }

    /* Gets the file name w/o the extension from the given path, and copies it to the
    given fixed string. */
    void CPathUtil::getNoPathExtName(FixStr<64>& outStr, const char* pPath){
        FixStr<64> temp;

        const char* pFilename = getFilePtrFromPath(pPath);

        if (pFilename == nullptr) {
            outStr = temp;
        }else{
            temp = pFilename;
            // The local keeps MWCC's eager lis/@l address materialisation in a
            // saved register (retail hoists the needle load above the length
            // branch); referencing the extern inline merges the LO into the
            // first use and shifts the load into the branch.
            const char* pNeedle = lbl_eu_80522458;
            int length = temp.rfind(pNeedle, -1);

            if ((u32)length + 1 <= 1) {
                outStr = temp;
            } else {
                outStr.copy(temp, 0, length);
            }
        }
    }

    void CPathUtil::itoa(FixStr<16>& outStr, int num, int digits) {
        // Retail empty-string reference: the pooled "" sits at lbl_eu_80522458+2
        // ("\0" right after the 2-byte "." string). A local keeps MWCC's
        // lis/@l base + separate +2 addi materialization (retail shape).
        const char* empty = &lbl_eu_80522458[2];
        outStr = empty;

        for(int i = 0; i < digits; i++) {
            char buffer[2] = {0,0};
            int factor = 1;

            //Calculate the current factor
            for(int j = 0; j < i; j++){
                factor *= 10;
            }

            //Extract the current digit, and convert it to a character
            int digit = (num % (factor * 10)) / factor;
            buffer[0] = (char)(digit + (int)'0');
            //Prepend the character to the output string
            FixStr<16> string1 = buffer;
            outStr = string1 + outStr;
        }
    }

} //namespace ml
