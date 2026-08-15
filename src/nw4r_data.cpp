/*
 * High-level C++ reconstruction of nw4r_data (data-only TU).
 *
 * Reproduces the retail split object build/us/obj/nw4r_data.o byte-for-byte
 * (and reloc-for-reloc where the target symbol is expressible in C++).
 *
 * Section inventory (retail):
 *   .rodata 0x4F70 (20336)  .data 0x21A8 (8616)  .bss 0x39A08 (236040)
 *   .sdata 0xAA (170)       .sbss 0x110 (272)    .sdata2 0x710 (1808)
 *   .sbss2 0x8 (8)
 *
 * 1,026 relocs total: 944 referenced by name (extern "C" mangled
 * declarations or in-unit data symbols); 82 emitted as placeholder 0
 * because their retail symbol names cannot be spelled in C++:
 *   - 56 @unnamed@ anonymous-namespace function pointers (.rodata)
 *   - 19 @NNN@ MI adjusted-this thunks (.data)
 *   - 7  template <w>/<c> TagProcessorBase methods (.data)
 * (offsets+names listed at the end of this file / in the handoff report)
 */

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

/* ---- extern "C" declarations for referenced functions (retail mangled names) ---- */
extern "C" {
    void Add__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d6ScnObjbb();
    void AllocStringBuffer__Q34nw4r3lyt7TextBoxFUs();
    void AllocTrack__Q44nw4r3snd6detail20MmlSeqTrackAllocatorFPQ44nw4r3snd6detail9SeqPlayer();
    void Alloc__Q44nw4r3snd6detail10PlayerHeapFUl();
    void AnimateSelf__Q34nw4r3lyt4PaneFUl();
    void AnimateSelf__Q34nw4r3lyt6WindowFUl();
    void Animate__Q34nw4r3lyt18AnimTransformBasicFUlPQ34nw4r3lyt4Pane();
    void Animate__Q34nw4r3lyt18AnimTransformBasicFUlPQ34nw4r3lyt8Material();
    void Animate__Q34nw4r3lyt4PaneFUl();
    void Animate__Q34nw4r3lyt6LayoutFUl();
    void Animate__Q34nw4r3lyt8MaterialFv();
    void Append__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt6TexMap();
    void AssignWorkBuffer__Q34nw4r3snd11FxDelayDpl2FPvUl();
    void AssignWorkBuffer__Q34nw4r3snd12FxChorusDpl2FPvUl();
    void AssignWorkBuffer__Q34nw4r3snd15FxReverbStdDpl2FPvUl();
    void Attach__Q34nw4r3g3d12AnmObjMatClrFiPQ34nw4r3g3d15AnmObjMatClrRes();
    void Attach__Q34nw4r3g3d12AnmObjTexPatFiPQ34nw4r3g3d15AnmObjTexPatRes();
    void Attach__Q34nw4r3g3d12AnmObjTexSrtFiPQ34nw4r3g3d15AnmObjTexSrtRes();
    void Attach__Q34nw4r3g3d13AnmObjChrNodeFiPQ34nw4r3g3d12AnmObjChrRes();
    void Attach__Q34nw4r3g3d6AnmScnFiPQ34nw4r3g3d9AnmScnRes();
    void Attach__Q34nw4r3g3d9AnmObjChrFiPQ34nw4r3g3d12AnmObjChrRes();
    void BindAnimationAuto__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor();
    void BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb();
    void BindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform();
    void BindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform();
    void Bind__Q34nw4r3g3d12AnmObjChrResFQ34nw4r3g3d6ResMdl();
    void Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdl();
    void Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption();
    void Bind__Q34nw4r3g3d15AnmObjMatClrResFQ34nw4r3g3d6ResMdl();
    void Bind__Q34nw4r3g3d15AnmObjTexPatResFQ34nw4r3g3d6ResMdl();
    void Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdl();
    void Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption();
    void Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt4Panebb();
    void Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt8Materialb();
    void Build__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor();
    void CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam();
    void CalcWorldMtx__Q34nw4r3g3d6ScnObjFPCQ34nw4r4math5MTX34PUl();
    void CalcWorldMtx__Q34nw4r3g3d7ScnLeafFPCQ34nw4r4math5MTX34PUl();
    void CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo();
    void CalculateMtx__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo();
    void CanAsync__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv();
    void CanCancel__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv();
    void CanRead__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv();
    void CanSeek__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv();
    void CanWrite__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv();
    void CancelAsync__Q34nw4r2ut10FileStreamFPFlPQ34nw4r2ut8IOStreamPv_vPv();
    void Cancel__Q34nw4r2ut10FileStreamFv();
    void Cancel__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv();
    void Cancel__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTaskFv();
    void Cancel__Q54nw4r3snd6detail8SeqSound11SeqLoadTaskFv();
    void ChannelCallbackFunc__Q44nw4r3snd6detail8SeqTrackFPQ44nw4r3snd6detail7ChannelQ54nw4r3snd6detail7Channel21ChannelCallbackStatusUl();
    void ChannelCallback__Q44nw4r3snd6detail9SeqPlayerFPQ44nw4r3snd6detail7Channel();
    void CheckScnObj__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d6ScnObj();
    void Clear__Q34nw4r3g3d12ScnObjGatherFv();
    void Close__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamFv();
    void CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll();
    void CreateAnimTransform__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor();
    void CreateAnimTransform__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor();
    void CreateAnimTransform__Q34nw4r3lyt6LayoutFv();
    void DetachAll__Q34nw4r3g3d12AnmObjMatClrFv();
    void DetachAll__Q34nw4r3g3d12AnmObjTexPatFv();
    void DetachAll__Q34nw4r3g3d12AnmObjTexSrtFv();
    void DetachAll__Q34nw4r3g3d13AnmObjChrNodeFv();
    void DetachAll__Q34nw4r3g3d9AnmObjChrFv();
    void DetachTempSpecialHandle__Q44nw4r3snd6detail8SeqSoundFv();
    void DetachTempSpecialHandle__Q44nw4r3snd6detail9StrmSoundFv();
    void DetachTempSpecialHandle__Q44nw4r3snd6detail9WaveSoundFv();
    void Detach__Q34nw4r3g3d12AnmObjMatClrFi();
    void Detach__Q34nw4r3g3d12AnmObjTexPatFi();
    void Detach__Q34nw4r3g3d12AnmObjTexSrtFi();
    void Detach__Q34nw4r3g3d13AnmObjChrNodeFi();
    void Detach__Q34nw4r3g3d6AnmScnFi();
    void Detach__Q34nw4r3g3d9AnmObjChrFi();
    void DrawContent__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2RCQ34nw4r3lyt15WindowFrameSizeUc();
    void DrawFrame4__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc();
    void DrawFrame8__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc();
    void DrawFrame__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2RCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc();
    void DrawOpa__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d14ResMdlDrawMode();
    void DrawSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo();
    void DrawSelf__Q34nw4r3lyt6WindowFRCQ34nw4r3lyt8DrawInfo();
    void DrawSelf__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt8DrawInfo();
    void DrawSelf__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo();
    void DrawSelf__Q34nw4r3lyt8BoundingFRCQ34nw4r3lyt8DrawInfo();
    void DrawXlu__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d14ResMdlDrawMode();
    void Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo();
    void Draw__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo();
    void Execute__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv();
    void Execute__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTaskFv();
    void Execute__Q54nw4r3snd6detail8SeqSound11SeqLoadTaskFv();
    void FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform();
    void FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource();
    void FindAnimationLink__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform();
    void FindAnimationLink__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResource();
    void FindMaterialByName__Q34nw4r3lyt4PaneFPCcb();
    void FindMaterialByName__Q34nw4r3lyt6WindowFPCcb();
    void FindPaneByName__Q34nw4r3lyt4PaneFPCcb();
    void ForEach__Q34nw4r3g3d7ScnLeafFPFPQ34nw4r3g3d6ScnObjPv_Q44nw4r3g3d6ScnObj13ForEachResultPvb();
    void ForEach__Q34nw4r3g3d8ScnGroupFPFPQ34nw4r3g3d6ScnObjPv_Q44nw4r3g3d6ScnObj13ForEachResultPvb();
    void FreeStringBuffer__Q34nw4r3lyt7TextBoxFv();
    void FreeTrack__Q44nw4r3snd6detail20MmlSeqTrackAllocatorFPQ44nw4r3snd6detail8SeqTrack();
    void G3dProc__Q34nw4r3g3d12AnmObjChrResFUlUlPv();
    void G3dProc__Q34nw4r3g3d12ScnMdlExpandFUlUlPv();
    void G3dProc__Q34nw4r3g3d12ScnMdlSimpleFUlUlPv();
    void G3dProc__Q34nw4r3g3d13AnmObjChrNodeFUlUlPv();
    void G3dProc__Q34nw4r3g3d15AnmObjMatClrResFUlUlPv();
    void G3dProc__Q34nw4r3g3d15AnmObjTexPatResFUlUlPv();
    void G3dProc__Q34nw4r3g3d15AnmObjTexSrtResFUlUlPv();
    void G3dProc__Q34nw4r3g3d6ScnMdlFUlUlPv();
    void G3dProc__Q34nw4r3g3d7ScnProcFUlUlPv();
    void G3dProc__Q34nw4r3g3d7ScnRootFUlUlPv();
    void G3dProc__Q34nw4r3g3d8ScnGroupFUlUlPv();
    void G3dProc__Q34nw4r3g3d9AnmScnResFUlUlPv();
    void GetAllocatableTrackCount__Q44nw4r3snd6detail20MmlSeqTrackAllocatorCFv();
    void GetAmbLightColor__Q34nw4r3g3d9AnmScnResFUl();
    void GetAmbLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv();
    void GetAmbLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d17AmbLightAnmResultUl();
    void GetAnmObj__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d12ScnMdlSimple10AnmObjType();
    void GetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType();
    void GetAnmObj__Q34nw4r3g3d6ScnMdlCFQ44nw4r3g3d12ScnMdlSimple10AnmObjType();
    void GetAnmObj__Q34nw4r3g3d6ScnMdlFQ44nw4r3g3d12ScnMdlSimple10AnmObjType();
    void GetAscent__Q34nw4r2ut7RomFontCFv();
    void GetAscent__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetBaselinePos__Q34nw4r2ut7RomFontCFv();
    void GetBaselinePos__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetBasicPlayer__Q44nw4r3snd6detail8SeqSoundCFv();
    void GetBasicPlayer__Q44nw4r3snd6detail8SeqSoundFv();
    void GetBasicPlayer__Q44nw4r3snd6detail9StrmSoundCFv();
    void GetBasicPlayer__Q44nw4r3snd6detail9StrmSoundFv();
    void GetBasicPlayer__Q44nw4r3snd6detail9WaveSoundCFv();
    void GetBasicPlayer__Q44nw4r3snd6detail9WaveSoundFv();
    void GetBufferAlign__Q34nw4r2ut8IOStreamCFv();
    void GetCameraMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv();
    void GetCameraResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d15CameraAnmResultUl();
    void GetCamera__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d6CameraUl();
    void GetCellHeight__Q34nw4r2ut7RomFontCFv();
    void GetCellHeight__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetCellWidth__Q34nw4r2ut7RomFontCFv();
    void GetCellWidth__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetCharWidth__Q34nw4r2ut7RomFontCFUs();
    void GetCharWidth__Q44nw4r2ut6detail11ResFontBaseCFUs();
    void GetCharWidths__Q34nw4r2ut7RomFontCFUs();
    void GetCharWidths__Q44nw4r2ut6detail11ResFontBaseCFUs();
    void GetCharWidths__Q44nw4r2ut6detail15ArchiveFontBaseCFUs();
    void GetCoef__Q44nw4r3snd6detail15BiquadFilterHpfCFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef();
    void GetCoef__Q44nw4r3snd6detail15BiquadFilterLpfCFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef();
    void GetCoef__Q44nw4r3snd6detail18BiquadFilterBpf512CFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef();
    void GetCoef__Q44nw4r3snd6detail19BiquadFilterBpf1024CFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef();
    void GetCoef__Q44nw4r3snd6detail19BiquadFilterBpf2048CFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef();
    void GetColorElement__Q34nw4r3lyt4PaneCFUl();
    void GetContentMaterial__Q34nw4r3lyt6WindowCFv();
    void GetDefaultCharWidths__Q34nw4r2ut7RomFontCFv();
    void GetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetDescent__Q34nw4r2ut7RomFontCFv();
    void GetDescent__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetDiffuseLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv();
    void GetEncoding__Q34nw4r2ut7RomFontCFv();
    void GetEncoding__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetFogMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv();
    void GetFogResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d12FogAnmResultUl();
    void GetFog__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d3FogUl();
    void GetFont__Q34nw4r3lyt16ResourceAccessorFPCc();
    void GetFont__Q34nw4r3lyt19ArcResourceAccessorFPCc();
    void GetFrameMaterial__Q34nw4r3lyt6WindowCFUl();
    void GetFrame__Q34nw4r3g3d12AnmObjChrResCFv();
    void GetFrame__Q34nw4r3g3d13AnmObjChrNodeCFv();
    void GetFrame__Q34nw4r3g3d15AnmObjMatClrResCFv();
    void GetFrame__Q34nw4r3g3d15AnmObjTexPatResCFv();
    void GetFrame__Q34nw4r3g3d15AnmObjTexSrtResCFv();
    void GetFrame__Q34nw4r3g3d9AnmScnResCFv();
    void GetGlyph__Q34nw4r2ut10PackedFontCFPQ34nw4r2ut5GlyphUs();
    void GetGlyph__Q34nw4r2ut7RomFontCFPQ34nw4r2ut5GlyphUs();
    void GetGlyph__Q44nw4r2ut6detail11ResFontBaseCFPQ34nw4r2ut5GlyphUs();
    void GetHeight__Q34nw4r2ut7RomFontCFv();
    void GetHeight__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d14LightAnmResultUl();
    void GetLightSetMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv();
    void GetLightSet__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d8LightSetUl();
    void GetLight__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d8LightObjPQ34nw4r3g3d8LightObjUl();
    void GetLineFeed__Q34nw4r2ut7RomFontCFv();
    void GetLineFeed__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetMaterialNum__Q34nw4r3lyt4PaneCFv();
    void GetMaterialNum__Q34nw4r3lyt6WindowCFv();
    void GetMaterial__Q34nw4r3lyt4PaneCFUl();
    void GetMaterial__Q34nw4r3lyt4PaneCFv();
    void GetMaterial__Q34nw4r3lyt6WindowCFUl();
    void GetMaxCharWidth__Q34nw4r2ut7RomFontCFv();
    void GetMaxCharWidth__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetNumAmbLight__Q34nw4r3g3d9AnmScnResCFv();
    void GetNumCamera__Q34nw4r3g3d9AnmScnResCFv();
    void GetNumDiffuseLight__Q34nw4r3g3d9AnmScnResCFv();
    void GetNumFog__Q34nw4r3g3d9AnmScnResCFv();
    void GetNumLightSet__Q34nw4r3g3d9AnmScnResCFv();
    void GetNumSpecularLight__Q34nw4r3g3d9AnmScnResCFv();
    void GetOffsetAlign__Q34nw4r2ut8IOStreamCFv();
    void GetResource__Q34nw4r3lyt19ArcResourceAccessorFUlPCcPUl();
    void GetResult__Q34nw4r3g3d12AnmObjChrResFPQ34nw4r3g3d12ChrAnmResultUl();
    void GetResult__Q34nw4r3g3d14AnmObjChrBlendFPQ34nw4r3g3d12ChrAnmResultUl();
    void GetResult__Q34nw4r3g3d15AnmObjMatClrResFPQ34nw4r3g3d12ClrAnmResultUl();
    void GetResult__Q34nw4r3g3d15AnmObjTexPatResFPQ34nw4r3g3d15TexPatAnmResultUl();
    void GetResult__Q34nw4r3g3d15AnmObjTexSrtResFPQ34nw4r3g3d15TexSrtAnmResultUl();
    void GetRuntimeTypeInfo__Q34nw4r2ut10FileStreamCFv();
    void GetRuntimeTypeInfo__Q34nw4r3lyt4PaneCFv();
    void GetRuntimeTypeInfo__Q34nw4r3lyt6WindowCFv();
    void GetRuntimeTypeInfo__Q34nw4r3lyt7PictureCFv();
    void GetRuntimeTypeInfo__Q34nw4r3lyt7TextBoxCFv();
    void GetRuntimeTypeInfo__Q34nw4r3lyt8BoundingCFv();
    void GetRuntimeTypeInfo__Q44nw4r3snd6detail10BasicSoundCFv();
    void GetRuntimeTypeInfo__Q44nw4r3snd6detail8SeqSoundCFv();
    void GetRuntimeTypeInfo__Q44nw4r3snd6detail9StrmSoundCFv();
    void GetRuntimeTypeInfo__Q44nw4r3snd6detail9WaveSoundCFv();
    void GetScnObjOption__Q34nw4r3g3d12ScnMdlSimpleCFUlPUl();
    void GetScnObjOption__Q34nw4r3g3d6ScnMdlCFUlPUl();
    void GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl();
    void GetScnObjOption__Q34nw4r3g3d7ScnLeafCFUlPUl();
    void GetSizeAlign__Q34nw4r2ut8IOStreamCFv();
    void GetSize__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv();
    void GetSpecularLightID__Q34nw4r3g3d9AnmScnResCFUl();
    void GetTextColor__Q34nw4r3lyt7TextBoxCFUl();
    void GetTextureFormat__Q34nw4r2ut7RomFontCFv();
    void GetTextureFormat__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetTypeName__Q34nw4r3g3d12AnmObjChrResCFv();
    void GetTypeName__Q34nw4r3g3d12AnmObjMatClrCFv();
    void GetTypeName__Q34nw4r3g3d12AnmObjTexPatCFv();
    void GetTypeName__Q34nw4r3g3d12ScnMdlExpandCFv();
    void GetTypeName__Q34nw4r3g3d12ScnMdlSimpleCFv();
    void GetTypeName__Q34nw4r3g3d13AnmObjChrNodeCFv();
    void GetTypeName__Q34nw4r3g3d14AnmObjChrBlendCFv();
    void GetTypeName__Q34nw4r3g3d15AnmObjMatClrResCFv();
    void GetTypeName__Q34nw4r3g3d15AnmObjTexPatResCFv();
    void GetTypeName__Q34nw4r3g3d15AnmObjTexSrtResCFv();
    void GetTypeName__Q34nw4r3g3d20AnmObjTexSrtOverrideCFv();
    void GetTypeName__Q34nw4r3g3d6AnmScnCFv();
    void GetTypeName__Q34nw4r3g3d6G3dObjCFv();
    void GetTypeName__Q34nw4r3g3d6ScnMdlCFv();
    void GetTypeName__Q34nw4r3g3d6ScnObjCFv();
    void GetTypeName__Q34nw4r3g3d7ScnProcCFv();
    void GetTypeName__Q34nw4r3g3d7ScnRootCFv();
    void GetTypeName__Q34nw4r3g3d8ScnGroupCFv();
    void GetTypeName__Q34nw4r3g3d9AnmObjChrCFv();
    void GetTypeName__Q34nw4r3g3d9AnmScnResCFv();
    void GetTypeObj__Q34nw4r3g3d12AnmObjChrResCFv();
    void GetTypeObj__Q34nw4r3g3d12AnmObjMatClrCFv();
    void GetTypeObj__Q34nw4r3g3d12AnmObjTexPatCFv();
    void GetTypeObj__Q34nw4r3g3d12ScnMdlExpandCFv();
    void GetTypeObj__Q34nw4r3g3d12ScnMdlSimpleCFv();
    void GetTypeObj__Q34nw4r3g3d13AnmObjChrNodeCFv();
    void GetTypeObj__Q34nw4r3g3d14AnmObjChrBlendCFv();
    void GetTypeObj__Q34nw4r3g3d15AnmObjMatClrResCFv();
    void GetTypeObj__Q34nw4r3g3d15AnmObjTexPatResCFv();
    void GetTypeObj__Q34nw4r3g3d15AnmObjTexSrtResCFv();
    void GetTypeObj__Q34nw4r3g3d20AnmObjTexSrtOverrideCFv();
    void GetTypeObj__Q34nw4r3g3d6AnmScnCFv();
    void GetTypeObj__Q34nw4r3g3d6G3dObjCFv();
    void GetTypeObj__Q34nw4r3g3d6ScnMdlCFv();
    void GetTypeObj__Q34nw4r3g3d6ScnObjCFv();
    void GetTypeObj__Q34nw4r3g3d7ScnProcCFv();
    void GetTypeObj__Q34nw4r3g3d7ScnRootCFv();
    void GetTypeObj__Q34nw4r3g3d8ScnGroupCFv();
    void GetTypeObj__Q34nw4r3g3d9AnmObjChrCFv();
    void GetTypeObj__Q34nw4r3g3d9AnmScnResCFv();
    void GetType__Q34nw4r2ut7RomFontCFv();
    void GetType__Q44nw4r2ut6detail11ResFontBaseCFv();
    void GetUpdateRate__Q34nw4r3g3d12AnmObjChrResCFv();
    void GetUpdateRate__Q34nw4r3g3d13AnmObjChrNodeCFv();
    void GetUpdateRate__Q34nw4r3g3d15AnmObjMatClrResCFv();
    void GetUpdateRate__Q34nw4r3g3d15AnmObjTexPatResCFv();
    void GetUpdateRate__Q34nw4r3g3d15AnmObjTexSrtResCFv();
    void GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv();
    void GetValueForSortOpa__Q34nw4r3g3d6ScnObjCFv();
    void GetValueForSortXlu__Q34nw4r3g3d6ScnObjCFv();
    void GetVtxColorElement__Q34nw4r3lyt4PaneCFUl();
    void GetVtxColorElement__Q34nw4r3lyt6WindowCFUl();
    void GetVtxColorElement__Q34nw4r3lyt7PictureCFUl();
    void GetVtxColorElement__Q34nw4r3lyt7TextBoxCFUl();
    void GetVtxColor__Q34nw4r3lyt4PaneCFUl();
    void GetVtxColor__Q34nw4r3lyt6WindowCFUl();
    void GetVtxColor__Q34nw4r3lyt7PictureCFUl();
    void GetWaveSoundData__Q44nw4r3snd18SoundArchivePlayer11WsdCallbackCFPQ44nw4r3snd6detail13WaveSoundInfoPQ44nw4r3snd6detail17WaveSoundNoteInfoPQ44nw4r3snd6detail8WaveInfoPCviiUl();
    void GetWeight__Q34nw4r3g3d14AnmObjChrBlendCFi();
    void GetWeight__Q34nw4r3g3d9AnmObjChrCFi();
    void GetWidth__Q34nw4r2ut7RomFontCFv();
    void GetWidth__Q44nw4r2ut6detail11ResFontBaseCFv();
    void HasGlyph__Q34nw4r2ut7RomFontCFUs();
    void HasGlyph__Q44nw4r2ut6detail11ResFontBaseCFUs();
    void HasGlyph__Q44nw4r2ut6detail15ArchiveFontBaseCFUs();
    void HasSpecularLight__Q34nw4r3g3d9AnmScnResCFUl();
    void InitParam__Q44nw4r3snd6detail10BasicSoundFv();
    void InitParam__Q44nw4r3snd6detail8SeqSoundFv();
    void InitParam__Q44nw4r3snd6detail9StrmSoundFv();
    void Insert__Q34nw4r3g3d12ScnMdlExpandFUlPQ34nw4r3g3d6ScnObj();
    void Insert__Q34nw4r3g3d8ScnGroupFUlPQ34nw4r3g3d6ScnObj();
    void InvalidateData__Q34nw4r3snd18SoundArchivePlayerFPCvPCv();
    void InvalidateData__Q44nw4r3snd6detail5VoiceFPCvPCv();
    void InvalidateData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv();
    void InvalidateData__Q44nw4r3snd6detail9WsdPlayerFPCvPCv();
    void InvalidateWaveData__Q34nw4r3snd18SoundArchivePlayerFPCvPCv();
    void InvalidateWaveData__Q44nw4r3snd6detail5VoiceFPCvPCv();
    void InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv();
    void InvalidateWaveData__Q44nw4r3snd6detail9WsdPlayerFPCvPCv();
    void IsActive__Q44nw4r3snd6detail10StrmPlayerCFv();
    void IsActive__Q44nw4r3snd6detail9SeqPlayerCFv();
    void IsActive__Q44nw4r3snd6detail9WsdPlayerCFv();
    void IsAttachedTempSpecialHandle__Q44nw4r3snd6detail8SeqSoundFv();
    void IsAttachedTempSpecialHandle__Q44nw4r3snd6detail9StrmSoundFv();
    void IsAttachedTempSpecialHandle__Q44nw4r3snd6detail9WaveSoundFv();
    void IsBusy__Q34nw4r2ut8IOStreamCFv();
    void IsDerivedFrom__Q34nw4r3g3d12AnmObjChrResCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d12AnmObjMatClrCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d12AnmObjTexPatCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d12AnmObjTexSrtCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d12ScnMdlExpandCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d13AnmObjChrNodeCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d14AnmObjChrBlendCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d15AnmObjMatClrResCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d15AnmObjTexPatResCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d15AnmObjTexSrtResCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d6AnmScnCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d6G3dObjCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d6ScnMdlCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d6ScnObjCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d7ScnProcCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d7ScnRootCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d8ScnGroupCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d9AnmObjChrCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsDerivedFrom__Q34nw4r3g3d9AnmScnResCFQ44nw4r3g3d6G3dObj7TypeObj();
    void IsPause__Q44nw4r3snd6detail10StrmPlayerCFv();
    void IsPause__Q44nw4r3snd6detail9SeqPlayerCFv();
    void IsPause__Q44nw4r3snd6detail9WsdPlayerCFv();
    void IsPrepared__Q44nw4r3snd6detail8SeqSoundCFv();
    void IsPrepared__Q44nw4r3snd6detail9StrmSoundCFv();
    void IsPrepared__Q44nw4r3snd6detail9WaveSoundCFv();
    void IsStarted__Q44nw4r3snd6detail10StrmPlayerCFv();
    void IsStarted__Q44nw4r3snd6detail9SeqPlayerCFv();
    void IsStarted__Q44nw4r3snd6detail9WsdPlayerCFv();
    void LoadMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo();
    void LoadMtx__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo();
    void NoteOnCommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackiilb();
    void NoteOn__Q44nw4r3snd18SoundArchivePlayer17SeqNoteOnCallbackFPQ44nw4r3snd6detail9SeqPlayeriRCQ44nw4r3snd6detail10NoteOnInfo();
    void OnCancel__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv();
    void OnCancel__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTaskFv();
    void OnCancel__Q54nw4r3snd6detail8SeqSound11SeqLoadTaskFv();
    void OnChangeOutputMode__Q34nw4r3snd11FxDelayDpl2Fv();
    void OnChangeOutputMode__Q34nw4r3snd12FxChorusDpl2Fv();
    void OnChangeOutputMode__Q34nw4r3snd15FxReverbStdDpl2Fv();
    void OnShutdownSoundThread__Q44nw4r3snd6detail10StrmPlayerFv();
    void OnShutdownSoundThread__Q44nw4r3snd6detail9SeqPlayerFv();
    void OnShutdownSoundThread__Q44nw4r3snd6detail9WsdPlayerFv();
    void OnUpdateFrameSoundThread__Q44nw4r3snd6detail10StrmPlayerFv();
    void OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv();
    void OnUpdateFrameSoundThread__Q44nw4r3snd6detail9WsdPlayerFv();
    void OnUpdatePlayerPriority__Q44nw4r3snd6detail10BasicSoundFv();
    void OnUpdatePlayerPriority__Q44nw4r3snd6detail8SeqSoundFv();
    void OnUpdatePlayerPriority__Q44nw4r3snd6detail9StrmSoundFv();
    void OnUpdatePlayerPriority__Q44nw4r3snd6detail9WaveSoundFv();
    void OnUpdateVoiceSoundThread__Q44nw4r3snd6detail10StrmPlayerFv();
    void OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv();
    void OpenExtStream__Q34nw4r3snd18MemorySoundArchiveCFPviPCcUlUl();
    void OpenStream__Q34nw4r3snd18MemorySoundArchiveCFPviUlUl();
    void Parse__Q44nw4r3snd6detail11MmlSeqTrackFb();
    void Pause__Q44nw4r3snd6detail10StrmPlayerFb();
    void Pause__Q44nw4r3snd6detail9SeqPlayerFb();
    void Pause__Q44nw4r3snd6detail9WsdPlayerFb();
    void PlayPolicy_Loop__Q24nw4r3g3dFfff();
    void PlayPolicy_Onetime__Q24nw4r3g3dFfff();
    void ReadAsync__Q34nw4r2ut8IOStreamFPvUlPFlPQ34nw4r2ut8IOStreamPv_vPv();
    void ReadNextCharCP1252__Q34nw4r2ut14CharStrmReaderFv();
    void ReadNextCharSJIS__Q34nw4r2ut14CharStrmReaderFv();
    void ReadNextCharUTF16__Q34nw4r2ut14CharStrmReaderFv();
    void ReadNextCharUTF8__Q34nw4r2ut14CharStrmReaderFv();
    void Read__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamFPvUl();
    void ReleaseWorkBuffer__Q34nw4r3snd11FxDelayDpl2Fv();
    void ReleaseWorkBuffer__Q34nw4r3snd12FxChorusDpl2Fv();
    void ReleaseWorkBuffer__Q34nw4r3snd15FxReverbStdDpl2Fv();
    void Release__Q34nw4r3g3d12AnmObjChrResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption();
    void Release__Q34nw4r3g3d12AnmObjMatClrFv();
    void Release__Q34nw4r3g3d12AnmObjTexPatFv();
    void Release__Q34nw4r3g3d12AnmObjTexSrtFv();
    void Release__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption();
    void Release__Q34nw4r3g3d13AnmObjChrNodeFv();
    void Release__Q34nw4r3g3d9AnmObjChrFv();
    void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObj();
    void RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType();
    void RemoveAnmObj__Q34nw4r3g3d6ScnMdlFPQ34nw4r3g3d6AnmObj();
    void RemoveAnmObj__Q34nw4r3g3d6ScnMdlFQ44nw4r3g3d12ScnMdlSimple10AnmObjType();
    void Remove__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObj();
    void Remove__Q34nw4r3g3d12ScnMdlExpandFUl();
    void Remove__Q34nw4r3g3d8ScnGroupFPQ34nw4r3g3d6ScnObj();
    void Remove__Q34nw4r3g3d8ScnGroupFUl();
    void Reset__Q44nw4r3g3d8G3DState11IndMtxOpStdFv();
    void Seek__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamFlUl();
    void SetAlternateChar__Q34nw4r2ut7RomFontFUs();
    void SetAlternateChar__Q44nw4r2ut6detail11ResFontBaseFUs();
    void SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb();
    void SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb();
    void SetAnimationEnable__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransformb();
    void SetAnimationEnable__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransformb();
    void SetAnimationEnable__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResourceb();
    void SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType();
    void SetAnmObj__Q34nw4r3g3d6ScnMdlFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType();
    void SetColorElement__Q34nw4r3lyt4PaneFUlUc();
    void SetDefaultCharWidths__Q34nw4r2ut7RomFontFRCQ34nw4r2ut10CharWidths();
    void SetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseFRCQ34nw4r2ut10CharWidths();
    void SetFrame__Q34nw4r3g3d12AnmObjChrResFf();
    void SetFrame__Q34nw4r3g3d13AnmObjChrNodeFf();
    void SetFrame__Q34nw4r3g3d15AnmObjMatClrResFf();
    void SetFrame__Q34nw4r3g3d15AnmObjTexPatResFf();
    void SetFrame__Q34nw4r3g3d15AnmObjTexSrtResFf();
    void SetFrame__Q34nw4r3g3d9AnmScnResFf();
    void SetLineFeed__Q34nw4r2ut7RomFontFi();
    void SetLineFeed__Q44nw4r2ut6detail11ResFontBaseFi();
    void SetNrmMapMtx__Q44nw4r3g3d8G3DState11IndMtxOpStdF14_GXIndTexMtxIDPCQ34nw4r4math4VEC3PCQ34nw4r4math5MTX34Q44nw4r3g3d14ResMatMiscData14IndirectMethod();
    void SetResource__Q34nw4r3lyt18AnimTransformBasicFPCQ44nw4r3lyt3res14AnimationBlockPQ34nw4r3lyt16ResourceAccessor();
    void SetResource__Q34nw4r3lyt18AnimTransformBasicFPCQ44nw4r3lyt3res14AnimationBlockPQ34nw4r3lyt16ResourceAccessorUs();
    void SetScnObjOption__Q34nw4r3g3d12ScnMdlSimpleFUlUl();
    void SetScnObjOption__Q34nw4r3g3d6ScnMdlFUlUl();
    void SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl();
    void SetScnObjOption__Q34nw4r3g3d7ScnLeafFUlUl();
    void SetString__Q34nw4r3lyt7TextBoxFPCwUs();
    void SetString__Q34nw4r3lyt7TextBoxFPCwUsUs();
    void SetUpdateRate__Q34nw4r3g3d12AnmObjChrResFf();
    void SetUpdateRate__Q34nw4r3g3d13AnmObjChrNodeFf();
    void SetUpdateRate__Q34nw4r3g3d15AnmObjMatClrResFf();
    void SetUpdateRate__Q34nw4r3g3d15AnmObjTexPatResFf();
    void SetUpdateRate__Q34nw4r3g3d15AnmObjTexSrtResFf();
    void SetUpdateRate__Q34nw4r3g3d9AnmScnResFf();
    void SetVtxColorElement__Q34nw4r3lyt4PaneFUlUc();
    void SetVtxColorElement__Q34nw4r3lyt6WindowFUlUc();
    void SetVtxColorElement__Q34nw4r3lyt7PictureFUlUc();
    void SetVtxColorElement__Q34nw4r3lyt7TextBoxFUlUc();
    void SetVtxColor__Q34nw4r3lyt4PaneFUlQ34nw4r2ut5Color();
    void SetVtxColor__Q34nw4r3lyt6WindowFUlQ34nw4r2ut5Color();
    void SetVtxColor__Q34nw4r3lyt7PictureFUlQ34nw4r2ut5Color();
    void SetVtxColor__Q34nw4r3lyt7TextBoxFUlQ34nw4r2ut5Color();
    void SetWeight__Q34nw4r3g3d14AnmObjChrBlendFif();
    void SetWeight__Q34nw4r3g3d9AnmObjChrFif();
    void SetupGX__Q34nw4r3lyt8MaterialFbUc();
    void Shutdown__Q34nw4r3snd11FxDelayDpl2Fv();
    void Shutdown__Q34nw4r3snd12FxChorusDpl2Fv();
    void Shutdown__Q34nw4r3snd15FxReverbStdDpl2Fv();
    void Shutdown__Q44nw4r3snd6detail10BasicSoundFv();
    void Shutdown__Q44nw4r3snd6detail8SeqSoundFv();
    void Shutdown__Q44nw4r3snd6detail9StrmSoundFv();
    void Shutdown__Q44nw4r3snd6detail9WaveSoundFv();
    void Sort__Q34nw4r3g3d12ScnObjGatherFPFPCQ34nw4r3g3d6ScnObjPCQ34nw4r3g3d6ScnObj_bPFPCQ34nw4r3g3d6ScnObjPCQ34nw4r3g3d6ScnObj_b();
    void Sort__Q34nw4r3g3d12ScnObjGatherFv();
    void StartUp__Q34nw4r3snd11FxDelayDpl2Fv();
    void StartUp__Q34nw4r3snd12FxChorusDpl2Fv();
    void StartUp__Q34nw4r3snd15FxReverbStdDpl2Fv();
    void Start__Q44nw4r3snd6detail10StrmPlayerFv();
    void Start__Q44nw4r3snd6detail9SeqPlayerFv();
    void Start__Q44nw4r3snd6detail9WsdPlayerFv();
    void Stop__Q44nw4r3snd6detail10StrmPlayerFv();
    void Stop__Q44nw4r3snd6detail9SeqPlayerFv();
    void Stop__Q44nw4r3snd6detail9WsdPlayerFv();
    void StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl();
    void Tell__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv();
    void UnbindAllAnimation__Q34nw4r3lyt4PaneFb();
    void UnbindAllAnimation__Q34nw4r3lyt6LayoutFv();
    void UnbindAllAnimation__Q34nw4r3lyt8MaterialFv();
    void UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform();
    void UnbindAnimationSelf__Q34nw4r3lyt6WindowFPQ34nw4r3lyt13AnimTransform();
    void UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb();
    void UnbindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform();
    void UnbindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform();
    void UpdateBuffer__Q34nw4r3snd11FxDelayDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode();
    void UpdateBuffer__Q34nw4r3snd12FxChorusDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode();
    void UpdateBuffer__Q34nw4r3snd15FxReverbStdDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode();
    void UpdateFrame__Q34nw4r3g3d12AnmObjChrResFv();
    void UpdateFrame__Q34nw4r3g3d13AnmObjChrNodeFv();
    void UpdateFrame__Q34nw4r3g3d15AnmObjMatClrResFv();
    void UpdateFrame__Q34nw4r3g3d15AnmObjTexPatResFv();
    void UpdateFrame__Q34nw4r3g3d15AnmObjTexSrtResFv();
    void UpdateFrame__Q34nw4r3g3d9AnmScnResFv();
    void UpdateMoveValue__Q44nw4r3snd6detail10BasicSoundFv();
    void UpdateMoveValue__Q44nw4r3snd6detail9StrmSoundFv();
    void UpdateParam__Q44nw4r3snd6detail10BasicSoundFv();
    void UpdateParam__Q44nw4r3snd6detail9StrmSoundFv();
    void WriteAsync__Q34nw4r2ut8IOStreamFPCvUlPFlPQ34nw4r2ut8IOStreamPv_vPv();
    void Write__Q34nw4r2ut8IOStreamFPCvUl();
    void ZSort__Q34nw4r3g3d12ScnObjGatherFv();
    void __cl__Q44nw4r3g3d8G3DState11IndMtxOpStdFPQ44nw4r3g3d8G3DState13IndTexMtxInfo();
    void __dt__Q34nw4r2ut10PackedFontFv();
    void __dt__Q34nw4r2ut4FontFv();
    void __dt__Q34nw4r2ut7ResFontFv();
    void __dt__Q34nw4r2ut7RomFontFv();
    void __dt__Q34nw4r3g3d12AnmObjChrResFv();
    void __dt__Q34nw4r3g3d12AnmObjMatClrFv();
    void __dt__Q34nw4r3g3d12AnmObjTexPatFv();
    void __dt__Q34nw4r3g3d12AnmObjTexSrtFv();
    void __dt__Q34nw4r3g3d12ScnMdlExpandFv();
    void __dt__Q34nw4r3g3d12ScnMdlSimpleFv();
    void __dt__Q34nw4r3g3d12ScnObjGatherFv();
    void __dt__Q34nw4r3g3d13AnmObjChrNodeFv();
    void __dt__Q34nw4r3g3d14AnmObjChrBlendFv();
    void __dt__Q34nw4r3g3d15AnmObjMatClrResFv();
    void __dt__Q34nw4r3g3d15AnmObjTexPatResFv();
    void __dt__Q34nw4r3g3d15AnmObjTexSrtResFv();
    void __dt__Q34nw4r3g3d6AnmScnFv();
    void __dt__Q34nw4r3g3d6G3dObjFv();
    void __dt__Q34nw4r3g3d6ScnMdlFv();
    void __dt__Q34nw4r3g3d6ScnObjFv();
    void __dt__Q34nw4r3g3d7ScnProcFv();
    void __dt__Q34nw4r3g3d7ScnRootFv();
    void __dt__Q34nw4r3g3d8ScnGroupFv();
    void __dt__Q34nw4r3g3d9AnmObjChrFv();
    void __dt__Q34nw4r3g3d9AnmScnResFv();
    void __dt__Q34nw4r3lyt16ResourceAccessorFv();
    void __dt__Q34nw4r3lyt18AnimTransformBasicFv();
    void __dt__Q34nw4r3lyt19ArcResourceAccessorFv();
    void __dt__Q34nw4r3lyt4PaneFv();
    void __dt__Q34nw4r3lyt5GroupFv();
    void __dt__Q34nw4r3lyt6LayoutFv();
    void __dt__Q34nw4r3lyt6WindowFv();
    void __dt__Q34nw4r3lyt7PictureFv();
    void __dt__Q34nw4r3lyt7TextBoxFv();
    void __dt__Q34nw4r3lyt8BoundingFv();
    void __dt__Q34nw4r3lyt8DrawInfoFv();
    void __dt__Q34nw4r3lyt8MaterialFv();
    void __dt__Q34nw4r3snd11FxDelayDpl2Fv();
    void __dt__Q34nw4r3snd12FxChorusDpl2Fv();
    void __dt__Q34nw4r3snd12SoundArchiveFv();
    void __dt__Q34nw4r3snd15FxReverbStdDpl2Fv();
    void __dt__Q34nw4r3snd18MemorySoundArchiveFv();
    void __dt__Q34nw4r3snd18SoundArchivePlayerFv();
    void __dt__Q44nw4r2ut6detail11ResFontBaseFv();
    void __dt__Q44nw4r2ut6detail15ArchiveFontBaseFv();
    void __dt__Q44nw4r3g3d8G3DState11IndMtxOpStdFv();
    void __dt__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamFv();
    void __dt__Q44nw4r3snd18SoundArchivePlayer11WsdCallbackFv();
    void __dt__Q44nw4r3snd18SoundArchivePlayer17SeqNoteOnCallbackFv();
    void __dt__Q44nw4r3snd6detail10BasicSoundFv();
    void __dt__Q44nw4r3snd6detail10PlayerHeapFv();
    void __dt__Q44nw4r3snd6detail10StrmPlayerFv();
    void __dt__Q44nw4r3snd6detail11BasicPlayerFv();
    void __dt__Q44nw4r3snd6detail11MmlSeqTrackFv();
    void __dt__Q44nw4r3snd6detail15BiquadFilterHpfFv();
    void __dt__Q44nw4r3snd6detail15BiquadFilterLpfFv();
    void __dt__Q44nw4r3snd6detail18BiquadFilterBpf512Fv();
    void __dt__Q44nw4r3snd6detail19BiquadFilterBpf1024Fv();
    void __dt__Q44nw4r3snd6detail19BiquadFilterBpf2048Fv();
    void __dt__Q44nw4r3snd6detail20MmlSeqTrackAllocatorFv();
    void __dt__Q44nw4r3snd6detail5VoiceFv();
    void __dt__Q44nw4r3snd6detail8SeqSoundFv();
    void __dt__Q44nw4r3snd6detail8SeqTrackFv();
    void __dt__Q44nw4r3snd6detail9SeqPlayerFv();
    void __dt__Q44nw4r3snd6detail9StrmSoundFv();
    void __dt__Q44nw4r3snd6detail9WaveSoundFv();
    void __dt__Q44nw4r3snd6detail9WsdPlayerFv();
    void __dt__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv();
    void __dt__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTaskFv();
    void __dt__Q54nw4r3snd6detail8SeqSound11SeqLoadTaskFv();
    void detail_ConvertLabelStringToSoundId__Q34nw4r3snd18SoundArchivePlayerFPCc();
    void detail_GetFileAddress__Q34nw4r3snd18MemorySoundArchiveCFUl();
    void detail_GetRequiredStreamBufferSize__Q34nw4r3snd18MemorySoundArchiveCFv();
    void detail_GetWaveDataFileAddress__Q34nw4r3snd18MemorySoundArchiveCFUl();
    void detail_SetupSound__Q34nw4r3snd18SoundArchivePlayerFPQ34nw4r3snd11SoundHandleUlbPCQ44nw4r3snd14SoundStartable9StartInfo();
}

