# Demoted FULL_MATCH after ElfSymbolError resolver fix (2026-09-19)

After fixing symbol extraction (`symbol__` prefix preference, `@NN@` strip,
`.init` fallback, pair code-disambiguation), 28 of 465 previously unscannable
ElfSymbolError rows became extractable. **15** failed
byte+reloc identity and were demoted to `NOT_STARTED`; **13** are truly identical and left as `FULL_MATCH`.

## Summary

- Demoted: **15**
- Newly confirmed identical: **13**
- Still unextractable (mostly `base:gone`): **437**

## By unit

| n | unit |
|--:|------|
| 5 | `kyoshin/cf/CfGimmick` |
| 3 | `nw4r/src/snd/snd_MidiSeqPlayer` |
| 2 | `monolib/src/device/CDeviceVI` |
| 2 | `monolib/src/lib/CLibCri` |
| 1 | `RVL_SDK/src/revolution/os/__start` |
| 1 | `kyoshin/cf/CTaskCulling` |
| 1 | `monolib/src/lib/CLibCriMoviePlay` |

## Demoted rows

| id | unit | symbol | retail→decomp | sizes |
|----|------|--------|---------------|------:|
| `us-80006480` | `RVL_SDK/src/revolution/os/__start` | `__init_registers` | `__init_registers` → `__init_registers` | 144/144 |
| `us-801a47c8` | `kyoshin/cf/CTaskCulling` | `@88@cbRenderBefore__Q22cf12CTaskCullingFv` | `@88@cbRenderBefore__Q22cf12CTaskCullingF` → `cbRenderBefore__Q22cf12CTaskCullingFv` | 8/4 |
| `us-8020aa28` | `kyoshin/cf/CfGimmick` | `CfGimmick_PlaySoundAtPos` | `CfGimmick_PlaySoundAtPos` → `CfGimmick_PlaySoundAtPos__FUlPC13CfGimmi` | 24/24 |
| `us-8020ad14` | `kyoshin/cf/CfGimmick` | `CfGimmick_LoadBdatAreaPos` | `CfGimmick_LoadBdatAreaPos` → `CfGimmick_LoadBdatAreaPos__FPQ22cf9CfGim` | 236/236 |
| `us-8020ae00` | `kyoshin/cf/CfGimmick` | `CfGimmick_LoadBdatAreaExtents` | `CfGimmick_LoadBdatAreaExtents` → `CfGimmick_LoadBdatAreaExtents__FPQ22cf9C` | 316/316 |
| `us-8020b068` | `kyoshin/cf/CfGimmick` | `CfGimmick_LoadBdatAreaRotation` | `CfGimmick_LoadBdatAreaRotation` → `CfGimmick_LoadBdatAreaRotation__FPQ22cf9` | 260/260 |
| `us-8020bf04` | `kyoshin/cf/CfGimmick` | `CfGimmick_ApplyPartyMoveSpeed` | `CfGimmick_ApplyPartyMoveSpeed` → `CfGimmick_ApplyPartyMoveSpeed__Ff` | 184/184 |
| `us-8044b9a0` | `monolib/src/device/CDeviceVI` | `@456@errorWiiCB__9CDeviceVIFv` | `@456@errorWiiCB__9CDeviceVIFv` → `errorWiiCB__9CDeviceVIFv` | 8/16 |
| `us-8044b9a8` | `monolib/src/device/CDeviceVI` | `@456@__dt__9CDeviceVIFv` | `@456@__dt__9CDeviceVIFv` → `__dt__9CDeviceVIFv` | 8/300 |
| `us-8045dc98` | `monolib/src/lib/CLibCri` | `@452@__dt__7CLibCriFv` | `@452@__dt__7CLibCriFv` → `__dt__7CLibCriFv` | 8/160 |
| `us-8045dca8` | `monolib/src/lib/CLibCri` | `@456@__dt__7CLibCriFv` | `@456@__dt__7CLibCriFv` → `__dt__7CLibCriFv` | 8/160 |
| `us-8045f410` | `monolib/src/lib/CLibCriMoviePlay` | `@452@__dt__16CLibCriMoviePlayFv` | `@452@__dt__16CLibCriMoviePlayFv` → `__dt__16CLibCriMoviePlayFv` | 8/112 |
| `us-80419934` | `nw4r/src/snd/snd_MidiSeqPlayer` | `@208@InvalidateWaveData__Q44nw4r3snd6detail9SeqPlayerFPCvPCv` | `@208@InvalidateWaveData__Q44nw4r3snd6det` → `InvalidateWaveData__Q44nw4r3snd6detail9S` | 8/4 |
| `us-80419944` | `nw4r/src/snd/snd_MidiSeqPlayer` | `@220@OnShutdownSoundThread__Q44nw4r3snd6detail9SeqPlayerFv` | `@220@OnShutdownSoundThread__Q44nw4r3snd6` → `OnShutdownSoundThread__Q44nw4r3snd6detai` | 8/16 |
| `us-8041994c` | `nw4r/src/snd/snd_MidiSeqPlayer` | `@220@OnUpdateFrameSoundThread__Q44nw4r3snd6detail9SeqPlayerFv` | `@220@OnUpdateFrameSoundThread__Q44nw4r3s` → `OnUpdateFrameSoundThread__Q44nw4r3snd6de` | 8/4 |

## Confirmed identical (not demoted)

- `us-800062c0`
- `us-800062f0`
- `us-80006300`
- `us-80006310`
- `us-80006510`
- `us-80047380`
- `us-80066684`
- `us-80066688`
- `us-801547dc`
- `us-801d59e4`
- `us-801efbfc`
- `us-8020bdc4`
- `us-8020beac`

