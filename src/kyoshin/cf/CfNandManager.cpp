extern "C" void dummy() {}

#pragma section data_type ".data" ".data"

// --- typified .data (layout from build/us/asm/kyoshin/cf/CfNandManager.s) ---
// Slots carrying relocs emit zero bytes + ADDR32 relocs in the .o, matching
// retail (the gate compares bytes; reloc names are ignored once bytes match).
// Slot targets owned by other TUs are extern (UNDEF here, resolved at link
// via the global symbol map). The four CTTask<cf::CfNandManager> template
// slots spell '<' and are not expressible in C++; they stay zero with the
// retail target in comments. The 0x90 switch jumptable targets local case
// labels of func_8023EABC (not in this stub TU), so it stays a named zero
// table. The trailing zero word is split off so MWCC's 8-alignment of
// size%8==0 .data objects cannot pad the section (retail packs at 4).
extern "C" {
void __dt__Q22cf13CfNandManagerFv();
void Reset__14CChildListNodeFv();
void Init__Q22cf13CfNandManagerFv();
void Term__Q22cf13CfNandManagerFv();
void Move__Q22cf13CfNandManagerFv();
void Tail__8CProcessFv();
void destroyViaWorkEvent__Q22cf13CfNandManagerFv();
void WorkEvent1__10IWorkEventFPvPCc();
void OnFileEvent__Q22cf13CfNandManagerFv();
void WorkEvent3__10IWorkEventFPv();
void WorkEvent4__10IWorkEventFv();
void OnPauseTrigger__10IWorkEventFb();
void WorkEvent6__10IWorkEventFv();
void WorkEvent7__10IWorkEventFv();
void WorkEvent8__10IWorkEventFv();
void WorkEvent9__10IWorkEventFv();
void WorkEvent10__10IWorkEventFv();
void WorkEvent11__10IWorkEventFv();
void WorkEvent12__10IWorkEventFv();
void WorkEvent13__10IWorkEventFv();
void WorkEvent14__10IWorkEventFv();
void WorkEvent15__10IWorkEventFv();
void WorkEvent16__10IWorkEventFv();
void WorkEvent17__10IWorkEventFv();
void WorkEvent18__10IWorkEventFv();
void WorkEvent19__10IWorkEventFv();
void WorkEvent20__10IWorkEventFv();
void WorkEvent21__10IWorkEventFv();
void WorkEvent22__10IWorkEventFv();
void WorkEvent23__10IWorkEventFv();
void WorkEvent24__10IWorkEventFv();
void WorkEvent25__10IWorkEventFv();
void WorkEvent26__10IWorkEventFv();
void WorkEvent27__10IWorkEventFv();
void WorkEvent28__10IWorkEventFv();
void WorkEvent29__10IWorkEventFv();
void WorkEvent30__10IWorkEventFv();
void WorkEvent31__10IWorkEventFv();
void destroyViaRender__Q22cf13CfNandManagerFv();
void handleRenderCallback__Q22cf13CfNandManagerFv();
void handleFileEvent__Q22cf13CfNandManagerFv();
void cbRenderBefore__Q22cf13CfNandManagerFv();
}
extern char lbl_eu_80661940[];
extern char lbl_eu_80661948[];
extern char lbl_eu_80661950[];
extern char lbl_eu_80661958[];
extern char __RTTI__10IWorkEvent[];
// this TU's own .sdata pair tables (defined below; vtable slots point at them)
extern const void* lbl_eu_806628C8[2];
extern const void* lbl_eu_806628D0[2];
// fwd decls so the .sdata initializers below see the tables (defs inherit
// external linkage, matching retail's global .obj bindings)
extern const void* lbl_eu_80536BBC[49];
extern const void* lbl_eu_80536C80[13];
extern const void* lbl_eu_80536CB4[9];
extern const void* lbl_eu_80536CD8[7];

