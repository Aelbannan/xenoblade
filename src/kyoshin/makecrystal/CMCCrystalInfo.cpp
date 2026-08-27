// Auto-scaffolded catalog TU for kyoshin/makecrystal/CMCCrystalInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/makecrystal/CMCCrystalInfo.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/work/CEventFile.hpp"

namespace nw4r { namespace lyt { class DrawInfo; } }

extern "C" void func_80137924(void*, void*, void*, void*);
void func_801D1F9C(void*, u32);
void func_801C4B60(void*, u32, u32, u32, u32);
extern "C" void func_8021AF74(CMCCrystalInfo*);
extern "C" void func_8021AFC0(CMCCrystalInfo*);
extern "C" void func_8021B00C(CMCCrystalInfo*);
extern "C" void func_8021B058(CMCCrystalInfo*);
extern "C" void func_8021B0A4(CMCCrystalInfo*);
extern "C" void func_8021B0F0(CMCCrystalInfo*);
extern "C" void func_8021B13C(CMCCrystalInfo*);

// Small-data symbol (plain C++ extern; lives in another TU).
extern u32 lbl_eu_806640D8;

void func_8021B188(CrystalItemBuf* out, CMCCrystalInfo*, u32, CMCCItemHandle*);

// The retail ctor is emitted under the unmangled symbol `__ct__CMCCrystalInfo`
// (not a mangled member name), so it is written as a C-linkage function that
// explicitly sets the vptr and constructs the UnkClass sub-objects.
extern "C" CMCCrystalInfo* __ct__CMCCrystalInfo(CMCCrystalInfo* self)
{
    *(void**)self = (void*)lbl_eu_80535CF8;   // vptr at +0x00
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion1);
    __ct__17UnkClass_8045F564Fv(&self->mMemRegion2);

    self->mFileHandle1 = 0;
    self->mFileHandle2 = 0;
    self->mArcResAccessor = 0;
    self->mField30 = 0;
    self->mLayout = 0;
    self->mAnimTransform1 = 0;
    self->mAnimTransform2 = 0;
    self->mAnimTransform3 = 0;
    self->mAnimTransform4 = 0;
    self->mEnabled = 0;
    self->mState = 0;
    self->mIsActive = 0;
    self->mField51 = 1;
    return self;
}

// The retail dtor is emitted under the mangled symbol `__dt__14CMCCrystalInfoFv`.
// Written as a C-linkage function (CItemBoxInfo precedent) for byte-exact
// control over the null/delete-flag checks.
#pragma push
#pragma optimize_for_size on
extern "C" CMCCrystalInfo* __dt__14CMCCrystalInfoFv(CMCCrystalInfo* self, s32 flags)
{
    if (self != NULL) {
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion2, -1);
        __dt__17UnkClass_8045F564Fv(&self->mMemRegion1, -1);
        if (flags > 0) __dl__FPv(self);
    }
    return self;
}
#pragma pop

#pragma push
#pragma optimize_for_size on
void func_8021A718(CMCCrystalInfo* self)
{
    // Load the two crystal-info layout files (names at lbl_eu_80508DF8/+0x1a).
    u32 handle = (u32)mtl::MemManager::getHandleMEM2();
    self->mFileHandle1 = (u32)CDeviceFile::readFile(handle, &lbl_eu_80508DF8[0], self, 0, 0);
    handle = (u32)mtl::MemManager::getHandleMEM2();
    self->mFileHandle2 = (u32)CDeviceFile::readFile(handle, &lbl_eu_80508DF8[0x1a], self, 0, 0);
}
#pragma pop

void func_8021A780(CMCCrystalInfo* self)
{
    if (self->mEnabled) {
        switch (self->mState) {
        case 1:
            func_8021AF74(self);
            break;
        case 2:
            func_8021AFC0(self);
            break;
        case 4:
            func_8021B00C(self);
            break;
        case 5:
            func_8021B058(self);
            break;
        case 6:
            func_8021B0A4(self);
            break;
        case 7:
            func_8021B0F0(self);
            break;
        case 8:
            func_8021B13C(self);
            break;
        }
        ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
    }
}