/* ---- extern declarations for data targets (defined later in this TU, or external) ---- */
extern const u32 lbl_eu_805212D4[];
extern const u32 lbl_eu_805216D8[];
extern const u32 lbl_eu_80521ADC[];
extern u32 lbl_eu_80569828[];
extern u32 lbl_eu_805698A8[];
extern u32 lbl_eu_8056AC30[];
extern u8 lbl_eu_8061FA20[];

/* ======================= .rodata ======================= */
__declspec(align(8)) const u32 lbl_eu_8051D4A0[8] = {
        0x00000004, 0x00000005, 0x000000FF, 0x000000FF,
        0x000000FF, 0x00000007, 0x00000008, 0x00000006,
    };

__declspec(align(8)) const u32 lbl_eu_8051D4C0[16] = {
        0x000000A0, 0x000000A0, 0x000000C0, 0x000000C0,
        0x00000100, 0x00000100, 0x00000120, 0x00000120,
        0x00000160, 0x00000160, 0x00000180, 0x00000180,
        0x000001C0, 0x000001C0, 0x000001E0, 0x000001E0,
    };

__declspec(align(8)) const u32 lbl_eu_8051D500[8] = {
        0, 0, 0, 0,
        0, 0, 0, 0,
    };

__declspec(align(8)) const u32 lbl_eu_8051D520[4] = {
        0x0000000A, 0x416E6D4F, 0x626A5669, 0x73000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D530[6] = {
        0x0000000D, 0x416E6D4F, 0x626A4D61, 0x74436C72,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D548[6] = {
        0x00000010, 0x416E6D4F, 0x626A4D61, 0x74436C72,
        0x52657300, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D560[6] = {
        0x0000000D, 0x416E6D4F, 0x626A5465, 0x78506174,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D578[6] = {
        0x00000010, 0x416E6D4F, 0x626A5465, 0x78506174,
        0x52657300, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D590[6] = {
        0x0000000D, 0x416E6D4F, 0x626A5465, 0x78537274,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D5A8[6] = {
        0x00000010, 0x416E6D4F, 0x626A5465, 0x78537274,
        0x52657300, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D5C0[4] = {
        0x0000000A, 0x416E6D4F, 0x626A4368, 0x72000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D5D0[5] = {
        0x0000000E, 0x416E6D4F, 0x626A4368, 0x724E6F64,
        0x65000000,
    };

__declspec(align(4)) const u32 lbl_eu_8051D5E4[5] = {
        0x0000000F, 0x416E6D4F, 0x626A4368, 0x72426C65,
        0x6E640000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D5F8[6] = {
        0x0000000D, 0x416E6D4F, 0x626A4368, 0x72526573,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D610[4] = {
        0x0000000A, 0x416E6D4F, 0x626A5368, 0x70000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D620[4] = {
        0x00000007, 0x416E6D53, 0x636E0000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D630[4] = {
        0x0000000A, 0x416E6D53, 0x636E5265, 0x73000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D640[4] = {
        0x00000007, 0x4733644F, 0x626A0000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D650[4] = {
        0x00000007, 0x416E6D4F, 0x626A0000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D660[12] = {
        0x3F000000, 0x00000000, 0x00000000, 0x3F000000,
        0x00000000, 0xBF000000, 0x00000000, 0x3F000000,
        0x00000000, 0x00000000, 0x00000000, 0x3F800000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D690[4] = {
        0x00000000, 0x00000002, 0x00000001, 0x00000003,
    };

__declspec(align(8)) const u32 lbl_eu_8051D6A0[8] = {
        0x00000000, 0, 0, 0,
        0, 0, 0, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D6C0[7] = {
        0, 0, 0, 0,
        0, 0, 0,
    };

__declspec(align(4)) const u32 lbl_eu_8051D6DC[7] = {
        0, 0, 0, 0,
        0, 0, 0,
    };

__declspec(align(8)) const u32 lbl_eu_8051D6F8[7] = {
        0, 0, 0, 0,
        0, 0, 0,
    };

__declspec(align(4)) const u32 lbl_eu_8051D714[7] = {
        0, 0, 0, 0,
        0, 0, 0,
    };

__declspec(align(8)) const u32 lbl_eu_8051D730[7] = {
        0, 0, 0, 0,
        0, 0, 0,
    };

__declspec(align(4)) const u32 lbl_eu_8051D74C[7] = {
        0, 0, 0, 0,
        0, 0, 0,
    };

__declspec(align(8)) const u32 lbl_eu_8051D768[4] = {
        0x00000007, 0x53636E4F, 0x626A0000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D778[4] = {
        0x00000008, 0x53636E4C, 0x65616600, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D788[4] = {
        0x00000009, 0x53636E47, 0x726F7570, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D798[4] = {
        0x00000008, 0x53636E52, 0x6F6F7400, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D7A8[6] = {
        0x0000000D, 0x53636E4D, 0x646C5369, 0x6D706C65,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D7C0[4] = {
        0x00000007, 0x53636E4D, 0x646C0000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D7D0[6] = {
        0x0000000D, 0x53636E4D, 0x646C4578, 0x70616E64,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D7E8[4] = {
        0x00000008, 0x53636E50, 0x726F6300, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051D7F8[1028] = {
        0x00000000, 0x3F800000, 0x3CC90A35, 0xB99DCF89,
        0x3CC90A35, 0x3F7FEC46, 0x3CC8EAC0, 0xBA6CB74E,
        0x3D48FB87, 0x3F7FB107, 0x3CC8ADEF, 0xBAC5436C,
        0x3D96A93F, 0x3F7F4E66, 0x3CC851A8, 0xBB0A04D1,
        0x3DC8BD23, 0x3F7EC472, 0x3CC7D5ED, 0xBB31465F,
        0x3DFAB29E, 0x3F7E132B, 0x3CC73ABD, 0xBB587725,
        0x3E164064, 0x3F7D3AB4, 0x3CC68017, 0xBB7F865D,
        0x3E2F10AA, 0x3F7C3B2E, 0x3CC5A815, 0xBB9331A1,
        0x3E47C5AC, 0x3F7B14BA, 0x3CC4B2B7, 0xBBA68F4B,
        0x3E605C03, 0x3F79C79B, 0x3CC39DE5, 0xBBB9D3CC,
        0x3E78CFC0, 0x3F7853F4, 0x3CC26BB5, 0xBBCCF6BE,
        0x3E888E9B, 0x3F76BA06, 0x3CC11A11, 0xBBE00086,
        0x3E94A03C, 0x3F74FA05, 0x3CBFAB11, 0xBBF2E05D,
        0x3EA09AED, 0x3F731444, 0x3CBE1EB4, 0xBC02CF53,
        0x3EAC7CD9, 0x3F710907, 0x3CBC74FB, 0xBC0C197E,
        0x3EB84428, 0x3F6ED8A1, 0x3CBAADE6, 0xBC154EB1,
        0x3EC3EF07, 0x3F6C8366, 0x3CB8CB8E, 0xBC1E6EEB,
        0x3ECF7BC0, 0x3F6A09AB, 0x3CB6CBDA, 0xBC2771C9,
        0x3EDAE87D, 0x3F676BD3, 0x3CB4AEC9, 0xBC305FAF,
        0x3EE6336A, 0x3F64AA54, 0x3CB27675, 0xBC393038,
        0x3EF15AF3, 0x3F61C593, 0x3CB022DD, 0xBC41E365,
        0x3EFC5D20, 0x3F5EBE06, 0x3CADB61C, 0xBC4A7935,
        0x3F039C41, 0x3F5B9421, 0x3CAB2BFE, 0xBC52F1AA,
        0x3F08F5A1, 0x3F58485A, 0x3CA888B5, 0xBC5B4891,
        0x3F0E39D6, 0x3F54DB38, 0x3CA5CA2A, 0xBC637DE9,
        0x3F136827, 0x3F514D40, 0x3CA2F273, 0xBC6B8D82,
        0x3F187FBB, 0x3F4D9F0A, 0x3CA00193, 0xBC737B8D,
        0x3F1D7FD8, 0x3F49D10B, 0x3C9CF9A0, 0xBC7B43D9,
        0x3F226794, 0x3F45E3FC, 0x3C99D884, 0xBC817332,
        0x3F273659, 0x3F41D873, 0x3C969E3D, 0xBC852F7F,
        0x3F2BEB4A, 0x3F3DAEF7, 0x3C934EFD, 0xBC88D6D4,
        0x3F3085C2, 0x3F396840, 0x3C8FE692, 0xBC8C692F,
        0x3F3504F7, 0x3F3504F7, 0x3C8C692F, 0xBC8FE692,
        0x3F396840, 0x3F3085C2, 0x3C88D6D4, 0xBC934EFD,
        0x3F3DAEF7, 0x3F2BEB4A, 0x3C852F7F, 0xBC969E3D,
        0x3F41D873, 0x3F273659, 0x3C817332, 0xBC99D884,
        0x3F45E3FC, 0x3F226794, 0x3C7B43D9, 0xBC9CF9A0,
        0x3F49D10B, 0x3F1D7FD8, 0x3C737B8D, 0xBCA00193,
        0x3F4D9F0A, 0x3F187FBB, 0x3C6B8D82, 0xBCA2F273,
        0x3F514D40, 0x3F136827, 0x3C637DE9, 0xBCA5CA2A,
        0x3F54DB38, 0x3F0E39D6, 0x3C5B4891, 0xBCA888B5,
        0x3F58485A, 0x3F08F5A1, 0x3C52F1AA, 0xBCAB2BFE,
        0x3F5B9421, 0x3F039C41, 0x3C4A7935, 0xBCADB61C,
        0x3F5EBE06, 0x3EFC5D20, 0x3C41E365, 0xBCB022DD,
        0x3F61C593, 0x3EF15AF3, 0x3C393038, 0xBCB27675,
        0x3F64AA54, 0x3EE6336A, 0x3C305FAF, 0xBCB4AEC9,
        0x3F676BD3, 0x3EDAE87D, 0x3C2771C9, 0xBCB6CBDA,
        0x3F6A09AB, 0x3ECF7BC0, 0x3C1E6EEB, 0xBCB8CB8E,
        0x3F6C8366, 0x3EC3EF07, 0x3C154EB1, 0xBCBAADE6,
        0x3F6ED8A1, 0x3EB84428, 0x3C0C197E, 0xBCBC74FB,
        0x3F710907, 0x3EAC7CD9, 0x3C02CF53, 0xBCBE1EB4,
        0x3F731444, 0x3EA09AED, 0x3BF2E05D, 0xBCBFAB11,
        0x3F74FA05, 0x3E94A03C, 0x3BE00086, 0xBCC11A11,
        0x3F76BA06, 0x3E888E9B, 0x3BCCF6BE, 0xBCC26BB5,
        0x3F7853F4, 0x3E78CFC0, 0x3BB9D3CC, 0xBCC39DE5,
        0x3F79C79B, 0x3E605C03, 0x3BA68F4B, 0xBCC4B2B7,
        0x3F7B14BA, 0x3E47C5AC, 0x3B9331A1, 0xBCC5A815,
        0x3F7C3B2E, 0x3E2F10AA, 0x3B7F865D, 0xBCC68017,
        0x3F7D3AB4, 0x3E164064, 0x3B587725, 0xBCC73ABD,
        0x3F7E132B, 0x3DFAB29E, 0x3B31465F, 0xBCC7D5ED,
        0x3F7EC472, 0x3DC8BD23, 0x3B0A04D1, 0xBCC851A8,
        0x3F7F4E66, 0x3D96A93F, 0x3AC5436C, 0xBCC8ADEF,
        0x3F7FB107, 0x3D48FB87, 0x3A6CB74E, 0xBCC8EAC0,
        0x3F7FEC46, 0x3CC90A35, 0x399DCF89, 0xBCC90A35,
        0x3F800000, 0x00000000, 0xB99DCF89, 0xBCC90A35,
        0x3F7FEC46, 0xBCC90A35, 0xBA6CB74E, 0xBCC8EAC0,
        0x3F7FB107, 0xBD48FB87, 0xBAC5436C, 0xBCC8ADEF,
        0x3F7F4E66, 0xBD96A93F, 0xBB0A04D1, 0xBCC851A8,
        0x3F7EC472, 0xBDC8BD23, 0xBB31465F, 0xBCC7D5ED,
        0x3F7E132B, 0xBDFAB29E, 0xBB587725, 0xBCC73ABD,
        0x3F7D3AB4, 0xBE164064, 0xBB7F865D, 0xBCC68017,
        0x3F7C3B2E, 0xBE2F10AA, 0xBB9331A1, 0xBCC5A815,
        0x3F7B14BA, 0xBE47C5AC, 0xBBA68F4B, 0xBCC4B2B7,
        0x3F79C79B, 0xBE605C03, 0xBBB9D3CC, 0xBCC39DE5,
        0x3F7853F4, 0xBE78CFC0, 0xBBCCF6BE, 0xBCC26BB5,
        0x3F76BA06, 0xBE888E9B, 0xBBE00086, 0xBCC11A11,
        0x3F74FA05, 0xBE94A03C, 0xBBF2E05D, 0xBCBFAB11,
        0x3F731444, 0xBEA09AED, 0xBC02CF53, 0xBCBE1EB4,
        0x3F710907, 0xBEAC7CD9, 0xBC0C197E, 0xBCBC74FB,
        0x3F6ED8A1, 0xBEB84428, 0xBC154EB1, 0xBCBAADE6,
        0x3F6C8366, 0xBEC3EF07, 0xBC1E6EEB, 0xBCB8CB8E,
        0x3F6A09AB, 0xBECF7BC0, 0xBC2771C9, 0xBCB6CBDA,
        0x3F676BD3, 0xBEDAE87D, 0xBC305FAF, 0xBCB4AEC9,
        0x3F64AA54, 0xBEE6336A, 0xBC393038, 0xBCB27675,
        0x3F61C593, 0xBEF15AF3, 0xBC41E365, 0xBCB022DD,
        0x3F5EBE06, 0xBEFC5D20, 0xBC4A7935, 0xBCADB61C,
        0x3F5B9421, 0xBF039C41, 0xBC52F1AA, 0xBCAB2BFE,
        0x3F58485A, 0xBF08F5A1, 0xBC5B4891, 0xBCA888B5,
        0x3F54DB38, 0xBF0E39D6, 0xBC637DE9, 0xBCA5CA2A,
        0x3F514D40, 0xBF136827, 0xBC6B8D82, 0xBCA2F273,
        0x3F4D9F0A, 0xBF187FBB, 0xBC737B8D, 0xBCA00193,
        0x3F49D10B, 0xBF1D7FD8, 0xBC7B43D9, 0xBC9CF9A0,
        0x3F45E3FC, 0xBF226794, 0xBC817332, 0xBC99D884,
        0x3F41D873, 0xBF273659, 0xBC852F7F, 0xBC969E3D,
        0x3F3DAEF7, 0xBF2BEB4A, 0xBC88D6D4, 0xBC934EFD,
        0x3F396840, 0xBF3085C2, 0xBC8C692F, 0xBC8FE692,
        0x3F3504F7, 0xBF3504F7, 0xBC8FE692, 0xBC8C692F,
        0x3F3085C2, 0xBF396840, 0xBC934EFD, 0xBC88D6D4,
        0x3F2BEB4A, 0xBF3DAEF7, 0xBC969E3D, 0xBC852F7F,
        0x3F273659, 0xBF41D873, 0xBC99D884, 0xBC817332,
        0x3F226794, 0xBF45E3FC, 0xBC9CF9A0, 0xBC7B43D9,
        0x3F1D7FD8, 0xBF49D10B, 0xBCA00193, 0xBC737B8D,
        0x3F187FBB, 0xBF4D9F0A, 0xBCA2F273, 0xBC6B8D82,
        0x3F136827, 0xBF514D40, 0xBCA5CA2A, 0xBC637DE9,
        0x3F0E39D6, 0xBF54DB38, 0xBCA888B5, 0xBC5B4891,
        0x3F08F5A1, 0xBF58485A, 0xBCAB2BFE, 0xBC52F1AA,
        0x3F039C41, 0xBF5B9421, 0xBCADB61C, 0xBC4A7935,
        0x3EFC5D20, 0xBF5EBE06, 0xBCB022DD, 0xBC41E365,
        0x3EF15AF3, 0xBF61C593, 0xBCB27675, 0xBC393038,
        0x3EE6336A, 0xBF64AA54, 0xBCB4AEC9, 0xBC305FAF,
        0x3EDAE87D, 0xBF676BD3, 0xBCB6CBDA, 0xBC2771C9,
        0x3ECF7BC0, 0xBF6A09AB, 0xBCB8CB8E, 0xBC1E6EEB,
        0x3EC3EF07, 0xBF6C8366, 0xBCBAADE6, 0xBC154EB1,
        0x3EB84428, 0xBF6ED8A1, 0xBCBC74FB, 0xBC0C197E,
        0x3EAC7CD9, 0xBF710907, 0xBCBE1EB4, 0xBC02CF53,
        0x3EA09AED, 0xBF731444, 0xBCBFAB11, 0xBBF2E05D,
        0x3E94A03C, 0xBF74FA05, 0xBCC11A11, 0xBBE00086,
        0x3E888E9B, 0xBF76BA06, 0xBCC26BB5, 0xBBCCF6BE,
        0x3E78CFC0, 0xBF7853F4, 0xBCC39DE5, 0xBBB9D3CC,
        0x3E605C03, 0xBF79C79B, 0xBCC4B2B7, 0xBBA68F4B,
        0x3E47C5AC, 0xBF7B14BA, 0xBCC5A815, 0xBB9331A1,
        0x3E2F10AA, 0xBF7C3B2E, 0xBCC68017, 0xBB7F865D,
        0x3E164064, 0xBF7D3AB4, 0xBCC73ABD, 0xBB587725,
        0x3DFAB29E, 0xBF7E132B, 0xBCC7D5ED, 0xBB31465F,
        0x3DC8BD23, 0xBF7EC472, 0xBCC851A8, 0xBB0A04D1,
        0x3D96A93F, 0xBF7F4E66, 0xBCC8ADEF, 0xBAC5436C,
        0x3D48FB87, 0xBF7FB107, 0xBCC8EAC0, 0xBA6CB74E,
        0x3CC90A35, 0xBF7FEC46, 0xBCC90A35, 0xB99DCF89,
        0x00000000, 0xBF800000, 0xBCC90A35, 0x399DCF89,
        0xBCC90A35, 0xBF7FEC46, 0xBCC8EAC0, 0x3A6CB74E,
        0xBD48FB87, 0xBF7FB107, 0xBCC8ADEF, 0x3AC5436C,
        0xBD96A93F, 0xBF7F4E66, 0xBCC851A8, 0x3B0A04D1,
        0xBDC8BD23, 0xBF7EC472, 0xBCC7D5ED, 0x3B31465F,
        0xBDFAB29E, 0xBF7E132B, 0xBCC73ABD, 0x3B587725,
        0xBE164064, 0xBF7D3AB4, 0xBCC68017, 0x3B7F865D,
        0xBE2F10AA, 0xBF7C3B2E, 0xBCC5A815, 0x3B9331A1,
        0xBE47C5AC, 0xBF7B14BA, 0xBCC4B2B7, 0x3BA68F4B,
        0xBE605C03, 0xBF79C79B, 0xBCC39DE5, 0x3BB9D3CC,
        0xBE78CFC0, 0xBF7853F4, 0xBCC26BB5, 0x3BCCF6BE,
        0xBE888E9B, 0xBF76BA06, 0xBCC11A11, 0x3BE00086,
        0xBE94A03C, 0xBF74FA05, 0xBCBFAB11, 0x3BF2E05D,
        0xBEA09AED, 0xBF731444, 0xBCBE1EB4, 0x3C02CF53,
        0xBEAC7CD9, 0xBF710907, 0xBCBC74FB, 0x3C0C197E,
        0xBEB84428, 0xBF6ED8A1, 0xBCBAADE6, 0x3C154EB1,
        0xBEC3EF07, 0xBF6C8366, 0xBCB8CB8E, 0x3C1E6EEB,
        0xBECF7BC0, 0xBF6A09AB, 0xBCB6CBDA, 0x3C2771C9,
        0xBEDAE87D, 0xBF676BD3, 0xBCB4AEC9, 0x3C305FAF,
        0xBEE6336A, 0xBF64AA54, 0xBCB27675, 0x3C393038,
        0xBEF15AF3, 0xBF61C593, 0xBCB022DD, 0x3C41E365,
        0xBEFC5D20, 0xBF5EBE06, 0xBCADB61C, 0x3C4A7935,
        0xBF039C41, 0xBF5B9421, 0xBCAB2BFE, 0x3C52F1AA,
        0xBF08F5A1, 0xBF58485A, 0xBCA888B5, 0x3C5B4891,
        0xBF0E39D6, 0xBF54DB38, 0xBCA5CA2A, 0x3C637DE9,
        0xBF136827, 0xBF514D40, 0xBCA2F273, 0x3C6B8D82,
        0xBF187FBB, 0xBF4D9F0A, 0xBCA00193, 0x3C737B8D,
        0xBF1D7FD8, 0xBF49D10B, 0xBC9CF9A0, 0x3C7B43D9,
        0xBF226794, 0xBF45E3FC, 0xBC99D884, 0x3C817332,
        0xBF273659, 0xBF41D873, 0xBC969E3D, 0x3C852F7F,
        0xBF2BEB4A, 0xBF3DAEF7, 0xBC934EFD, 0x3C88D6D4,
        0xBF3085C2, 0xBF396840, 0xBC8FE692, 0x3C8C692F,
        0xBF3504F7, 0xBF3504F7, 0xBC8C692F, 0x3C8FE692,
        0xBF396840, 0xBF3085C2, 0xBC88D6D4, 0x3C934EFD,
        0xBF3DAEF7, 0xBF2BEB4A, 0xBC852F7F, 0x3C969E3D,
        0xBF41D873, 0xBF273659, 0xBC817332, 0x3C99D884,
        0xBF45E3FC, 0xBF226794, 0xBC7B43D9, 0x3C9CF9A0,
        0xBF49D10B, 0xBF1D7FD8, 0xBC737B8D, 0x3CA00193,
        0xBF4D9F0A, 0xBF187FBB, 0xBC6B8D82, 0x3CA2F273,
        0xBF514D40, 0xBF136827, 0xBC637DE9, 0x3CA5CA2A,
        0xBF54DB38, 0xBF0E39D6, 0xBC5B4891, 0x3CA888B5,
        0xBF58485A, 0xBF08F5A1, 0xBC52F1AA, 0x3CAB2BFE,
        0xBF5B9421, 0xBF039C41, 0xBC4A7935, 0x3CADB61C,
        0xBF5EBE06, 0xBEFC5D20, 0xBC41E365, 0x3CB022DD,
        0xBF61C593, 0xBEF15AF3, 0xBC393038, 0x3CB27675,
        0xBF64AA54, 0xBEE6336A, 0xBC305FAF, 0x3CB4AEC9,
        0xBF676BD3, 0xBEDAE87D, 0xBC2771C9, 0x3CB6CBDA,
        0xBF6A09AB, 0xBECF7BC0, 0xBC1E6EEB, 0x3CB8CB8E,
        0xBF6C8366, 0xBEC3EF07, 0xBC154EB1, 0x3CBAADE6,
        0xBF6ED8A1, 0xBEB84428, 0xBC0C197E, 0x3CBC74FB,
        0xBF710907, 0xBEAC7CD9, 0xBC02CF53, 0x3CBE1EB4,
        0xBF731444, 0xBEA09AED, 0xBBF2E05D, 0x3CBFAB11,
        0xBF74FA05, 0xBE94A03C, 0xBBE00086, 0x3CC11A11,
        0xBF76BA06, 0xBE888E9B, 0xBBCCF6BE, 0x3CC26BB5,
        0xBF7853F4, 0xBE78CFC0, 0xBBB9D3CC, 0x3CC39DE5,
        0xBF79C79B, 0xBE605C03, 0xBBA68F4B, 0x3CC4B2B7,
        0xBF7B14BA, 0xBE47C5AC, 0xBB9331A1, 0x3CC5A815,
        0xBF7C3B2E, 0xBE2F10AA, 0xBB7F865D, 0x3CC68017,
        0xBF7D3AB4, 0xBE164064, 0xBB587725, 0x3CC73ABD,
        0xBF7E132B, 0xBDFAB29E, 0xBB31465F, 0x3CC7D5ED,
        0xBF7EC472, 0xBDC8BD23, 0xBB0A04D1, 0x3CC851A8,
        0xBF7F4E66, 0xBD96A93F, 0xBAC5436C, 0x3CC8ADEF,
        0xBF7FB107, 0xBD48FB87, 0xBA6CB74E, 0x3CC8EAC0,
        0xBF7FEC46, 0xBCC90A35, 0xB99DCF89, 0x3CC90A35,
        0xBF800000, 0x80000000, 0x399DCF89, 0x3CC90A35,
        0xBF7FEC46, 0x3CC90A35, 0x3A6CB74E, 0x3CC8EAC0,
        0xBF7FB107, 0x3D48FB87, 0x3AC5436C, 0x3CC8ADEF,
        0xBF7F4E66, 0x3D96A93F, 0x3B0A04D1, 0x3CC851A8,
        0xBF7EC472, 0x3DC8BD23, 0x3B31465F, 0x3CC7D5ED,
        0xBF7E132B, 0x3DFAB29E, 0x3B587725, 0x3CC73ABD,
        0xBF7D3AB4, 0x3E164064, 0x3B7F865D, 0x3CC68017,
        0xBF7C3B2E, 0x3E2F10AA, 0x3B9331A1, 0x3CC5A815,
        0xBF7B14BA, 0x3E47C5AC, 0x3BA68F4B, 0x3CC4B2B7,
        0xBF79C79B, 0x3E605C03, 0x3BB9D3CC, 0x3CC39DE5,
        0xBF7853F4, 0x3E78CFC0, 0x3BCCF6BE, 0x3CC26BB5,
        0xBF76BA06, 0x3E888E9B, 0x3BE00086, 0x3CC11A11,
        0xBF74FA05, 0x3E94A03C, 0x3BF2E05D, 0x3CBFAB11,
        0xBF731444, 0x3EA09AED, 0x3C02CF53, 0x3CBE1EB4,
        0xBF710907, 0x3EAC7CD9, 0x3C0C197E, 0x3CBC74FB,
        0xBF6ED8A1, 0x3EB84428, 0x3C154EB1, 0x3CBAADE6,
        0xBF6C8366, 0x3EC3EF07, 0x3C1E6EEB, 0x3CB8CB8E,
        0xBF6A09AB, 0x3ECF7BC0, 0x3C2771C9, 0x3CB6CBDA,
        0xBF676BD3, 0x3EDAE87D, 0x3C305FAF, 0x3CB4AEC9,
        0xBF64AA54, 0x3EE6336A, 0x3C393038, 0x3CB27675,
        0xBF61C593, 0x3EF15AF3, 0x3C41E365, 0x3CB022DD,
        0xBF5EBE06, 0x3EFC5D20, 0x3C4A7935, 0x3CADB61C,
        0xBF5B9421, 0x3F039C41, 0x3C52F1AA, 0x3CAB2BFE,
        0xBF58485A, 0x3F08F5A1, 0x3C5B4891, 0x3CA888B5,
        0xBF54DB38, 0x3F0E39D6, 0x3C637DE9, 0x3CA5CA2A,
        0xBF514D40, 0x3F136827, 0x3C6B8D82, 0x3CA2F273,
        0xBF4D9F0A, 0x3F187FBB, 0x3C737B8D, 0x3CA00193,
        0xBF49D10B, 0x3F1D7FD8, 0x3C7B43D9, 0x3C9CF9A0,
        0xBF45E3FC, 0x3F226794, 0x3C817332, 0x3C99D884,
        0xBF41D873, 0x3F273659, 0x3C852F7F, 0x3C969E3D,
        0xBF3DAEF7, 0x3F2BEB4A, 0x3C88D6D4, 0x3C934EFD,
        0xBF396840, 0x3F3085C2, 0x3C8C692F, 0x3C8FE692,
        0xBF3504F7, 0x3F3504F7, 0x3C8FE692, 0x3C8C692F,
        0xBF3085C2, 0x3F396840, 0x3C934EFD, 0x3C88D6D4,
        0xBF2BEB4A, 0x3F3DAEF7, 0x3C969E3D, 0x3C852F7F,
        0xBF273659, 0x3F41D873, 0x3C99D884, 0x3C817332,
        0xBF226794, 0x3F45E3FC, 0x3C9CF9A0, 0x3C7B43D9,
        0xBF1D7FD8, 0x3F49D10B, 0x3CA00193, 0x3C737B8D,
        0xBF187FBB, 0x3F4D9F0A, 0x3CA2F273, 0x3C6B8D82,
        0xBF136827, 0x3F514D40, 0x3CA5CA2A, 0x3C637DE9,
        0xBF0E39D6, 0x3F54DB38, 0x3CA888B5, 0x3C5B4891,
        0xBF08F5A1, 0x3F58485A, 0x3CAB2BFE, 0x3C52F1AA,
        0xBF039C41, 0x3F5B9421, 0x3CADB61C, 0x3C4A7935,
        0xBEFC5D20, 0x3F5EBE06, 0x3CB022DD, 0x3C41E365,
        0xBEF15AF3, 0x3F61C593, 0x3CB27675, 0x3C393038,
        0xBEE6336A, 0x3F64AA54, 0x3CB4AEC9, 0x3C305FAF,
        0xBEDAE87D, 0x3F676BD3, 0x3CB6CBDA, 0x3C2771C9,
        0xBECF7BC0, 0x3F6A09AB, 0x3CB8CB8E, 0x3C1E6EEB,
        0xBEC3EF07, 0x3F6C8366, 0x3CBAADE6, 0x3C154EB1,
        0xBEB84428, 0x3F6ED8A1, 0x3CBC74FB, 0x3C0C197E,
        0xBEAC7CD9, 0x3F710907, 0x3CBE1EB4, 0x3C02CF53,
        0xBEA09AED, 0x3F731444, 0x3CBFAB11, 0x3BF2E05D,
        0xBE94A03C, 0x3F74FA05, 0x3CC11A11, 0x3BE00086,
        0xBE888E9B, 0x3F76BA06, 0x3CC26BB5, 0x3BCCF6BE,
        0xBE78CFC0, 0x3F7853F4, 0x3CC39DE5, 0x3BB9D3CC,
        0xBE605C03, 0x3F79C79B, 0x3CC4B2B7, 0x3BA68F4B,
        0xBE47C5AC, 0x3F7B14BA, 0x3CC5A815, 0x3B9331A1,
        0xBE2F10AA, 0x3F7C3B2E, 0x3CC68017, 0x3B7F865D,
        0xBE164064, 0x3F7D3AB4, 0x3CC73ABD, 0x3B587725,
        0xBDFAB29E, 0x3F7E132B, 0x3CC7D5ED, 0x3B31465F,
        0xBDC8BD23, 0x3F7EC472, 0x3CC851A8, 0x3B0A04D1,
        0xBD96A93F, 0x3F7F4E66, 0x3CC8ADEF, 0x3AC5436C,
        0xBD48FB87, 0x3F7FB107, 0x3CC8EAC0, 0x3A6CB74E,
        0xBCC90A35, 0x3F7FEC46, 0x3CC90A35, 0x399DCF89,
        0x80000000, 0x3F800000, 0x3CC90A35, 0xB99DCF89,
    };

__declspec(align(8)) const u32 lbl_eu_8051E808[280] = {
        0x3AB37566, 0x3AB383B8, 0xC391371B, 0x6E36371B,
        0x8BF8CA8C, 0x33C7678D, 0x33C79428, 0xD07830B0,
        0x616130B0, 0x9C32D582, 0x2DD25BA5, 0x2DD2A40C,
        0xD9CF2B27, 0x564E2B27, 0xABAEDD7D, 0x28A95152,
        0x28A9B316, 0xE0A32654, 0x4CA82654, 0xBA45E355,
        0x24254849, 0x2425C13A, 0xE5A22217, 0x442F2217,
        0xC7F8E797, 0x20294051, 0x2029CE7F, 0xE93E1E57,
        0x3CAD1E57, 0xD4D3EAA2, 0x1C9F393D, 0x1C9FDAF5,
        0xEBC81AFF, 0x35FD1AFF, 0xE0E6ECB9, 0x197532EB,
        0x1975E6AA, 0xED781801, 0x30021801, 0xEC40EE0B,
        0x16A02D40, 0x16A0F1AB, 0xEE771552, 0x2AA41552,
        0xF6EDEEBE, 0x1415282A, 0x1415FC05, 0xEEE512E8,
        0x25D012E8, 0x00F6EEEE, 0x11CB2396, 0x11CB05C1,
        0xEEDB10BD, 0x217A10BD, 0x0A65EEB0, 0x0FBD1F79,
        0x0FBD0EE5, 0xEE6D0ECA, 0x1D930ECA, 0x1341EE17,
        0x0DE31BC7, 0x0DE3177A, 0xEDAD0D09, 0x1A130D09,
        0x1B90ED33, 0x0C3B1876, 0x0C3B1F85, 0xECA90B78,
        0x16EF0B78, 0x2358EC12, 0x0ABF157E, 0x0ABF270B,
        0xEB6E0A10, 0x14210A10, 0x2A9DEABF, 0x096B12D7,
        0x096B2E11, 0xEA0608D0, 0x119F08D0, 0x3166E945,
        0x083D107A, 0x083D349D, 0xE87C07B2, 0x0F6507B2,
        0x37B7E7AD, 0x07300E60, 0x07303AB4, 0xE6D806B5,
        0x0D6A06B5, 0x3D95E5FF, 0x06420C83, 0x0642405B,
        0xE52205D5, 0x0BAA05D5, 0x4306E443, 0x056F0ADF,
        0x056F4598, 0xE3610510, 0x0A1F0510, 0x4810E27E,
        0x04B6096C, 0x04B64A70, 0xE19B0462, 0x08C40462,
        0x4CB8E0B7, 0x04130826, 0x04134EE9, 0xDFD303C9,
        0x079303C9, 0x5104DEF1, 0x03840709, 0x03845309,
        0xDE100344, 0x06880344, 0x54F8DD31, 0x03080610,
        0x030856D4, 0xDC5402D0, 0x059F02D0, 0x589CDB7A,
        0x029B0537, 0x029B5A51, 0xDAA3026B, 0x04D5026B,
        0x5BF4D9CF, 0x023D047A, 0x023D5D85, 0xD8FE0213,
        0x04260213, 0x5F06D831, 0x01EB03D7, 0x01EB6076,
        0xD76801C7, 0x038D01C7, 0x61D6D6A3, 0x01A50349,
        0x01A56327, 0xD5E20185, 0x030A0185, 0x646AD526,
        0x016802CF, 0x0168659F, 0xD46D014C, 0x0299014C,
        0x66C6D3BA, 0x01330266, 0x013367E1, 0xD30A011C,
        0x0237011C, 0x68EFD25F, 0x0106020C, 0x010669F1,
        0xD1B900F2, 0x01E400F2, 0x6AE9D117, 0x00DF01BE,
        0x00DF6BD5, 0xD07A00CE, 0x019C00CE, 0x6CB7CFE1,
        0x00BE017C, 0x00BE6D8F, 0xCF4D00AF, 0x015E00AF,
        0x6E5DCEBD, 0x00A10143, 0x00A16F23, 0xCE310095,
        0x012A0095, 0x6FE0CDAA, 0x00890112, 0x00897094,
        0xCD27007E, 0x00FD007E, 0x7140CCA8, 0x007400E9,
        0x007471E5, 0xCC2E006B, 0x00D6006B, 0x7283CBB7,
        0x006300C5, 0x00637319, 0xCB44005B, 0x00B6005B,
        0x73A9CAD5, 0x005400A7, 0x00547433, 0xCA6A004D,
        0x009A004D, 0x74B7CA03, 0x0047008E, 0x00477534,
        0xC99F0041, 0x00820041, 0x75ACC93F, 0x003C0078,
        0x003C761F, 0xC8E20037, 0x006E0037, 0x768DC889,
        0x00330066, 0x003376F6, 0xC833002F, 0x005D002F,
        0x775AC7E0, 0x002B0056, 0x002B77BA, 0xC7900027,
        0x004F0027, 0x7816C743, 0x00240049, 0x0024786E,
        0xC6F80021, 0x00430021, 0x78C2C6B1, 0x001F003D,
        0x001F7912, 0xC66C001C, 0x0038001C, 0x795FC62A,
        0x001A0034, 0x001A79A8, 0xC5EA0018, 0x00300018,
        0x79EEC5AD, 0x0016002C, 0x00167A31, 0xC5720014,
        0x00280014, 0x7A71C53A, 0x00120025, 0x00127AAE,
        0xC5030011, 0x00220011, 0x7AE9C4CF, 0x0010001F,
        0x00107B21, 0xC49D000E, 0x001D000E, 0x7B57C46D,
        0x000D001A, 0x000D7B8A, 0xC43E000C, 0x0018000C,
        0x7BBBC412, 0x000B0016, 0x000B7BEA, 0xC3E7000A,
        0x0014000A, 0x7C17C3BE, 0x00090013, 0x00097C42,
        0xC3960009, 0x00110009, 0x7C6BC371, 0x00080010,
        0x00087C92, 0xC34C0007, 0x000E0007, 0x7CB8C329,
        0x0007000D, 0x00077CDC, 0xC3080006, 0x000C0006,
        0x7CFEC2E8, 0x0006000B, 0x00067D1F, 0xC2C90005,
        0x000A0005, 0x7D3FC2AB, 0x00050009, 0x00057D5D,
        0xC28F0004, 0x00090004, 0x7D7AC274, 0x00040008,
        0x00047D96, 0xC25A0004, 0x00070004, 0x7DB0C241,
    };

__declspec(align(8)) const u32 lbl_eu_8051EC68[243] = {
        0x3BB58895, 0x3BB57E7B, 0xC17F3BAD, 0x88A63BAD,
        0x7E69C18F, 0x3BA588B6, 0x3BA57E57, 0xC1A13B9C,
        0x88C83B9C, 0x7E44C1B3, 0x3B9388DA, 0x3B937E30,
        0xC1C63B89, 0x88ED3B89, 0x7E1CC1DA, 0x3B7F8901,
        0x3B7F7E06, 0xC1EE3B75, 0x89163B75, 0x7DF0C204,
        0x3B6A892C, 0x3B6A7DD8, 0xC21A3B5F, 0x89423B5F,
        0x7DBFC232, 0x3B53895A, 0x3B537DA6, 0xC24A3B47,
        0x89733B47, 0x7D8BC264, 0x3B3A898D, 0x3B3A7D6F,
        0xC27E3B2C, 0x89A83B2C, 0x7D51C29A, 0x3B1E89C4,
        0x3B1E7D33, 0xC2B73B10, 0x89E13B10, 0x7D13C2D5,
        0x3B008A00, 0x3B007CF1, 0xC2F43AF0, 0x8A203AF0,
        0x7CCEC315, 0x3AE08A41, 0x3AE07CA9, 0xC3373ACE,
        0x8A643ACE, 0x7C83C35A, 0x3ABC8A88, 0x3ABC7C5B,
        0xC37F3AA9, 0x8AAE3AA9, 0x7C31C3A6, 0x3A958AD6,
        0x3A957C05, 0xC3CE3A81, 0x8AFF3A81, 0x7BD8C3F8,
        0x3A6B8B2A, 0x3A6B7BA8, 0xC4233A55, 0x8B573A55,
        0x7B76C451, 0x3A3D8B86, 0x3A3D7B42, 0xC4803A25,
        0x8BB73A25, 0x7B0BC4B1, 0x3A0B8BEA, 0x3A0B7AD2,
        0xC4E439F0, 0x8C1F39F0, 0x7A96C519, 0x39D58C57,
        0x39D57A58, 0xC55039B8, 0x8C9139B8, 0x7A16C58A,
        0x39998CCD, 0x399979D2, 0xC5C6397A, 0x8D0C397A,
        0x798BC604, 0x39598D4E, 0x39597940, 0xC6453937,
        0x8D933937, 0x78F2C688, 0x39138DDA, 0x391378A0,
        0xC6CE38ED, 0x8E2538ED, 0x784AC716, 0x38C78E73,
        0x38C777F1, 0xC762389E, 0x8EC4389E, 0x7793C7B0,
        0x38748F19, 0x38747731, 0xC8023847, 0x8F713847,
        0x76CBC856, 0x38198FCD, 0x38197660, 0xC8AE37E9,
        0x902D37E9, 0x75F0C909, 0x37B79091, 0x37B7757A,
        0xC9683783, 0x90F93783, 0x74FFC9CA, 0x374D9166,
        0x374D747F, 0xCA2F3714, 0x91D73714, 0x73F8CA98,
        0x36D9924D, 0x36D9736B, 0xCB05369C, 0x92C8369C,
        0x72D8CB76, 0x365C9348, 0x365C723E, 0xCBEB3619,
        0x93CD3619, 0x719CCC64, 0x35D49458, 0x35D470F3,
        0xCCE1358C, 0x94E9358C, 0x7042CD63, 0x3540957F,
        0x35406F89, 0xCDE834F2, 0x961C34F2, 0x6EC7CE73,
        0x34A096BF, 0x34A06DFB, 0xCF01344B, 0x9769344B,
        0x6D27CF94, 0x33F3981A, 0x33F36C48, 0xD02C3397,
        0x98D23397, 0x6B5FD0C9, 0x33379992, 0x33376A6A,
        0xD16A32D4, 0x9A5932D4, 0x696AD210, 0x326C9B28,
        0x326C685E, 0xD2BB3200, 0x9BFF3200, 0x6746D36B,
        0x31909CDF, 0x31906620, 0xD41F311C, 0x9DC8311C,
        0x64ECD4D9, 0x30A39EBA, 0x30A363A9, 0xD5973025,
        0x9FB63025, 0x6258D65A, 0x2FA3A0BB, 0x2FA360F6,
        0xD7212F1B, 0xA1CA2F1B, 0x5F83D7EE, 0x2E8EA2E4,
        0x2E8E5DFF, 0xD8BE2DFC, 0xA4092DFC, 0x5C68D993,
        0x2D64A539, 0x2D645ABE, 0xDA6D2CC6, 0xA6742CC6,
        0x58FFDB4A, 0x2C22A7BB, 0x2C22572B, 0xDC2B2B79,
        0xA90E2B79, 0x5541DD10, 0x2AC9AA6E, 0x2AC9533F,
        0xDDF82A12, 0xABDC2A12, 0x5125DEE3, 0x2955AD56,
        0x29554EF0, 0xDFD12891, 0xAEDF2891, 0x4CA1E0C0,
        0x27C5B075, 0x27C54A34, 0xE1B126F3, 0xB21B26F3,
        0x47AAE2A4, 0x2618B3CF, 0x26184500, 0xE3962536,
        0xB5932536, 0x4234E489, 0x244CB768, 0x244C3F45,
        0xE57A235A, 0xB94D235A, 0x3C31E669, 0x225FBB43,
        0x225F38F6, 0xE756215B, 0xBD4A215B, 0x3591E83E,
        0x204EBF64, 0x204E3200, 0xE9201F38, 0xC1911F38,
        0x2E40E9FC, 0x1E17C3D1, 0x1E172A4E, 0xEACF1CED,
        0xC6251CED, 0x2628EB97, 0x1BB9C88E, 0x1BB921C8,
        0xEC521A7A, 0xCB0B1A7A, 0x1D2DECFD, 0x1930CD9F,
        0x19301850, 0xED9617DC, 0xD04917DC, 0x132EEE18,
        0x167BD30A, 0x167B0DC1, 0xEE800000,
    };
    /* f32[] table (retail lbl_eu_8051EC68) */

__declspec(align(4)) const u32 lbl_eu_8051F034[305] = {
        0x2EFA0000, 0xD10621D7, 0x1DF42E8F, 0x0000D171,
        0x22AD1D1E, 0x2E240000, 0xD1DC2381, 0x1C492DBA,
        0x0000D246, 0x24551B74, 0x2D500000, 0xD2B02527,
        0x1AA12CE7, 0x0000D319, 0x25F919CE, 0x2C7E0000,
        0xD38226CB, 0x18FC2C15, 0x0000D3EB, 0x279C182A,
        0x2BAD0000, 0xD453286C, 0x17592B44, 0x0000D4BC,
        0x293C1689, 0x2ADD0000, 0xD5232A0B, 0x15B92A75,
        0x0000D58B, 0x2ADA14EA, 0x2A0D0000, 0xD5F32BA8,
        0x141A29A6, 0x0000D65A, 0x2C76134C, 0x293F0000,
        0xD6C12D43, 0x127D28D8, 0x0000D728, 0x2E1111AF,
        0x28710000, 0xD78F2EDE, 0x10E2280A, 0x0000D7F6,
        0x2FAA1014, 0x27A40000, 0xD85C3076, 0x0F47273D,
        0x0000D8C3, 0x31420E7A, 0x26D70000, 0xD929320E,
        0x0DAE2671, 0x0000D98F, 0x32DA0CE2, 0x260B0000,
        0xD9F533A5, 0x0C1625A5, 0x0000DA5B, 0x34700B4A,
        0x253F0000, 0xDAC1353A, 0x0A7E24DA, 0x0000DB26,
        0x360509B3, 0x24740000, 0xDB8C36CF, 0x08E8240F,
        0x0000DBF1, 0x3799081D, 0x23A90000, 0xDC573863,
        0x07532344, 0x0000DCBC, 0x392C0689, 0x22DF0000,
        0xDD2139F5, 0x05BF227B, 0x0000DD85, 0x3ABE04F5,
        0x22160000, 0xDDEA3B86, 0x042C21B1, 0x0000DE4F,
        0x3C4F0363, 0x214D0000, 0xDEB33D17, 0x029A20E9,
        0x0000DF17, 0x3DDE01D1, 0x20850000, 0xDF7B3EA6,
        0x01092021, 0x0000DFDF, 0x3F6D0041, 0x1FBD0000,
        0xE0434033, 0xFF7A1F59, 0x0000E0A7, 0x40FAFEB3,
        0x1EF60000, 0xE10A41C0, 0xFDEC1E93, 0x0000E16D,
        0x4285FD25, 0x1E300000, 0xE1D0434A, 0xFC5F1DCD,
        0x0000E233, 0x440FFB9A, 0x1D6A0000, 0xE29644D4,
        0xFAD41D08, 0x0000E2F8, 0x4597FA10, 0x1CA60000,
        0xE35A465B, 0xF94B1C44, 0x0000E3BC, 0x471EF887,
        0x1BE20000, 0xE41E47E1, 0xF7C41B80, 0x0000E480,
        0x48A3F701, 0x1B1F0000, 0xE4E14964, 0xF63F1ABE,
        0x0000E542, 0x4A25F57D, 0x1A5E0000, 0xE5A24AE6,
        0xF4BB19FD, 0x0000E603, 0x4BA6F3FA, 0x199D0000,
        0xE6634C65, 0xF33A193D, 0x0000E6C3, 0x4D24F27A,
        0x18DD0000, 0xE7234DE3, 0xF1BB187E, 0x0000E782,
        0x4EA0F0FC, 0x181F0000, 0xE7E14F5E, 0xF03E17C0,
        0x0000E840, 0x501AEF81, 0x17620000, 0xE89E50D6,
        0xEEC41704, 0x0000E8FC, 0x5192EE07, 0x16A60000,
        0xE95A524D, 0xED4C1648, 0x0000E9B8, 0x5307EC91,
        0x15EB0000, 0xEA1553C0, 0xEBD6158E, 0x0000EA72,
        0x5479EB1C, 0x15320000, 0xEACE5532, 0xEA6314D5,
        0x0000EB2B, 0x55EAE9AB, 0x14790000, 0xEB8756A1,
        0xE8F3141E, 0x0000EBE2, 0x5757E83B, 0x13C20000,
        0xEC3E580D, 0xE7851367, 0x0000EC99, 0x58C2E6CE,
        0x130D0000, 0xECF35977, 0xE61912B2, 0x0000ED4E,
        0x5A2BE564, 0x12580000, 0xEDA85ADE, 0xE4B011FE,
        0x0000EE02, 0x5B91E3FC, 0x11A50000, 0xEE5B5C43,
        0xE349114B, 0x0000EEB5, 0x5CF5E297, 0x10F20000,
        0xEF0E5DA6, 0xE1E5109A, 0x0000EF66, 0x5E56E134,
        0x10410000, 0xEFBF5F06, 0xE0830FE9, 0x0000F017,
        0x5FB6DFD3, 0x0F910000, 0xF06F6065, 0xDF230F3A,
        0x0000F0C6, 0x6113DE74, 0x0EE20000, 0xF11E61C1,
        0xDDC50E8B, 0x0000F175, 0x626FDD16, 0x0E340000,
        0xF1CC631C, 0xDC690DDE, 0x0000F222, 0x63C8DBBB,
        0x0D870000, 0xF2796475, 0xDB0E0D31, 0x0000F2CF,
        0x6520DA61, 0x0CDA0000, 0xF32665CC, 0xD9B50C84,
        0x0000F37C, 0x6677D909, 0x0C2F0000, 0xF3D16722,
        0xD85D0BD9, 0x0000F427, 0x67CDD7B2, 0x0B830000,
        0xF47D6878, 0xD7060B2D, 0x0000F4D3, 0x6922D65B,
        0x0AD80000, 0xF52869CC, 0xD5B00A82, 0x0000F57E,
        0x6A77D505, 0x0A2D0000, 0xF5D36B21, 0xD45A09D7,
        0x0000F629, 0x6BCBD3AF, 0x09820000, 0xF67E6C75,
        0xD304092C, 0x0000F6D4, 0x6D1FD259, 0x08D70000,
        0xF7296DCA, 0xD1AE0881, 0x0000F77F, 0x6E74D102,
        0x082B0000, 0xF7D56F1F, 0xD05607D5, 0x0000F82B,
        0x6FCACFAA, 0x077F0000, 0xF8817076, 0xCEFE0729,
        0x0000F8D7, 0x7122CE51, 0x06D20000, 0xF92E71CE,
        0xCDA4067B, 0x0000F985, 0x727BCCF6, 0x06240000,
        0xF9DC7329, 0xCC4805CC, 0x0000FA34, 0x73D7CB98,
        0x05740000, 0xFA8C7487, 0xCAE8051C, 0x0000FAE4,
        0x7537CA38, 0x04C30000, 0xFB3D75E7, 0xC986046A,
        0x0000FB96, 0x7699C8D3, 0x04100000, 0xFBF0774C,
        0xC81F03B5, 0x0000FC4B, 0x7800C76A, 0x035A0000,
        0xFCA678B6, 0xC6B402FE, 0x0000FD02, 0x796DC5FC,
        0x02A20000, 0xFD5E7A25, 0xC5430244, 0x0000FDBC,
        0x7ADFC488,
    };

__declspec(align(8)) const u32 lbl_eu_8051F4F8[233] = {
        0x2F040000, 0xD0FC21DE, 0x1E082E99, 0x0000D167,
        0x22B31D32, 0x2E2F0000, 0xD1D12386, 0x1C5D2DC5,
        0x0000D23B, 0x24591B89, 0x2D5B0000, 0xD2A5252B,
        0x1AB62CF2, 0x0000D30E, 0x25FD19E4, 0x2C890000,
        0xD37726CE, 0x19122C20, 0x0000D3E0, 0x279E1841,
        0x2BB80000, 0xD448286D, 0x17702B50, 0x0000D4B0,
        0x293C16A0, 0x2AE80000, 0xD5182A0B, 0x15D02A80,
        0x0000D580, 0x2AD91500, 0x2A190000, 0xD5E72BA7,
        0x143129B1, 0x0000D64F, 0x2C741363, 0x294A0000,
        0xD6B62D41, 0x129428E3, 0x0000D71D, 0x2E0E11C6,
        0x287C0000, 0xD7842EDA, 0x10F82815, 0x0000D7EB,
        0x2FA6102A, 0x27AF0000, 0xD8513072, 0x0F5D2748,
        0x0000D8B8, 0x313E0E90, 0x26E10000, 0xD91F3209,
        0x0DC3267B, 0x0000D985, 0x32D40CF6, 0x26140000,
        0xD9EC339F, 0x0C2925AE, 0x0000DA52, 0x346A0B5C,
        0x25480000, 0xDAB83535, 0x0A8F24E1, 0x0000DB1F,
        0x35FF09C3, 0x247B0000, 0xDB8536CA, 0x08F62415,
        0x0000DBEB, 0x3794082A, 0x23AF0000, 0xDC51385E,
        0x075D2349, 0x0000DCB7, 0x39280691, 0x22E20000,
        0xDD1E39F3, 0x05C5227C, 0x0000DD84, 0x3ABD04F8,
        0x22160000, 0xDDEA3B86, 0x042C21B0, 0x0000DE50,
        0x3C50035F, 0x21490000, 0xDEB73D1A, 0x029320E3,
        0x0000DF1D, 0x3DE401C6, 0x207D0000, 0xDF833EAE,
        0x00F92016, 0x0000DFEA, 0x3F78002C, 0x1FB00000,
        0xE0504042, 0xFF5F1F49, 0x0000E0B7, 0x410CFE92,
        0x1EE20000, 0xE11E41D7, 0xFDC41E7B, 0x0000E185,
        0x42A1FCF7, 0x1E140000, 0xE1EC436C, 0xFC291DAD,
        0x0000E253, 0x4436FB5B, 0x1D460000, 0xE2BA4501,
        0xFA8C1CDF, 0x0000E321, 0x45CDF9BD, 0x1C770000,
        0xE3894698, 0xF8EE1C0F, 0x0000E3F1, 0x4764F81E,
        0x1BA70000, 0xE4594831, 0xF74D1B3E, 0x0000E4C2,
        0x48FDF67C, 0x1AD50000, 0xE52B49CB, 0xF5AB1A6C,
        0x0000E594, 0x4A99F4D8, 0x1A030000, 0xE5FD4B67,
        0xF4051998, 0x0000E668, 0x4C37F331, 0x192E0000,
        0xE6D24D07, 0xF25C18C3, 0x0000E73D, 0x4DD8F186,
        0x18570000, 0xE7A94EAA, 0xF0AF17EB, 0x0000E815,
        0x4F7DEFD6, 0x177E0000, 0xE8825051, 0xEEFC1710,
        0x0000E8F0, 0x5127EE21, 0x16A20000, 0xE95E51FE,
        0xED441632, 0x0000E9CE, 0x52D7EC65, 0x15C20000,
        0xEA3E53B2, 0xEB841550, 0x0000EAB0, 0x548EEAA1,
        0x14DE0000, 0xEB22556D, 0xE9BC146A, 0x0000EB96,
        0x564EE8D4, 0x13F50000, 0xEC0B5731, 0xE7E9137E,
        0x0000EC82, 0x5817E6FC, 0x13060000, 0xECFA5900,
        0xE60B128C, 0x0000ED74, 0x59EDE517, 0x12100000,
        0xEDF05ADC, 0xE41F1192, 0x0000EE6E, 0x5BD0E323,
        0x11110000, 0xEEEF5CC8, 0xE223108F, 0x0000EF71,
        0x5DC4E11E, 0x100A0000, 0xEFF65EC5, 0xE0130F82,
        0x0000F07E, 0x5FCBDF04, 0x0EF70000, 0xF10960D7,
        0xDDEE0E69, 0x0000F197, 0x61E9DCD1, 0x0DD70000,
        0xF2296301, 0xDBAE0D41, 0x0000F2BF, 0x6421DA83,
        0x0CA80000, 0xF3586549, 0xD94F0C09, 0x0000F3F7,
        0x6679D813, 0x0B660000, 0xF49A67B3, 0xD6CD0ABE,
        0x0000F542, 0x68F6D57C, 0x0A100000, 0xF5F06A45,
        0xD420095B, 0x0000F6A5, 0x6B9FD2B7, 0x08A00000,
        0xF7606D06, 0xD14107DE, 0x0000F822, 0x6E7BCFBC,
        0x07140000, 0xF8EC6FFF, 0xCE270640, 0x0000F9C0,
        0x7195CC81, 0x05640000, 0xFA9C733C, 0xCAC7047D,
        0x0000FB83, 0x74F7C8F9, 0x038A0000, 0xFC7676C8,
        0xC7140000,
    };

__declspec(align(4)) const u32 lbl_eu_8051F89C[233] = {
        0x3F420000, 0xC0BE0136, 0x3E833E8A, 0x0000C176,
        0x02A33D14, 0x3DD90000, 0xC2270401, 0x3BB33D2F,
        0x0000C2D1, 0x05533A5E, 0x3C8B0000, 0xC3750699,
        0x39163BEC, 0x0000C414, 0x07D537D7, 0x3B510000,
        0xC4AF0907, 0x36A23ABB, 0x0000C545, 0x0A313576,
        0x3A280000, 0xC5D80B53, 0x3451399A, 0x0000C666,
        0x0C6D3333, 0x390E0000, 0xC6F20D81, 0x321C3885,
        0x0000C77B, 0x0E8F310B, 0x37FF0000, 0xC8010F97,
        0x2FFF377C, 0x0000C884, 0x109B2EF7, 0x36FA0000,
        0xC906119A, 0x2DF5367B, 0x0000C985, 0x12942CF6,
        0x35FE0000, 0xCA02138B, 0x2BFB3582, 0x0000CA7E,
        0x147E2B04, 0x35080000, 0xCAF8156E, 0x2A0F348F,
        0x0000CB71, 0x165B291E, 0x34170000, 0xCBE91745,
        0x282F33A1, 0x0000CC5F, 0x182D2742, 0x332C0000,
        0xCCD41913, 0x265732B7, 0x0000CD49, 0x19F6256F,
        0x32440000, 0xCDBC1AD7, 0x248831D1, 0x0000CE2F,
        0x1BB723A2, 0x315F0000, 0xCEA11C95, 0x22BE30EE,
        0x0000CF12, 0x1D7221DC, 0x307D0000, 0xCF831E4D,
        0x20FA300C, 0x0000CFF4, 0x1F282019, 0x2F9C0000,
        0xD0642001, 0x1F392F2D, 0x0000D0D3, 0x20D91E59,
        0x2EBD0000, 0xD14321B1, 0x1D7A2E4E, 0x0000D1B2,
        0x22881C9C, 0x2DDF0000, 0xD221235E, 0x1BBE2D70,
        0x0000D290, 0x24341AE0, 0x2D010000, 0xD2FF250A,
        0x1A022C92, 0x0000D36E, 0x25DF1923, 0x2C230000,
        0xD3DD26B4, 0x18452BB3, 0x0000D44D, 0x27891767,
        0x2B440000, 0xD4BC285E, 0x16882AD4, 0x0000D52C,
        0x293415A8, 0x2A640000, 0xD59C2A09, 0x14C829F4,
        0x0000D60C, 0x2ADF13E8, 0x29830000, 0xD67D2BB6,
        0x13062912, 0x0000D6EE, 0x2C8D1224, 0x28A00000,
        0xD7602D64, 0x1140282E, 0x0000D7D2, 0x2E3C105C,
        0x27BB0000, 0xD8452F15, 0x0F762747, 0x0000D8B9,
        0x2FF00E8E, 0x26D30000, 0xD92D30CB, 0x0DA5265D,
        0x0000D9A3, 0x31A70CBB, 0x25E70000, 0xDA193285,
        0x0BCE2570, 0x0000DA90, 0x33640AE0, 0x24F70000,
        0xDB093445, 0x09EF247E, 0x0000DB82, 0x352808FC,
        0x24030000, 0xDBFD360C, 0x08062387, 0x0000DC79,
        0x36F3070D, 0x23090000, 0xDCF737DC, 0x0612228A,
        0x0000DD76, 0x38C80513, 0x22080000, 0xDDF839B6,
        0x04112185, 0x0000DE7B, 0x3AA8030B, 0x21000000,
        0xDF003B9D, 0x02012079, 0x0000DF87, 0x3C9500F2,
        0x1FF00000, 0xE0103D91, 0xFFDF1F63, 0x0000E09D,
        0x3E91FEC7, 0x1ED50000, 0xE12B3F96, 0xFDA91E43,
        0x0000E1BD, 0x409FFC85, 0x1DAE0000, 0xE25241AE,
        0xFB5B1D15, 0x0000E2EB, 0x42C3FA2A, 0x1C790000,
        0xE38743DE, 0xF8F21BD9, 0x0000E427, 0x44FFF7B1,
        0x1B340000, 0xE4CC4628, 0xF6681A8B, 0x0000E575,
        0x4759F515, 0x19DC0000, 0xE6244893, 0xF3B81928,
        0x0000E6D8, 0x49D6F250, 0x186E0000, 0xE7924B24,
        0xF0DC17AD, 0x0000E853, 0x4C7DEF5A, 0x16E50000,
        0xE91B4DE3, 0xEDCA1615, 0x0000E9EB, 0x4F56EC2A,
        0x153D0000, 0xEAC350D7, 0xEA79145A, 0x0000EBA6,
        0x526AE8B5, 0x136E0000, 0xEC92540E, 0xE6DC1276,
        0x0000ED8A, 0x55C6E4EC, 0x11720000, 0xEE8E5794,
        0xE2E3105F, 0x0000EFA1, 0x597AE0BE, 0x0F3D0000,
        0xF0C35B7B, 0xDE7A0E0A, 0x0000F1F6, 0x5D9BDC14,
        0x0CC30000, 0xF33D5FDC, 0xD9870B67, 0x0000F499,
        0x6242D6CF, 0x09F30000, 0xF60D64D3, 0xD3E70864,
        0x0000F79C, 0x6793D0C8, 0x06B60000, 0xF94A6A89,
        0xCD6C0000,
    };

__declspec(align(8)) const u32 lbl_eu_8051FC40[64] = {
        0xFD2DFD2E, 0xFD2FFD75, 0xFDA7FDCE, 0xFDEEFE09,
        0xFE20FE34, 0xFE46FE57, 0xFE66FE74, 0xFE81FE8D,
        0xFE98FEA3, 0xFEADFEB6, 0xFEBFFEC7, 0xFECFFED7,
        0xFEDFFEE6, 0xFEECFEF3, 0xFEF9FEFF, 0xFF05FF0B,
        0xFF11FF16, 0xFF1BFF20, 0xFF25FF2A, 0xFF2EFF33,
        0xFF37FF3C, 0xFF40FF44, 0xFF48FF4C, 0xFF50FF53,
        0xFF57FF5B, 0xFF5EFF62, 0xFF65FF68, 0xFF6BFF6F,
        0xFF72FF75, 0xFF78FF7B, 0xFF7EFF81, 0xFF83FF86,
        0xFF89FF8C, 0xFF8EFF91, 0xFF93FF96, 0xFF99FF9B,
        0xFF9DFFA0, 0xFFA2FFA5, 0xFFA7FFA9, 0xFFABFFAE,
        0xFFB0FFB2, 0xFFB4FFB6, 0xFFB8FFBA, 0xFFBCFFBE,
        0xFFC0FFC2, 0xFFC4FFC6, 0xFFC8FFCA, 0xFFCCFFCE,
        0xFFCFFFD1, 0xFFD3FFD5, 0xFFD6FFD8, 0xFFDAFFDC,
        0xFFDDFFDF, 0xFFE1FFE2, 0xFFE4FFE5, 0xFFE7FFE9,
        0xFFEAFFEC, 0xFFEDFFEF, 0xFFF0FFF2, 0xFFF3FFF5,
        0xFFF6FFF8, 0xFFF9FFFA, 0xFFFCFFFD, 0xFFFF0000,
    };

__declspec(align(8)) const u32 lbl_eu_8051FD40[128] = {
        0x3F7FCCB8, 0x3F7F9947, 0x3F7F65AD, 0x3F7F31E9,
        0x3F7EFDFA, 0x3F7EC9E1, 0x3F7E959D, 0x3F7E612F,
        0x3F7E2C94, 0x3F7DF7CF, 0x3F7DC2DD, 0x3F7D8DBF,
        0x3F7D5873, 0x3F7D22FD, 0x3F7CED57, 0x3F7CB784,
        0x3F7C8183, 0x3F7C4B54, 0x3F7C14F7, 0x3F7BDE6A,
        0x3F7BA7AD, 0x3F7B70C2, 0x3F7B39A4, 0x3F7B0258,
        0x3F7ACADB, 0x3F7A932B, 0x3F7A5B4B, 0x3F7A2338,
        0x3F79EAF3, 0x3F79B27C, 0x3F7979D1, 0x3F7940F1,
        0x3F7907DE, 0x3F78CE97, 0x3F78951A, 0x3F785B69,
        0x3F782181, 0x3F77E762, 0x3F77AD0F, 0x3F777282,
        0x3F7737C0, 0x3F76FCC4, 0x3F76C190, 0x3F768622,
        0x3F764A7B, 0x3F760E9C, 0x3F75D281, 0x3F75962A,
        0x3F755999, 0x3F751CCB, 0x3F74DFC1, 0x3F74A27A,
        0x3F7464F5, 0x3F742732, 0x3F73E930, 0x3F73AAEF,
        0x3F736C6D, 0x3F732DAD, 0x3F72EEAA, 0x3F72AF65,
        0x3F726FDF, 0x3F723015, 0x3F71F009, 0x3F71AFB8,
        0x3F716F22, 0x3F712E47, 0x3F70ED26, 0x3F70ABBE,
        0x3F706A10, 0x3F702818, 0x3F6FE5D8, 0x3F6FA34E,
        0x3F6F607B, 0x3F6F1D5B, 0x3F6ED9F2, 0x3F6E963A,
        0x3F6E5237, 0x3F6E0DE5, 0x3F6DC944, 0x3F6D8454,
        0x3F6D3F12, 0x3F6CF981, 0x3F6CB39C, 0x3F6C6D65,
        0x3F6C26DA, 0x3F6BDFFB, 0x3F6B98C5, 0x3F6B5139,
        0x3F6B0956, 0x3F6AC11C, 0x3F6A7887, 0x3F6A2F98,
        0x3F69E64C, 0x3F699CA6, 0x3F6952A2, 0x3F69083F,
        0x3F68BD7E, 0x3F68725B, 0x3F6826D8, 0x3F67DAF0,
        0x3F678EA6, 0x3F6741F5, 0x3F66F4E0, 0x3F66A763,
        0x3F66597C, 0x3F660B2D, 0x3F65BC70, 0x3F656D49,
        0x3F651DB3, 0x3F63DAF7, 0x3F61E90B, 0x3F603C9A,
        0x3F5E82FB, 0x3F5D178D, 0x3F5A8424, 0x3F57D016,
        0x3F542004, 0x3F509DEF, 0x3F4CDB09, 0x3F472F5A,
        0x3F4166CF, 0x3F3965FE, 0x3F2ECD8C, 0x3F2206D8,
        0x3F0F289E, 0x3EE90841, 0x3EA8E5A3, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051FF40[10] = {
        0x00060C13, 0x191F252B, 0x31363C41, 0x474C5155,
        0x5A5E6266, 0x6A6D7073, 0x75787A7B, 0x7D7E7E7F,
        0x7F000000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051FF68[10] = {
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, (unsigned int)&ChannelCallbackFunc__Q44nw4r3snd6detail8SeqTrackFPQ44nw4r3snd6detail7ChannelQ54nw4r3snd6detail7Channel21ChannelCallbackStatusUl,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_8051FF90[12] = {
        0x3F800000, 0x3F879C7D, 0x3F8FACD6, 0x3F9837F0,
        0x3FA14518, 0x3FAADC08, 0x3FB504F3, 0x3FBFC887,
        0x3FCB2FF5, 0x3FD744FD, 0x3FE411F0, 0x3FF1A1BF,
    };
    /* f32[] table (retail lbl_eu_8051FF90) */

__declspec(align(8)) const u32 lbl_eu_8051FFC0[256] = {
        0x3F800000, 0x3F800765, 0x3F800ECA, 0x3F801630,
        0x3F801D96, 0x3F8024FD, 0x3F802C64, 0x3F8033CC,
        0x3F803B34, 0x3F80429C, 0x3F804A05, 0x3F80516E,
        0x3F8058D8, 0x3F806042, 0x3F8067AC, 0x3F806F17,
        0x3F807683, 0x3F807DEF, 0x3F80855B, 0x3F808CC8,
        0x3F809435, 0x3F809BA2, 0x3F80A310, 0x3F80AA7E,
        0x3F80B1ED, 0x3F80B95D, 0x3F80C0CC, 0x3F80C83C,
        0x3F80CFAD, 0x3F80D71E, 0x3F80DE8F, 0x3F80E601,
        0x3F80ED73, 0x3F80F4E6, 0x3F80FC59, 0x3F8103CD,
        0x3F810B41, 0x3F8112B5, 0x3F811A2A, 0x3F81219F,
        0x3F812915, 0x3F81308B, 0x3F813802, 0x3F813F79,
        0x3F8146F0, 0x3F814E68, 0x3F8155E0, 0x3F815D59,
        0x3F8164D2, 0x3F816C4C, 0x3F8173C6, 0x3F817B40,
        0x3F8182BB, 0x3F818A36, 0x3F8191B2, 0x3F81992E,
        0x3F81A0AB, 0x3F81A828, 0x3F81AFA5, 0x3F81B723,
        0x3F81BEA1, 0x3F81C620, 0x3F81CD9F, 0x3F81D51F,
        0x3F81DC9F, 0x3F81E420, 0x3F81EBA1, 0x3F81F322,
        0x3F81FAA4, 0x3F820226, 0x3F8209A9, 0x3F82112C,
        0x3F8218AF, 0x3F822033, 0x3F8227B8, 0x3F822F3C,
        0x3F8236C2, 0x3F823E47, 0x3F8245CE, 0x3F824D54,
        0x3F8254DB, 0x3F825C63, 0x3F8263EB, 0x3F826B73,
        0x3F8272FC, 0x3F827A85, 0x3F82820E, 0x3F828998,
        0x3F829123, 0x3F8298AE, 0x3F82A039, 0x3F82A7C5,
        0x3F82AF51, 0x3F82B6DE, 0x3F82BE6B, 0x3F82C5F9,
        0x3F82CD87, 0x3F82D515, 0x3F82DCA4, 0x3F82E433,
        0x3F82EBC3, 0x3F82F353, 0x3F82FAE4, 0x3F830275,
        0x3F830A06, 0x3F831198, 0x3F83192A, 0x3F8320BD,
        0x3F832850, 0x3F832FE4, 0x3F833778, 0x3F833F0D,
        0x3F8346A2, 0x3F834E37, 0x3F8355CD, 0x3F835D63,
        0x3F8364FA, 0x3F836C91, 0x3F837429, 0x3F837BC1,
        0x3F838359, 0x3F838AF2, 0x3F83928C, 0x3F839A25,
        0x3F83A1C0, 0x3F83A95A, 0x3F83B0F5, 0x3F83B891,
        0x3F83C02D, 0x3F83C7C9, 0x3F83CF66, 0x3F83D704,
        0x3F83DEA1, 0x3F83E640, 0x3F83EDDE, 0x3F83F57D,
        0x3F83FD1D, 0x3F8404BD, 0x3F840C5D, 0x3F8413FE,
        0x3F841B9F, 0x3F842341, 0x3F842AE3, 0x3F843286,
        0x3F843A29, 0x3F8441CC, 0x3F844970, 0x3F845115,
        0x3F8458B9, 0x3F84605F, 0x3F846804, 0x3F846FAA,
        0x3F847751, 0x3F847EF8, 0x3F84869F, 0x3F848E47,
        0x3F8495F0, 0x3F849D98, 0x3F84A542, 0x3F84ACEB,
        0x3F84B495, 0x3F84BC40, 0x3F84C3EB, 0x3F84CB96,
        0x3F84D342, 0x3F84DAEF, 0x3F84E29B, 0x3F84EA49,
        0x3F84F1F6, 0x3F84F9A4, 0x3F850153, 0x3F850902,
        0x3F8510B1, 0x3F851861, 0x3F852012, 0x3F8527C2,
        0x3F852F74, 0x3F853725, 0x3F853ED7, 0x3F85468A,
        0x3F854E3D, 0x3F8555F0, 0x3F855DA4, 0x3F856558,
        0x3F856D0D, 0x3F8574C2, 0x3F857C78, 0x3F85842E,
        0x3F858BE5, 0x3F85939C, 0x3F859B53, 0x3F85A30B,
        0x3F85AAC3, 0x3F85B27C, 0x3F85BA35, 0x3F85C1EF,
        0x3F85C9A9, 0x3F85D164, 0x3F85D91F, 0x3F85E0DA,
        0x3F85E896, 0x3F85F052, 0x3F85F80F, 0x3F85FFCC,
        0x3F86078A, 0x3F860F48, 0x3F861707, 0x3F861EC6,
        0x3F862685, 0x3F862E45, 0x3F863606, 0x3F863DC7,
        0x3F864588, 0x3F864D4A, 0x3F86550C, 0x3F865CCE,
        0x3F866491, 0x3F866C55, 0x3F867419, 0x3F867BDD,
        0x3F8683A2, 0x3F868B67, 0x3F86932D, 0x3F869AF3,
        0x3F86A2BA, 0x3F86AA81, 0x3F86B249, 0x3F86BA11,
        0x3F86C1D9, 0x3F86C9A2, 0x3F86D16B, 0x3F86D935,
        0x3F86E0FF, 0x3F86E8CA, 0x3F86F095, 0x3F86F861,
        0x3F87002D, 0x3F8707F9, 0x3F870FC6, 0x3F871794,
        0x3F871F62, 0x3F872730, 0x3F872EFF, 0x3F8736CE,
        0x3F873E9E, 0x3F87466E, 0x3F874E3E, 0x3F87560F,
        0x3F875DE1, 0x3F8765B3, 0x3F876D85, 0x3F877558,
        0x3F877D2B, 0x3F8784FF, 0x3F878CD3, 0x3F8794A8,
    };
    /* f32[] table (retail lbl_eu_8051FFC0) */

__declspec(align(8)) const u32 lbl_eu_805203C0[965] = {
        0x00000000, 0x380021F4, 0x38019DD8, 0x38031E07,
        0x3804A2B9, 0x38062BED, 0x3807B9A4, 0x38094BF9,
        0x380AE2EC, 0x380C7E98, 0x380E1F19, 0x380FC46F,
        0x38116E9A, 0x38131DB6, 0x3814D1C1, 0x38168AF5,
        0x38184934, 0x381A0C9B, 0x381BD544, 0x381DA331,
        0x381F767C, 0x38214F40, 0x38232D64, 0x3825111C,
        0x3826FA6A, 0x3828E969, 0x382ADE18, 0x382CD8B0,
        0x382ED8F8, 0x3830DF5F, 0x3832EBAE, 0x3834FE00,
        0x3837168C, 0x38393538, 0x383B5A54, 0x383D85AB,
        0x383FB773, 0x3841EFC8, 0x38442EAA, 0x38467434,
        0x3848C081, 0x384B1393, 0x384D6D9F, 0x384FCE8A,
        0x3852368B, 0x3854A5BE, 0x38571C08, 0x385999BA,
        0x385C1EB9, 0x385EAB3C, 0x38613F5F, 0x3863DB06,
        0x38667E84, 0x386929BD, 0x386BDCE8, 0x386E9820,
        0x38715B66, 0x387426D5, 0x3876FAA5, 0x3879D6B8,
        0x387CBB63, 0x387FA889, 0x38814F31, 0x3882CE85,
        0x3884523F, 0x3885DA7C, 0x38876749, 0x3888F8B4,
        0x388A8EAF, 0x388C2972, 0x388DC8EE, 0x388F6D3F,
        0x38911665, 0x3892C47B, 0x38947790, 0x38962FA2,
        0x3897ECDD, 0x3899AF3E, 0x389B76C7, 0x389D43A1,
        0x389F15D9, 0x38A0ED61, 0x38A2CA72, 0x38A4AD09,
        0x38A69529, 0x38A882F9, 0x38AA7688, 0x38AC6FD6,
        0x38AE6EFD, 0x38B0740D, 0x38B27F20, 0x38B49043,
        0x38B6A786, 0x38B8C4F5, 0x38BAE8AC, 0x38BD12B9,
        0x38BF4338, 0x38C17A27, 0x38C3B7B1, 0x38C5FBE4,
        0x38C846CC, 0x38CA9878, 0x38CCF103, 0x38CF5089,
        0x38D1B717, 0x38D424CA, 0x38D69977, 0x38D915A8,
        0x38DB995D, 0x38DE240D, 0x38E0B6CB, 0x38E3510C,
        0x38E5F2D2, 0x38E89C1C, 0x38EB4DFD, 0x38EE0763,
        0x38F0C8D5, 0x38F392DF, 0x38F664F7, 0x38F93F1C,
        0x38FC21D8, 0x38FF0DB4, 0x390100CF, 0x39027F10,
        0x39040225, 0x39058941, 0x39071533, 0x3908A5B4,
        0x390A3AC6, 0x390BD468, 0x390D72DF, 0x390F162A,
        0x3910BE4B, 0x39126B86, 0x39141D95, 0x3915D479,
        0x391790BC, 0x39195218, 0x391B188F, 0x391CE463,
        0x391EB551, 0x39208BE2, 0x392267D2, 0x39244920,
        0x39263011, 0x39281CEA, 0x392A0F21, 0x392C0741,
        0x392E0548, 0x393008F2, 0x393212C8, 0x393422CC,
        0x393638B7, 0x393854CE, 0x393A7757, 0x393CA00D,
        0x393ECF33, 0x394104CB, 0x39434119, 0x394583D8,
        0x3947CD4D, 0x394A1D79, 0x394C749E, 0x394ED2BF,
        0x395137DA, 0x3953A435, 0x3956178A, 0x3958921F,
        0x395B1438, 0x395D9D91, 0x39602E6D, 0x3962C713,
        0x3965673C, 0x39680F2F, 0x396ABF2F, 0x396D76F8,
        0x39703713, 0x3972FEF7, 0x3975CF72, 0x3978A7FB,
        0x397B891A, 0x397E72D1, 0x3980B28F, 0x39823002,
        0x3983B204, 0x39853852, 0x3986C353, 0x398852C2,
        0x3989E6E3, 0x398B7FB7, 0x398D1D1B, 0x398EBF76,
        0x399066A6, 0x399212AB, 0x3993C3A8, 0x399579BE,
        0x399734CC, 0x3998F515, 0x399ABA78, 0x399C8518,
        0x399E5515, 0x39A02A71, 0x39A2054E, 0x39A3E589,
        0x39A5CB67, 0x39A7B6E9, 0x39A9A80D, 0x39AB9EF7,
        0x39AD9BA7, 0x39AF9E3E, 0x39B1A6DF, 0x39B3B58B,
        0x39B5CA41, 0x39B7E523, 0x39BA0632, 0x39BC2DB2,
        0x39BE5B81, 0x39C08FC1, 0x39C2CA95, 0x39C50BFD,
        0x39C7541B, 0x39C9A310, 0x39CBF8BC, 0x39CE5563,
        0x39D0B904, 0x39D323C3, 0x39D5959E, 0x39D80EDB,
        0x39DA8F36, 0x39DD1737, 0x39DFA699, 0x39E23D80,
        0x39E4DC30, 0x39E78286, 0x39EA30E9, 0x39ECE716,
        0x39EFA550, 0x39F26BBA, 0x39F53A54, 0x39F81141,
        0x39FAF0A2, 0x39FDD877, 0x3A006483, 0x3A01E116,
        0x3A036217, 0x3A04E797, 0x3A067185, 0x3A080014,
        0x3A099334, 0x3A0B2B06, 0x3A0CC79C, 0x3A0E68E4,
        0x3A100F12, 0x3A11BA16, 0x3A136A11, 0x3A151F14,
        0x3A16D920, 0x3A189845, 0x3A1A5CA7, 0x3A1C2633,
        0x3A1DF51E, 0x3A1FC956, 0x3A21A2FD, 0x3A238226,
        0x3A2566CF, 0x3A27511B, 0x3A29411B, 0x3A2B36E1,
        0x3A2D325B, 0x3A2F33CE, 0x3A313B29, 0x3A33488F,
        0x3A355BFE, 0x3A37759A, 0x3A399574, 0x3A3BBB8B,
        0x3A3DE814, 0x3A401B0E, 0x3A425479, 0x3A449489,
        0x3A46DB4F, 0x3A4928BA, 0x3A4B7D0E, 0x3A4DD84C,
        0x3A503A73, 0x3A52A3B8, 0x3A55142A, 0x3A578BCB,
        0x3A5A0ABD, 0x3A5C9110, 0x3A5F1EE7, 0x3A61B454,
        0x3A645168, 0x3A66F633, 0x3A69A2D8, 0x3A6C5768,
        0x3A6F1406, 0x3A71D8B1, 0x3A74A59E, 0x3A777ADC,
        0x3A7A587F, 0x3A7D3E96, 0x3A8016A2, 0x3A819256,
        0x3A83126F, 0x3A8496F5, 0x3A862004, 0x3A87AD9A,
        0x3A893FD2, 0x3A8AD6AB, 0x3A8C7236, 0x3A8E128E,
        0x3A8FB7B2, 0x3A9161BD, 0x3A9310AD, 0x3A94C4A6,
        0x3A967DA8, 0x3A983BC3, 0x3A99FF09, 0x3A9BC782,
        0x3A9D9549, 0x3A9F686E, 0x3AA140FA, 0x3AA31EFE,
        0x3AA50283, 0x3AA6EBAB, 0x3AA8DA7F, 0x3AAACF07,
        0x3AACC95D, 0x3AAEC98A, 0x3AB0CFB0, 0x3AB2DBCF,
        0x3AB4EE00, 0x3AB70656, 0x3AB924E9, 0x3ABB49BA,
        0x3ABD74EB, 0x3ABFA685, 0x3AC1DEA1, 0x3AC41D59,
        0x3AC662AE, 0x3AC8AEC1, 0x3ACB01AD, 0x3ACD5B71,
        0x3ACFBC2F, 0x3AD223FA, 0x3AD492EA, 0x3AD70911,
        0x3AD98680, 0x3ADC0B51, 0x3ADE979D, 0x3AE12B6D,
        0x3AE3C6ED, 0x3AE66A24, 0x3AE91524, 0x3AEBC818,
        0x3AEE8309, 0x3AF1460F, 0x3AF41146, 0x3AF6E4C6,
        0x3AF9C0A9, 0x3AFCA502, 0x3AFF91F1, 0x3B0143C4,
        0x3B02C2F1, 0x3B044690, 0x3B05CEAE, 0x3B075B54,
        0x3B08EC92, 0x3B0A8276, 0x3B0C1D0D, 0x3B0DBC67,
        0x3B0F608E, 0x3B110993, 0x3B12B781, 0x3B146A6C,
        0x3B162263, 0x3B17DF70, 0x3B19A1A3, 0x3B1B690E,
        0x3B1D35BD, 0x3B1F07C3, 0x3B20DF2F, 0x3B22BC0F,
        0x3B249E74, 0x3B268674, 0x3B287417, 0x3B2A6772,
        0x3B2C6093, 0x3B2E5F8F, 0x3B306477, 0x3B326F58,
        0x3B34804B, 0x3B36975B, 0x3B38B49F, 0x3B3AD825,
        0x3B3D0207, 0x3B3F324E, 0x3B416912, 0x3B43A66A,
        0x3B45EA63, 0x3B483512, 0x3B4A868C, 0x3B4CDEE7,
        0x3B4F3E39, 0x3B51A48D, 0x3B541204, 0x3B5686A8,
        0x3B590294, 0x3B5B85DE, 0x3B5E10A0, 0x3B60A2E5,
        0x3B633CCC, 0x3B65DE67, 0x3B6887CF, 0x3B6B391A,
        0x3B6DF266, 0x3B70B3BF, 0x3B737D43, 0x3B764F0D,
        0x3B792932, 0x3B7C0BCC, 0x3B7EF6F0, 0x3B80F560,
        0x3B8273A7, 0x3B83F65A, 0x3B857D89, 0x3B87093E,
        0x3B88998A, 0x3B8A2E77, 0x3B8BC815, 0x3B8D6672,
        0x3B8F0999, 0x3B90B19C, 0x3B925E89, 0x3B94106C,
        0x3B95C756, 0x3B978354, 0x3B994477, 0x3B9B0ACF,
        0x3B9CD667, 0x3B9EA753, 0x3BA07DA0, 0x3BA25960,
        0x3BA43AA1, 0x3BA62176, 0x3BA80DEF, 0x3BAA0019,
        0x3BABF80B, 0x3BADF5D0, 0x3BAFF97E, 0x3BB20323,
        0x3BB412D5, 0x3BB628A2, 0x3BB8449B, 0x3BBA66D7,
        0x3BBC8F66, 0x3BBEBE5B, 0x3BC0F3CA, 0x3BC32FC2,
        0x3BC5725D, 0x3BC7BBA9, 0x3BCA0BBD, 0x3BCC62AB,
        0x3BCEC089, 0x3BD1256C, 0x3BD39166, 0x3BD60491,
        0x3BD87EFB, 0x3BDB00C0, 0x3BDD89F4, 0x3BE01AAA,
        0x3BE2B2FE, 0x3BE55301, 0x3BE7FACC, 0x3BEAAA76,
        0x3BED6219, 0x3BF021C6, 0x3BF2E99B, 0x3BF5B9AF,
        0x3BF8921A, 0x3BFB72F3, 0x3BFE5C54, 0x3C00A72B,
        0x3C02248A, 0x3C03A654, 0x3C052C95, 0x3C06B75B,
        0x3C0846B4, 0x3C09DAAB, 0x3C0B7351, 0x3C0D10B3,
        0x3C0EB2DC, 0x3C1059DF, 0x3C1205C6, 0x3C13B6A2,
        0x3C156C81, 0x3C172773, 0x3C18E786, 0x3C1AACC9,
        0x3C1C774B, 0x3C1E471D, 0x3C201C4D, 0x3C21F6EC,
        0x3C23D70A, 0x3C25BCB8, 0x3C27A805, 0x3C299903,
        0x3C2B8FC1, 0x3C2D8C52, 0x3C2F8EC7, 0x3C319730,
        0x3C33A5A1, 0x3C35BA2A, 0x3C37D4DE, 0x3C39F5CE,
        0x3C3C1D0D, 0x3C3E4AAF, 0x3C407EC7, 0x3C42B965,
        0x3C44FAA0, 0x3C47428A, 0x3C499137, 0x3C4BE6BA,
        0x3C4E4329, 0x3C50A697, 0x3C53111B, 0x3C5582C7,
        0x3C57FBB2, 0x3C5A7BF1, 0x3C5D039B, 0x3C5F92C3,
        0x3C622984, 0x3C64C7F0, 0x3C676E1E, 0x3C6A1C28,
        0x3C6CD224, 0x3C6F9028, 0x3C72564E, 0x3C7524AC,
        0x3C77FB5C, 0x3C7ADA77, 0x3C7DC214, 0x3C805927,
        0x3C81D59F, 0x3C83567E, 0x3C84DBD3, 0x3C8665A9,
        0x3C87F40F, 0x3C898712, 0x3C8B1EC0, 0x3C8CBB26,
        0x3C8E5C53, 0x3C900254, 0x3C91AD39, 0x3C935D0F,
        0x3C9511E5, 0x3C96CBC9, 0x3C988ACC, 0x3C9A4EFC,
        0x3C9C1868, 0x3C9DE721, 0x3C9FBB34, 0x3CA194B4,
        0x3CA373AF, 0x3CA55836, 0x3CA74259, 0x3CA9322A,
        0x3CAB27B7, 0x3CAD2314, 0x3CAF2451, 0x3CB12B7F,
        0x3CB338B0, 0x3CB54BF6, 0x3CB76562, 0x3CB98509,
        0x3CBBAAFA, 0x3CBDD74A, 0x3CC00A0B, 0x3CC24350,
        0x3CC4832C, 0x3CC6C9B4, 0x3CC916FA, 0x3CCB6B13,
        0x3CCDC613, 0x3CD0280F, 0x3CD2911B, 0x3CD5014D,
        0x3CD778B8, 0x3CD9F773, 0x3CDC7D93, 0x3CDF0B2F,
        0x3CE1A05D, 0x3CE43D32, 0x3CE6E1C6, 0x3CE98E30,
        0x3CEC4286, 0x3CEEFEE1, 0x3CF1C358, 0x3CF49003,
        0x3CF764FA, 0x3CFA4257, 0x3CFD2831, 0x3D000B51,
        0x3D0186E3, 0x3D0306D9, 0x3D048B41, 0x3D061429,
        0x3D07A19D, 0x3D0933AC, 0x3D0ACA63, 0x3D0C65CF,
        0x3D0E05FE, 0x3D0FAB00, 0x3D1154E1, 0x3D1303B1,
        0x3D14B77E, 0x3D167057, 0x3D182E4B, 0x3D19F168,
        0x3D1BB9BF, 0x3D1D875F, 0x3D1F5A57, 0x3D2132B8,
        0x3D231090, 0x3D24F3F1, 0x3D26DCEB, 0x3D28CB8F,
        0x3D2ABFEC, 0x3D2CBA15, 0x3D2EBA1B, 0x3D30C00E,
        0x3D32CC01, 0x3D34DE05, 0x3D36F62B, 0x3D391488,
        0x3D3B392C, 0x3D3D642A, 0x3D3F9596, 0x3D41CD81,
        0x3D440C01, 0x3D465127, 0x3D489D08, 0x3D4AEFB8,
        0x3D4D494A, 0x3D4FA9D4, 0x3D52116A, 0x3D548021,
        0x3D56F60D, 0x3D597345, 0x3D5BF7DE, 0x3D5E83ED,
        0x3D611789, 0x3D63B2C9, 0x3D6655C3, 0x3D69008E,
        0x3D6BB340, 0x3D6E6DF2, 0x3D7130BC, 0x3D73FBB4,
        0x3D76CEF4, 0x3D79AA94, 0x3D7C8EAC, 0x3D7F7B57,
        0x3D813856, 0x3D82B764, 0x3D843AE0, 0x3D85C2DA,
        0x3D874F5D, 0x3D88E078, 0x3D8A7638, 0x3D8C10AB,
        0x3D8DAFDE, 0x3D8F53E0, 0x3D90FCBF, 0x3D92AA8A,
        0x3D945D4F, 0x3D96151C, 0x3D97D202, 0x3D99940E,
        0x3D9B5B50, 0x3D9D27D8, 0x3D9EF9B4, 0x3DA0D0F6,
        0x3DA2ADAD, 0x3DA48FE9, 0x3DA677BB, 0x3DA86532,
        0x3DAA5860, 0x3DAC5156, 0x3DAE5025, 0x3DB054DE,
        0x3DB25F93, 0x3DB47056, 0x3DB68738, 0x3DB8A44B,
        0x3DBAC7A2, 0x3DBCF150, 0x3DBF2167, 0x3DC157FB,
        0x3DC3951E, 0x3DC5D8E3, 0x3DC82360, 0x3DCA74A7,
        0x3DCCCCCD, 0x3DCF2BE6, 0x3DD19206, 0x3DD3FF43,
        0x3DD673B1, 0x3DD8EF67, 0x3DDB7279, 0x3DDDFCFD,
        0x3DE08F09, 0x3DE328B4, 0x3DE5CA15, 0x3DE87341,
        0x3DEB2451, 0x3DEDDD5B, 0x3DF09E78, 0x3DF367BF,
        0x3DF63948, 0x3DF9132C, 0x3DFBF584, 0x3DFEE068,
        0x3E00E9F9, 0x3E02681E, 0x3E03EAB0, 0x3E0571BC,
        0x3E06FD4F, 0x3E088D77, 0x3E0A2241, 0x3E0BBBBA,
        0x3E0D59F2, 0x3E0EFCF5, 0x3E10A4D3, 0x3E125199,
        0x3E140356, 0x3E15BA19, 0x3E1775F0, 0x3E1936EC,
        0x3E1AFD1A, 0x3E1CC88A, 0x3E1E994C, 0x3E206F70,
        0x3E224B06, 0x3E242C1E, 0x3E2612C7, 0x3E27FF14,
        0x3E29F113, 0x3E2BE8D7, 0x3E2DE670, 0x3E2FE9F0,
        0x3E31F368, 0x3E3402EA, 0x3E361887, 0x3E383452,
        0x3E3A565E, 0x3E3C7EBC, 0x3E3EAD7F, 0x3E40E2BB,
        0x3E431E82, 0x3E4560E9, 0x3E47AA02, 0x3E49F9E1,
        0x3E4C509B, 0x3E4EAE43, 0x3E5112EF, 0x3E537EB4,
        0x3E55F1A5, 0x3E586BD9, 0x3E5AED65, 0x3E5D765E,
        0x3E6006DC, 0x3E629EF3, 0x3E653EBB, 0x3E67E64B,
        0x3E6A95B9, 0x3E6D4D1C, 0x3E700C8D, 0x3E72D424,
        0x3E75A3F7, 0x3E787C21, 0x3E7B5CB9, 0x3E7E45D8,
        0x3E809BCC, 0x3E82190A, 0x3E839AB1, 0x3E8520D0,
        0x3E86AB73, 0x3E883AA8, 0x3E89CE7C, 0x3E8B66FE,
        0x3E8D043A, 0x3E8EA63F, 0x3E904D1C, 0x3E91F8DE,
        0x3E93A994, 0x3E955F4D, 0x3E971A17, 0x3E98DA02,
        0x3E9A9F1C, 0x3E9C6976, 0x3E9E391F, 0x3EA00E26,
        0x3EA1E89B, 0x3EA3C88F, 0x3EA5AE11, 0x3EA79933,
        0x3EA98A05, 0x3EAB8097, 0x3EAD7CFB, 0x3EAF7F42,
        0x3EB1877E, 0x3EB395C0, 0x3EB5AA1A, 0x3EB7C49E,
        0x3EB9E55E, 0x3EBC0C6D, 0x3EBE39DE, 0x3EC06DC3,
        0x3EC2A82F, 0x3EC4E937, 0x3EC730ED, 0x3EC97F65,
        0x3ECBD4B4, 0x3ECE30ED, 0x3ED09426, 0x3ED2FE72,
        0x3ED56FE7, 0x3ED7E89B, 0x3EDA68A1, 0x3EDCF012,
        0x3EDF7F01, 0x3EE21586, 0x3EE4B3B6, 0x3EE759AA,
        0x3EEA0777, 0x3EECBD35, 0x3EEF7AFB, 0x3EF240E2,
        0x3EF50F01, 0x3EF7E571, 0x3EFAC44A, 0x3EFDABA6,
        0x3F004DCE, 0x3F01CA25, 0x3F034AE2, 0x3F04D014,
        0x3F0659C8, 0x3F07E80B, 0x3F097AEA, 0x3F0B1274,
        0x3F0CAEB6, 0x3F0E4FBE, 0x3F0FF59A, 0x3F11A058,
        0x3F135008, 0x3F1504B7, 0x3F16BE75, 0x3F187D50,
        0x3F1A4158, 0x3F1C0A9C, 0x3F1DD92C, 0x3F1FAD16,
        0x3F21866C, 0x3F23653C, 0x3F254998, 0x3F273390,
        0x3F292335, 0x3F2B1896, 0x3F2D13C6, 0x3F2F14D5,
        0x3F311BD6, 0x3F3328D8, 0x3F353BEF, 0x3F37552D,
        0x3F3974A3, 0x3F3B9A63, 0x3F3DC682, 0x3F3FF911,
        0x3F423224, 0x3F4471CD, 0x3F46B822, 0x3F490534,
        0x3F4B5918, 0x3F4DB3E3, 0x3F5015A9, 0x3F527E7F,
        0x3F54EE78, 0x3F5765AC, 0x3F59E42F, 0x3F5C6A16,
        0x3F5EF778, 0x3F618C6B, 0x3F642905, 0x3F66CD5E,
        0x3F69798B, 0x3F6C2DA4, 0x3F6EE9C1, 0x3F71ADF9,
        0x3F747A65, 0x3F774F1C, 0x3F7A2C38, 0x3F7D11D1,
        0x3F800000, 0x3F817B70, 0x3F82FB44, 0x3F847F8A,
        0x3F86084F, 0x3F8795A0, 0x3F89278B, 0x3F8ABE1E,
        0x3F8C5966, 0x3F8DF971, 0x3F8F9E4D, 0x3F914809,
        0x3F92F6B3, 0x3F94AA59, 0x3F96630B, 0x3F9820D7,
        0x3F99E3CD, 0x3F9BABFC, 0x3F9D7972, 0x3F9F4C41,
        0x3FA12478, 0x3FA30226, 0x3FA4E55C, 0x3FA6CE2B,
        0x3FA8BCA3, 0x3FAAB0D5, 0x3FACAAD1, 0x3FAEAAA9,
        0x3FB0B06E, 0x3FB2BC33, 0x3FB4CE08, 0x3FB6E5FF,
        0x3FB9042C, 0x3FBB289F, 0x3FBD536C, 0x3FBF84A6,
        0x3FC1BC60, 0x3FC3FAAD, 0x3FC63F9F, 0x3FC88B4D,
        0x3FCADDC8, 0x3FCD3725, 0x3FCF9779, 0x3FD1FED8,
        0x3FD46D58, 0x3FD6E30D, 0x3FD9600C, 0x3FDBE46C,
        0x3FDE7042, 0x3FE103A4, 0x3FE39EA9, 0x3FE64167,
        0x3FE8EBF5, 0x3FEB9E6B, 0x3FEE58DF, 0x3FF11B6A,
        0x3FF3E623, 0x3FF6B923, 0x3FF99482, 0x3FFC7859,
        0x3FFF64C1,
    };

__declspec(align(4)) const u32 lbl_eu_805212D4[257] = {
        0x3F800000, 0x3F7F7FE0, 0x3F7EFF80, 0x3F7E7EDE,
        0x3F7DFDFC, 0x3F7D7CD8, 0x3F7CFB72, 0x3F7C79CA,
        0x3F7BF7DF, 0x3F7B75B1, 0x3F7AF340, 0x3F7A708B,
        0x3F79ED91, 0x3F796A52, 0x3F78E6CE, 0x3F786305,
        0x3F77DEF6, 0x3F775AA0, 0x3F76D603, 0x3F76511E,
        0x3F75CBF2, 0x3F75467E, 0x3F74C0C0, 0x3F743ABA,
        0x3F73B46A, 0x3F732DCF, 0x3F72A6EA, 0x3F721FBA,
        0x3F71983E, 0x3F711076, 0x3F708862, 0x3F700000,
        0x3F6F7751, 0x3F6EEE53, 0x3F6E6507, 0x3F6DDB6B,
        0x3F6D517F, 0x3F6CC744, 0x3F6C3CB7, 0x3F6BB1D9,
        0x3F6B26A9, 0x3F6A9B26, 0x3F6A0F50, 0x3F698327,
        0x3F68F6A9, 0x3F6869D6, 0x3F67DCAE, 0x3F674F2F,
        0x3F66C15A, 0x3F66332E, 0x3F65A4A9, 0x3F6515CC,
        0x3F648695, 0x3F63F704, 0x3F636719, 0x3F62D6D3,
        0x3F624630, 0x3F61B531, 0x3F6123D4, 0x3F609219,
        0x3F600000, 0x3F5F6D87, 0x3F5EDAAE, 0x3F5E4773,
        0x3F5DB3D7, 0x3F5D1FD9, 0x3F5C8B77, 0x3F5BF6B1,
        0x3F5B6186, 0x3F5ACBF5, 0x3F5A35FE, 0x3F599FA0,
        0x3F5908D9, 0x3F5871A9, 0x3F57DA10, 0x3F57420B,
        0x3F56A99B, 0x3F5610BF, 0x3F557775, 0x3F54DDBC,
        0x3F544395, 0x3F53A8FD, 0x3F530DF3, 0x3F527278,
        0x3F51D689, 0x3F513A26, 0x3F509D4E, 0x3F500000,
        0x3F4F623A, 0x3F4EC3FC, 0x3F4E2545, 0x3F4D8613,
        0x3F4CE665, 0x3F4C463A, 0x3F4BA592, 0x3F4B046A,
        0x3F4A62C2, 0x3F49C098, 0x3F491DEC, 0x3F487ABC,
        0x3F47D706, 0x3F4732CA, 0x3F468E06, 0x3F45E8B9,
        0x3F4542E1, 0x3F449C7E, 0x3F43F58D, 0x3F434E0D,
        0x3F42A5FE, 0x3F41FD5C, 0x3F415428, 0x3F40AA5F,
        0x3F400000, 0x3F3F5509, 0x3F3EA979, 0x3F3DFD4E,
        0x3F3D5087, 0x3F3CA321, 0x3F3BF51B, 0x3F3B4673,
        0x3F3A9728, 0x3F39E738, 0x3F3936A1, 0x3F388560,
        0x3F37D375, 0x3F3720DD, 0x3F366D96, 0x3F35B99E,
        0x3F3504F3, 0x3F344F93, 0x3F33997C, 0x3F32E2AC,
        0x3F322B20, 0x3F3172D6, 0x3F30B9CC, 0x3F300000,
        0x3F2F456F, 0x3F2E8A16, 0x3F2DCDF3, 0x3F2D1104,
        0x3F2C5345, 0x3F2B94B5, 0x3F2AD550, 0x3F2A1514,
        0x3F2953FD, 0x3F28920A, 0x3F27CF36, 0x3F270B7F,
        0x3F2646E1, 0x3F25815A, 0x3F24BAE7, 0x3F23F383,
        0x3F232B2B, 0x3F2261DC, 0x3F219792, 0x3F20CC4A,
        0x3F200000, 0x3F1F32AF, 0x3F1E6455, 0x3F1D94EC,
        0x3F1CC471, 0x3F1BF2DF, 0x3F1B2032, 0x3F1A4C65,
        0x3F197774, 0x3F18A15A, 0x3F17CA11, 0x3F16F196,
        0x3F1617E3, 0x3F153CF2, 0x3F1460BE, 0x3F138341,
        0x3F12A476, 0x3F11C456, 0x3F10E2DC, 0x3F100000,
        0x3F0F1BBD, 0x3F0E360B, 0x3F0D4EE4, 0x3F0C6641,
        0x3F0B7C1A, 0x3F0A9067, 0x3F09A320, 0x3F08B43D,
        0x3F07C3B6, 0x3F06D182, 0x3F05DD98, 0x3F04E7EE,
        0x3F03F07B, 0x3F02F734, 0x3F01FC10, 0x3F00FF02,
        0x3F000000, 0x3EFDFDFC, 0x3EFBF7DF, 0x3EF9ED91,
        0x3EF7DEF6, 0x3EF5CBF2, 0x3EF3B46A, 0x3EF1983E,
        0x3EEF7751, 0x3EED517F, 0x3EEB26A9, 0x3EE8F6A9,
        0x3EE6C15A, 0x3EE48695, 0x3EE24630, 0x3EE00000,
        0x3EDDB3D7, 0x3EDB6186, 0x3ED908D9, 0x3ED6A99B,
        0x3ED44395, 0x3ED1D689, 0x3ECF623A, 0x3ECCE665,
        0x3ECA62C2, 0x3EC7D706, 0x3EC542E1, 0x3EC2A5FE,
        0x3EC00000, 0x3EBD5087, 0x3EBA9728, 0x3EB7D375,
        0x3EB504F3, 0x3EB22B20, 0x3EAF456F, 0x3EAC5345,
        0x3EA953FD, 0x3EA646E1, 0x3EA32B2B, 0x3EA00000,
        0x3E9CC471, 0x3E997774, 0x3E9617E3, 0x3E92A476,
        0x3E8F1BBD, 0x3E8B7C1A, 0x3E87C3B6, 0x3E83F07B,
        0x3E800000, 0x3E77DEF6, 0x3E6F7751, 0x3E66C15A,
        0x3E5DB3D7, 0x3E544395, 0x3E4A62C2, 0x3E400000,
        0x3E3504F3, 0x3E2953FD, 0x3E1CC471, 0x3E0F1BBD,
        0x3E000000, 0x3DDDB3D7, 0x3DB504F3, 0x3D800000,
        0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_805216D8[257] = {
        0x3F800000, 0x3F7FFEC4, 0x3F7FFB11, 0x3F7FF4E6,
        0x3F7FEC43, 0x3F7FE129, 0x3F7FD398, 0x3F7FC38F,
        0x3F7FB10F, 0x3F7F9C18, 0x3F7F84AB, 0x3F7F6AC7,
        0x3F7F4E6D, 0x3F7F2F9D, 0x3F7F0E58, 0x3F7EEA9D,
        0x3F7EC46D, 0x3F7E9BC9, 0x3F7E70B0, 0x3F7E4323,
        0x3F7E1324, 0x3F7DE0B1, 0x3F7DABCC, 0x3F7D7474,
        0x3F7D3AAC, 0x3F7CFE73, 0x3F7CBFC9, 0x3F7C7EB0,
        0x3F7C3B28, 0x3F7BF531, 0x3F7BACCD, 0x3F7B61FC,
        0x3F7B14BE, 0x3F7AC516, 0x3F7A7302, 0x3F7A1E84,
        0x3F79C79D, 0x3F796E4E, 0x3F791298, 0x3F78B47B,
        0x3F7853F8, 0x3F77F110, 0x3F778BC5, 0x3F772417,
        0x3F76BA07, 0x3F764D97, 0x3F75DEC6, 0x3F756D97,
        0x3F74FA0B, 0x3F748422, 0x3F740BDD, 0x3F73913F,
        0x3F731447, 0x3F7294F8, 0x3F721352, 0x3F718F57,
        0x3F710908, 0x3F708066, 0x3F6FF573, 0x3F6F6830,
        0x3F6ED89E, 0x3F6E46BE, 0x3F6DB293, 0x3F6D1C1D,
        0x3F6C835E, 0x3F6BE858, 0x3F6B4B0C, 0x3F6AAB7B,
        0x3F6A09A7, 0x3F696591, 0x3F68BF3C, 0x3F6816A8,
        0x3F676BD8, 0x3F66BECC, 0x3F660F88, 0x3F655E0B,
        0x3F64AA59, 0x3F63F473, 0x3F633C5A, 0x3F628210,
        0x3F61C598, 0x3F6106F2, 0x3F604621, 0x3F5F8327,
        0x3F5EBE05, 0x3F5DF6BE, 0x3F5D2D53, 0x3F5C61C7,
        0x3F5B941A, 0x3F5AC450, 0x3F59F26A, 0x3F591E6A,
        0x3F584853, 0x3F577026, 0x3F5695E5, 0x3F55B993,
        0x3F54DB31, 0x3F53FAC3, 0x3F531849, 0x3F5233C6,
        0x3F514D3D, 0x3F5064AF, 0x3F4F7A1F, 0x3F4E8D90,
        0x3F4D9F02, 0x3F4CAE79, 0x3F4BBBF8, 0x3F4AC77F,
        0x3F49D112, 0x3F48D8B3, 0x3F47DE65, 0x3F46E22A,
        0x3F45E403, 0x3F44E3F5, 0x3F43E200, 0x3F42DE29,
        0x3F41D870, 0x3F40D0DA, 0x3F3FC767, 0x3F3EBC1B,
        0x3F3DAEF9, 0x3F3CA003, 0x3F3B8F3B, 0x3F3A7CA4,
        0x3F396842, 0x3F385216, 0x3F373A23, 0x3F36206C,
        0x3F3504F3, 0x3F33E7BC, 0x3F32C8C9, 0x3F31A81D,
        0x3F3085BB, 0x3F2F61A5, 0x3F2E3BDE, 0x3F2D1469,
        0x3F2BEB4A, 0x3F2AC082, 0x3F299415, 0x3F286605,
        0x3F273656, 0x3F26050A, 0x3F24D225, 0x3F239DA9,
        0x3F226799, 0x3F212FF9, 0x3F1FF6CB, 0x3F1EBC12,
        0x3F1D7FD1, 0x3F1C420C, 0x3F1B02C6, 0x3F19C200,
        0x3F187FC0, 0x3F173C07, 0x3F15F6D9, 0x3F14B039,
        0x3F13682A, 0x3F121EB0, 0x3F10D3CD, 0x3F0F8784,
        0x3F0E39DA, 0x3F0CEAD0, 0x3F0B9A6B, 0x3F0A48AD,
        0x3F08F59B, 0x3F07A136, 0x3F064B82, 0x3F04F484,
        0x3F039C3D, 0x3F0242B1, 0x3F00E7E4, 0x3EFF17B2,
        0x3EFC5D27, 0x3EF9A02D, 0x3EF6E0CB, 0x3EF41F07,
        0x3EF15AEA, 0x3EEE9479, 0x3EEBCBBB, 0x3EE900B7,
        0x3EE63375, 0x3EE363FA, 0x3EE0924F, 0x3EDDBE79,
        0x3EDAE880, 0x3ED8106B, 0x3ED53641, 0x3ED25A09,
        0x3ECF7BCA, 0x3ECC9B8B, 0x3EC9B953, 0x3EC6D529,
        0x3EC3EF15, 0x3EC1071E, 0x3EBE1D4A, 0x3EBB31A0,
        0x3EB8442A, 0x3EB554EC, 0x3EB263EF, 0x3EAF713A,
        0x3EAC7CD4, 0x3EA986C4, 0x3EA68F12, 0x3EA395C5,
        0x3EA09AE5, 0x3E9D9E78, 0x3E9AA086, 0x3E97A117,
        0x3E94A031, 0x3E919DDD, 0x3E8E9A22, 0x3E8B9507,
        0x3E888E93, 0x3E8586CE, 0x3E827DC0, 0x3E7EE6E1,
        0x3E78CFCC, 0x3E72B651, 0x3E6C9A7F, 0x3E667C66,
        0x3E605C13, 0x3E5A3997, 0x3E541501, 0x3E4DEE60,
        0x3E47C5C2, 0x3E419B37, 0x3E3B6ECF, 0x3E354098,
        0x3E2F10A2, 0x3E28DEFC, 0x3E22ABB6, 0x3E1C76DE,
        0x3E164083, 0x3E1008B7, 0x3E09CF86, 0x3E039502,
        0x3DFAB273, 0x3DEE3876, 0x3DE1BC2E, 0x3DD53DB9,
        0x3DC8BD36, 0x3DBC3AC3, 0x3DAFB680, 0x3DA3308C,
        0x3D96A905, 0x3D8A200A, 0x3D7B2B74, 0x3D621468,
        0x3D48FB2F, 0x3D2FE007, 0x3D16C32C, 0x3CFB49B9,
        0x3CC90AB0, 0x3C96C9B6, 0x3C490E90, 0x3BC90F89,
        0x00000000,
    };

__declspec(align(4)) const u32 lbl_eu_80521ADC[257] = {
        0x3F800000, 0x3F7F0000, 0x3F7E0000, 0x3F7D0000,
        0x3F7C0000, 0x3F7B0000, 0x3F7A0000, 0x3F790000,
        0x3F780000, 0x3F770000, 0x3F760000, 0x3F750000,
        0x3F740000, 0x3F730000, 0x3F720000, 0x3F710000,
        0x3F700000, 0x3F6F0000, 0x3F6E0000, 0x3F6D0000,
        0x3F6C0000, 0x3F6B0000, 0x3F6A0000, 0x3F690000,
        0x3F680000, 0x3F670000, 0x3F660000, 0x3F650000,
        0x3F640000, 0x3F630000, 0x3F620000, 0x3F610000,
        0x3F600000, 0x3F5F0000, 0x3F5E0000, 0x3F5D0000,
        0x3F5C0000, 0x3F5B0000, 0x3F5A0000, 0x3F590000,
        0x3F580000, 0x3F570000, 0x3F560000, 0x3F550000,
        0x3F540000, 0x3F530000, 0x3F520000, 0x3F510000,
        0x3F500000, 0x3F4F0000, 0x3F4E0000, 0x3F4D0000,
        0x3F4C0000, 0x3F4B0000, 0x3F4A0000, 0x3F490000,
        0x3F480000, 0x3F470000, 0x3F460000, 0x3F450000,
        0x3F440000, 0x3F430000, 0x3F420000, 0x3F410000,
        0x3F400000, 0x3F3F0000, 0x3F3E0000, 0x3F3D0000,
        0x3F3C0000, 0x3F3B0000, 0x3F3A0000, 0x3F390000,
        0x3F380000, 0x3F370000, 0x3F360000, 0x3F350000,
        0x3F340000, 0x3F330000, 0x3F320000, 0x3F310000,
        0x3F300000, 0x3F2F0000, 0x3F2E0000, 0x3F2D0000,
        0x3F2C0000, 0x3F2B0000, 0x3F2A0000, 0x3F290000,
        0x3F280000, 0x3F270000, 0x3F260000, 0x3F250000,
        0x3F240000, 0x3F230000, 0x3F220000, 0x3F210000,
        0x3F200000, 0x3F1F0000, 0x3F1E0000, 0x3F1D0000,
        0x3F1C0000, 0x3F1B0000, 0x3F1A0000, 0x3F190000,
        0x3F180000, 0x3F170000, 0x3F160000, 0x3F150000,
        0x3F140000, 0x3F130000, 0x3F120000, 0x3F110000,
        0x3F100000, 0x3F0F0000, 0x3F0E0000, 0x3F0D0000,
        0x3F0C0000, 0x3F0B0000, 0x3F0A0000, 0x3F090000,
        0x3F080000, 0x3F070000, 0x3F060000, 0x3F050000,
        0x3F040000, 0x3F030000, 0x3F020000, 0x3F010000,
        0x3F000000, 0x3EFE0000, 0x3EFC0000, 0x3EFA0000,
        0x3EF80000, 0x3EF60000, 0x3EF40000, 0x3EF20000,
        0x3EF00000, 0x3EEE0000, 0x3EEC0000, 0x3EEA0000,
        0x3EE80000, 0x3EE60000, 0x3EE40000, 0x3EE20000,
        0x3EE00000, 0x3EDE0000, 0x3EDC0000, 0x3EDA0000,
        0x3ED80000, 0x3ED60000, 0x3ED40000, 0x3ED20000,
        0x3ED00000, 0x3ECE0000, 0x3ECC0000, 0x3ECA0000,
        0x3EC80000, 0x3EC60000, 0x3EC40000, 0x3EC20000,
        0x3EC00000, 0x3EBE0000, 0x3EBC0000, 0x3EBA0000,
        0x3EB80000, 0x3EB60000, 0x3EB40000, 0x3EB20000,
        0x3EB00000, 0x3EAE0000, 0x3EAC0000, 0x3EAA0000,
        0x3EA80000, 0x3EA60000, 0x3EA40000, 0x3EA20000,
        0x3EA00000, 0x3E9E0000, 0x3E9C0000, 0x3E9A0000,
        0x3E980000, 0x3E960000, 0x3E940000, 0x3E920000,
        0x3E900000, 0x3E8E0000, 0x3E8C0000, 0x3E8A0000,
        0x3E880000, 0x3E860000, 0x3E840000, 0x3E820000,
        0x3E800000, 0x3E7C0000, 0x3E780000, 0x3E740000,
        0x3E700000, 0x3E6C0000, 0x3E680000, 0x3E640000,
        0x3E600000, 0x3E5C0000, 0x3E580000, 0x3E540000,
        0x3E500000, 0x3E4C0000, 0x3E480000, 0x3E440000,
        0x3E400000, 0x3E3C0000, 0x3E380000, 0x3E340000,
        0x3E300000, 0x3E2C0000, 0x3E280000, 0x3E240000,
        0x3E200000, 0x3E1C0000, 0x3E180000, 0x3E140000,
        0x3E100000, 0x3E0C0000, 0x3E080000, 0x3E040000,
        0x3E000000, 0x3DF80000, 0x3DF00000, 0x3DE80000,
        0x3DE00000, 0x3DD80000, 0x3DD00000, 0x3DC80000,
        0x3DC00000, 0x3DB80000, 0x3DB00000, 0x3DA80000,
        0x3DA00000, 0x3D980000, 0x3D900000, 0x3D880000,
        0x3D800000, 0x3D700000, 0x3D600000, 0x3D500000,
        0x3D400000, 0x3D300000, 0x3D200000, 0x3D100000,
        0x3D000000, 0x3CE00000, 0x3CC00000, 0x3CA00000,
        0x3C800000, 0x3C400000, 0x3C000000, 0x3B800000,
        0x00000000,
    };

__declspec(align(8)) const u32 lbl_eu_80521EE0[320] = {
        0x387C70F7, 0x387C8144, 0xC13E3549, 0x6A933549,
        0x88E9C7F2, 0x31C46389, 0x31C49211, 0xCE952E7C,
        0x5CF92E7C, 0x9B3ED422, 0x2B6F56DE, 0x2B6FA44C,
        0xD8C72899, 0x51332899, 0xAD24DCA7, 0x25F74BEE,
        0x25F7B5BA, 0xDFE42384, 0x47092384, 0xBE07E294,
        0x213D427A, 0x213DC608, 0xE4CE1F1D, 0x3E3A1F1D,
        0xCDBCE6A2, 0x1D223A44, 0x1D22D525, 0xE81D1B48,
        0x36901B48, 0xDC45E94C, 0x198D331A, 0x198DE31D,
        0xEA3917EE, 0x2FDC17EE, 0xE9B0EAEC, 0x16692CD2,
        0x1669F001, 0xEB6C14FD, 0x29F914FD, 0xF613EBBE,
        0x13A7274D, 0x13A7FBE7, 0xEBE91265, 0x24CB1265,
        0x0180EBF1, 0x11382270, 0x113806E2, 0xEBD9101C,
        0x2039101C, 0x0C0DEBA6, 0x0F121E24, 0x0F121104,
        0xEB5A0E18, 0x1C300E18, 0x15C9EAF8, 0x0D2D1A59,
        0x0D2D1A5D, 0xEA840C50, 0x189F0C50, 0x1EC3E9FE,
        0x0B801700, 0x0B8022FC, 0xE9690ABD, 0x15790ABD,
        0x270AE8C7, 0x0A05140B, 0x0A052AED, 0xE81A0959,
        0x12B30959, 0x2EA9E763, 0x08B81170, 0x08B8323D,
        0xE6A30820, 0x10410820, 0x35ACE5DD, 0x07920F25,
        0x079238F6, 0xE510070D, 0x0E1A070D, 0x3C1DE43E,
        0x06900D21, 0x06903F23, 0xE369061B, 0x0C37061B,
        0x4208E290, 0x05AE0B5C, 0x05AE44CD, 0xE1B60548,
        0x0A900548, 0x4774E0DA, 0x04E809D0, 0x04E849FE,
        0xDFFD048F, 0x091E048F, 0x4C6CDF20, 0x043B0877,
        0x043B4EBF, 0xDE4403ED, 0x07DB03ED, 0x50F8DD69,
        0x03A50749, 0x03A55317, 0xDC900361, 0x06C20361,
        0x551FDBB8, 0x03220643, 0x03225710, 0xDAE302E7,
        0x05CE02E7, 0x58EADA11, 0x02B00560, 0x02B05AAF,
        0xD941027D, 0x04FA027D, 0x5C60D875, 0x024E049B,
        0x024E5DFE, 0xD7AD0222, 0x04430222, 0x5F88D6E8,
        0x01F903F1, 0x01F96101, 0xD62701D3, 0x03A501D3,
        0x6269D56A, 0x01AF035F, 0x01AF63C0, 0xD4B1018F,
        0x031D018F, 0x6507D3FC, 0x017002E1, 0x01706640,
        0xD34C0154, 0x02A80154, 0x676AD2A0, 0x013A0274,
        0x013A6887, 0xD1F90122, 0x02440122, 0x6996D156,
        0x010C0217, 0x010C6A99, 0xD0B700F7, 0x01EE00F7,
        0x6B90D01D, 0x00E401C7, 0x00E46C7C, 0xCF8700D2,
        0x01A400D2, 0x6D5DCEF6, 0x00C20183, 0x00C26E33,
        0xCE6900B2, 0x016500B2, 0x6F00CDE0, 0x00A40149,
        0x00A46FC3, 0xCD5C0098, 0x012F0098, 0x707DCCDC,
        0x008C0117, 0x008C712F, 0xCC600081, 0x01010081,
        0x71D9CBE7, 0x007600ED, 0x0076727A, 0xCB73006D,
        0x00DA006D, 0x7315CB03, 0x006400C9, 0x006473A8,
        0xCA97005C, 0x00B9005C, 0x7434CA2E, 0x005500AA,
        0x005574BB, 0xC9C9004E, 0x009C004E, 0x753BC967,
        0x00480090, 0x004875B5, 0xC9090042, 0x00840042,
        0x7629C8AF, 0x003D007A, 0x003D7699, 0xC8570038,
        0x00700038, 0x7703C803, 0x00330067, 0x00337768,
        0xC7B1002F, 0x005F002F, 0x77C9C763, 0x002C0057,
        0x002C7826, 0xC7180028, 0x00500028, 0x787EC6CF,
        0x0025004A, 0x002578D2, 0xC6890022, 0x00440022,
        0x7923C646, 0x001F003E, 0x001F7970, 0xC606001D,
        0x0039001D, 0x79B9C5C7, 0x001A0034, 0x001A7A00,
        0xC58C0018, 0x00300018, 0x7A43C552, 0x0016002C,
        0x00167A83, 0xC51B0014, 0x00290014, 0x7AC0C4E6,
        0x00130025, 0x00137AFB, 0xC4B30011, 0x00220011,
        0x7B32C482, 0x00100020, 0x00107B68, 0xC452000E,
        0x001D000E, 0x7B9BC425, 0x000D001B, 0x000D7BCC,
        0xC3FA000C, 0x0018000C, 0x7BFAC3D0, 0x000B0016,
        0x000B7C27, 0xC3A8000A, 0x0015000A, 0x7C52C381,
        0x00090013, 0x00097C7A, 0xC35C0009, 0x00110009,
        0x7CA1C339, 0x00080010, 0x00087CC7, 0xC3170007,
        0x000F0007, 0x7CEAC2F6, 0x0007000D, 0x00077D0C,
        0xC2D70006, 0x000C0006, 0x7D2DC2B9, 0x0006000B,
        0x00067D4C, 0xC29C0005, 0x000A0005, 0x7D6AC280,
        0x0005000A, 0x00057D86, 0xC2650004, 0x00090004,
        0x7DA1C24C, 0x00040008, 0x00047DBB, 0xC2340004,
        0x00070004, 0x7DD4C21C, 0x00030007, 0x00037DEC,
        0xC2060003, 0x00060003, 0x7E03C1F0, 0x00030006,
        0x00037E19, 0xC1DB0003, 0x00050003, 0x7E2EC1C8,
        0x00020005, 0x00027E42, 0xC1B50002, 0x00040002,
        0x7E55C1A2, 0x00020004, 0x00027E67, 0xC1910002,
        0x00040002, 0x7E79C180, 0x00020003, 0x00027E89,
        0xC1700002, 0x00030002, 0x7E99C161, 0x00010003,
        0x00017EA9, 0xC1520001, 0x00030001, 0x7EB7C144,
        0x00010002, 0x00017EC5, 0xC1360001, 0x00020001,
        0x7ED3C129, 0x00010002, 0x00017EE0, 0xC11D0001,
        0x00020001, 0x7EECC111, 0x00010002, 0x00017EF8,
        0xC1050001, 0x00020001, 0x7F03C0FA, 0x00010001,
        0x00017F0E, 0xC0F00001, 0x00010001, 0x7F18C0E6,
    };

__declspec(align(8)) const u32 lbl_eu_805223E0[12] = {
        0x00500064, 0x008000A0, 0x00C80100, 0x01400190,
        0x01F40280, 0x032003E8, 0x05000640, 0x07D00A00,
        0x0C800FA0, 0x14001900, 0x1F402800, 0x32003E80,
    };

/* ======================= .data ======================= */
__declspec(align(8)) u8 lbl_eu_80568F38[15] = {
        0x25, 0x73, 0x3A, 0x25, 0x64, 0x20, 0x57, 0x61, 0x72, 0x6E, 0x69, 0x6E, 0x67, 0x3A, 0x00,
    };

__declspec(align(1)) u8 gap_07_80568F47_data[25] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

__declspec(align(8)) u32 lbl_eu_80568F60[8] = {
        0x0000000E, 0x33444D6F, 0x64656C73, 0x284E5734,
        0x52290000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80568F80[8] = {
        0x0000000E, 0x50616C65, 0x74746573, 0x284E5734,
        0x52290000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80568FA0[8] = {
        0x0000000E, 0x54657874, 0x75726573, 0x284E5734,
        0x52290000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80568FC0[16] = {
        0x0000000C, 0x416E6D43, 0x6872284E, 0x57345229,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x0000000C, 0x416E6D56, 0x6973284E, 0x57345229,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569000[8] = {
        0x0000000C, 0x416E6D43, 0x6C72284E, 0x57345229,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569020[8] = {
        0x0000000F, 0x416E6D54, 0x65785061, 0x74284E57,
        0x34522900, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569040[8] = {
        0x0000000F, 0x416E6D54, 0x65785372, 0x74284E57,
        0x34522900, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569060[8] = {
        0x0000000C, 0x416E6D53, 0x6870284E, 0x57345229,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569080[16] = {
        0x0000000C, 0x416E6D53, 0x636E284E, 0x57345229,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000008, 0x45787465, 0x726E616C, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_805690C0[8] = {
        0x00010001, 0x00010705, 0x06000000, 0x00000007,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_805690E0[8] = {
        0x0000000E, 0x4C696768, 0x74536574, 0x284E5734,
        0x52290000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569100[8] = {
        0x0000000F, 0x416D624C, 0x69676874, 0x73284E57,
        0x34522900, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569120[8] = {
        0x0000000C, 0x4C696768, 0x7473284E, 0x57345229,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569140[8] = {
        0x0000000A, 0x466F6773, 0x284E5734, 0x52290000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569160[8] = {
        0x0000000D, 0x43616D65, 0x72617328, 0x4E573452,
        0x29000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569180[18] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d15AnmObjMatClrResCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d15AnmObjMatClrResFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d15AnmObjMatClrResFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d15AnmObjMatClrResCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d15AnmObjMatClrResCFv, (unsigned int)&SetFrame__Q34nw4r3g3d15AnmObjMatClrResFf,
        (unsigned int)&GetFrame__Q34nw4r3g3d15AnmObjMatClrResCFv, (unsigned int)&UpdateFrame__Q34nw4r3g3d15AnmObjMatClrResFv, (unsigned int)&SetUpdateRate__Q34nw4r3g3d15AnmObjMatClrResFf, (unsigned int)&GetUpdateRate__Q34nw4r3g3d15AnmObjMatClrResCFv,
        (unsigned int)&Bind__Q34nw4r3g3d15AnmObjMatClrResFQ34nw4r3g3d6ResMdl, (unsigned int)&Release__Q34nw4r3g3d12AnmObjMatClrFv, (unsigned int)&GetResult__Q34nw4r3g3d15AnmObjMatClrResFPQ34nw4r3g3d12ClrAnmResultUl, (unsigned int)&Attach__Q34nw4r3g3d12AnmObjMatClrFiPQ34nw4r3g3d15AnmObjMatClrRes,
        (unsigned int)&Detach__Q34nw4r3g3d12AnmObjMatClrFi, (unsigned int)&DetachAll__Q34nw4r3g3d12AnmObjMatClrFv,
    };

__declspec(align(8)) u32 lbl_eu_805691C8[18] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d12AnmObjMatClrCFQ44nw4r3g3d6G3dObj7TypeObj, 0x00000000,
        (unsigned int)&__dt__Q34nw4r3g3d12AnmObjMatClrFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d12AnmObjMatClrCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d12AnmObjMatClrCFv, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, (unsigned int)&Release__Q34nw4r3g3d12AnmObjMatClrFv, 0x00000000, (unsigned int)&Attach__Q34nw4r3g3d12AnmObjMatClrFiPQ34nw4r3g3d15AnmObjMatClrRes,
        (unsigned int)&Detach__Q34nw4r3g3d12AnmObjMatClrFi, (unsigned int)&DetachAll__Q34nw4r3g3d12AnmObjMatClrFv,
    };

__declspec(align(8)) u32 lbl_eu_80569210[18] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d15AnmObjTexPatResCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d15AnmObjTexPatResFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d15AnmObjTexPatResFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d15AnmObjTexPatResCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d15AnmObjTexPatResCFv, (unsigned int)&SetFrame__Q34nw4r3g3d15AnmObjTexPatResFf,
        (unsigned int)&GetFrame__Q34nw4r3g3d15AnmObjTexPatResCFv, (unsigned int)&UpdateFrame__Q34nw4r3g3d15AnmObjTexPatResFv, (unsigned int)&SetUpdateRate__Q34nw4r3g3d15AnmObjTexPatResFf, (unsigned int)&GetUpdateRate__Q34nw4r3g3d15AnmObjTexPatResCFv,
        (unsigned int)&Bind__Q34nw4r3g3d15AnmObjTexPatResFQ34nw4r3g3d6ResMdl, (unsigned int)&Release__Q34nw4r3g3d12AnmObjTexPatFv, (unsigned int)&GetResult__Q34nw4r3g3d15AnmObjTexPatResFPQ34nw4r3g3d15TexPatAnmResultUl, (unsigned int)&Attach__Q34nw4r3g3d12AnmObjTexPatFiPQ34nw4r3g3d15AnmObjTexPatRes,
        (unsigned int)&Detach__Q34nw4r3g3d12AnmObjTexPatFi, (unsigned int)&DetachAll__Q34nw4r3g3d12AnmObjTexPatFv,
    };

__declspec(align(8)) u32 lbl_eu_80569258[18] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d12AnmObjTexPatCFQ44nw4r3g3d6G3dObj7TypeObj, 0x00000000,
        (unsigned int)&__dt__Q34nw4r3g3d12AnmObjTexPatFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d12AnmObjTexPatCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d12AnmObjTexPatCFv, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, (unsigned int)&Release__Q34nw4r3g3d12AnmObjTexPatFv, 0x00000000, (unsigned int)&Attach__Q34nw4r3g3d12AnmObjTexPatFiPQ34nw4r3g3d15AnmObjTexPatRes,
        (unsigned int)&Detach__Q34nw4r3g3d12AnmObjTexPatFi, (unsigned int)&DetachAll__Q34nw4r3g3d12AnmObjTexPatFv,
    };

__declspec(align(8)) u32 lbl_eu_805692A0[18] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d15AnmObjTexSrtResCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d15AnmObjTexSrtResFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d15AnmObjTexSrtResFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d15AnmObjTexSrtResCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d15AnmObjTexSrtResCFv, (unsigned int)&SetFrame__Q34nw4r3g3d15AnmObjTexSrtResFf,
        (unsigned int)&GetFrame__Q34nw4r3g3d15AnmObjTexSrtResCFv, (unsigned int)&UpdateFrame__Q34nw4r3g3d15AnmObjTexSrtResFv, (unsigned int)&SetUpdateRate__Q34nw4r3g3d15AnmObjTexSrtResFf, (unsigned int)&GetUpdateRate__Q34nw4r3g3d15AnmObjTexSrtResCFv,
        (unsigned int)&Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdl, (unsigned int)&Release__Q34nw4r3g3d12AnmObjTexSrtFv, (unsigned int)&GetResult__Q34nw4r3g3d15AnmObjTexSrtResFPQ34nw4r3g3d15TexSrtAnmResultUl, (unsigned int)&Attach__Q34nw4r3g3d12AnmObjTexSrtFiPQ34nw4r3g3d15AnmObjTexSrtRes,
        (unsigned int)&Detach__Q34nw4r3g3d12AnmObjTexSrtFi, (unsigned int)&DetachAll__Q34nw4r3g3d12AnmObjTexSrtFv,
    };

__declspec(align(8)) u32 lbl_eu_805692E8[18] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d12AnmObjTexSrtCFQ44nw4r3g3d6G3dObj7TypeObj, 0x00000000,
        (unsigned int)&__dt__Q34nw4r3g3d12AnmObjTexSrtFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d20AnmObjTexSrtOverrideCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d20AnmObjTexSrtOverrideCFv, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, (unsigned int)&Release__Q34nw4r3g3d12AnmObjTexSrtFv, 0x00000000, (unsigned int)&Attach__Q34nw4r3g3d12AnmObjTexSrtFiPQ34nw4r3g3d15AnmObjTexSrtRes,
        (unsigned int)&Detach__Q34nw4r3g3d12AnmObjTexSrtFi, (unsigned int)&DetachAll__Q34nw4r3g3d12AnmObjTexSrtFv,
    };

__declspec(align(8)) u32 lbl_eu_80569330[22] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d12AnmObjChrResCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d12AnmObjChrResFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d12AnmObjChrResFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d12AnmObjChrResCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d12AnmObjChrResCFv, (unsigned int)&SetFrame__Q34nw4r3g3d12AnmObjChrResFf,
        (unsigned int)&GetFrame__Q34nw4r3g3d12AnmObjChrResCFv, (unsigned int)&UpdateFrame__Q34nw4r3g3d12AnmObjChrResFv, (unsigned int)&SetUpdateRate__Q34nw4r3g3d12AnmObjChrResFf, (unsigned int)&GetUpdateRate__Q34nw4r3g3d12AnmObjChrResCFv,
        (unsigned int)&Bind__Q34nw4r3g3d12AnmObjChrResFQ34nw4r3g3d6ResMdl, (unsigned int)&Release__Q34nw4r3g3d9AnmObjChrFv, (unsigned int)&GetResult__Q34nw4r3g3d12AnmObjChrResFPQ34nw4r3g3d12ChrAnmResultUl, (unsigned int)&Attach__Q34nw4r3g3d9AnmObjChrFiPQ34nw4r3g3d12AnmObjChrRes,
        (unsigned int)&Detach__Q34nw4r3g3d9AnmObjChrFi, (unsigned int)&DetachAll__Q34nw4r3g3d9AnmObjChrFv, (unsigned int)&SetWeight__Q34nw4r3g3d9AnmObjChrFif, (unsigned int)&GetWeight__Q34nw4r3g3d9AnmObjChrCFi,
        (unsigned int)&Bind__Q34nw4r3g3d15AnmObjTexSrtResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption, (unsigned int)&Release__Q34nw4r3g3d12AnmObjChrResFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption,
    };

__declspec(align(8)) u32 lbl_eu_80569388[22] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d14AnmObjChrBlendCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d13AnmObjChrNodeFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d14AnmObjChrBlendFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d14AnmObjChrBlendCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d14AnmObjChrBlendCFv, (unsigned int)&SetFrame__Q34nw4r3g3d13AnmObjChrNodeFf,
        (unsigned int)&GetFrame__Q34nw4r3g3d13AnmObjChrNodeCFv, (unsigned int)&UpdateFrame__Q34nw4r3g3d13AnmObjChrNodeFv, (unsigned int)&SetUpdateRate__Q34nw4r3g3d13AnmObjChrNodeFf, (unsigned int)&GetUpdateRate__Q34nw4r3g3d13AnmObjChrNodeCFv,
        (unsigned int)&Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdl, (unsigned int)&Release__Q34nw4r3g3d13AnmObjChrNodeFv, (unsigned int)&GetResult__Q34nw4r3g3d14AnmObjChrBlendFPQ34nw4r3g3d12ChrAnmResultUl, (unsigned int)&Attach__Q34nw4r3g3d13AnmObjChrNodeFiPQ34nw4r3g3d12AnmObjChrRes,
        (unsigned int)&Detach__Q34nw4r3g3d13AnmObjChrNodeFi, (unsigned int)&DetachAll__Q34nw4r3g3d13AnmObjChrNodeFv, (unsigned int)&SetWeight__Q34nw4r3g3d14AnmObjChrBlendFif, (unsigned int)&GetWeight__Q34nw4r3g3d14AnmObjChrBlendCFi,
        (unsigned int)&Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption, (unsigned int)&Release__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption,
    };

__declspec(align(8)) u32 lbl_eu_805693E0[22] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d13AnmObjChrNodeCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d13AnmObjChrNodeFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d13AnmObjChrNodeFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d13AnmObjChrNodeCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d13AnmObjChrNodeCFv, (unsigned int)&SetFrame__Q34nw4r3g3d13AnmObjChrNodeFf,
        (unsigned int)&GetFrame__Q34nw4r3g3d13AnmObjChrNodeCFv, (unsigned int)&UpdateFrame__Q34nw4r3g3d13AnmObjChrNodeFv, (unsigned int)&SetUpdateRate__Q34nw4r3g3d13AnmObjChrNodeFf, (unsigned int)&GetUpdateRate__Q34nw4r3g3d13AnmObjChrNodeCFv,
        (unsigned int)&Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdl, (unsigned int)&Release__Q34nw4r3g3d13AnmObjChrNodeFv, 0x00000000, (unsigned int)&Attach__Q34nw4r3g3d13AnmObjChrNodeFiPQ34nw4r3g3d12AnmObjChrRes,
        (unsigned int)&Detach__Q34nw4r3g3d13AnmObjChrNodeFi, (unsigned int)&DetachAll__Q34nw4r3g3d13AnmObjChrNodeFv, (unsigned int)&SetWeight__Q34nw4r3g3d9AnmObjChrFif, (unsigned int)&GetWeight__Q34nw4r3g3d9AnmObjChrCFi,
        (unsigned int)&Bind__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption, (unsigned int)&Release__Q34nw4r3g3d13AnmObjChrNodeFQ34nw4r3g3d6ResMdlUlQ44nw4r3g3d9AnmObjChr10BindOption,
    };

__declspec(align(8)) u32 lbl_eu_80569438[22] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d9AnmObjChrCFQ44nw4r3g3d6G3dObj7TypeObj, 0x00000000,
        (unsigned int)&__dt__Q34nw4r3g3d9AnmObjChrFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d9AnmObjChrCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d9AnmObjChrCFv, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, (unsigned int)&Release__Q34nw4r3g3d9AnmObjChrFv, 0x00000000, (unsigned int)&Attach__Q34nw4r3g3d9AnmObjChrFiPQ34nw4r3g3d12AnmObjChrRes,
        (unsigned int)&Detach__Q34nw4r3g3d9AnmObjChrFi, (unsigned int)&DetachAll__Q34nw4r3g3d9AnmObjChrFv, (unsigned int)&SetWeight__Q34nw4r3g3d9AnmObjChrFif, (unsigned int)&GetWeight__Q34nw4r3g3d9AnmObjChrCFi,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569490[36] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d9AnmScnResCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d9AnmScnResFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d9AnmScnResFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&SetFrame__Q34nw4r3g3d9AnmScnResFf,
        (unsigned int)&GetFrame__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&SetUpdateRate__Q34nw4r3g3d9AnmScnResFf, (unsigned int)&GetUpdateRate__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&UpdateFrame__Q34nw4r3g3d9AnmScnResFv,
        (unsigned int)&Attach__Q34nw4r3g3d6AnmScnFiPQ34nw4r3g3d9AnmScnRes, (unsigned int)&Detach__Q34nw4r3g3d6AnmScnFi, (unsigned int)&GetNumLightSet__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetNumAmbLight__Q34nw4r3g3d9AnmScnResCFv,
        (unsigned int)&GetNumDiffuseLight__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetNumSpecularLight__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetNumFog__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetNumCamera__Q34nw4r3g3d9AnmScnResCFv,
        (unsigned int)&GetLightSetMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetAmbLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetDiffuseLightMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetFogMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv,
        (unsigned int)&GetCameraMaxRefNumber__Q34nw4r3g3d9AnmScnResCFv, (unsigned int)&GetLightSet__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d8LightSetUl, (unsigned int)&GetAmbLightColor__Q34nw4r3g3d9AnmScnResFUl, (unsigned int)&GetLight__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d8LightObjPQ34nw4r3g3d8LightObjUl,
        (unsigned int)&GetFog__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d3FogUl, (unsigned int)&GetCamera__Q34nw4r3g3d9AnmScnResFQ34nw4r3g3d6CameraUl, (unsigned int)&GetSpecularLightID__Q34nw4r3g3d9AnmScnResCFUl, (unsigned int)&HasSpecularLight__Q34nw4r3g3d9AnmScnResCFUl,
        (unsigned int)&GetAmbLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d17AmbLightAnmResultUl, (unsigned int)&GetLightResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d14LightAnmResultUl, (unsigned int)&GetFogResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d12FogAnmResultUl, (unsigned int)&GetCameraResult__Q34nw4r3g3d9AnmScnResFPQ34nw4r3g3d15CameraAnmResultUl,
    };

__declspec(align(8)) u32 lbl_eu_80569520[36] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d6AnmScnCFQ44nw4r3g3d6G3dObj7TypeObj, 0x00000000,
        (unsigned int)&__dt__Q34nw4r3g3d6AnmScnFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d6AnmScnCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d6AnmScnCFv, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        (unsigned int)&Attach__Q34nw4r3g3d6AnmScnFiPQ34nw4r3g3d9AnmScnRes, (unsigned int)&Detach__Q34nw4r3g3d6AnmScnFi, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_805695B0[8] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d6G3dObjCFQ44nw4r3g3d6G3dObj7TypeObj, 0x00000000,
        (unsigned int)&__dt__Q34nw4r3g3d6G3dObjFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d6G3dObjCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d6G3dObjCFv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_805695D0[6] = {
        0x00000000, 0x00000000, (unsigned int)&__cl__Q44nw4r3g3d8G3DState11IndMtxOpStdFPQ44nw4r3g3d8G3DState13IndTexMtxInfo, (unsigned int)&__dt__Q44nw4r3g3d8G3DState11IndMtxOpStdFv,
        (unsigned int)&Reset__Q44nw4r3g3d8G3DState11IndMtxOpStdFv, (unsigned int)&SetNrmMapMtx__Q44nw4r3g3d8G3DState11IndMtxOpStdF14_GXIndTexMtxIDPCQ34nw4r4math4VEC3PCQ34nw4r4math5MTX34Q44nw4r3g3d14ResMatMiscData14IndirectMethod,
    };

__declspec(align(8)) u8 lbl_eu_805695E8[9] = {
        0x4E, 0x6F, 0x64, 0x65, 0x54, 0x72, 0x65, 0x65, 0x00,
    };

__declspec(align(1)) u8 gap_07_805695F1_data[7] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

__declspec(align(8)) u32 jumptable_eu_805695F8[10] = {
        (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 392), (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 32), (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 72), (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 112),
        (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 152), (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 192), (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 232), (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 272),
        (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 312), (unsigned int)((char *)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl + 352),
    };

__declspec(align(8)) u32 jumptable_eu_80569620[10] = {
        (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 192), (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 48), (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 64), (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 80),
        (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 96), (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 112), (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 128), (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 144),
        (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 160), (unsigned int)((char *)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl + 176),
    };

__declspec(align(8)) u32 lbl_eu_80569648[16] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d8ScnGroupCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d8ScnGroupFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d8ScnGroupFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d8ScnGroupCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d8ScnGroupCFv, (unsigned int)&ForEach__Q34nw4r3g3d8ScnGroupFPFPQ34nw4r3g3d6ScnObjPv_Q44nw4r3g3d6ScnObj13ForEachResultPvb,
        (unsigned int)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl, (unsigned int)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl, (unsigned int)&GetValueForSortOpa__Q34nw4r3g3d6ScnObjCFv, (unsigned int)&GetValueForSortXlu__Q34nw4r3g3d6ScnObjCFv,
        (unsigned int)&CalcWorldMtx__Q34nw4r3g3d6ScnObjFPCQ34nw4r4math5MTX34PUl, (unsigned int)&Insert__Q34nw4r3g3d8ScnGroupFUlPQ34nw4r3g3d6ScnObj, (unsigned int)&Remove__Q34nw4r3g3d8ScnGroupFUl, (unsigned int)&Remove__Q34nw4r3g3d8ScnGroupFPQ34nw4r3g3d6ScnObj,
    };

__declspec(align(8)) u32 lbl_eu_80569688[14] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d6ScnObjCFQ44nw4r3g3d6G3dObj7TypeObj, 0x00000000,
        (unsigned int)&__dt__Q34nw4r3g3d6ScnObjFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d6ScnObjCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d6ScnObjCFv, 0x00000000,
        (unsigned int)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl, (unsigned int)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl, (unsigned int)&GetValueForSortOpa__Q34nw4r3g3d6ScnObjCFv, (unsigned int)&GetValueForSortXlu__Q34nw4r3g3d6ScnObjCFv,
        (unsigned int)&CalcWorldMtx__Q34nw4r3g3d6ScnObjFPCQ34nw4r4math5MTX34PUl, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_805696C0[12] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3g3d12ScnObjGatherFv, (unsigned int)&Add__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d6ScnObjbb,
        (unsigned int)&Clear__Q34nw4r3g3d12ScnObjGatherFv, (unsigned int)&ZSort__Q34nw4r3g3d12ScnObjGatherFv, (unsigned int)&Sort__Q34nw4r3g3d12ScnObjGatherFv, (unsigned int)&Sort__Q34nw4r3g3d12ScnObjGatherFPFPCQ34nw4r3g3d6ScnObjPCQ34nw4r3g3d6ScnObj_bPFPCQ34nw4r3g3d6ScnObjPCQ34nw4r3g3d6ScnObj_b,
        (unsigned int)&DrawOpa__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d14ResMdlDrawMode, (unsigned int)&DrawXlu__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d14ResMdlDrawMode, (unsigned int)&CheckScnObj__Q34nw4r3g3d12ScnObjGatherFPQ34nw4r3g3d6ScnObj, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_805696F0[16] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d7ScnRootCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d7ScnRootFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d7ScnRootFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d7ScnRootCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d7ScnRootCFv, (unsigned int)&ForEach__Q34nw4r3g3d8ScnGroupFPFPQ34nw4r3g3d6ScnObjPv_Q44nw4r3g3d6ScnObj13ForEachResultPvb,
        (unsigned int)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl, (unsigned int)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl, (unsigned int)&GetValueForSortOpa__Q34nw4r3g3d6ScnObjCFv, (unsigned int)&GetValueForSortXlu__Q34nw4r3g3d6ScnObjCFv,
        (unsigned int)&CalcWorldMtx__Q34nw4r3g3d6ScnObjFPCQ34nw4r4math5MTX34PUl, (unsigned int)&Insert__Q34nw4r3g3d8ScnGroupFUlPQ34nw4r3g3d6ScnObj, (unsigned int)&Remove__Q34nw4r3g3d8ScnGroupFUl, (unsigned int)&Remove__Q34nw4r3g3d8ScnGroupFPQ34nw4r3g3d6ScnObj,
    };

__declspec(align(8)) u32 jumptable_eu_80569730[7] = {
        (unsigned int)((char *)&SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType + 92), (unsigned int)((char *)&SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType + 180), (unsigned int)((char *)&SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType + 268), (unsigned int)((char *)&SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType + 356),
        (unsigned int)((char *)&SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType + 444), (unsigned int)((char *)&SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType + 960), (unsigned int)((char *)&SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType + 528),
    };

__declspec(align(4)) u8 lbl_eu_8056974C[9] = {
        0x4E, 0x6F, 0x64, 0x65, 0x54, 0x72, 0x65, 0x65, 0x00,
    };

__declspec(align(1)) u8 gap_07_80569755_data[3] = {
        0x00, 0x00, 0x00,
    };

__declspec(align(8)) u32 lbl_eu_80569758[18] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d12ScnMdlSimpleFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d12ScnMdlSimpleFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d12ScnMdlSimpleCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d12ScnMdlSimpleCFv, (unsigned int)&ForEach__Q34nw4r3g3d7ScnLeafFPFPQ34nw4r3g3d6ScnObjPv_Q44nw4r3g3d6ScnObj13ForEachResultPvb,
        (unsigned int)&SetScnObjOption__Q34nw4r3g3d12ScnMdlSimpleFUlUl, (unsigned int)&GetScnObjOption__Q34nw4r3g3d12ScnMdlSimpleCFUlPUl, (unsigned int)&GetValueForSortOpa__Q34nw4r3g3d6ScnObjCFv, (unsigned int)&GetValueForSortXlu__Q34nw4r3g3d6ScnObjCFv,
        (unsigned int)&CalcWorldMtx__Q34nw4r3g3d7ScnLeafFPCQ34nw4r4math5MTX34PUl, (unsigned int)&SetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType, (unsigned int)&RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFPQ34nw4r3g3d6AnmObj, (unsigned int)&RemoveAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType,
        (unsigned int)&GetAnmObj__Q34nw4r3g3d12ScnMdlSimpleFQ44nw4r3g3d12ScnMdlSimple10AnmObjType, (unsigned int)&GetAnmObj__Q34nw4r3g3d12ScnMdlSimpleCFQ44nw4r3g3d12ScnMdlSimple10AnmObjType,
    };

__declspec(align(8)) u32 lbl_eu_805697A0[18] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d6ScnMdlCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d6ScnMdlFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d6ScnMdlFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d6ScnMdlCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d6ScnMdlCFv, (unsigned int)&ForEach__Q34nw4r3g3d7ScnLeafFPFPQ34nw4r3g3d6ScnObjPv_Q44nw4r3g3d6ScnObj13ForEachResultPvb,
        (unsigned int)&SetScnObjOption__Q34nw4r3g3d6ScnMdlFUlUl, (unsigned int)&GetScnObjOption__Q34nw4r3g3d6ScnMdlCFUlPUl, (unsigned int)&GetValueForSortOpa__Q34nw4r3g3d6ScnObjCFv, (unsigned int)&GetValueForSortXlu__Q34nw4r3g3d6ScnObjCFv,
        (unsigned int)&CalcWorldMtx__Q34nw4r3g3d7ScnLeafFPCQ34nw4r4math5MTX34PUl, (unsigned int)&SetAnmObj__Q34nw4r3g3d6ScnMdlFPQ34nw4r3g3d6AnmObjQ44nw4r3g3d12ScnMdlSimple10AnmObjType, (unsigned int)&RemoveAnmObj__Q34nw4r3g3d6ScnMdlFPQ34nw4r3g3d6AnmObj, (unsigned int)&RemoveAnmObj__Q34nw4r3g3d6ScnMdlFQ44nw4r3g3d12ScnMdlSimple10AnmObjType,
        (unsigned int)&GetAnmObj__Q34nw4r3g3d6ScnMdlFQ44nw4r3g3d12ScnMdlSimple10AnmObjType, (unsigned int)&GetAnmObj__Q34nw4r3g3d6ScnMdlCFQ44nw4r3g3d12ScnMdlSimple10AnmObjType,
    };

__declspec(align(8)) u32 lbl_eu_805697E8[16] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d12ScnMdlExpandCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d12ScnMdlExpandFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d12ScnMdlExpandFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d12ScnMdlExpandCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d12ScnMdlExpandCFv, (unsigned int)&ForEach__Q34nw4r3g3d8ScnGroupFPFPQ34nw4r3g3d6ScnObjPv_Q44nw4r3g3d6ScnObj13ForEachResultPvb,
        (unsigned int)&SetScnObjOption__Q34nw4r3g3d6ScnObjFUlUl, (unsigned int)&GetScnObjOption__Q34nw4r3g3d6ScnObjCFUlPUl, (unsigned int)&GetValueForSortOpa__Q34nw4r3g3d6ScnObjCFv, (unsigned int)&GetValueForSortXlu__Q34nw4r3g3d6ScnObjCFv,
        (unsigned int)&CalcWorldMtx__Q34nw4r3g3d6ScnObjFPCQ34nw4r4math5MTX34PUl, (unsigned int)&Insert__Q34nw4r3g3d12ScnMdlExpandFUlPQ34nw4r3g3d6ScnObj, (unsigned int)&Remove__Q34nw4r3g3d12ScnMdlExpandFUl, (unsigned int)&Remove__Q34nw4r3g3d12ScnMdlExpandFPQ34nw4r3g3d6ScnObj,
    };

__declspec(align(8)) u32 lbl_eu_80569828[18] = {
        0x3C3C204E, 0x57345220, 0x2020202D, 0x20473344,
        0x20096669, 0x6E616C20, 0x20206275, 0x696C643A,
        0x204E6F76, 0x20323020, 0x32303038, 0x2030333A,
        0x32333A31, 0x34202830, 0x78343139, 0x395F3630,
        0x38333129, 0x203E3E00,
    };
    /* f32[] table (retail lbl_eu_80569828) */

__declspec(align(8)) u32 lbl_eu_80569870[14] = {
        0x00000000, 0x00000000, (unsigned int)&IsDerivedFrom__Q34nw4r3g3d7ScnProcCFQ44nw4r3g3d6G3dObj7TypeObj, (unsigned int)&G3dProc__Q34nw4r3g3d7ScnProcFUlUlPv,
        (unsigned int)&__dt__Q34nw4r3g3d7ScnProcFv, (unsigned int)&GetTypeObj__Q34nw4r3g3d7ScnProcCFv, (unsigned int)&GetTypeName__Q34nw4r3g3d7ScnProcCFv, (unsigned int)&ForEach__Q34nw4r3g3d7ScnLeafFPFPQ34nw4r3g3d6ScnObjPv_Q44nw4r3g3d6ScnObj13ForEachResultPvb,
        (unsigned int)&SetScnObjOption__Q34nw4r3g3d7ScnLeafFUlUl, (unsigned int)&GetScnObjOption__Q34nw4r3g3d7ScnLeafCFUlPUl, (unsigned int)&GetValueForSortOpa__Q34nw4r3g3d6ScnObjCFv, (unsigned int)&GetValueForSortXlu__Q34nw4r3g3d6ScnObjCFv,
        (unsigned int)&CalcWorldMtx__Q34nw4r3g3d7ScnLeafFPCQ34nw4r4math5MTX34PUl, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_805698A8[18] = {
        0x3C3C204E, 0x57345220, 0x2020202D, 0x204C5954,
        0x20096669, 0x6E616C20, 0x20206275, 0x696C643A,
        0x20417567, 0x20323120, 0x32303038, 0x2030353A,
        0x32313A35, 0x38202830, 0x78343139, 0x395F3630,
        0x38333129, 0x203E3E00,
    };
    /* f32[] table (retail lbl_eu_805698A8) */

__declspec(align(8)) u32 lbl_eu_805698F0[30] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt4PaneFv, (unsigned int)&GetRuntimeTypeInfo__Q34nw4r3lyt4PaneCFv,
        (unsigned int)&CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&DrawSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Animate__Q34nw4r3lyt4PaneFUl,
        (unsigned int)&AnimateSelf__Q34nw4r3lyt4PaneFUl, (unsigned int)&GetVtxColor__Q34nw4r3lyt4PaneCFUl, (unsigned int)&SetVtxColor__Q34nw4r3lyt4PaneFUlQ34nw4r2ut5Color, (unsigned int)&GetColorElement__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&SetColorElement__Q34nw4r3lyt4PaneFUlUc, (unsigned int)&GetVtxColorElement__Q34nw4r3lyt4PaneCFUl, (unsigned int)&SetVtxColorElement__Q34nw4r3lyt4PaneFUlUc, (unsigned int)&FindPaneByName__Q34nw4r3lyt4PaneFPCcb,
        (unsigned int)&FindMaterialByName__Q34nw4r3lyt4PaneFPCcb, (unsigned int)&BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (unsigned int)&UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb, (unsigned int)&UnbindAllAnimation__Q34nw4r3lyt4PaneFb,
        (unsigned int)&UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource, (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb,
        (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb, (unsigned int)&GetMaterialNum__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&LoadMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569968[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt5GroupFv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569978[16] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt6LayoutFv, (unsigned int)&Build__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor,
        (unsigned int)&CreateAnimTransform__Q34nw4r3lyt6LayoutFv, (unsigned int)&CreateAnimTransform__Q34nw4r3lyt6LayoutFPCvPQ34nw4r3lyt16ResourceAccessor, (unsigned int)&CreateAnimTransform__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor, (unsigned int)&BindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform,
        (unsigned int)&UnbindAnimation__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransform, (unsigned int)&UnbindAllAnimation__Q34nw4r3lyt6LayoutFv, (unsigned int)&BindAnimationAuto__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt12AnimResourcePQ34nw4r3lyt16ResourceAccessor, (unsigned int)&SetAnimationEnable__Q34nw4r3lyt6LayoutFPQ34nw4r3lyt13AnimTransformb,
        (unsigned int)&CalculateMtx__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Draw__Q34nw4r3lyt6LayoutFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Animate__Q34nw4r3lyt6LayoutFUl, 0,
    };

__declspec(align(8)) u32 lbl_eu_805699B8[30] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt7PictureFv, (unsigned int)&GetRuntimeTypeInfo__Q34nw4r3lyt7PictureCFv,
        (unsigned int)&CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&DrawSelf__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Animate__Q34nw4r3lyt4PaneFUl,
        (unsigned int)&AnimateSelf__Q34nw4r3lyt4PaneFUl, (unsigned int)&GetVtxColor__Q34nw4r3lyt7PictureCFUl, (unsigned int)&SetVtxColor__Q34nw4r3lyt7PictureFUlQ34nw4r2ut5Color, (unsigned int)&GetColorElement__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&SetColorElement__Q34nw4r3lyt4PaneFUlUc, (unsigned int)&GetVtxColorElement__Q34nw4r3lyt7PictureCFUl, (unsigned int)&SetVtxColorElement__Q34nw4r3lyt7PictureFUlUc, (unsigned int)&FindPaneByName__Q34nw4r3lyt4PaneFPCcb,
        (unsigned int)&FindMaterialByName__Q34nw4r3lyt4PaneFPCcb, (unsigned int)&BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (unsigned int)&UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb, (unsigned int)&UnbindAllAnimation__Q34nw4r3lyt4PaneFb,
        (unsigned int)&UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource, (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb,
        (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb, (unsigned int)&GetMaterialNum__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&LoadMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Append__Q34nw4r3lyt7PictureFRCQ34nw4r3lyt6TexMap,
    };

__declspec(align(8)) u32 lbl_eu_80569A30[34] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt7TextBoxFv, (unsigned int)&GetRuntimeTypeInfo__Q34nw4r3lyt7TextBoxCFv,
        (unsigned int)&CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&DrawSelf__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Animate__Q34nw4r3lyt4PaneFUl,
        (unsigned int)&AnimateSelf__Q34nw4r3lyt4PaneFUl, (unsigned int)&GetTextColor__Q34nw4r3lyt7TextBoxCFUl, (unsigned int)&SetVtxColor__Q34nw4r3lyt7TextBoxFUlQ34nw4r2ut5Color, (unsigned int)&GetColorElement__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&SetColorElement__Q34nw4r3lyt4PaneFUlUc, (unsigned int)&GetVtxColorElement__Q34nw4r3lyt7TextBoxCFUl, (unsigned int)&SetVtxColorElement__Q34nw4r3lyt7TextBoxFUlUc, (unsigned int)&FindPaneByName__Q34nw4r3lyt4PaneFPCcb,
        (unsigned int)&FindMaterialByName__Q34nw4r3lyt4PaneFPCcb, (unsigned int)&BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (unsigned int)&UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb, (unsigned int)&UnbindAllAnimation__Q34nw4r3lyt4PaneFb,
        (unsigned int)&UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource, (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb,
        (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb, (unsigned int)&GetMaterialNum__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&LoadMtx__Q34nw4r3lyt7TextBoxFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&AllocStringBuffer__Q34nw4r3lyt7TextBoxFUs, (unsigned int)&FreeStringBuffer__Q34nw4r3lyt7TextBoxFv, (unsigned int)&SetString__Q34nw4r3lyt7TextBoxFPCwUs,
        (unsigned int)&SetString__Q34nw4r3lyt7TextBoxFPCwUsUs, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569AB8[15] = {
        0x00000100, 0x00010101, 0x00010100, 0x00000101,
        0x00010001, 0x00010101, 0x00000100, 0x00010001,
        0x00000101, 0x01000100, 0x01010001, 0x01000000,
        0x00010100, 0x01010000, 0x00010100,
    };

__declspec(align(4)) u32 lbl_eu_80569AF4[35] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt6WindowFv, (unsigned int)&GetRuntimeTypeInfo__Q34nw4r3lyt6WindowCFv,
        (unsigned int)&CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&DrawSelf__Q34nw4r3lyt6WindowFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Animate__Q34nw4r3lyt4PaneFUl,
        (unsigned int)&AnimateSelf__Q34nw4r3lyt6WindowFUl, (unsigned int)&GetVtxColor__Q34nw4r3lyt6WindowCFUl, (unsigned int)&SetVtxColor__Q34nw4r3lyt6WindowFUlQ34nw4r2ut5Color, (unsigned int)&GetColorElement__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&SetColorElement__Q34nw4r3lyt4PaneFUlUc, (unsigned int)&GetVtxColorElement__Q34nw4r3lyt6WindowCFUl, (unsigned int)&SetVtxColorElement__Q34nw4r3lyt6WindowFUlUc, (unsigned int)&FindPaneByName__Q34nw4r3lyt4PaneFPCcb,
        (unsigned int)&FindMaterialByName__Q34nw4r3lyt6WindowFPCcb, (unsigned int)&BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (unsigned int)&UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb, (unsigned int)&UnbindAllAnimation__Q34nw4r3lyt4PaneFb,
        (unsigned int)&UnbindAnimationSelf__Q34nw4r3lyt6WindowFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource, (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb,
        (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb, (unsigned int)&GetMaterialNum__Q34nw4r3lyt6WindowCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt6WindowCFUl,
        (unsigned int)&LoadMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&GetContentMaterial__Q34nw4r3lyt6WindowCFv, (unsigned int)&GetFrameMaterial__Q34nw4r3lyt6WindowCFUl, (unsigned int)&DrawContent__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2RCQ34nw4r3lyt15WindowFrameSizeUc,
        (unsigned int)&DrawFrame__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2RCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc, (unsigned int)&DrawFrame4__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc, (unsigned int)&DrawFrame8__Q34nw4r3lyt6WindowFRCQ34nw4r4math4VEC2PCQ44nw4r3lyt6Window5FrameRCQ34nw4r3lyt15WindowFrameSizeUc,
    };

__declspec(align(8)) u32 lbl_eu_80569B80[30] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt8BoundingFv, (unsigned int)&GetRuntimeTypeInfo__Q34nw4r3lyt8BoundingCFv,
        (unsigned int)&CalculateMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Draw__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&DrawSelf__Q34nw4r3lyt8BoundingFRCQ34nw4r3lyt8DrawInfo, (unsigned int)&Animate__Q34nw4r3lyt4PaneFUl,
        (unsigned int)&AnimateSelf__Q34nw4r3lyt4PaneFUl, (unsigned int)&GetVtxColor__Q34nw4r3lyt4PaneCFUl, (unsigned int)&SetVtxColor__Q34nw4r3lyt4PaneFUlQ34nw4r2ut5Color, (unsigned int)&GetColorElement__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&SetColorElement__Q34nw4r3lyt4PaneFUlUc, (unsigned int)&GetVtxColorElement__Q34nw4r3lyt4PaneCFUl, (unsigned int)&SetVtxColorElement__Q34nw4r3lyt4PaneFUlUc, (unsigned int)&FindPaneByName__Q34nw4r3lyt4PaneFPCcb,
        (unsigned int)&FindMaterialByName__Q34nw4r3lyt4PaneFPCcb, (unsigned int)&BindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb, (unsigned int)&UnbindAnimation__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformb, (unsigned int)&UnbindAllAnimation__Q34nw4r3lyt4PaneFb,
        (unsigned int)&UnbindAnimationSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLinkSelf__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResource, (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFPQ34nw4r3lyt13AnimTransformbb,
        (unsigned int)&SetAnimationEnable__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt12AnimResourcebb, (unsigned int)&GetMaterialNum__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFv, (unsigned int)&GetMaterial__Q34nw4r3lyt4PaneCFUl,
        (unsigned int)&LoadMtx__Q34nw4r3lyt4PaneFRCQ34nw4r3lyt8DrawInfo, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569BF8[8] = {
        0x0000001F, 0x0000001B, 0x00000017, 0x00000013,
        0x0000001E, 0x0000001A, 0x00000016, 0x00000012,
    };

__declspec(align(8)) u32 lbl_eu_80569C18[8] = {
        0x0000001F, 0x0000001B, 0x00000017, 0x00000013,
        0x0000001E, 0x0000001A, 0x00000016, 0x00000012,
    };

__declspec(align(8)) u32 lbl_eu_80569C38[12] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt8MaterialFv, (unsigned int)&SetupGX__Q34nw4r3lyt8MaterialFbUc,
        (unsigned int)&BindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform, (unsigned int)&UnbindAnimation__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform, (unsigned int)&UnbindAllAnimation__Q34nw4r3lyt8MaterialFv, (unsigned int)&Animate__Q34nw4r3lyt8MaterialFv,
        (unsigned int)&FindAnimationLink__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransform, (unsigned int)&FindAnimationLink__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResource, (unsigned int)&SetAnimationEnable__Q34nw4r3lyt8MaterialFPQ34nw4r3lyt13AnimTransformb, (unsigned int)&SetAnimationEnable__Q34nw4r3lyt8MaterialFRCQ34nw4r3lyt12AnimResourceb,
    };

__declspec(align(8)) u32 lbl_eu_80569C68[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt8DrawInfoFv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569C78[10] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt18AnimTransformBasicFv, (unsigned int)&SetResource__Q34nw4r3lyt18AnimTransformBasicFPCQ44nw4r3lyt3res14AnimationBlockPQ34nw4r3lyt16ResourceAccessor,
        (unsigned int)&SetResource__Q34nw4r3lyt18AnimTransformBasicFPCQ44nw4r3lyt3res14AnimationBlockPQ34nw4r3lyt16ResourceAccessorUs, (unsigned int)&Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt4Panebb, (unsigned int)&Bind__Q34nw4r3lyt18AnimTransformBasicFPQ34nw4r3lyt8Materialb, (unsigned int)&Animate__Q34nw4r3lyt18AnimTransformBasicFUlPQ34nw4r3lyt4Pane,
        (unsigned int)&Animate__Q34nw4r3lyt18AnimTransformBasicFUlPQ34nw4r3lyt8Material, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569CA0[6] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt16ResourceAccessorFv, 0x00000000,
        (unsigned int)&GetFont__Q34nw4r3lyt16ResourceAccessorFPCc, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569CB8[6] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3lyt19ArcResourceAccessorFv, (unsigned int)&GetResource__Q34nw4r3lyt19ArcResourceAccessorFUlPCcPUl,
        (unsigned int)&GetFont__Q34nw4r3lyt19ArcResourceAccessorFPCc, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80569CD0[66] = {
        0x3F000000, 0x3CB5586D, 0x3F05AAC3, 0x3CBD5FCF,
        0x3F0B95C2, 0x3CC5C232, 0x3F11C3D3, 0x3CCE839C,
        0x3F1837F0, 0x3CD7A842, 0x3F1EF532, 0x3CE13489,
        0x3F25FED7, 0x3CEB2D08, 0x3F2D583F, 0x3CF59689,
        0x3F3504F3, 0x3D003B07, 0x3F3D08A4, 0x3D05E867,
        0x3F45672A, 0x3D0BD620, 0x3F4E248C, 0x3D12070B,
        0x3F5744FD, 0x3D187E22, 0x3F60CCDF, 0x3D1F3E80,
        0x3F6AC0C7, 0x3D264B63, 0x3F75257D, 0x3D2DA82F,
        0x3F800000, 0x3D35586D, 0x3F85AAC3, 0x3D3D5FCF,
        0x3F8B95C2, 0x3D45C232, 0x3F91C3D3, 0x3D4E839C,
        0x3F9837F0, 0x3D57A842, 0x3F9EF532, 0x3D613489,
        0x3FA5FED7, 0x3D6B2D08, 0x3FAD583F, 0x3D759689,
        0x3FB504F3, 0x3D803B07, 0x3FBD08A4, 0x3D85E867,
        0x3FC5672A, 0x3D8BD620, 0x3FCE248C, 0x3D92070B,
        0x3FD744FD, 0x3D987E22, 0x3FE0CCDF, 0x3D9F3E80,
        0x3FEAC0C7, 0x3DA64B63, 0x3FF5257D, 0x3DADA82F,
        0x40000000, 0x3DB5586D,
    };
    /* f32[] table (retail lbl_eu_80569CD0) */

__declspec(align(8)) u32 lbl_eu_80569DD8[514] = {
        0x00000000, 0x3B7F8053, 0x3B7F8053, 0x3B7E8251,
        0x3BFF0152, 0x3B7D8646, 0x3C3EE23B, 0x3B7C8C29,
        0x3C7E0546, 0x3B7B93F9, 0x3C9E7522, 0x3B7A9DAF,
        0x3CBDC8D8, 0x3B79A946, 0x3CDCFE01, 0x3B78B6BA,
        0x3CFC14D9, 0x3B77C602, 0x3D0D86CC, 0x3B76D719,
        0x3D1CF43E, 0x3B75E9FD, 0x3D2C52DE, 0x3B74FEA3,
        0x3D3BA2C8, 0x3B74150C, 0x3D4AE418, 0x3B732D2F,
        0x3D5A16EC, 0x3B72470D, 0x3D693B5C, 0x3B716299,
        0x3D785186, 0x3B707FD6, 0x3D83ACC2, 0x3B6F9EB8,
        0x3D8B29B7, 0x3B6EBF3F, 0x3D929FB1, 0x3B6DE167,
        0x3D9A0EBD, 0x3B6D0527, 0x3DA176E6, 0x3B6C2A83,
        0x3DA8D83A, 0x3B6B516A, 0x3DB032C5, 0x3B6A79E0,
        0x3DB78694, 0x3B69A3E2, 0x3DBED3B3, 0x3B68CF6A,
        0x3DC61A2F, 0x3B67FC71, 0x3DCD5A12, 0x3B672AF6,
        0x3DD4936A, 0x3B665AF1, 0x3DDBC641, 0x3B658C66,
        0x3DE2F2A4, 0x3B64BF47, 0x3DEA189F, 0x3B63F396,
        0x3DF1383B, 0x3B632952, 0x3DF85186, 0x3B626072,
        0x3DFF648A, 0x3B6198F3, 0x3E0338A9, 0x3B60D2D4,
        0x3E06BBF4, 0x3B600E10, 0x3E0A3C2C, 0x3B5F4AA0,
        0x3E0DB957, 0x3B5E8888, 0x3E113379, 0x3B5DC7BE,
        0x3E14AA98, 0x3B5D083F, 0x3E181EB9, 0x3B5C4A10,
        0x3E1B8FE1, 0x3B5B8D22, 0x3E1EFE16, 0x3B5AD177,
        0x3E22695B, 0x3B5A170D, 0x3E25D1B8, 0x3B595DE2,
        0x3E29372F, 0x3B58A5EB, 0x3E2C99C7, 0x3B57EF2F,
        0x3E2FF984, 0x3B5739A3, 0x3E33566A, 0x3B568548,
        0x3E36B07F, 0x3B55D21E, 0x3E3A07C8, 0x3B552018,
        0x3E3D5C48, 0x3B546F3F, 0x3E40AE05, 0x3B53BF8A,
        0x3E43FD03, 0x3B5310F4, 0x3E474947, 0x3B52637E,
        0x3E4A92D5, 0x3B51B724, 0x3E4DD9B1, 0x3B510BE1,
        0x3E511DE1, 0x3B5061BA, 0x3E545F68, 0x3B4FB8A5,
        0x3E579E4A, 0x3B4F109F, 0x3E5ADA8D, 0x3B4E69AC,
        0x3E5E1434, 0x3B4DC3C3, 0x3E614B43, 0x3B4D1EE4,
        0x3E647FBE, 0x3B4C7B0B, 0x3E67B1AA, 0x3B4BD839,
        0x3E6AE10B, 0x3B4B366C, 0x3E6E0DE5, 0x3B4A959D,
        0x3E71383B, 0x3B49F5CB, 0x3E746013, 0x3B4956F2,
        0x3E77856E, 0x3B48B917, 0x3E7AA853, 0x3B481C2C,
        0x3E7DC8C3, 0x3B47803A, 0x3E807362, 0x3B46E53D,
        0x3E82012D, 0x3B464B2D, 0x3E838DC3, 0x3B45B20D,
        0x3E851927, 0x3B4519D4, 0x3E86A35B, 0x3B448288,
        0x3E882C60, 0x3B43EC24, 0x3E89B438, 0x3B4356A4,
        0x3E8B3AE5, 0x3B42C207, 0x3E8CC069, 0x3B422E4E,
        0x3E8E44C6, 0x3B419B74, 0x3E8FC7FD, 0x3B410975,
        0x3E914A10, 0x3B407856, 0x3E92CB01, 0x3B3FE80D,
        0x3E944AD1, 0x3B3F589B, 0x3E95C982, 0x3B3ECA00,
        0x3E974716, 0x3B3E3C3B, 0x3E98C38E, 0x3B3DAF45,
        0x3E9A3EED, 0x3B3D2321, 0x3E9BB933, 0x3B3C97CB,
        0x3E9D3263, 0x3B3C0D40, 0x3E9EAA7D, 0x3B3B8382,
        0x3EA02184, 0x3B3AFA8E, 0x3EA19779, 0x3B3A7260,
        0x3EA30C5E, 0x3B39EAF7, 0x3EA48034, 0x3B396454,
        0x3EA5F2FD, 0x3B38DE72, 0x3EA764BA, 0x3B385952,
        0x3EA8D56C, 0x3B37D4F2, 0x3EAA4516, 0x3B375150,
        0x3EABB3B9, 0x3B36CE66, 0x3EAD2156, 0x3B364C3A,
        0x3EAE8DEE, 0x3B35CAC6, 0x3EAFF984, 0x3B354A06,
        0x3EB16418, 0x3B34C9FF, 0x3EB2CDAC, 0x3B344AAD,
        0x3EB43641, 0x3B33CC0F, 0x3EB59DD9, 0x3B334E21,
        0x3EB70475, 0x3B32D0E3, 0x3EB86A17, 0x3B325451,
        0x3EB9CEC0, 0x3B31D86F, 0x3EBB3271, 0x3B315D38,
        0x3EBC952B, 0x3B30E2AA, 0x3EBDF6F0, 0x3B3068C7,
        0x3EBF57C2, 0x3B2FEF87, 0x3EC0B7A1, 0x3B2F76F3,
        0x3EC2168F, 0x3B2EFF02, 0x3EC3748D, 0x3B2E87B0,
        0x3EC4D19C, 0x3B2E1106, 0x3EC62DBE, 0x3B2D9AFB,
        0x3EC788F4, 0x3B2D258E, 0x3EC8E33F, 0x3B2CB0C5,
        0x3ECA3CA1, 0x3B2C3C92, 0x3ECB951A, 0x3B2BC8FE,
        0x3ECCECAC, 0x3B2B5609, 0x3ECE4358, 0x3B2AE3A6,
        0x3ECF991F, 0x3B2A71E2, 0x3ED0EE03, 0x3B2A00AF,
        0x3ED24205, 0x3B299018, 0x3ED39525, 0x3B292012,
        0x3ED4E765, 0x3B28B09F, 0x3ED638C6, 0x3B2841C2,
        0x3ED7894A, 0x3B27D372, 0x3ED8D8F0, 0x3B2765B5,
        0x3EDA27BC, 0x3B26F885, 0x3EDB75AD, 0x3B268BE4,
        0x3EDCC2C5, 0x3B261FD4, 0x3EDE0F04, 0x3B25B449,
        0x3EDF5A6D, 0x3B25494C, 0x3EE0A500, 0x3B24DEDD,
        0x3EE1EEBD, 0x3B2474F2, 0x3EE337A7, 0x3B240B8D,
        0x3EE47FBE, 0x3B23A2B2, 0x3EE5C704, 0x3B233A5C,
        0x3EE70D78, 0x3B22D28A, 0x3EE8531D, 0x3B226B3E,
        0x3EE997F4, 0x3B220473, 0x3EEADBFD, 0x3B219E28,
        0x3EEC1F39, 0x3B213863, 0x3EED61AA, 0x3B20D31A,
        0x3EEEA350, 0x3B206E53, 0x3EEFE42D, 0x3B200A07,
        0x3EF12441, 0x3B1FA639, 0x3EF2638D, 0x3B1F42E6,
        0x3EF3A213, 0x3B1EE011, 0x3EF4DFD3, 0x3B1E7DB3,
        0x3EF61CCF, 0x3B1E1BCE, 0x3EF75906, 0x3B1DBA66,
        0x3EF8947B, 0x3B1D5975, 0x3EF9CF2E, 0x3B1CF8F9,
        0x3EFB0920, 0x3B1C98F1, 0x3EFC4252, 0x3B1C3960,
        0x3EFD7AC4, 0x3B1BDA44, 0x3EFEB279, 0x3B1B7B9C,
        0x3EFFE970, 0x3B1B1D68, 0x3F008FD6, 0x3B1ABFA3,
        0x3F012A95, 0x3B1A6253, 0x3F01C4F7, 0x3B1A056D,
        0x3F025EFD, 0x3B19A8FC, 0x3F02F8A6, 0x3B194CF6,
        0x3F0391F3, 0x3B18F160, 0x3F042AE4, 0x3B189639,
        0x3F04C37A, 0x3B183B7A, 0x3F055BB6, 0x3B17E12A,
        0x3F05F397, 0x3B178746, 0x3F068B1E, 0x3B172DC8,
        0x3F07224C, 0x3B16D4B6, 0x3F07B921, 0x3B167C0B,
        0x3F084F9D, 0x3B1623CC, 0x3F08E5C1, 0x3B15CBEF,
        0x3F097B8D, 0x3B15747E, 0x3F0A1101, 0x3B151D6F,
        0x3F0AA61F, 0x3B14C6C8, 0x3F0B3AE5, 0x3B147083,
        0x3F0BCF56, 0x3B141AA1, 0x3F0C6371, 0x3B13C522,
        0x3F0CF736, 0x3B13700A, 0x3F0D8AA6, 0x3B131B50,
        0x3F0E1DC1, 0x3B12C6F9, 0x3F0EB088, 0x3B127301,
        0x3F0F42FB, 0x3B121F67, 0x3F0FD51A, 0x3B11CC30,
        0x3F1066E7, 0x3B117958, 0x3F10F860, 0x3B1126D9,
        0x3F118987, 0x3B10D4BE, 0x3F121A5B, 0x3B1082FC,
        0x3F12AADE, 0x3B103195, 0x3F133B10, 0x3B0FE08D,
        0x3F13CAF1, 0x3B0F8FDA, 0x3F145A80, 0x3B0F3F85,
        0x3F14E9C0, 0x3B0EEF8B, 0x3F1578B0, 0x3B0E9FE7,
        0x3F16074F, 0x3B0E50A1, 0x3F1695A0, 0x3B0E01AD,
        0x3F1723A2, 0x3B0DB313, 0x3F17B155, 0x3B0D64CF,
        0x3F183EBA, 0x3B0D16E1, 0x3F18CBD0, 0x3B0CC94D,
        0x3F19589A, 0x3B0C7C0B, 0x3F19E516, 0x3B0C2F1A,
        0x3F1A7145, 0x3B0BE284, 0x3F1AFD27, 0x3B0B963B,
        0x3F1B88BE, 0x3B0B4A47, 0x3F1C1408, 0x3B0AFEAA,
        0x3F1C9F07, 0x3B0AB35A, 0x3F1D29BA, 0x3B0A685B,
        0x3F1DB422, 0x3B0A1DAE, 0x3F1E3E40, 0x3B09D353,
        0x3F1EC813, 0x3B098949, 0x3F1F519D, 0x3B093F8D,
        0x3F1FDADC, 0x3B08F61E, 0x3F2063D2, 0x3B08ACFC,
        0x3F20EC7F, 0x3B08642C, 0x3F2174E3, 0x3B081BA9,
        0x3F21FCFF, 0x3B07D374, 0x3F2284D3, 0x3B078B87,
        0x3F230C5E, 0x3B0743E8, 0x3F2393A2, 0x3B06FC96,
        0x3F241A9F, 0x3B06B58D, 0x3F24A154, 0x3B066ED1,
        0x3F2527C3, 0x3B06285F, 0x3F25ADEB, 0x3B05E235,
        0x3F2633CE, 0x3B059C55, 0x3F26B96A, 0x3B0556BD,
        0x3F273EC1, 0x3B05116F, 0x3F27C3D2, 0x3B04CC65,
        0x3F28489E, 0x3B0487A4, 0x3F28CD26, 0x3B04432C,
        0x3F295169, 0x3B03FEF9, 0x3F29D568, 0x3B03BB0F,
        0x3F2A5923, 0x3B03776A, 0x3F2ADC9B, 0x3B033409,
        0x3F2B5FCF, 0x3B02F0EE, 0x3F2BE2C0, 0x3B02AE16,
        0x3F2C656E, 0x3B026B80, 0x3F2CE7D9, 0x3B022932,
        0x3F2D6A02, 0x3B01E725, 0x3F2DEBEA, 0x3B01A55C,
        0x3F2E6D8F, 0x3B0163D4, 0x3F2EEEF3, 0x3B012291,
        0x3F2F7015, 0x3B00E18E, 0x3F2FF0F7, 0x3B00A0CB,
        0x3F307198, 0x3B006049, 0x3F30F1F8, 0x3B00200B,
        0x3F317218, 0x3AFFC014,
    };

__declspec(align(8)) u32 lbl_eu_8056A5E0[66] = {
        0x00000000, 0x3FA2EBF1, 0x3FA2EBF1, 0x3FA29AB1,
        0x4022C351, 0x3FA1F923, 0x4073BFE2, 0x3FA10925,
        0x40A2223B, 0x3F9FCD71, 0x40CA1597, 0x3F9E498C,
        0x40F1A7FA, 0x3F9C81AB, 0x410C6432, 0x3F9A7A9B,
        0x411FB386, 0x3F98399D, 0x4132BAB9, 0x3F95C44B,
        0x41457343, 0x3F932075, 0x4157D751, 0x3F905408,
        0x4169E1D2, 0x3F8D64EB, 0x417B8E70, 0x3F8A58EC,
        0x41866CC7, 0x3F8735A9, 0x418EE021, 0x3F84007D,
        0x41972029, 0x3F80BE73, 0x419F2C10, 0x3F7AE877,
        0x41A70354, 0x3F744C4A, 0x41AEA5B6, 0x3F6DB032,
        0x41B61338, 0x3F671B36, 0x41BD4C11, 0x3F60938F,
        0x41C450AE, 0x3F5A1EB2, 0x41CB21A3, 0x3F53C157,
        0x41D1BFAE, 0x3F4D7F84, 0x41D82BAA, 0x3F475C97,
        0x41DE668F, 0x3F415B59, 0x41E4716A, 0x3F3B7E02,
        0x41EA4D5A, 0x3F35C652, 0x41EFFB8C, 0x3F303593,
        0x41F57D39, 0x3F2ACCAE, 0x41FAD39E, 0x3F258C33,
        0x42000000, 0x3F207467,
    };

__declspec(align(8)) u32 lbl_eu_8056A6E8[10] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail11BasicPlayerFv, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056A710[14] = {
        0x00000000, 0x00000000, (unsigned int)&GetRuntimeTypeInfo__Q44nw4r3snd6detail10BasicSoundCFv, (unsigned int)&__dt__Q44nw4r3snd6detail10BasicSoundFv,
        (unsigned int)&Shutdown__Q44nw4r3snd6detail10BasicSoundFv, 0x00000000, 0x00000000, 0x00000000,
        (unsigned int)&InitParam__Q44nw4r3snd6detail10BasicSoundFv, 0x00000000, 0x00000000, (unsigned int)&OnUpdatePlayerPriority__Q44nw4r3snd6detail10BasicSoundFv,
        (unsigned int)&UpdateMoveValue__Q44nw4r3snd6detail10BasicSoundFv, (unsigned int)&UpdateParam__Q44nw4r3snd6detail10BasicSoundFv,
    };

__declspec(align(8)) u32 lbl_eu_8056A748[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail19BiquadFilterBpf2048Fv, (unsigned int)&GetCoef__Q44nw4r3snd6detail19BiquadFilterBpf2048CFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef,
    };

__declspec(align(8)) u32 lbl_eu_8056A758[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail19BiquadFilterBpf1024Fv, (unsigned int)&GetCoef__Q44nw4r3snd6detail19BiquadFilterBpf1024CFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef,
    };

__declspec(align(8)) u32 lbl_eu_8056A768[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail18BiquadFilterBpf512Fv, (unsigned int)&GetCoef__Q44nw4r3snd6detail18BiquadFilterBpf512CFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef,
    };

__declspec(align(8)) u32 lbl_eu_8056A778[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail15BiquadFilterHpfFv, (unsigned int)&GetCoef__Q44nw4r3snd6detail15BiquadFilterHpfCFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef,
    };

__declspec(align(8)) u32 lbl_eu_8056A788[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail15BiquadFilterLpfFv, (unsigned int)&GetCoef__Q44nw4r3snd6detail15BiquadFilterLpfCFifPQ44nw4r3snd20BiquadFilterCallback10BiquadCoef,
    };

__declspec(align(8)) u32 lbl_eu_8056A798[10] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3snd12FxChorusDpl2Fv, (unsigned int)&StartUp__Q34nw4r3snd12FxChorusDpl2Fv,
        (unsigned int)&Shutdown__Q34nw4r3snd12FxChorusDpl2Fv, (unsigned int)&UpdateBuffer__Q34nw4r3snd12FxChorusDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode, (unsigned int)&OnChangeOutputMode__Q34nw4r3snd12FxChorusDpl2Fv, (unsigned int)&AssignWorkBuffer__Q34nw4r3snd12FxChorusDpl2FPvUl,
        (unsigned int)&ReleaseWorkBuffer__Q34nw4r3snd12FxChorusDpl2Fv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056A7C0[10] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3snd11FxDelayDpl2Fv, (unsigned int)&StartUp__Q34nw4r3snd11FxDelayDpl2Fv,
        (unsigned int)&Shutdown__Q34nw4r3snd11FxDelayDpl2Fv, (unsigned int)&UpdateBuffer__Q34nw4r3snd11FxDelayDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode, (unsigned int)&OnChangeOutputMode__Q34nw4r3snd11FxDelayDpl2Fv, (unsigned int)&AssignWorkBuffer__Q34nw4r3snd11FxDelayDpl2FPvUl,
        (unsigned int)&ReleaseWorkBuffer__Q34nw4r3snd11FxDelayDpl2Fv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056A7E8[10] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3snd15FxReverbStdDpl2Fv, (unsigned int)&StartUp__Q34nw4r3snd15FxReverbStdDpl2Fv,
        (unsigned int)&Shutdown__Q34nw4r3snd15FxReverbStdDpl2Fv, (unsigned int)&UpdateBuffer__Q34nw4r3snd15FxReverbStdDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode, (unsigned int)&OnChangeOutputMode__Q34nw4r3snd15FxReverbStdDpl2Fv, (unsigned int)&AssignWorkBuffer__Q34nw4r3snd15FxReverbStdDpl2FPvUl,
        (unsigned int)&ReleaseWorkBuffer__Q34nw4r3snd15FxReverbStdDpl2Fv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056A810[24] = {
        0x00000000, 0x00000000, (unsigned int)&GetRuntimeTypeInfo__Q34nw4r2ut10FileStreamCFv, (unsigned int)&__dt__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamFv,
        (unsigned int)&Close__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamFv, (unsigned int)&Read__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamFPvUl, (unsigned int)&ReadAsync__Q34nw4r2ut8IOStreamFPvUlPFlPQ34nw4r2ut8IOStreamPv_vPv, (unsigned int)&Write__Q34nw4r2ut8IOStreamFPCvUl,
        (unsigned int)&WriteAsync__Q34nw4r2ut8IOStreamFPCvUlPFlPQ34nw4r2ut8IOStreamPv_vPv, (unsigned int)&IsBusy__Q34nw4r2ut8IOStreamCFv, (unsigned int)&CanAsync__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv, (unsigned int)&CanRead__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv,
        (unsigned int)&CanWrite__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv, (unsigned int)&GetOffsetAlign__Q34nw4r2ut8IOStreamCFv, (unsigned int)&GetSizeAlign__Q34nw4r2ut8IOStreamCFv, (unsigned int)&GetBufferAlign__Q34nw4r2ut8IOStreamCFv,
        (unsigned int)&GetSize__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv, (unsigned int)&Seek__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamFlUl, (unsigned int)&Cancel__Q34nw4r2ut10FileStreamFv, (unsigned int)&CancelAsync__Q34nw4r2ut10FileStreamFPFlPQ34nw4r2ut8IOStreamPv_vPv,
        (unsigned int)&CanSeek__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv, (unsigned int)&CanCancel__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv, (unsigned int)&Tell__Q44nw4r3snd18MemorySoundArchive16MemoryFileStreamCFv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056A870[8] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3snd18MemorySoundArchiveFv, (unsigned int)&detail_GetFileAddress__Q34nw4r3snd18MemorySoundArchiveCFUl,
        (unsigned int)&detail_GetWaveDataFileAddress__Q34nw4r3snd18MemorySoundArchiveCFUl, (unsigned int)&detail_GetRequiredStreamBufferSize__Q34nw4r3snd18MemorySoundArchiveCFv, (unsigned int)&OpenStream__Q34nw4r3snd18MemorySoundArchiveCFPviUlUl, (unsigned int)&OpenExtStream__Q34nw4r3snd18MemorySoundArchiveCFPviPCcUlUl,
    };

__declspec(align(8)) u8 lbl_eu_8056A890[39] = {
        0x23, 0x25, 0x30, 0x38, 0x78, 0x5B, 0x25, 0x64, 0x5D, 0x3A, 0x20, 0x70, 0x72, 0x69, 0x6E, 0x74, 0x76, 0x61, 0x72, 0x20, 0x25, 0x73, 0x56, 0x41, 0x52, 0x5F, 0x25, 0x64, 0x28, 0x25, 0x64, 0x29, 0x20, 0x3D, 0x20, 0x25, 0x64, 0x0A, 0x00,
    };

__declspec(align(1)) u8 gap_07_8056A8B7_data[1] = {
        0x00,
    };

__declspec(align(8)) u32 jumptable_eu_8056A8B8[125] = {
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 136), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1192),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1256), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1272), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 128),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 700), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 816), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 260), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 944),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 952), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 176), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 268), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 276), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 284),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 292), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 492), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 500), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 776),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 860), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 528), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 564), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 600),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 608), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 884), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 520), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 668),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 676), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 684), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 692), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1416),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 252), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1020), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 404), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 904),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 988), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 996), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1012), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 392),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 160), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1004), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 740), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 616),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 96), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 632), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 2300), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 712), (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1472),
        (unsigned int)((char *)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll + 1336),
    };

