#pragma once

#include <types.h>

class UI_CPassiveSkillCur {
public:
    UI_CPassiveSkillCur();

    // TODO: add fields
};

class UI_CPassiveSkillInfo {
public:
    UI_CPassiveSkillInfo();

    // TODO: add fields
};

class UI_CPassiveSkillLine {
public:
    UI_CPassiveSkillLine();

    // TODO: add fields
};

class UI_CPassiveSkill {
public:
    UI_CPassiveSkill();

    // TODO: add fields
    u8 _pad180[0x180];
    u8 field_180; // 0x180
};

class CPassiveSkill {
public:
    void OnFileEvent();

    // TODO: add fields
};