void func_8021A840(CMCCrystalInfo* self, nw4r::lyt::DrawInfo* drawInfo)
{
    if (self->mEnabled) {
        drawLayout(
            (nw4r::lyt::Layout*)self->mLayout, drawInfo, 0, 1);
    }
}

void func_8021A860(CMCCrystalInfo* self)
{
    func_801390E0((CFileHandle**)&self->mFileHandle1);
    func_801390E0((CFileHandle**)&self->mFileHandle2);
    nw4r::lyt::Layout* layout = (nw4r::lyt::Layout*)self->mLayout;
    self->mEnabled = 0;
    if (layout != 0) {
        // Deleting dtor: MWCC emits a virtual call to ~Layout (vtable +8)
        // with the delete flag in r4, guarded by its own null check.
        delete layout;
        self->mLayout = 0;
    }
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->mArcResAccessor);
    releaseArcResourceAccessor((nw4r::lyt::ArcResourceAccessor*)self->mField30);
    self->mMemRegion1.func_8045F778();
    self->mMemRegion2.func_8045F778();
}

// Converted to inline member function in header

void func_8021A8F4(CMCCrystalInfo* self)
{
    if (self->mState == 0) {
        self->mState = 1;
        self->mField51 = 0;
        return func_8021B52C(self);
    }
}

void func_8021A918(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 4;
        self->mField51 = 0;
        return func_8021B5B4(self);
    }
}

void func_8021A93C(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 6;
        self->mField51 = 0;
        return func_8021B6C4(self);
    }
}

void func_8021A960(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 7;
        self->mField51 = 0;
        return func_8021B63C(self);
    }
}

void func_8021A984(CMCCrystalInfo* self)
{
    if (self->mState == 3) {
        self->mState = 8;
        self->mField51 = 0;
        return func_8021B63C(self);
    }
}

#pragma push
#pragma optimize_for_size on
void func_8021A9A8(CMCCrystalInfo* self, u32 arg4, CMCCItemData* item)
{
    CMCCItemData* p;
    if (item != 0) {
        p = item;
    } else {
        p = 0;
    }
    u32 val;
    if (p != 0) {
        val = p->word0 >> 20;
    } else {
        val = arg4;
    }
    int code = func_801392E4(val & 0xFFFF);
    if (p != 0 && p->word0 != 0) {
        // Type nibble lives at bits 16-19; crystal type 9 with sub-flag 2/3
        // forces the crystal display path.
        u32 type = (p->word0 >> 16) & 0xF;
        if ((type == 9 && (u32)(p->field07 & 3) == 2) ||
            (type == 9 && (u32)(p->field07 & 3) == 3))
            code = 9;
    }
    // Type 9 (crystal) routes to the crystal slot fill; otherwise clear them.
    if ((code & 0xFFFF) == 9) {
        func_8021B2E0(self, (u16)val, item);
    } else {
        func_8021B42C(self);
    }
}
#pragma pop

