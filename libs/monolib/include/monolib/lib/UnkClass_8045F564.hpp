#pragma once

#include <types.h>

class UnkClass_8045F564{
public:
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;

    UnkClass_8045F564();
    ~UnkClass_8045F564();

    UnkClass_8045F564& operator=(const UnkClass_8045F564& rhs) {
        unk0 = rhs.unk0;
        unk4 = rhs.unk4;
        unk8 = rhs.unk8;
        unkC = rhs.unkC;
        return *this;
    }

    
    void createRegion(int, int, const char*, int);
    void deleteRegion();
    void clearFrmHeap();
    void validateHeap();
};

class Class_8045F858{
public:
    UnkClass_8045F564* unk0;

    Class_8045F858(UnkClass_8045F564* unkClass);
    ~Class_8045F858();
};
