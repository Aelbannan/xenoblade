#include <types.h>
#include <revolution/gx/GXFrameBuf.h>
#include "cview_mock.h"

static void cview_stub_void(void) {}

u8 rb_cacheInstance__9CDeviceGX[0x400];
u8 dc_cacheInstance__9CDeviceGX[0x400];
void (*rb_jumptable_eu_8056B5C0[8])(void);
void (*dc_jumptable_eu_8056B5C0[8])(void);
void cview_init_jumptables(void) {
    int i;
    for (i = 0; i < 8; ++i) {
        rb_jumptable_eu_8056B5C0[i] = cview_stub_void;
        dc_jumptable_eu_8056B5C0[i] = cview_stub_void;
    }
}
u8 rb_lbl_8065A0C8[0x10];
u8 dc_lbl_8065A0C8[0x10];
u8 rb_lbl_eu_80522630[0x10];
u8 dc_lbl_eu_80522630[0x10];
u8 rb_lbl_eu_8056B280[0x10];
u8 dc_lbl_eu_8056B280[0x10];
u8 rb_lbl_eu_8056B298[0x10];
u8 dc_lbl_eu_8056B298[0x10];
u8 rb_lbl_eu_8056B5E0[0x10];
u8 dc_lbl_eu_8056B5E0[0x10];
u8 rb_lbl_eu_8056B6CC[0x10];
u8 dc_lbl_eu_8056B6CC[0x10];
u8 rb_lbl_eu_8056B6D8[0x10];
u8 dc_lbl_eu_8056B6D8[0x10];
u8 rb_lbl_eu_8056B6F0[0x10];
u8 dc_lbl_eu_8056B6F0[0x10];
CView* rb_lbl_eu_806655C8;
CView* dc_lbl_eu_806655C8;
u8 rb_lbl_eu_8066A2D0[0x10];
u8 dc_lbl_eu_8066A2D0[0x10];
u8 rb_lbl_eu_8066A2D4[0x10];
u8 dc_lbl_eu_8066A2D4[0x10];
u8 rb_lbl_eu_8066A2E0[0x10];
u8 dc_lbl_eu_8066A2E0[0x10];
u8 rb_lbl_eu_8066A2E4[0x10];
u8 dc_lbl_eu_8066A2E4[0x10];
u8 rb_sFrameColor__5CView[0x10];
u8 dc_sFrameColor__5CView[0x10];
u8 rb_white__Q22ml5CCol4[0x10];
u8 dc_white__Q22ml5CCol4[0x10];