void func_8021AA9C(CMCCrystalInfo* self, u32 idxBase, u32 arg5, u8 arg6, u32 arg7)
{
    char buf[0x20];
    u32 msgId = arg5;   // materialized first: retail spills it (r25) before idxBase
    u32 idx;
    char* msgName;

    // Crystal name for slot idx, then colour it (colour pair depends on arg7).
    msgName = func_8013639C((char*)lbl_eu_806640D8, &lbl_eu_80508DF8[0x36], msgId);
    idx = idxBase + 1;
    sprintf(buf, &lbl_eu_80508DF8[0x3b], idx);
    func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, msgName, 0);
    func_80139A18((nw4r::lyt::Layout*)self->mLayout, buf,
        (GXColorS10*)(arg7 ? &lbl_eu_806646E8 : &lbl_eu_806646D8),
        (GXColorS10*)(arg7 ? &lbl_eu_806646F0 : &lbl_eu_806646E0));

    // Slot number text + element image index, coloured with the second pair.
    sprintf(buf, &lbl_eu_80508DF8[0x4c], idx);
    setLayoutTextBoxNumber__FPQ34nw4r3lyt6LayoutPcUc((nw4r::lyt::Layout*)self->mLayout, buf, arg6);
    func_80139A18((nw4r::lyt::Layout*)self->mLayout, buf,
        (GXColorS10*)(arg7 ? &lbl_eu_80664708 : &lbl_eu_806646F8),
        (GXColorS10*)(arg7 ? &lbl_eu_80664710 : &lbl_eu_80664700));

    // Description string + colour.
    sprintf(buf, &lbl_eu_80508DF8[0x5e], idx);
    func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf,
        func_80136190(&lbl_eu_80508DF8[0x6f], &lbl_eu_80508DF8[0x36], 0x21), 0);
    func_80139A18((nw4r::lyt::Layout*)self->mLayout, buf,
        (GXColorS10*)(arg7 ? &lbl_eu_80664708 : &lbl_eu_806646F8),
        (GXColorS10*)(arg7 ? &lbl_eu_80664710 : &lbl_eu_80664700));

    // Pick the crystal picture to show for this slot based on the item code.
    void* res = 0;
    u8 code = func_801361E8((u32)lbl_eu_806640D8, &lbl_eu_80508DF8[0x78], msgId);
    switch (code) {
    case 0:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          &lbl_eu_80508DF8[0x81], 0);
        break;
    case 4:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          &lbl_eu_80508DF8[0x97], 0);
        break;
    case 5:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          &lbl_eu_80508DF8[0xad], 0);
        break;
    case 6:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          &lbl_eu_80508DF8[0xc3], 0);
        break;
    case 7:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          &lbl_eu_80508DF8[0xd9], 0);
        break;
    case 8:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          &lbl_eu_80508DF8[0xef], 0);
        break;
    case 9:
        res = ((nw4r::lyt::ArcResourceAccessor*)self->mField30)
            ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                          &lbl_eu_80508DF8[0x105], 0);
        break;
    }

    if (res != 0) {
        sprintf(buf, &lbl_eu_80508DF8[0x11b], idx);
        func_80137E7C((nw4r::lyt::Layout*)self->mLayout, buf, res);
    }
}

// Clears every crystal-slot display buffer. noinline: retail calls these as
// separate functions (bl), so inlining would balloon the caller sizes.
extern "C" __declspec(noinline) void func_8021ADC4(CMCCrystalInfo* self)
{
    char buf[0x20];
    for (u8 i = 1; i <= 8; i++) {
        sprintf(buf, &lbl_eu_80508DF8[0x3b], i);
        func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, &lbl_eu_80508DF8[0x12a], 0);
        sprintf(buf, &lbl_eu_80508DF8[0x4c], i);
        func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, &lbl_eu_80508DF8[0x12a], 0);
        sprintf(buf, &lbl_eu_80508DF8[0x5e], i);
        func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, &lbl_eu_80508DF8[0x12a], 0);
        void* res = ((nw4r::lyt::ArcResourceAccessor*)self->mArcResAccessor)
                        ->GetResource(nw4r::lyt::ArcResourceAccessor::RES_TYPE_TEXTURE,
                                      &lbl_eu_80508DF8[0x12b], NULL);
        if (res != 0) {
            sprintf(buf, &lbl_eu_80508DF8[0x11b], i);
            func_80137E7C((nw4r::lyt::Layout*)self->mLayout, buf, res);
        }
    }
}

#pragma push
#pragma optimize_for_size on
void func_8021AED0(CMCCrystalInfo* self, CMCCrystalInfo* other, u32 r5)
{
    char buf[0x20];
    sprintf(buf, &lbl_eu_80508DF8[0x13e], r5 + 1);
    nw4r::lyt::Pane* r1 =
        (*(nw4r::lyt::Pane**)((u8*)other->mLayout + 0x10))->FindPaneByName(buf, true);
    // Retail reloads other->mLayout and its root pane for every lookup.
    nw4r::lyt::Pane* r2 =
        (*(nw4r::lyt::Pane**)((u8*)other->mLayout + 0x10))
            ->FindPaneByName(&lbl_eu_80508DF8[0x14b], true);
    func_80137924(self, r1, r2, *(nw4r::lyt::Pane**)((u8*)other->mLayout + 0x10));
}
#pragma pop

