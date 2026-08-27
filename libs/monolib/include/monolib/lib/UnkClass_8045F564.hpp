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

    
    void createRegion(int, int, const char*, int);
    void deleteRegion();
    void clearFrmHeap();
    void validateHeap();
    // Compat aliases for external callers not yet renamed
    void func_8045F778() { deleteRegion(); }
    void func_8045F7E8() { clearFrmHeap(); }
    void func_8045F810() { validateHeap(); }
};

class Class_8045F858{
public:
    UnkClass_8045F564* unk0;

    Class_8045F858(UnkClass_8045F564* unkClass);
    ~Class_8045F858();
};