__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char jumptable_eu_80536B2C[0x90] = {0};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536BBC[49] = {
    lbl_eu_806628C8, 0,
    (const void*)__dt__Q22cf13CfNandManagerFv,
    (const void*)Reset__14CChildListNodeFv,
    (const void*)Init__Q22cf13CfNandManagerFv,
    (const void*)Term__Q22cf13CfNandManagerFv,
    (const void*)Move__Q22cf13CfNandManagerFv,
    0, // retail Draw__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    (const void*)Tail__8CProcessFv,
    lbl_eu_806628C8, (const void*)0xFFFFFFAC,
    (const void*)destroyViaWorkEvent__Q22cf13CfNandManagerFv,
    (const void*)WorkEvent1__10IWorkEventFPvPCc,
    (const void*)OnFileEvent__Q22cf13CfNandManagerFv,
    (const void*)WorkEvent3__10IWorkEventFPv,
    (const void*)WorkEvent4__10IWorkEventFv,
    (const void*)OnPauseTrigger__10IWorkEventFb,
    (const void*)WorkEvent6__10IWorkEventFv,
    (const void*)WorkEvent7__10IWorkEventFv,
    (const void*)WorkEvent8__10IWorkEventFv,
    (const void*)WorkEvent9__10IWorkEventFv,
    (const void*)WorkEvent10__10IWorkEventFv,
    (const void*)WorkEvent11__10IWorkEventFv,
    (const void*)WorkEvent12__10IWorkEventFv,
    (const void*)WorkEvent13__10IWorkEventFv,
    (const void*)WorkEvent14__10IWorkEventFv,
    (const void*)WorkEvent15__10IWorkEventFv,
    (const void*)WorkEvent16__10IWorkEventFv,
    (const void*)WorkEvent17__10IWorkEventFv,
    (const void*)WorkEvent18__10IWorkEventFv,
    (const void*)WorkEvent19__10IWorkEventFv,
    (const void*)WorkEvent20__10IWorkEventFv,
    (const void*)WorkEvent21__10IWorkEventFv,
    (const void*)WorkEvent22__10IWorkEventFv,
    (const void*)WorkEvent23__10IWorkEventFv,
    (const void*)WorkEvent24__10IWorkEventFv,
    (const void*)WorkEvent25__10IWorkEventFv,
    (const void*)WorkEvent26__10IWorkEventFv,
    (const void*)WorkEvent27__10IWorkEventFv,
    (const void*)WorkEvent28__10IWorkEventFv,
    (const void*)WorkEvent29__10IWorkEventFv,
    (const void*)WorkEvent30__10IWorkEventFv,
    (const void*)WorkEvent31__10IWorkEventFv,
    lbl_eu_806628C8, (const void*)0xFFFFFFA8,
    (const void*)destroyViaRender__Q22cf13CfNandManagerFv,
    (const void*)handleRenderCallback__Q22cf13CfNandManagerFv,
    (const void*)handleFileEvent__Q22cf13CfNandManagerFv,
    (const void*)cbRenderBefore__Q22cf13CfNandManagerFv,
};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536C80[13] = {
    lbl_eu_80661940, (const void*)0x58,
    __RTTI__10IWorkEvent, (const void*)0x54,
    lbl_eu_80661958, 0, lbl_eu_80661950, 0, lbl_eu_80661948, 0,
    lbl_eu_806628D0, 0, 0,
};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536CB4[9] = {
    lbl_eu_806628D0, 0,
    0, // retail __dt__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    (const void*)Reset__14CChildListNodeFv,
    0, 0,
    0, // retail Move__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    0, // retail Draw__28CTTask<Q22cf13CfNandManager>Fv (unspellable '<')
    (const void*)Tail__8CProcessFv,
};
__declspec(section ".data") __attribute__((used)) const void* lbl_eu_80536CD8[7] = {
    lbl_eu_80661958, 0, lbl_eu_80661950, 0, lbl_eu_80661948, 0, 0,
};
__attribute__((section(".data"), used, nocommon)) __attribute__((aligned(8))) const volatile unsigned char s_nandTail[4] = {0}; // retail +0x1C8 null word
#pragma section data_type ".data" ".bss"
// typed .rodata strings (each sized to its retail gap).
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_8050B440[0x14] = "cf::CfNandManager";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_8050B454[0x1C] = "CTTask<cf::CfNandManager>";
__declspec(section ".rodata") __attribute__((used)) const char lbl_eu_8050B470[0x18] = "%s%02d\0banner.bin\0name";
// typed .sdata pointer pairs (replaces old sdata blob which MWCC migrated
// to nobits as all-zero bytes): relocs to the string/pool labels keep the
// section live.
__declspec(section ".sdata") __attribute__((used)) const void* lbl_eu_806628C8[2] = { lbl_eu_8050B440, lbl_eu_80536C80 };
__declspec(section ".sdata") __attribute__((used)) const void* lbl_eu_806628D0[2] = { lbl_eu_8050B454, lbl_eu_80536CD8 };
// typed .bss/.sbss zero-fill (nobits compare sizes only; small sbss objects
// stay in .sbss automatically, no section attr needed).
char lbl_eu_80576CC0[0x38];
unsigned int lbl_eu_80664768;
unsigned char lbl_eu_8066476C;
unsigned char lbl_eu_8066476D;
unsigned short lbl_eu_8066476E;
unsigned short lbl_eu_80664770;
unsigned short lbl_eu_80664772;
unsigned short lbl_eu_80664774;
unsigned short lbl_eu_80664776_gap;
unsigned int lbl_eu_80664778;
unsigned int lbl_eu_8066477C;
unsigned int lbl_eu_80664780_a;
unsigned int lbl_eu_80664780_b;
unsigned int lbl_eu_80664788_a;
unsigned int lbl_eu_80664788_b;