__declspec(align(4)) u32 gap_07_8056AAAC_data[1] = {
        0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056AAB0[4] = {
        0x00000000, 0x00000000, (unsigned int)&CommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackUlll, (unsigned int)&NoteOnCommandProc__Q44nw4r3snd6detail9MmlParserCFPQ44nw4r3snd6detail11MmlSeqTrackiilb,
    };

__declspec(align(8)) u32 lbl_eu_8056AAC0[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail11MmlSeqTrackFv, (unsigned int)&Parse__Q44nw4r3snd6detail11MmlSeqTrackFb,
    };

__declspec(align(8)) u32 lbl_eu_8056AAD0[6] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail20MmlSeqTrackAllocatorFv, (unsigned int)&AllocTrack__Q44nw4r3snd6detail20MmlSeqTrackAllocatorFPQ44nw4r3snd6detail9SeqPlayer,
        (unsigned int)&FreeTrack__Q44nw4r3snd6detail20MmlSeqTrackAllocatorFPQ44nw4r3snd6detail8SeqTrack, (unsigned int)&GetAllocatableTrackCount__Q44nw4r3snd6detail20MmlSeqTrackAllocatorCFv,
    };

__declspec(align(8)) u32 lbl_eu_8056AAE8[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail10PlayerHeapFv, (unsigned int)&Alloc__Q44nw4r3snd6detail10PlayerHeapFUl,
    };

