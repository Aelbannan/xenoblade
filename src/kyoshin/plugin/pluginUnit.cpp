

void pluginUnitRegist() {
    extern const char lbl_eu_80507FC0[];
    extern const char lbl_eu_80535460[];
    extern void vmPluginRegist(const char*, const char*);
    vmPluginRegist(lbl_eu_80507FC0 + 0xb, lbl_eu_80535460);
}

void getPcHp(){}
void getPcHpRate(){}
void getEneHp(){}
void getEneHpRate(){}
void setPcBtlState(){}
void clearPcBtlState(){}
void setEneBtlState(){}
void clearEneBtlState(){}
void onPcArtsAttack(){}
void onEneArtsAttack(){}
void synchro(){}
void learnArts(){}
