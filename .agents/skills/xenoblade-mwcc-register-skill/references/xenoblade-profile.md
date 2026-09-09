# Xenoblade compiler profile (game code)

Game code (`kyoshin` lib in `xbret/xenoblade`) is built with
**Wii/1.1** (`config.linker_version`), i.e. the `Wii/1.1` entry of the
decomp-toolkit compilers bundle (`compilers_tag 20250812` in
`configure.py`). The allocator rules in this skill were established in
Wii 1.1 `mwcceppc.exe` with SHA-256:

```text
14f99570099c6ddee35759d7f9f668ee014c460758f89e0cdf2a51f8f4d8fbe5
```

Step 0 of `SKILL.md` (fingerprint your local binary against this hash)
is mandatory: if the bundle's build differs, every `inferred` rule must
be re-validated.

## Game flags (`cflags_game`)

Base, common to most objects:

```text
-nodefaults -proc gekko -align powerpc -enum int -fp hardware
-Cpp_exceptions off -O4,p -inline auto
-pragma "cats off" -pragma "warn_notinlined off"
-maxerrors 1 -nosyspath -RTTI off -fp_contract on -str reuse -enc SJIS
-DNDEBUG=1 -DBUILD_VERSION=<n> -DVERSION_<JP|EU|US>
```

Game additions (these are what the allocator actually runs under):

```text
-ipa file -inline auto -use_lmw_stmw on -str reuse,pool,readonly
-RTTI on -Cpp_exceptions on -func_align 4
```

For isolated allocation probes, retain the code-generation-relevant
subset:

```text
-nodefaults -proc gekko -align powerpc -enum int -fp hardware
-O4,p -inline auto -ipa file -use_lmw_stmw on -str reuse,pool,readonly
-RTTI on -Cpp_exceptions on -fp_contract on -func_align 4 -c
```

`-O4,p` includes optimization, peepholes, scheduling, and function
optimization. Hold all of these fixed when attributing a difference to
register coloring.

## Per-object exceptions inside `kyoshin`

Some game objects override `-O4,p` with `-O4,s` (size mode), which
changes codegen upstream of the allocator. Known cases in
`configure.py`: `kyoshin/CGame.cpp`, `kyoshin/CTaskGame.cpp`,
`kyoshin/CBgTex.cpp`, `kyoshin/CTitleAHelp.cpp`,
`kyoshin/makecrystal/CMCEffStart.cpp`, `kyoshin/cf/CTaskGameCf.cpp`.
Always check the object's `extra_cflags` before tuning allocation.

## Library exceptions (different flags, same or other builds)

- Runtime / MSL / TRK / Dolphin SDK / Ndev / HBM: same `-O4,p` core
  but different inline, string-pooling, sdata, float-contract, and
  language settings (see `cflags_runtime`, `cflags_mslc`, `cflags_trk`,
  `cflags_sdk`, `cflags_hbm`, `cflags_ndev` in `configure.py`).
  Notably the SDK uses `-lang=c -fp_contract off -func_align 16`.
- nw4r and CriWare use **GC/3.0a5.2** — outside this skill's scope.
- HBM uses **Wii/1.0a** for JP and Wii/1.1 for EU/US — JP HBM objects
  are outside this skill's scope until Wii/1.0a is validated.
- Like Melee's libraries, do not generalize game-code behavior to
  library objects or vice versa.