__declspec(align(8)) u32 lbl_eu_8056AAF8[26] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail9SeqPlayerFv, (unsigned int)&Start__Q44nw4r3snd6detail9SeqPlayerFv,
        (unsigned int)&Stop__Q44nw4r3snd6detail9SeqPlayerFv, (unsigned int)&Pause__Q44nw4r3snd6detail9SeqPlayerFb, (unsigned int)&IsActive__Q44nw4r3snd6detail9SeqPlayerCFv, (unsigned int)&IsStarted__Q44nw4r3snd6detail9SeqPlayerCFv,
        (unsigned int)&IsPause__Q44nw4r3snd6detail9SeqPlayerCFv, 0x00000000, 0x00000000, 0,
        0, 0, 0x00000000, 0x00000000,
        0, 0, (unsigned int)&OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv, 0,
        (unsigned int)&InvalidateData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv, (unsigned int)&InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv, (unsigned int)&ChannelCallback__Q44nw4r3snd6detail9SeqPlayerFPQ44nw4r3snd6detail7Channel, (unsigned int)&OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv,
        (unsigned int)&OnShutdownSoundThread__Q44nw4r3snd6detail9SeqPlayerFv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056AB60[14] = {
        0x00000000, 0x00000000, (unsigned int)&GetRuntimeTypeInfo__Q44nw4r3snd6detail8SeqSoundCFv, (unsigned int)&__dt__Q44nw4r3snd6detail8SeqSoundFv,
        (unsigned int)&Shutdown__Q44nw4r3snd6detail8SeqSoundFv, (unsigned int)&IsPrepared__Q44nw4r3snd6detail8SeqSoundCFv, (unsigned int)&IsAttachedTempSpecialHandle__Q44nw4r3snd6detail8SeqSoundFv, (unsigned int)&DetachTempSpecialHandle__Q44nw4r3snd6detail8SeqSoundFv,
        (unsigned int)&InitParam__Q44nw4r3snd6detail8SeqSoundFv, (unsigned int)&GetBasicPlayer__Q44nw4r3snd6detail8SeqSoundFv, (unsigned int)&GetBasicPlayer__Q44nw4r3snd6detail8SeqSoundCFv, (unsigned int)&OnUpdatePlayerPriority__Q44nw4r3snd6detail8SeqSoundFv,
        (unsigned int)&UpdateMoveValue__Q44nw4r3snd6detail10BasicSoundFv, (unsigned int)&UpdateParam__Q44nw4r3snd6detail10BasicSoundFv,
    };