__declspec(noinline) void func_8021AF74(CMCCrystalInfo* self)
{
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->mAnimTransform1, 1.0f)) {
        self->mState = 2;
        return func_8021B5B4(self);
    }
}

__declspec(noinline) void func_8021AFC0(CMCCrystalInfo* self)
{
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->mAnimTransform2, 1.0f)) {
        self->mState = 3;
        self->mField51 = 1;
    }
}

__declspec(noinline) void func_8021B00C(CMCCrystalInfo* self)
{
    if (func_80137510((nw4r::lyt::AnimTransform*)self->mAnimTransform2, 1.0f)) {
        self->mState = 5;
        return func_8021B52C(self);
    }
}

__declspec(noinline) void func_8021B058(CMCCrystalInfo* self)
{
    if (func_80137510((nw4r::lyt::AnimTransform*)self->mAnimTransform1, 1.0f)) {
        self->mState = 0;
        self->mField51 = 1;
    }
}

__declspec(noinline) void func_8021B0A4(CMCCrystalInfo* self)
{
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->mAnimTransform4, 1.0f)) {
        self->mField51 = 1;
        self->mState = 0;
    }
}

__declspec(noinline) void func_8021B0F0(CMCCrystalInfo* self)
{
    if (advanceAnimTransform((nw4r::lyt::AnimTransform*)self->mAnimTransform3, 1.0f)) {
        self->mField51 = 1;
        self->mState = 3;
    }
}

__declspec(noinline) void func_8021B13C(CMCCrystalInfo* self)
{
    if (func_80137510((nw4r::lyt::AnimTransform*)self->mAnimTransform3, lbl_eu_80668498)) {
        self->mField51 = 1;
        self->mState = 3;
    }
}

// Build the crystal-slot display buffers. noinline: retail calls these as
// separate functions (bl), so inlining would balloon the caller sizes.
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void func_8021B188(CrystalItemBuf* out, CMCCrystalInfo* self, u32 data, CMCCItemHandle* item)
{
    CrystalItemBuf buf;
    CMCCItemHandle* item2;
    if (item != 0) {
        item2 = item;
    } else {
        item2 = 0;
    }
    func_801392E4(data);
    func_80139358(data);
    CItemImplInstancesFacade* inst = (CItemImplInstancesFacade*)CItem_initItemImplInstances(item2);
    u8 count = inst->GetCount(item2);
    buf.count = count;
    buf.str = (char*)func_80136190(&lbl_eu_80508DF8[0x6f], &lbl_eu_80508DF8[0x36],
                                   0x1e - (count - 1));
    buf.field21 = 0;
    for (u32 i = 0; i < 4; i++) {
        CItemImplInstancesFacade* inst2 = (CItemImplInstancesFacade*)CItem_initItemImplInstances(item2);
        u16 n = inst2->GetName(item2, (u8)i);
        if (n > 0) {
            buf.names[buf.field21] = func_8013639C((char*)lbl_eu_806640D8,
                                                   &lbl_eu_80508DF8[0x36], n);
            CItemImplInstancesFacade* inst3 = (CItemImplInstancesFacade*)CItem_initItemImplInstances(item2);
            buf.flags[buf.field21] = inst3->GetFlag(item2, (u8)i);
            buf.field21++;
        }
    }
    // Word-pair block copy of the buffer; optimize_for_size keeps it as the
    // retail counted lwzu/stwu loop.
    u32* dst = (u32*)((char*)out - 4);
    u32* src = (u32*)((char*)&buf - 4);
    for (int j = 4; j != 0; j--) {
        *++dst = *++src;
        *++dst = *++src;
    }
    *++dst = *++src;
}

