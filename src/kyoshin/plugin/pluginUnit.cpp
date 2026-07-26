

extern "C" void pluginUnitRegist() {
    extern const char lbl_eu_80507FC0[];
    extern const char lbl_eu_80535460[];
    extern void vmPluginRegist(const char*, const char*);
    vmPluginRegist(lbl_eu_80507FC0 + 0xb, lbl_eu_80535460);
}

extern "C" void getPcHp() {}
extern "C" void getPcHpRate() {}
extern "C" void getEneHp() {}
extern "C" void getEneHpRate() {}
extern "C" void setPcBtlState() {}
extern "C" void clearPcBtlState() {}
extern "C" void setEneBtlState() {}
extern "C" void clearEneBtlState() {}
extern "C" void onPcArtsAttack() {}
extern "C" void onEneArtsAttack() {}
extern "C" void synchro() {}
extern "C" void learnArts() {}