__declspec(align(8)) u32 lbl_eu_8056AB98[6] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q54nw4r3snd6detail8SeqSound11SeqLoadTaskFv, (unsigned int)&Execute__Q54nw4r3snd6detail8SeqSound11SeqLoadTaskFv,
        (unsigned int)&Cancel__Q54nw4r3snd6detail8SeqSound11SeqLoadTaskFv, (unsigned int)&OnCancel__Q54nw4r3snd6detail8SeqSound11SeqLoadTaskFv,
    };

__declspec(align(8)) u32 lbl_eu_8056ABB0[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail8SeqTrackFv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056ABC0[8] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3snd12SoundArchiveFv, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056ABE0[12] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r3snd18SoundArchivePlayerFv, (unsigned int)&InvalidateData__Q34nw4r3snd18SoundArchivePlayerFPCvPCv,
        (unsigned int)&InvalidateWaveData__Q34nw4r3snd18SoundArchivePlayerFPCvPCv, 0x00000000, 0x00000000, 0,
        0, 0, (unsigned int)&detail_ConvertLabelStringToSoundId__Q34nw4r3snd18SoundArchivePlayerFPCc, (unsigned int)&detail_SetupSound__Q34nw4r3snd18SoundArchivePlayerFPQ34nw4r3snd11SoundHandleUlbPCQ44nw4r3snd14SoundStartable9StartInfo,
    };