CView* rb_getView1__11CSplitFrameFv(void* splitFrame) { return cview_mock_get_view1(splitFrame, 0); }
CView* dc_getView1__11CSplitFrameFv(void* splitFrame) { return cview_mock_get_view1(splitFrame, 1); }
CView* rb_getView2__11CSplitFrameFv(void* splitFrame) { return cview_mock_get_view2(splitFrame, 0); }
CView* dc_getView2__11CSplitFrameFv(void* splitFrame) { return cview_mock_get_view2(splitFrame, 1); }
s16 rb_getSplitLine__11CSplitFrameFv(void* splitFrame) { return cview_mock_get_split_line(splitFrame, 0); }
s16 dc_getSplitLine__11CSplitFrameFv(void* splitFrame) { return cview_mock_get_split_line(splitFrame, 1); }
void rb_setSplitLine__11CSplitFrameFs(void* splitFrame, s16 line) { cview_mock_set_split_line(splitFrame, line, 0); }
void dc_setSplitLine__11CSplitFrameFs(void* splitFrame, s16 line) { cview_mock_set_split_line(splitFrame, line, 1); }
int rb_isActive__11CSplitFrameFv(void* splitFrame) { return cview_mock_is_active(splitFrame, 0); }
int dc_isActive__11CSplitFrameFv(void* splitFrame) { return cview_mock_is_active(splitFrame, 1); }
void rb_apply__11CSplitFrameFv(void* splitFrame) { cview_mock_apply(splitFrame, 0); }
void dc_apply__11CSplitFrameFv(void* splitFrame) { cview_mock_apply(splitFrame, 1); }
CView* rb_getCurrent__9CViewRootFv(void) { return cview_mock_get_current(0); }
CView* dc_getCurrent__9CViewRootFv(void) { return cview_mock_get_current(1); }
void* rb_getInstance__9CViewRootFv(void) { return cview_mock_get_instance(0); }
void* dc_getInstance__9CViewRootFv(void) { return cview_mock_get_instance(1); }
void rb_invalidCurrent__9CViewRootFP5CView(void* view) { cview_mock_invalid_current((CView*)view, 0); }
void dc_invalidCurrent__9CViewRootFP5CView(void* view) { cview_mock_invalid_current((CView*)view, 1); }
void dc_updateMsg__5CViewFv(void* view) { (void)view; }
void rb_wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(CWorkThread* t, int evt) { cview_mock_wk_set_event(t, evt, 0); }
void dc_wkSetEvent__11CWorkThreadFQ211CWorkThread3EVT(CWorkThread* t, int evt) { cview_mock_wk_set_event(t, evt, 1); }
void rb_detachRenderWork__10CViewFrameFP11CWorkThread(void* frame, CWorkThread* t) { (void)frame; (void)t; }
void dc_detachRenderWork__10CViewFrameFP11CWorkThread(void* frame, CWorkThread* t) { (void)frame; (void)t; }
void rb_CView_UnkVirtualFunc1__10CViewFrameFv(void) {}
void dc_CView_UnkVirtualFunc1__10CViewFrameFv(void) {}
void rb_CView_UnkVirtualFunc8__10CViewFrameFv(void) {}
void dc_CView_UnkVirtualFunc8__10CViewFrameFv(void) {}
void rb_CView_UnkVirtualFunc9__10CViewFrameFv(void) {}
void dc_CView_UnkVirtualFunc9__10CViewFrameFv(void) {}
void rb_GXSetColorUpdate(void) {}
void dc_GXSetColorUpdate(void) {}
void rb___ct__11CWorkThreadFPCcP11CWorkThreadi(void* a) { (void)a; }
void dc___ct__11CWorkThreadFPCcP11CWorkThreadi(void* a) { (void)a; }
void rb___ct__7CDrawGXFv(void) {}
void dc___ct__7CDrawGXFv(void) {}
void rb___ct__CFontLayer(void) {}
void dc___ct__CFontLayer(void) {}
void rb___ct__CViewFrame(void) {}
void dc___ct__CViewFrame(void) {}
void rb___dl__FPv(void* p) { (void)p; }
void dc___dl__FPv(void* p) { (void)p; }
void rb___dla__FPv(void* p) { (void)p; }
void dc___dla__FPv(void* p) { (void)p; }
void rb___dt__10CFontLayerFv(void) {}
void dc___dt__10CFontLayerFv(void) {}
void rb___dt__11CWorkThreadFv(void) {}
void dc___dt__11CWorkThreadFv(void) {}
void rb___dt__7CDrawGXFv(void) {}
void dc___dt__7CDrawGXFv(void) {}
void rb__restgpr_25(void) {}
void dc__restgpr_25(void) {}
void rb__savegpr_25(void) {}
void dc__savegpr_25(void) {}
void rb_add__7CDrawGXFRCQ22ml7CRect16(void* a, void* b) { (void)a; (void)b; }
void dc_add__7CDrawGXFRCQ22ml7CRect16(void* a, void* b) { (void)a; (void)b; }
void* rb_allocate_array__Q23mtl10MemManagerFUlUl(u32 a, u32 b) { (void)a; (void)b; return 0; }
void* dc_allocate_array__Q23mtl10MemManagerFUlUl(u32 a, u32 b) { (void)a; (void)b; return 0; }
void* rb_begin__7CDrawGXFUlUl(u32 a, u32 b) { (void)a; (void)b; return 0; }
void* dc_begin__7CDrawGXFUlUl(u32 a, u32 b) { (void)a; (void)b; return 0; }
void rb_end__7CDrawGXFv(void) {}
void dc_end__7CDrawGXFv(void) {}
void rb_fontFlush__10CFontLayerFi(void* a, int b) { (void)a; (void)b; }
void dc_fontFlush__10CFontLayerFi(void* a, int b) { (void)a; (void)b; }
void* rb_func_8043FC60__10CViewFrameFUl(u32 a) { (void)a; return 0; }
void* dc_func_8043FC60__10CViewFrameFUl(u32 a) { (void)a; return 0; }
void rb_func_8043FD10__10CViewFrameFR7CRect16PC10CViewFrame(void* a, void* b) { (void)a; (void)b; }
void dc_func_8043FD10__10CViewFrameFR7CRect16PC10CViewFrame(void* a, void* b) { (void)a; (void)b; }
void rb_func_80441EF0__10CViewFrameFR7CRect16PC10CViewFrame(void* a, void* b) { (void)a; (void)b; }
void dc_func_80441EF0__10CViewFrameFR7CRect16PC10CViewFrame(void* a, void* b) { (void)a; (void)b; }
void rb_func_80442B54__9CViewRootFPvPv(void* a) { (void)a; }
void dc_func_80442B54__9CViewRootFPvPv(void* a) { (void)a; }
void rb_func_80442C68__9CViewRootFv(void) {}
void dc_func_80442C68__9CViewRootFv(void) {}
void rb_func_80442DA8__9CViewRootFv(void) {}
void dc_func_80442DA8__9CViewRootFv(void) {}
void rb_func_8044A578__8CGXCacheFv(void) {}
void dc_func_8044A578__8CGXCacheFv(void) {}
void rb_func_8044BE2C__8CGXCacheFv(void) {}
void dc_func_8044BE2C__8CGXCacheFv(void) {}
void rb_func_8044BE38__8CGXCacheFv(void) {}
void dc_func_8044BE38__8CGXCacheFv(void) {}
void rb_func_80456570__7CDrawGXFi(void* a, int b) { (void)a; (void)b; }
void dc_func_80456570__7CDrawGXFi(void* a, int b) { (void)a; (void)b; }
void rb_func_8045657C__7CDrawGXFi(void* a, int b) { (void)a; (void)b; }
void dc_func_8045657C__7CDrawGXFi(void* a, int b) { (void)a; (void)b; }
void rb_func_80459270__17CViewRectDataCoreFv(void) {}
void dc_func_80459270__17CViewRectDataCoreFv(void) {}
void rb_func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(void* a, void* b) { (void)a; (void)b; }
void dc_func_804592F0__17CViewRectDataCoreFRCQ22ml6CPnt16(void* a, void* b) { (void)a; (void)b; }
void rb_func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(void* a, void* b) { (void)a; (void)b; }
void dc_func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(void* a, void* b) { (void)a; (void)b; }
void rb_getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(void* a, void* b) { (void)a; (void)b; }
void dc_getFrame2ViewOffset__10CViewFrameFR7CRect16PC10CViewFrame(void* a, void* b) { (void)a; (void)b; }
void rb_getFullScreenView__9CViewRootFv(void) {}
void dc_getFullScreenView__9CViewRootFv(void) {}
void rb_getScissorRect1__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(void* a, void* b) { (void)a; (void)b; }
void dc_getScissorRect1__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(void* a, void* b) { (void)a; (void)b; }
void rb_getScissorRect2__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(void* a, void* b) { (void)a; (void)b; }
void dc_getScissorRect2__11CSplitFrameFRQ22ml7CRect16PC11CSplitFrame(void* a, void* b) { (void)a; (void)b; }
void* rb_getWorkThread__9CWorkUtilFUl(u32 a) { (void)a; return 0; }
void* dc_getWorkThread__9CWorkUtilFUl(u32 a) { (void)a; return 0; }
void rb_isCurrentChild__9CViewRootFPC5CViewPC5CView(void* a) { (void)a; }
void dc_isCurrentChild__9CViewRootFPC5CViewPC5CView(void* a) { (void)a; }
void rb_isCurrent__9CViewRootFPC5CView(void* a) { (void)a; }
void dc_isCurrent__9CViewRootFPC5CView(void* a) { (void)a; }
void rb_isInitialized__4CLibFv(void) {}
void dc_isInitialized__4CLibFv(void) {}
void rb_isInitialized__9CViewRootFv(void) {}
void dc_isInitialized__9CViewRootFv(void) {}
void rb_pssGetRoot__5CProcFP5CProc(void* a) { (void)a; }
void dc_pssGetRoot__5CProcFP5CProc(void* a) { (void)a; }
void rb_render__10CViewFrameFv(void) {}
void dc_render__10CViewFrameFv(void) {}
void rb_setCol__7CDrawGXFRCQ22ml5CCol4(void* a, void* b) { (void)a; (void)b; }
void dc_setCol__7CDrawGXFRCQ22ml5CCol4(void* a, void* b) { (void)a; (void)b; }
void rb_setCurrent__9CViewRootFP5CView(void* a) { (void)a; }
void dc_setCurrent__9CViewRootFP5CView(void* a) { (void)a; }
int rb_wkCheckTimeout__11CWorkThreadFUlbPCc(u32 a, int b, const char* c) { (void)a; (void)b; (void)c; return 0; }
int dc_wkCheckTimeout__11CWorkThreadFUlbPCc(u32 a, int b, const char* c) { (void)a; (void)b; (void)c; return 0; }
void rb_wkStandbyLogin__11CWorkThreadFv(void) {}
void dc_wkStandbyLogin__11CWorkThreadFv(void) {}
void rb_wkStandbyLogout__11CWorkThreadFv(void) {}
void dc_wkStandbyLogout__11CWorkThreadFv(void) {}