// Fill the crystal slot texts from the item's crystal entries.
__declspec(noinline) void func_8021B2E0(CMCCrystalInfo* self, u16 arg2, CMCCItemHandle* item)
{
    char buf[0x20];
    func_8021B42C(self);
    CrystalItemBuf bufB;
    func_8021B188(&bufB, self, arg2, item);
    // Word-pair block copy of the filled buffer into a fixed frame slot;
    // under optimize_for_size (scope extended from func_8021B188 above) MWCC
    // keeps this as the retail counted lwzu/stwu loop.
    CrystalItemBuf slots;
    u32* dst = (u32*)((char*)&slots - 4);
    u32* src = (u32*)((char*)&bufB - 4);
    for (int j = 4; j != 0; j--) {
        *++dst = *++src;
        *++dst = *++src;
    }
    *++dst = *++src;
    func_80136B4C((nw4r::lyt::Layout*)self->mLayout, &lbl_eu_80508DF8[0x15a],
                  slots.str, 0);
    u8 count = slots.field21;
    for (u8 i = 0; i < count; i++) {
        sprintf(buf, &lbl_eu_80508DF8[0x166], (i * 2) + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, slots.names[i], 0);
        sprintf(buf, &lbl_eu_80508DF8[0x173], i + 0x1f);
        setLayoutTextBoxNumber((nw4r::lyt::Layout*)self->mLayout, buf, slots.flags[i]);
        sprintf(buf, &lbl_eu_80508DF8[0x166], (i * 2) + 0x20);
        char* s = func_80136190(&lbl_eu_80508DF8[0x6f], &lbl_eu_80508DF8[0x36], 0x21);
        func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, s, 0);
    }
}

// Clears every crystal-slot text entry (used before refilling the display).
#pragma push
#pragma optimize_for_size on
__declspec(noinline) void func_8021B42C(CMCCrystalInfo* self)
{
    char buf[0x20];
    func_80136B4C((nw4r::lyt::Layout*)self->mLayout, &lbl_eu_80508DF8[0x15a],
                  &lbl_eu_80508DF8[0x12a], 0);
    for (u8 i = 0; i < 4; i++) {
        sprintf(buf, &lbl_eu_80508DF8[0x166], (i * 2) + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, &lbl_eu_80508DF8[0x12a], 0);
        sprintf(buf, &lbl_eu_80508DF8[0x173], i + 0x1f);
        func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, &lbl_eu_80508DF8[0x12a], 0);
        sprintf(buf, &lbl_eu_80508DF8[0x166], (i * 2) + 0x20);
        func_80136B4C((nw4r::lyt::Layout*)self->mLayout, buf, &lbl_eu_80508DF8[0x12a], 0);
    }
}
#pragma pop

// Sets the ready/active flags once both the layout and its resource accessor
// are present. noinline: retail calls this as a separate function (bl).
extern "C" __declspec(noinline) void func_8021B500(void* this_) {
    unsigned int* p34 = reinterpret_cast<unsigned int*>(static_cast<char*>(this_) + 0x34);
    unsigned int* p30 = reinterpret_cast<unsigned int*>(static_cast<char*>(this_) + 0x30);
    if (*p34 != 0) {
        if (*p30 != 0) {
            *reinterpret_cast<unsigned char*>(static_cast<char*>(this_) + 0x48) = 1;
            *reinterpret_cast<unsigned char*>(static_cast<char*>(this_) + 0x50) = 1;
        }
    }
}

// Bind the crystal-info intro animation to the layout and start it playing.
// (Called on state 0->1 and 4->5 transitions.)
__declspec(noinline) void func_8021B52C(CMCCrystalInfo* self)
{
    ((nw4r::lyt::Layout*)self->mLayout)->UnbindAllAnimation();
    ((nw4r::lyt::Layout*)self->mLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mAnimTransform1);
    ((nw4r::lyt::Layout*)self->mLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform1, true);
    ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
}

// Bind the crystal-info outro animation to the layout and start it playing.
// (Called on state 3->4 transitions.)
__declspec(noinline) void func_8021B5B4(CMCCrystalInfo* self)
{
    ((nw4r::lyt::Layout*)self->mLayout)->UnbindAllAnimation();
    ((nw4r::lyt::Layout*)self->mLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mAnimTransform2);
    ((nw4r::lyt::Layout*)self->mLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform2, true);
    ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
}