__declspec(align(8)) u32 lbl_eu_8056AC10[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd18SoundArchivePlayer11WsdCallbackFv, (unsigned int)&GetWaveSoundData__Q44nw4r3snd18SoundArchivePlayer11WsdCallbackCFPQ44nw4r3snd6detail13WaveSoundInfoPQ44nw4r3snd6detail17WaveSoundNoteInfoPQ44nw4r3snd6detail8WaveInfoPCviiUl,
    };

__declspec(align(8)) u32 lbl_eu_8056AC20[4] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd18SoundArchivePlayer17SeqNoteOnCallbackFv, (unsigned int)&NoteOn__Q44nw4r3snd18SoundArchivePlayer17SeqNoteOnCallbackFPQ44nw4r3snd6detail9SeqPlayeriRCQ44nw4r3snd6detail10NoteOnInfo,
    };

__declspec(align(8)) u32 lbl_eu_8056AC30[18] = {
        0x3C3C204E, 0x57345220, 0x2020202D, 0x20534E44,
        0x20096669, 0x6E616C20, 0x20206275, 0x696C643A,
        0x204E6F76, 0x20323020, 0x32303038, 0x2030333A,
        0x34303A30, 0x30202830, 0x78343139, 0x395F3630,
        0x38333129, 0x203E3E00,
    };
    /* f32[] table (retail lbl_eu_8056AC30) */

__declspec(align(8)) u32 lbl_eu_8056AC78[18] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail10StrmPlayerFv, (unsigned int)&Start__Q44nw4r3snd6detail10StrmPlayerFv,
        (unsigned int)&Stop__Q44nw4r3snd6detail10StrmPlayerFv, (unsigned int)&Pause__Q44nw4r3snd6detail10StrmPlayerFb, (unsigned int)&IsActive__Q44nw4r3snd6detail10StrmPlayerCFv, (unsigned int)&IsStarted__Q44nw4r3snd6detail10StrmPlayerCFv,
        (unsigned int)&IsPause__Q44nw4r3snd6detail10StrmPlayerCFv, 0x00000000, 0x00000000, 0,
        0, 0, 0, (unsigned int)&OnUpdateFrameSoundThread__Q44nw4r3snd6detail10StrmPlayerFv,
        (unsigned int)&OnUpdateVoiceSoundThread__Q44nw4r3snd6detail10StrmPlayerFv, (unsigned int)&OnShutdownSoundThread__Q44nw4r3snd6detail10StrmPlayerFv,
    };

__declspec(align(8)) u32 lbl_eu_8056ACC0[6] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv, (unsigned int)&Execute__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv,
        (unsigned int)&Cancel__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv, (unsigned int)&OnCancel__Q54nw4r3snd6detail10StrmPlayer16StrmDataLoadTaskFv,
    };

__declspec(align(8)) u32 lbl_eu_8056ACD8[6] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTaskFv, (unsigned int)&Execute__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTaskFv,
        (unsigned int)&Cancel__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTaskFv, (unsigned int)&OnCancel__Q54nw4r3snd6detail10StrmPlayer18StrmHeaderLoadTaskFv,
    };

__declspec(align(8)) u32 lbl_eu_8056ACF0[14] = {
        0x00000000, 0x00000000, (unsigned int)&GetRuntimeTypeInfo__Q44nw4r3snd6detail9StrmSoundCFv, (unsigned int)&__dt__Q44nw4r3snd6detail9StrmSoundFv,
        (unsigned int)&Shutdown__Q44nw4r3snd6detail9StrmSoundFv, (unsigned int)&IsPrepared__Q44nw4r3snd6detail9StrmSoundCFv, (unsigned int)&IsAttachedTempSpecialHandle__Q44nw4r3snd6detail9StrmSoundFv, (unsigned int)&DetachTempSpecialHandle__Q44nw4r3snd6detail9StrmSoundFv,
        (unsigned int)&InitParam__Q44nw4r3snd6detail9StrmSoundFv, (unsigned int)&GetBasicPlayer__Q44nw4r3snd6detail9StrmSoundFv, (unsigned int)&GetBasicPlayer__Q44nw4r3snd6detail9StrmSoundCFv, (unsigned int)&OnUpdatePlayerPriority__Q44nw4r3snd6detail9StrmSoundFv,
        (unsigned int)&UpdateMoveValue__Q44nw4r3snd6detail9StrmSoundFv, (unsigned int)&UpdateParam__Q44nw4r3snd6detail9StrmSoundFv,
    };

__declspec(align(8)) u32 jumptable_eu_8056AD28[9] = {
        (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 496), (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 504), (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 520), (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 540),
        (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 552), (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 568), (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 588), (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 600),
        (unsigned int)((char *)&CalcMixParam__Q44nw4r3snd6detail5VoiceFiiPQ54nw4r3snd6detail7AxVoice8MixParamPQ54nw4r3snd6detail7AxVoice14RemoteMixParam + 620),
    };

__declspec(align(4)) u32 lbl_eu_8056AD4C[5] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail5VoiceFv, (unsigned int)&InvalidateData__Q44nw4r3snd6detail5VoiceFPCvPCv,
        (unsigned int)&InvalidateWaveData__Q44nw4r3snd6detail5VoiceFPCvPCv,
    };

__declspec(align(8)) u32 lbl_eu_8056AD60[4] = {
        (unsigned int)&lbl_eu_805212D4, (unsigned int)&lbl_eu_805216D8, (unsigned int)&lbl_eu_80521ADC, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056AD70[14] = {
        0x00000000, 0x00000000, (unsigned int)&GetRuntimeTypeInfo__Q44nw4r3snd6detail9WaveSoundCFv, (unsigned int)&__dt__Q44nw4r3snd6detail9WaveSoundFv,
        (unsigned int)&Shutdown__Q44nw4r3snd6detail9WaveSoundFv, (unsigned int)&IsPrepared__Q44nw4r3snd6detail9WaveSoundCFv, (unsigned int)&IsAttachedTempSpecialHandle__Q44nw4r3snd6detail9WaveSoundFv, (unsigned int)&DetachTempSpecialHandle__Q44nw4r3snd6detail9WaveSoundFv,
        (unsigned int)&InitParam__Q44nw4r3snd6detail10BasicSoundFv, (unsigned int)&GetBasicPlayer__Q44nw4r3snd6detail9WaveSoundFv, (unsigned int)&GetBasicPlayer__Q44nw4r3snd6detail9WaveSoundCFv, (unsigned int)&OnUpdatePlayerPriority__Q44nw4r3snd6detail9WaveSoundFv,
        (unsigned int)&UpdateMoveValue__Q44nw4r3snd6detail10BasicSoundFv, (unsigned int)&UpdateParam__Q44nw4r3snd6detail10BasicSoundFv,
    };

__declspec(align(8)) u32 lbl_eu_8056ADA8[24] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r3snd6detail9WsdPlayerFv, (unsigned int)&Start__Q44nw4r3snd6detail9WsdPlayerFv,
        (unsigned int)&Stop__Q44nw4r3snd6detail9WsdPlayerFv, (unsigned int)&Pause__Q44nw4r3snd6detail9WsdPlayerFb, (unsigned int)&IsActive__Q44nw4r3snd6detail9WsdPlayerCFv, (unsigned int)&IsStarted__Q44nw4r3snd6detail9WsdPlayerCFv,
        (unsigned int)&IsPause__Q44nw4r3snd6detail9WsdPlayerCFv, 0x00000000, 0x00000000, 0,
        0, 0, 0x00000000, 0x00000000,
        0, 0, (unsigned int)&OnUpdateVoiceSoundThread__Q54nw4r3snd6detail11SoundThread14PlayerCallbackFv, 0,
        (unsigned int)&InvalidateData__Q44nw4r3snd6detail9WsdPlayerFPCvPCv, (unsigned int)&InvalidateWaveData__Q44nw4r3snd6detail9WsdPlayerFPCvPCv, (unsigned int)&OnUpdateFrameSoundThread__Q44nw4r3snd6detail9WsdPlayerFv, (unsigned int)&OnShutdownSoundThread__Q44nw4r3snd6detail9WsdPlayerFv,
    };

__declspec(align(8)) u32 lbl_eu_8056AE08[5] = {
        0x00000000, 0x00000000, 0, 0,
        0,
    };

__declspec(align(4)) u32 lbl_eu_8056AE1C[5] = {
        0x00000000, 0x00000000, 0, 0,
        0,
    };

__declspec(align(8)) u32 lbl_eu_8056AE30[12] = {
        0x00000000, 0xFFFFFFFF, (unsigned int)&ReadNextCharUTF8__Q34nw4r2ut14CharStrmReaderFv, 0x00000000,
        0xFFFFFFFF, (unsigned int)&ReadNextCharUTF16__Q34nw4r2ut14CharStrmReaderFv, 0x00000000, 0xFFFFFFFF,
        (unsigned int)&ReadNextCharSJIS__Q34nw4r2ut14CharStrmReaderFv, 0x00000000, 0xFFFFFFFF, (unsigned int)&ReadNextCharCP1252__Q34nw4r2ut14CharStrmReaderFv,
    };

__declspec(align(8)) u32 lbl_eu_8056AE60[3] = {
        0x00000000, 0xFFFFFFFF, (unsigned int)&ReadNextCharCP1252__Q34nw4r2ut14CharStrmReaderFv,
    };

__declspec(align(4)) u32 __vt__Q34nw4r2ut7RomFont[23] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r2ut7RomFontFv, (unsigned int)&GetWidth__Q34nw4r2ut7RomFontCFv,
        (unsigned int)&GetHeight__Q34nw4r2ut7RomFontCFv, (unsigned int)&GetAscent__Q34nw4r2ut7RomFontCFv, (unsigned int)&GetDescent__Q34nw4r2ut7RomFontCFv, (unsigned int)&GetBaselinePos__Q34nw4r2ut7RomFontCFv,
        (unsigned int)&GetCellHeight__Q34nw4r2ut7RomFontCFv, (unsigned int)&GetCellWidth__Q34nw4r2ut7RomFontCFv, (unsigned int)&GetMaxCharWidth__Q34nw4r2ut7RomFontCFv, (unsigned int)&GetType__Q34nw4r2ut7RomFontCFv,
        (unsigned int)&GetTextureFormat__Q34nw4r2ut7RomFontCFv, (unsigned int)&GetLineFeed__Q34nw4r2ut7RomFontCFv, (unsigned int)&GetDefaultCharWidths__Q34nw4r2ut7RomFontCFv, (unsigned int)&SetDefaultCharWidths__Q34nw4r2ut7RomFontFRCQ34nw4r2ut10CharWidths,
        (unsigned int)&SetAlternateChar__Q34nw4r2ut7RomFontFUs, (unsigned int)&SetLineFeed__Q34nw4r2ut7RomFontFi, (unsigned int)&GetCharWidth__Q34nw4r2ut7RomFontCFUs, (unsigned int)&GetCharWidths__Q34nw4r2ut7RomFontCFUs,
        (unsigned int)&GetGlyph__Q34nw4r2ut7RomFontCFPQ34nw4r2ut5GlyphUs, (unsigned int)&HasGlyph__Q34nw4r2ut7RomFontCFUs, (unsigned int)&GetEncoding__Q34nw4r2ut7RomFontCFv,
    };

__declspec(align(8)) u32 __vt__Q34nw4r2ut4Font[24] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r2ut4FontFv, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056AF28[3] = {
        0x00000000, 0xFFFFFFFF, (unsigned int)&ReadNextCharCP1252__Q34nw4r2ut14CharStrmReaderFv,
    };

__declspec(align(4)) u32 lbl_eu_8056AF34[23] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r2ut6detail11ResFontBaseFv, (unsigned int)&GetWidth__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetHeight__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetAscent__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetDescent__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetBaselinePos__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetCellHeight__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetCellWidth__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetMaxCharWidth__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetType__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetTextureFormat__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetLineFeed__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&SetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseFRCQ34nw4r2ut10CharWidths,
        (unsigned int)&SetAlternateChar__Q44nw4r2ut6detail11ResFontBaseFUs, (unsigned int)&SetLineFeed__Q44nw4r2ut6detail11ResFontBaseFi, (unsigned int)&GetCharWidth__Q44nw4r2ut6detail11ResFontBaseCFUs, (unsigned int)&GetCharWidths__Q44nw4r2ut6detail11ResFontBaseCFUs,
        (unsigned int)&GetGlyph__Q44nw4r2ut6detail11ResFontBaseCFPQ34nw4r2ut5GlyphUs, (unsigned int)&HasGlyph__Q44nw4r2ut6detail11ResFontBaseCFUs, (unsigned int)&GetEncoding__Q44nw4r2ut6detail11ResFontBaseCFv,
    };

__declspec(align(8)) u32 lbl_eu_8056AF90[24] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r2ut7ResFontFv, (unsigned int)&GetWidth__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetHeight__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetAscent__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetDescent__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetBaselinePos__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetCellHeight__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetCellWidth__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetMaxCharWidth__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetType__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetTextureFormat__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetLineFeed__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&SetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseFRCQ34nw4r2ut10CharWidths,
        (unsigned int)&SetAlternateChar__Q44nw4r2ut6detail11ResFontBaseFUs, (unsigned int)&SetLineFeed__Q44nw4r2ut6detail11ResFontBaseFi, (unsigned int)&GetCharWidth__Q44nw4r2ut6detail11ResFontBaseCFUs, (unsigned int)&GetCharWidths__Q44nw4r2ut6detail11ResFontBaseCFUs,
        (unsigned int)&GetGlyph__Q44nw4r2ut6detail11ResFontBaseCFPQ34nw4r2ut5GlyphUs, (unsigned int)&HasGlyph__Q44nw4r2ut6detail11ResFontBaseCFUs, (unsigned int)&GetEncoding__Q44nw4r2ut6detail11ResFontBaseCFv, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_8056AFF0[24] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q44nw4r2ut6detail15ArchiveFontBaseFv, (unsigned int)&GetWidth__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetHeight__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetAscent__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetDescent__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetBaselinePos__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetCellHeight__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetCellWidth__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetMaxCharWidth__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetType__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetTextureFormat__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetLineFeed__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&SetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseFRCQ34nw4r2ut10CharWidths,
        (unsigned int)&SetAlternateChar__Q44nw4r2ut6detail11ResFontBaseFUs, (unsigned int)&SetLineFeed__Q44nw4r2ut6detail11ResFontBaseFi, (unsigned int)&GetCharWidth__Q44nw4r2ut6detail11ResFontBaseCFUs, (unsigned int)&GetCharWidths__Q44nw4r2ut6detail15ArchiveFontBaseCFUs,
        (unsigned int)&GetGlyph__Q44nw4r2ut6detail11ResFontBaseCFPQ34nw4r2ut5GlyphUs, (unsigned int)&HasGlyph__Q44nw4r2ut6detail15ArchiveFontBaseCFUs, (unsigned int)&GetEncoding__Q44nw4r2ut6detail11ResFontBaseCFv, 0x00000000,
    };

__declspec(align(8)) u32 jumptable_eu_8056B050[13] = {
        (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 132), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 152), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 172), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 196),
        (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 216), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 236), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 316), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 400),
        (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 336), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 256), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 276), (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 416),
        (unsigned int)((char *)&StreamingConstruct__Q34nw4r2ut10PackedFontFPQ54nw4r2ut6detail15ArchiveFontBase16ConstructContextPCvUl + 296),
    };