// Bind the crystal-info intro animation variant for the 3rd anim transform
// (mAnimTransform3) and start it playing. Called on state 3->7/3->8.
__declspec(noinline) void func_8021B63C(CMCCrystalInfo* self)
{
    ((nw4r::lyt::Layout*)self->mLayout)->UnbindAllAnimation();
    ((nw4r::lyt::Layout*)self->mLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mAnimTransform3);
    ((nw4r::lyt::Layout*)self->mLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform3, true);
    ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
}

// Bind the crystal-info intro animation variant for the 4th anim transform
// (mAnimTransform4) and start it playing. Called on state 3->6.
__declspec(noinline) void func_8021B6C4(CMCCrystalInfo* self)
{
    ((nw4r::lyt::Layout*)self->mLayout)->UnbindAllAnimation();
    ((nw4r::lyt::Layout*)self->mLayout)->BindAnimation((nw4r::lyt::AnimTransform*)self->mAnimTransform4);
    ((nw4r::lyt::Layout*)self->mLayout)->SetAnimationEnable((nw4r::lyt::AnimTransform*)self->mAnimTransform4, true);
    ((nw4r::lyt::Layout*)self->mLayout)->Animate(0);
}

bool CMCCrystalInfo::OnFileEvent(CEventFile* pEventFile)
{
    if (mFileHandle1 == (u32)pEventFile->mFileHandle) {
        // === crystal info file 1 loaded ===
        mMemRegion1.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x18000,
            &lbl_eu_80508DF8[0x181], 0);
        Class_8045F858 regionGuard1(&mMemRegion1);

        CFileHandle* h1 = (CFileHandle*)mFileHandle1;
        void* fileData = h1->mData;
        h1->mData = nullptr;
        mtl::MemManager::setMemInitFlag(false);

        mArcResAccessor = CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)mArcResAccessor)
            ->Attach(fileData, &lbl_eu_80508DF8[0x190]);

        buildLayout__FPPQ34nw4r3lyt6LayoutPQ34nw4r3lyt19ArcResourceAccessorPCc(
            (nw4r::lyt::Layout**)&mLayout,
            (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, &lbl_eu_80508DF8[0x194]);

        bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)mLayout,
            (nw4r::lyt::AnimTransform**)&mAnimTransform1,
            (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, &lbl_eu_80508DF8[0x1ab]);
        bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)mLayout,
            (nw4r::lyt::AnimTransform**)&mAnimTransform2,
            (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, &lbl_eu_80508DF8[0x1c5]);
        bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)mLayout,
            (nw4r::lyt::AnimTransform**)&mAnimTransform3,
            (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, &lbl_eu_80508DF8[0x1e4]);
        bindLayoutAnimTransform__FPQ34nw4r3lyt6LayoutPPQ34nw4r3lyt13AnimTransformPQ34nw4r3lyt19ArcResourceAccessorPc(
            (nw4r::lyt::Layout*)mLayout,
            (nw4r::lyt::AnimTransform**)&mAnimTransform4,
            (nw4r::lyt::ArcResourceAccessor*)mArcResAccessor, &lbl_eu_80508DF8[0x1ff]);

        // Bind the loaded font's pane into the layout root.
        nw4r::lyt::Pane* rootPane =
            ((nw4r::lyt::Layout*)mLayout)->GetRootPane();
        func_8013676C(rootPane,
            reinterpret_cast<CMCCrystalInfoFontView*>(
                CDeviceFont::getFontInfo(1, (nw4r::lyt::Layout*)mLayout))
                ->vf7());

        // If a character set is loaded, stamp every text pane with it.
        u32 sh = func_801355BC();
        if (sh != 0) {
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x15a], sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x21a], sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x226], sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x232], sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x23e], sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x24a], sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x255], sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x260], sh);
            setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x26b], sh);
            char buf[0x20];
            for (u8 i = 1; i <= 8; i++) {
                sprintf(buf, &lbl_eu_80508DF8[0x4c], i);
                setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, buf, sh);
                sprintf(buf, &lbl_eu_80508DF8[0x5e], i);
                setLayoutTextBoxFont((nw4r::lyt::Layout*)mLayout, buf, sh);
            }
        }

        func_8021B52C(this);
        ((nw4r::lyt::Layout*)mLayout)->Animate(0);

        func_80136B4C((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x276],
            (char*)func_80136190(&lbl_eu_80508DF8[0x6f], &lbl_eu_80508DF8[0x36], 0x18), 0);
        func_80136B4C((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x292],
            (char*)func_80136190(&lbl_eu_80508DF8[0x281], &lbl_eu_80508DF8[0x28d], 0x2c), 0);
        func_80136B4C((nw4r::lyt::Layout*)mLayout, &lbl_eu_80508DF8[0x2a1],
            (char*)func_80136190(&lbl_eu_80508DF8[0x281], &lbl_eu_80508DF8[0x28d], 0x2d), 0);

        func_8021ADC4(this);

        // Pull the two colour pairs off their panes and refresh the alpha
        // channel of the shared slot colours.
        nw4r::lyt::Pane* pane1 = ((nw4r::lyt::Layout*)mLayout)->GetRootPane()
            ->FindPaneByName(&lbl_eu_80508DF8[0x2b1], true);
        FourShorts fs1a = func_801397AC(pane1, 0);
        CopyVec4s(lbl_eu_806646D8, (short*)&fs1a);
        FourShorts fs1b = func_801397AC(pane1, 1);
        CopyVec4s(lbl_eu_806646E0, (short*)&fs1b);
        lbl_eu_806646E8[3] = lbl_eu_806646D8[3];
        lbl_eu_806646F0[3] = lbl_eu_806646E0[3];

        nw4r::lyt::Pane* pane2 = ((nw4r::lyt::Layout*)mLayout)->GetRootPane()
            ->FindPaneByName(&lbl_eu_80508DF8[0x2c0], true);
        FourShorts fs2a = func_801397AC(pane2, 0);
        CopyVec4s(lbl_eu_806646F8, (short*)&fs2a);
        FourShorts fs2b = func_801397AC(pane2, 1);
        CopyVec4s(lbl_eu_80664700, (short*)&fs2b);
        lbl_eu_80664708[3] = lbl_eu_806646F8[3];
        lbl_eu_80664710[3] = lbl_eu_80664700[3];

        func_8021B500(this);
        mFileHandle1 = 0;
        mMemRegion1.func_8045F810();
        return true;
    } else if (mFileHandle2 == (u32)pEventFile->mFileHandle) {
        // === crystal info file 2 loaded ===
        mMemRegion2.createRegion(
            mtl::MemManager::getHandleMEM2(), 0x100, &lbl_eu_80508DF8[0x2d0], 0);
        Class_8045F858 regionGuard2(&mMemRegion2);

        CFileHandle* h2 = (CFileHandle*)mFileHandle2;
        void* fileData = h2->mData;
        h2->mData = nullptr;
        mtl::MemManager::setMemInitFlag(false);

        mField30 = (u32)CLibLayout::createArcResourceAccessor();
        ((nw4r::lyt::ArcResourceAccessor*)mField30)
            ->Attach(fileData, &lbl_eu_80508DF8[0x190]);

        func_8021B500(this);
        mFileHandle2 = 0;
        mMemRegion2.func_8045F810();
        return true;
    }

    return false;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8021BBC4()
{
    func_801D1F9C(lbl_eu_806646D8, 0);
    func_801D1F9C(lbl_eu_806646E0, 0);
    func_801C4B60(lbl_eu_806646E8, 0xd2, 0x28, 0x14, 0);
    func_801C4B60(lbl_eu_806646F0, 0xd2, 0x28, 0x14, 0);
    func_801D1F9C(lbl_eu_806646F8, 0);
    func_801D1F9C(lbl_eu_80664700, 0);
    func_801C4B60(lbl_eu_80664708, 0xff, 0xff, 0xfa, 0);
    func_801C4B60(lbl_eu_80664710, 0xd2, 0x28, 0x14, 0);
}