__declspec(align(4)) u32 lbl_eu_8056B084[23] = {
        0x00000000, 0x00000000, (unsigned int)&__dt__Q34nw4r2ut10PackedFontFv, (unsigned int)&GetWidth__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetHeight__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetAscent__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetDescent__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetBaselinePos__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetCellHeight__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetCellWidth__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetMaxCharWidth__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetType__Q44nw4r2ut6detail11ResFontBaseCFv,
        (unsigned int)&GetTextureFormat__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetLineFeed__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&GetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseCFv, (unsigned int)&SetDefaultCharWidths__Q44nw4r2ut6detail11ResFontBaseFRCQ34nw4r2ut10CharWidths,
        (unsigned int)&SetAlternateChar__Q44nw4r2ut6detail11ResFontBaseFUs, (unsigned int)&SetLineFeed__Q44nw4r2ut6detail11ResFontBaseFi, (unsigned int)&GetCharWidth__Q44nw4r2ut6detail11ResFontBaseCFUs, (unsigned int)&GetCharWidths__Q44nw4r2ut6detail15ArchiveFontBaseCFUs,
        (unsigned int)&GetGlyph__Q34nw4r2ut10PackedFontCFPQ34nw4r2ut5GlyphUs, (unsigned int)&HasGlyph__Q44nw4r2ut6detail15ArchiveFontBaseCFUs, (unsigned int)&GetEncoding__Q44nw4r2ut6detail11ResFontBaseCFv,
    };

/* ======================= .sdata ======================= */
__declspec(align(8)) u8 lbl_eu_80663450[1] = {
        0x01,
    };

__declspec(align(1)) u8 gap_09_80663451_sdata[3] = {
        0x00, 0x00, 0x00,
    };

__declspec(align(4)) u16 lbl_eu_80663454[1] = {
        0x0A00,
    };

__declspec(align(2)) u16 gap_09_80663456_sdata[1] = {
        0x0000,
    };

__declspec(align(8)) u32 lbl_eu_80663458[2] = {
        (unsigned int)&PlayPolicy_Onetime__Q24nw4r3g3dFfff, (unsigned int)&PlayPolicy_Loop__Q24nw4r3g3dFfff,
    };

__declspec(align(8)) u32 lbl_eu_80663460[2] = {
        0x3F800000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_80663468[1] = {
        0xFFFFFFFF,
    };

__declspec(align(4)) u32 lbl_eu_8066346C[1] = {
        (unsigned int)&lbl_eu_8061FA20,
    };

__declspec(align(8)) u32 lbl_eu_80663470[2] = {
        0x4E6F6465, 0x4D697800,
    };
    /* f32[] table (retail lbl_eu_80663470) */

__declspec(align(8)) u32 lbl_eu_80663478[1] = {
        0xFFFFFFFC,
    };

__declspec(align(4)) u32 lbl_eu_8066347C[1] = {
        0xFFFFFFFC,
    };

__declspec(align(8)) u32 lbl_eu_80663480[1] = {
        0x00000020,
    };

__declspec(align(4)) u32 lbl_eu_80663484[1] = {
        0x00000020,
    };

__declspec(align(8)) u32 lbl_eu_80663488[1] = {
        0xFFFFFFFC,
    };

__declspec(align(4)) u32 lbl_eu_8066348C[1] = {
        0xFFFFFFFC,
    };

__declspec(align(8)) u32 lbl_eu_80663490[2] = {
        0x4E6F6465, 0x4D697800,
    };
    /* f32[] table (retail lbl_eu_80663490) */

__declspec(align(8)) u32 lbl_eu_80663498[2] = {
        0x44726177, 0x4F706100,
    };
    /* f32[] table (retail lbl_eu_80663498) */

__declspec(align(8)) u32 lbl_eu_806634A0[2] = {
        0x44726177, 0x586C7500,
    };
    /* f32[] table (retail lbl_eu_806634A0) */

__declspec(align(8)) u32 lbl_eu_806634A8[2] = {
        0xFFFFFFFF, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_806634B0[2] = {
        (unsigned int)&lbl_eu_80569828, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_806634B8[2] = {
        (unsigned int)&lbl_eu_805698A8, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_806634C0[1] = {
        0x000A0000,
    };

__declspec(align(4)) u32 gap_09_806634C4_sdata[1] = {
        0x00000000,
    };

__declspec(align(8)) u16 lbl_eu_806634C8[1] = {
        0x2E00,
    };

__declspec(align(2)) u16 gap_09_806634CA_sdata[1] = {
        0x0000,
    };

__declspec(align(4)) u32 lbl_eu_806634CC[1] = {
        0x2E2E0000,
    };
    /* f32[] table (retail lbl_eu_806634CC) */

__declspec(align(8)) u32 lbl_eu_806634D0[2] = {
        0x38D1B717, 0x00000000,
    };

__declspec(align(8)) u16 lbl_eu_806634D8[1] = {
        0x5400,
    };

__declspec(align(2)) u16 gap_09_806634DA_sdata[1] = {
        0x0000,
    };

__declspec(align(4)) u16 lbl_eu_806634DC[1] = {
        0x4700,
    };

__declspec(align(2)) u16 gap_09_806634DE_sdata[1] = {
        0x0000,
    };

__declspec(align(8)) u32 lbl_eu_806634E0[2] = {
        0x00000000, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_806634E8[2] = {
        (unsigned int)&lbl_eu_8056AC30, 0x00000000,
    };

__declspec(align(8)) u32 lbl_eu_806634F0[2] = {
        0x12345678, 0x00000000,
    };

__declspec(align(8)) u16 mFontEncode__Q34nw4r2ut7RomFont[1] = {
        0xFFFF,
    };

/* ======================= .sdata2 ======================= */
__declspec(align(8)) extern const u32 lbl_eu_80669A68 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669A6C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669A70 = 0x3FE3333340000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669A78 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669A80 = 0x40000000;  /* f32: 2.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669A84 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u64 lbl_eu_80669A88 = 0x3A80000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669A90 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669A98 = 0x4480000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669AA0 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669AA4 = 0x3F360B61;  /* f32: 0.7111111283302307 */

__declspec(align(8)) extern const u64 lbl_eu_80669AA8 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669AB0 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669AB4 = 0x40000000;  /* f32: 2.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669AB8 = 0x40400000;  /* f32: 3.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669ABC = 0x00000000;

__declspec(align(8)) extern const u64 lbl_eu_80669AC0 = 0x4700000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669AC8 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669AD0 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669AD8 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669ADC = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669AE0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669AE4 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669AE8 = 0x40000000;  /* f32: 2.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669AEC = 0x40400000;  /* f32: 3.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669AF0 = 0x3F360B6100000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669AF8 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669B00 = 0x3FB40000;  /* f32: 1.40625 */

__declspec(align(4)) extern const u32 lbl_eu_80669B04 = 0x42B40000;  /* f32: 90.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669B08 = 0xC2B4000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B10 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B18 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B20 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B28 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B30 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B38 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B40 = 0x03AA242500000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669B48 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669B4C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669B50 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669B58 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669B5C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669B60 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669B68 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669B6C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669B70 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B78 = 0x219392EF00000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B80 = 0x3FF0000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669B88 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669B8C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669B90 = 0x7FC0000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669B98 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669BA0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669BA4 = 0xD01502F9;  /* f32: -10000000000.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669BA8 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669BB0 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669BB8 = 0x3F00000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669BC0 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669BC8 = 0x00020103;

__declspec(align(4)) extern const u32 lbl_eu_80669BCC = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669BD0 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(4)) extern const u32 lbl_eu_80669BD4 = 0x40000000;  /* f32: 2.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669BD8 = 0x44800000;  /* f32: 1024.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669BDC = 0x00000000;

__declspec(align(8)) extern const u64 lbl_eu_80669BE0 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669BE8 = 0x00020103;

__declspec(align(4)) extern const u32 lbl_eu_80669BEC = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669BF0 = 0xBF800000;  /* f32: -1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669BF4 = 0x358637BD;  /* f32: 9.999999974752427e-07 */

__declspec(align(8)) extern const u32 lbl_eu_80669BF8 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669BFC = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u64 lbl_eu_80669C00 = 0x3B80808100000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669C08 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669C10 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669C18 = 0x4010000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669C20 = 0x437F000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669C28 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669C30 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669C34 = 0x219392EF;  /* f32: 1.000000045813705e-18 */

__declspec(align(8)) extern const u64 lbl_eu_80669C38 = 0x219392EF00000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669C40 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669C44 = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669C48 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669C4C = 0xC0000000;  /* f32: -2.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669C50 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669C58 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669C5C = 0x41700000;  /* f32: 15.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669C60 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669C64 = 0x42700000;  /* f32: 60.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669C68 = 0x3FAAAAAB;  /* f32: 1.3333333730697632 */

__declspec(align(4)) extern const u32 lbl_eu_80669C6C = 0x3DCCCCCD;  /* f32: 0.10000000149011612 */

__declspec(align(8)) extern const u32 lbl_eu_80669C70 = 0x447A0000;  /* f32: 1000.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669C74 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u64 lbl_eu_80669C78 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669C80 = 0xBF800000;  /* f32: -1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669C84 = 0x3F360B61;  /* f32: 0.7111111283302307 */

__declspec(align(8)) extern const u32 lbl_eu_80669C88 = 0x3F360B61;  /* f32: 0.7111111283302307 */

__declspec(align(4)) extern const u32 lbl_eu_80669C8C = 0x00000000;

__declspec(align(8)) extern const u64 lbl_eu_80669C90 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669C98 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669C9C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669CA0 = 0x3F360B61;  /* f32: 0.7111111283302307 */

__declspec(align(4)) extern const u32 lbl_eu_80669CA4 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u64 lbl_eu_80669CA8 = 0xBF00000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669CB0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669CB4 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669CB8 = 0x3F360B6100000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669CC0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669CC4 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u32 lbl_eu_80669CC8 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669CCC = 0x3F360B61;  /* f32: 0.7111111283302307 */

__declspec(align(8)) extern const u64 lbl_eu_80669CD0 = 0xBF00000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669CD8 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669CDC = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669CE0 = 0x3F00000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669CE8 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669CF0 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669CF8 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669D00 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669D04 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669D08 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(4)) extern const u8 lbl_eu_80669D0C = 0xFF;

__declspec(align(1)) extern const u8 lbl_eu_80669D0D = 0xFF;

__declspec(align(2)) extern const u8 lbl_eu_80669D0E = 0xFF;

__declspec(align(1)) extern const u8 lbl_eu_80669D0F = 0xFF;

__declspec(align(8)) extern const u8 lbl_eu_80669D10 = 0xFF;

__declspec(align(1)) extern const u8 lbl_eu_80669D11 = 0xFF;

__declspec(align(2)) extern const u8 lbl_eu_80669D12 = 0xFF;

__declspec(align(1)) extern const u8 lbl_eu_80669D13 = 0xFF;

__declspec(align(4)) extern const u32 lbl_eu_80669D14 = 0x4A742400;  /* f32: 4000000.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669D18 = 0xBF800000;  /* f32: -1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669D1C = 0x42B40000;  /* f32: 90.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669D20 = 0x41200000;  /* f32: 10.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669D24 = 0x41000000;  /* f32: 8.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669D28 = 0xC0E0000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669D30 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669D38 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669D3C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669D40 = 0x3C8EFA35;  /* f32: 0.01745329238474369 */

__declspec(align(4)) extern const u32 lbl_eu_80669D44 = 0x3B808081;  /* f32: 0.003921568859368563 */

__declspec(align(8)) extern const u64 lbl_eu_80669D48 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669D50 = 0x3F00000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669D58 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669D5C = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u64 lbl_eu_80669D60 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669D68 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669D70 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669D78 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(4)) extern const u32 lbl_eu_80669D7C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669D80 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669D88 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669D90 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669D98 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(4)) extern const u32 lbl_eu_80669D9C = 0x3F360B61;  /* f32: 0.7111111283302307 */

__declspec(align(8)) extern const u32 lbl_eu_80669DA0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669DA4 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669DA8 = 0x4000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669DB0 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669DB8 = 0x3B80000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669DC0 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669DC8 = 0x4380000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669DD0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669DD4 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669DD8 = 0xBA83126F;  /* f32: -0.0010000000474974513 */

__declspec(align(4)) extern const u32 lbl_eu_80669DDC = 0x3A83126F;  /* f32: 0.0010000000474974513 */

__declspec(align(8)) extern const u32 lbl_eu_80669DE0 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669DE4 = 0x40000000;  /* f32: 2.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669DE8 = 0x40400000;  /* f32: 3.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669DEC = 0xC0000000;  /* f32: -2.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669DF0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669DF4 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u32 lbl_eu_80669DF8 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669DFC = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669E00 = 0x3FB8AA3B;  /* f32: 1.4426950216293335 */

__declspec(align(4)) extern const u32 lbl_eu_80669E04 = 0x3F317218;  /* f32: 0.6931471824645996 */

__declspec(align(8)) extern const u32 lbl_eu_80669E08 = 0x41B8AA3B;  /* f32: 23.083120346069336 */

__declspec(align(4)) extern const u32 lbl_eu_80669E0C = 0x43800000;  /* f32: 256.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669E10 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669E14 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u64 lbl_eu_80669E18 = 0x4040000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669E20 = 0x47800000;  /* f32: 65536.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669E24 = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669E28 = 0x42000000;  /* f32: 32.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669E2C = 0x42800000;  /* f32: 64.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669E30 = 0xC2800000;  /* f32: -64.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669E34 = 0x43000000;  /* f32: 128.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669E38 = 0xC300000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669E40 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669E44 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669E48 = 0x3CC90FDB00000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669E50 = 0x4780000047800000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669E58 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(4)) extern const u32 lbl_eu_80669E5C = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669E60 = 0x3CC90FDB;  /* f32: 0.02454369328916073 */

__declspec(align(4)) extern const u32 lbl_eu_80669E64 = 0x3F360B61;  /* f32: 0.7111111283302307 */

__declspec(align(8)) extern const u64 lbl_eu_80669E68 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669E70 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669E78 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669E7C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669E80 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669E88 = 0x47000000;  /* f32: 32768.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669E8C = 0x46FA0000;  /* f32: 32000.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669E90 = 0x46FA0000;  /* f32: 32000.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669E94 = 0x3FAAAAAB;  /* f32: 1.3333333730697632 */

__declspec(align(8)) extern const u64 lbl_eu_80669E98 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669EA0 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669EA8 = 0x477FFF00;  /* f32: 65535.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669EAC = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669EB0 = 0x47800000;  /* f32: 65536.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669EB4 = 0x46FFFE00;  /* f32: 32767.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669EB8 = 0x42FE0000;  /* f32: 127.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669EBC = 0x427C0000;  /* f32: 63.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669EC0 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669EC8 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669ED0 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669ED8 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669EE0 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669EE4 = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669EE8 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669EEC = 0x00000000;

__declspec(align(8)) extern const u64 lbl_eu_80669EF0 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669EF8 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669F00 = 0x4000000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669F08 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669F0C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669F10 = 0x437F0000;  /* f32: 255.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F14 = 0x40C00000;  /* f32: 6.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669F18 = 0x4380000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669F20 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669F28 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669F30 = 0xC2B4CCCD;  /* f32: -90.4000015258789 */

__declspec(align(4)) extern const u32 lbl_eu_80669F34 = 0x477FFF00;  /* f32: 65535.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669F38 = 0x41200000;  /* f32: 10.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F3C = 0x00000000;

__declspec(align(8)) extern const u64 lbl_eu_80669F40 = 0xBD00000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669F48 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669F50 = 0x41C00000;  /* f32: 24.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F54 = 0x3C000000;  /* f32: 0.0078125 */

__declspec(align(8)) extern const u32 lbl_eu_80669F58 = 0x40A00000;  /* f32: 5.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F5C = 0x42700000;  /* f32: 60.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669F60 = 0x41200000;  /* f32: 10.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F64 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(8)) extern const u32 lbl_eu_80669F68 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F6C = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669F70 = 0x42480000;  /* f32: 50.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F74 = 0x3DCCCCCD;  /* f32: 0.10000000149011612 */

__declspec(align(8)) extern const u32 lbl_eu_80669F78 = 0x40000000;  /* f32: 2.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F7C = 0x3F7D70A4;  /* f32: 0.9900000095367432 */

__declspec(align(8)) extern const u32 lbl_eu_80669F80 = 0x43200000;  /* f32: 160.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F84 = 0x3ECCCCCD;  /* f32: 0.4000000059604645 */

__declspec(align(8)) extern const u32 lbl_eu_80669F88 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669F8C = 0x3D23D70A;  /* f32: 0.03999999910593033 */

__declspec(align(8)) extern const u32 lbl_eu_80669F90 = 0x3F7D70A4;  /* f32: 0.9900000095367432 */

__declspec(align(4)) extern const u32 lbl_eu_80669F94 = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669F98 = 0x3CA3D70A;  /* f32: 0.019999999552965164 */

__declspec(align(4)) extern const u32 lbl_eu_80669F9C = 0x40400000;  /* f32: 3.0 */

__declspec(align(8)) extern const u32 lbl_eu_80669FA0 = 0x3F19999A;  /* f32: 0.6000000238418579 */

__declspec(align(4)) extern const u32 lbl_eu_80669FA4 = 0x3ECCCCCD;  /* f32: 0.4000000059604645 */

__declspec(align(8)) extern const u32 lbl_eu_80669FA8 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669FAC = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_80669FB0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_80669FB4 = 0x40C80000;  /* f32: 6.25 */

__declspec(align(8)) extern const u64 lbl_eu_80669FB8 = 0x447A000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669FC0 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669FC8 = 0x40800000;  /* f32: 4.0 */

__declspec(align(4)) extern const u32 lbl_eu_80669FCC = 0x42000000;  /* f32: 32.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669FD0 = 0x42FE000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669FD8 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_80669FE0 = 0x3C000000;  /* f32: 0.0078125 */

__declspec(align(4)) extern const u32 lbl_eu_80669FE4 = 0x3EC80000;  /* f32: 0.390625 */

__declspec(align(8)) extern const u32 lbl_eu_80669FE8 = 0x3C800000;  /* f32: 0.015625 */

__declspec(align(4)) extern const u32 lbl_eu_80669FEC = 0x42FE0000;  /* f32: 127.0 */

__declspec(align(8)) extern const u64 lbl_eu_80669FF0 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_80669FF8 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A000 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A004 = 0x00000000;

__declspec(align(8)) extern const u64 lbl_eu_8066A008 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A010 = 0x476A600000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A018 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A01C = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_8066A020 = 0x42FE0000;  /* f32: 127.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A024 = 0x3C000000;  /* f32: 0.0078125 */

__declspec(align(8)) extern const u32 lbl_eu_8066A028 = 0x427C0000;  /* f32: 63.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A02C = 0xBF800000;  /* f32: -1.0 */

__declspec(align(8)) extern const u64 lbl_eu_8066A030 = 0x4000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A038 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A040 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A048 = 0x42FE000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A050 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A058 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A05C = 0x00000000;

__declspec(align(8)) extern const u64 lbl_eu_8066A060 = 0x0000000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A068 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A070 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A078 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A07C = 0x42FE0000;  /* f32: 127.0 */

__declspec(align(8)) extern const u64 lbl_eu_8066A080 = 0x427C000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A088 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_8066A08C = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_8066A090 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A098 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A09C = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_8066A0A0 = 0xBDF5C28F;  /* f32: -0.11999999731779099 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0A4 = 0x3F6147AE;  /* f32: 0.8799999952316284 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0A8 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0AC = 0x3F8F5C29;  /* f32: 1.1200000047683716 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0B0 = 0x3F59999A;  /* f32: 0.8500000238418579 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0B4 = 0x3E199998;  /* f32: 0.1499999761581421 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0B8 = 0x40000000;  /* f32: 2.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0BC = 0x3EB33334;  /* f32: 0.3500000238418579 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0C0 = 0xBF800000;  /* f32: -1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0C4 = 0xC0400000;  /* f32: -3.0 */

__declspec(align(8)) extern const u64 lbl_eu_8066A0C8 = 0x4700000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A0D0 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0D4 = 0x40000000;  /* f32: 2.0 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0D8 = 0x3F000000;  /* f32: 0.5 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0DC = 0x40C00000;  /* f32: 6.0 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0E0 = 0xC2B4CCCD;  /* f32: -90.4000015258789 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0E4 = 0x41200000;  /* f32: 10.0 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0E8 = 0xBF800000;  /* f32: -1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0EC = 0x43800000;  /* f32: 256.0 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0F0 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_8066A0F4 = 0x3E0ADE7F;  /* f32: 0.13561438024044037 */

__declspec(align(8)) extern const u32 lbl_eu_8066A0F8 = 0x3F666666;  /* f32: 0.8999999761581421 */

__declspec(align(4)) extern const u32 lbl_eu_8066A0FC = 0x3D088889;  /* f32: 0.03333333507180214 */

__declspec(align(8)) extern const u64 lbl_eu_8066A100 = 0x3F80000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A108 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A10C = 0x00000000;

__declspec(align(8)) extern const u32 lbl_eu_8066A110 = 0x427C0000;  /* f32: 63.0 */

__declspec(align(4)) extern const u32 lbl_eu_8066A114 = 0x3C800000;  /* f32: 0.015625 */

__declspec(align(8)) extern const u64 lbl_eu_8066A118 = 0x42FE000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A120 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A128 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A130 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A138 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A140 = 0x00000000;

__declspec(align(4)) extern const u32 lbl_eu_8066A144 = 0x3F800000;  /* f32: 1.0 */

__declspec(align(8)) extern const u64 lbl_eu_8066A148 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A150 = 0x3F00000000000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A158 = 0x4330000000000000ULL;

__declspec(align(8)) extern const u32 lbl_eu_8066A160 = 0x7F7FFFFF;  /* f32: 3.4028234663852886e+38 */

__declspec(align(4)) extern const u32 lbl_eu_8066A164 = 0x00000000;

__declspec(align(8)) extern const u64 lbl_eu_8066A168 = 0x4330000080000000ULL;

__declspec(align(8)) extern const u64 lbl_eu_8066A170 = 0x3F00000000000000ULL;

/* ======================= .sbss ======================= */
__declspec(align(8)) u32 lbl_eu_80665438;

__declspec(align(4)) u32 lbl_eu_8066543C;

__declspec(align(8)) u64 lbl_eu_80665440;

__declspec(align(8)) u8 lbl_eu_80665448;

__declspec(align(1)) u8 gap_10_80665449_sbss[3];

__declspec(align(4)) u8 lbl_eu_8066544C;

__declspec(align(1)) u8 gap_10_8066544D_sbss[3];

__declspec(align(8)) u32 lbl_eu_80665450;

__declspec(align(4)) u32 lbl_eu_80665454;

__declspec(align(8)) u64 lbl_eu_80665458;

__declspec(align(8)) u64 lbl_eu_80665460;

__declspec(align(8)) u64 lbl_eu_80665468;

__declspec(align(8)) u64 lbl_eu_80665470;

__declspec(align(8)) u64 lbl_eu_80665478;

__declspec(align(8)) u64 lbl_eu_80665480;

__declspec(align(8)) u64 lbl_eu_80665488;

__declspec(align(8)) u64 lbl_eu_80665490;

__declspec(align(8)) u64 lbl_eu_80665498;

__declspec(align(8)) u64 lbl_eu_806654A0;

__declspec(align(8)) u32 lbl_eu_806654A8;

__declspec(align(4)) u32 lbl_eu_806654AC;

__declspec(align(8)) u32 lbl_eu_806654B0;

__declspec(align(4)) u32 lbl_eu_806654B4;

__declspec(align(8)) u32 lbl_eu_806654B8;

__declspec(align(4)) u8 lbl_eu_806654BC;

__declspec(align(1)) u8 gap_10_806654BD_sbss[3];

__declspec(align(8)) u64 lbl_eu_806654C0;

__declspec(align(8)) u32 lbl_eu_806654C8;

__declspec(align(4)) u32 lbl_eu_806654CC;

__declspec(align(8)) u64 lbl_eu_806654D0;

__declspec(align(8)) u64 lbl_eu_806654D8;

__declspec(align(8)) u64 lbl_eu_806654E0;

__declspec(align(8)) u64 lbl_eu_806654E8;

__declspec(align(8)) u64 lbl_eu_806654F0;

__declspec(align(8)) u64 lbl_eu_806654F8;

__declspec(align(8)) u64 lbl_eu_80665500;

__declspec(align(8)) u8 lbl_eu_80665508;

__declspec(align(1)) u8 gap_10_80665509_sbss[3];

__declspec(align(4)) u32 lbl_eu_8066550C;

__declspec(align(8)) u64 lbl_eu_80665510;

__declspec(align(8)) u64 lbl_eu_80665518;

__declspec(align(8)) u64 lbl_eu_80665520;

__declspec(align(8)) u64 lbl_eu_80665528;

__declspec(align(8)) u64 lbl_eu_80665530;

__declspec(align(8)) u64 lbl_eu_80665538;

__declspec(align(8)) u64 lbl_eu_80665540;

/* ======================= .bss ======================= */
__declspec(align(8)) u8 lbl_eu_8061A4D8[0x48];

__declspec(align(8)) u8 lbl_eu_8061A520[0xc];

__declspec(align(4)) u8 lbl_eu_8061A52C[0x74];

__declspec(align(8)) u8 lbl_eu_8061A5A0[0x120];

__declspec(align(8)) u8 lbl_eu_8061A6C0[0x70];

__declspec(align(8)) u8 lbl_eu_8061A730[0x20];

__declspec(align(8)) u8 lbl_eu_8061A750[0x10];

__declspec(align(8)) u8 lbl_eu_8061A760[0x800];

__declspec(align(8)) u8 lbl_eu_8061AF60[0x614];

__declspec(align(4)) u8 lbl_eu_8061B574[0x2a2c];

__declspec(align(8)) u8 lbl_eu_8061DFA0[0x1a34];

__declspec(align(4)) u8 lbl_eu_8061F9D4[0x4c];

__declspec(align(8)) u8 lbl_eu_8061FA20[0x98];

__declspec(align(8)) u8 lbl_eu_8061FAB8[0x48];

__declspec(align(8)) u8 lbl_eu_8061FB00[0x18000];

__declspec(align(8)) u8 lbl_eu_80637B00[0x20];

__declspec(align(8)) u8 lbl_eu_80637B20[0x100];

__declspec(align(8)) u8 lbl_eu_80637C20[0x23c];

__declspec(align(4)) u8 lbl_eu_80637E5C[0xc];

__declspec(align(8)) u8 lbl_eu_80637E68[0x100];

__declspec(align(8)) u8 lbl_eu_80637F68[0xc];

__declspec(align(4)) u8 lbl_eu_80637F74[0x2c];

__declspec(align(8)) u8 lbl_eu_80637FA0[0x10];

__declspec(align(8)) u8 lbl_eu_80637FB0[0x20];

__declspec(align(8)) u8 lbl_eu_80637FD0[0xc];

__declspec(align(4)) u8 lbl_eu_80637FDC[0xc];

__declspec(align(8)) u8 lbl_eu_80637FE8[0x2d8];

__declspec(align(8)) u8 lbl_eu_806382C0[0x20];

__declspec(align(8)) u8 lbl_eu_806382E0[0x10];

__declspec(align(8)) u8 lbl_eu_806382F0[0x610];

__declspec(align(8)) u8 lbl_eu_80638900[0x10];

__declspec(align(8)) u8 lbl_eu_80638910[0x330];

__declspec(align(8)) u8 lbl_eu_80638C40[0x16e00];

__declspec(align(8)) u8 lbl_eu_8064FA40[0x10];

__declspec(align(8)) u8 lbl_eu_8064FA50[0x3b0];

__declspec(align(8)) u8 lbl_eu_8064FE00[0x4000];

__declspec(align(8)) u8 lbl_eu_80653E00[0x18];

__declspec(align(8)) u8 lbl_eu_80653E18[0xc];

__declspec(align(4)) u8 lbl_eu_80653E24[0x3c];

__declspec(align(8)) u8 lbl_eu_80653E60[0xc];

__declspec(align(4)) u8 lbl_eu_80653E6C[0x1c];

__declspec(align(8)) u8 lbl_eu_80653E88[0x20];

__declspec(align(8)) u8 lbl_eu_80653EA8[0x10];

__declspec(align(8)) u8 lbl_eu_80653EB8[0x10];

__declspec(align(8)) u8 lbl_eu_80653EC8[0xc];

__declspec(align(4)) u8 lbl_eu_80653ED4[0xc];

/* ======================= .sbss2 ======================= */
__declspec(align(8)) extern const u16 DefaultBlackColor_8066B550[4] = {0, 0, 0, 0};

/*
 * RESIDUALS (reloc targets that cannot be referenced from C++ with MWCC,
 * emitted as placeholder 0 at their retail offsets; bytes already match):
 */
/* .rodata: 56 placeholders */
/*   +0x60  GetAnmResult___Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r3g3d12ChrAnmResultRCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataf  addend=0 */
/*   +0x64  GetAnmResult_S__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r3g3d12ChrAnmResultRCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataf  addend=0 */
/*   +0x68  GetAnmResult_R__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r3g3d12ChrAnmResultRCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataf  addend=0 */
/*   +0x6C  GetAnmResult_SR__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r3g3d12ChrAnmResultRCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataf  addend=0 */
/*   +0x70  GetAnmResult_T__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r3g3d12ChrAnmResultRCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataf  addend=0 */
/*   +0x74  GetAnmResult_ST__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r3g3d12ChrAnmResultRCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataf  addend=0 */
/*   +0x78  GetAnmResult_RT__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r3g3d12ChrAnmResultRCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataf  addend=0 */
/*   +0x7C  GetAnmResult_SRT__Q34nw4r3g3d27@unnamed@g3d_resanmchr_cpp@FPQ34nw4r3g3d12ChrAnmResultRCQ34nw4r3g3d17ResAnmChrInfoDataPCQ34nw4r3g3d17ResAnmChrNodeDataf  addend=0 */
/*   +0x204  Calc_BILLBOARD_STD__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl  addend=0 */
/*   +0x208  Calc_BILLBOARD_PERSP_STD__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl  addend=0 */
/*   +0x20C  Calc_BILLBOARD_ROT__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl  addend=0 */
/*   +0x210  Calc_BILLBOARD_PERSP_ROT__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl  addend=0 */
/*   +0x214  Calc_BILLBOARD_Y__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl  addend=0 */
/*   +0x218  Calc_BILLBOARD_PERSP_Y__Q34nw4r3g3d26@unnamed@g3d_calcview_cpp@FPQ34nw4r4math5MTX34PCQ34nw4r4math5MTX34bPCQ34nw4r4math5MTX34Q34nw4r3g3d6ResMdlUl  addend=0 */
/*   +0x220  MakeTexSrtMtx_SRT__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x224  MakeTexSrtMtx_RT__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x228  MakeTexSrtMtx_ST__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x22C  MakeTexSrtMtx_T__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x230  MakeTexSrtMtx_SR__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x234  MakeTexSrtMtx_R__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x238  MakeTexSrtMtx_S__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x23C  ProductTexSrtMtx_SRT__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x240  ProductTexSrtMtx_RT__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x244  ProductTexSrtMtx_ST__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x248  ProductTexSrtMtx_T__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x24C  ProductTexSrtMtx_SR__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x250  ProductTexSrtMtx_R__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x254  ProductTexSrtMtx_S__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_maya_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x258  MakeTexSrtMtx_SRT__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x25C  MakeTexSrtMtx_RT__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x260  MakeTexSrtMtx_ST__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x264  MakeTexSrtMtx_T__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x268  MakeTexSrtMtx_SR__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x26C  MakeTexSrtMtx_R__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x270  MakeTexSrtMtx_S__Q54nw4r3g3d6detail3dcc21@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x274  ProductTexSrtMtx_SRT__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x278  ProductTexSrtMtx_RT__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x27C  ProductTexSrtMtx_ST__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x280  ProductTexSrtMtx_T__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x284  ProductTexSrtMtx_SR__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x288  ProductTexSrtMtx_R__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x28C  ProductTexSrtMtx_S__Q54nw4r3g3d6detail3dcc22@unnamed@g3d_xsi_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x290  MakeTexSrtMtx_SRT__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x294  MakeTexSrtMtx_RT__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x298  MakeTexSrtMtx_ST__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x29C  MakeTexSrtMtx_T__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2A0  MakeTexSrtMtx_SR__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2A4  MakeTexSrtMtx_R__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2A8  MakeTexSrtMtx_S__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2AC  ProductTexSrtMtx_SRT__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2B0  ProductTexSrtMtx_RT__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2B4  ProductTexSrtMtx_ST__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2B8  ProductTexSrtMtx_T__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2BC  ProductTexSrtMtx_SR__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2C0  ProductTexSrtMtx_R__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/*   +0x2C4  ProductTexSrtMtx_S__Q54nw4r3g3d6detail3dcc24@unnamed@g3d_3dsmax_cpp@FPQ34nw4r4math5MTX34RCQ34nw4r3g3d6TexSrt  addend=0 */
/* .data: 26 placeholders */
/*   +0xA7C  SetTagProcessor__Q34nw4r3lyt6LayoutFPQ34nw4r2ut19TagProcessorBase<w>  addend=0 */
/*   +0x1BEC  @212@__dt__Q44nw4r3snd6detail9SeqPlayerFv  addend=0 */
/*   +0x1BF0  @208@InvalidateData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv  addend=0 */
/*   +0x1BF4  @208@InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv  addend=0 */
/*   +0x1C00  @224@__dt__Q44nw4r3snd6detail9SeqPlayerFv  addend=0 */
/*   +0x1C04  @220@OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv  addend=0 */
/*   +0x1C0C  @220@OnShutdownSoundThread__Q44nw4r3snd6detail9SeqPlayerFv  addend=0 */
/*   +0x1CC4  @12@__dt__Q34nw4r3snd18SoundArchivePlayerFv  addend=0 */
/*   +0x1CC8  @12@detail_SetupSound__Q34nw4r3snd18SoundArchivePlayerFPQ34nw4r3snd11SoundHandleUlbPCQ44nw4r3snd14SoundStartable9StartInfo  addend=0 */
/*   +0x1CCC  @12@detail_ConvertLabelStringToSoundId__Q34nw4r3snd18SoundArchivePlayerFPCc  addend=0 */
/*   +0x1D6C  @208@__dt__Q44nw4r3snd6detail10StrmPlayerFv  addend=0 */
/*   +0x1D70  @208@OnUpdateFrameSoundThread__Q44nw4r3snd6detail10StrmPlayerFv  addend=0 */
/*   +0x1D74  @208@OnUpdateVoiceSoundThread__Q44nw4r3snd6detail10StrmPlayerFv  addend=0 */
/*   +0x1D78  @208@OnShutdownSoundThread__Q44nw4r3snd6detail10StrmPlayerFv  addend=0 */
/*   +0x1E9C  @212@__dt__Q44nw4r3snd6detail9WsdPlayerFv  addend=0 */
/*   +0x1EA0  @212@InvalidateData__Q44nw4r3snd6detail9WsdPlayerFPCvPCv  addend=0 */
/*   +0x1EA4  @212@InvalidateWaveData__Q44nw4r3snd6detail9WsdPlayerFPCvPCv  addend=0 */
/*   +0x1EB0  @224@__dt__Q44nw4r3snd6detail9WsdPlayerFv  addend=0 */
/*   +0x1EB4  @224@OnUpdateFrameSoundThread__Q44nw4r3snd6detail9WsdPlayerFv  addend=0 */
/*   +0x1EBC  @224@OnShutdownSoundThread__Q44nw4r3snd6detail9WsdPlayerFv  addend=0 */
/*   +0x1ED8  __dt__Q34nw4r2ut19TagProcessorBase<w>Fv  addend=0 */
/*   +0x1EDC  Process__Q34nw4r2ut19TagProcessorBase<w>FUsPQ34nw4r2ut15PrintContext<w>  addend=0 */
/*   +0x1EE0  CalcRect__Q34nw4r2ut19TagProcessorBase<w>FPQ34nw4r2ut4RectUsPQ34nw4r2ut15PrintContext<w>  addend=0 */
/*   +0x1EEC  __dt__Q34nw4r2ut19TagProcessorBase<c>Fv  addend=0 */
/*   +0x1EF0  Process__Q34nw4r2ut19TagProcessorBase<c>FUsPQ34nw4r2ut15PrintContext<c>  addend=0 */
/*   +0x1EF4  CalcRect__Q34nw4r2ut19TagProcessorBase<c>FPQ34nw4r2ut4RectUsPQ34nw4r2ut15PrintContext<c>  addend=0 */

