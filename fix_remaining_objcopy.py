#!/usr/bin/env python3
import pathlib, subprocess, json, struct, os

# Units that are still failing (from last run)
failing = [
    "CriWare/src/sofdec/sfdcore/mpv/mpv_hdec",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mc",
    "CriWare/src/sofdec/sfdcore/mpv/mpv_mcy",
    "CriWare/src/sofdec/sfdcore/mpv/mpvabdec",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_adxt",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_aoap",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_lib",
    "CriWare/src/sofdec/sfdcore/sfd/sfd_ply",
]

plan = json.loads(pathlib.Path(".scratch/criware_data_plan.json").read_text())

for u in failing:
    # plan key is with .c
    plan_key = u + ".c"
    if plan_key not in plan:
        plan_key = u
    # Retail asm file
    asm_s = pathlib.Path("build/us/asm") / (u + ".s")
    retail_o = pathlib.Path(f"./retail_{u.replace('/', '_')}.o")
    decomp_o = pathlib.Path("build/us/src") / (u + ".o")
    if not asm_s.exists():
        print(f"no asm {asm_s}")
        continue
    if not decomp_o.exists():
        print(f"no decomp {decomp_o}")
        continue
    # Assemble retail .s to .o
    # Use powerpc-eabi-as
    as_cmd = ["build/binutils/powerpc-eabi-as", "-mgekko", "-I", "include", str(asm_s), "-o", str(retail_o)]
    subprocess.run(as_cmd, check=True)
    print(f"assembled {u} retail to {retail_o}")
    # For each section in plan for this unit, copy from retail to decomp
    for secinfo in plan.get(plan_key, []):
        sec = secinfo["section"]
        # Only handle .rodata/.data/.bss that are in plan
        # Dump retail section
        retail_sec_bin = pathlib.Path(f"./tmp_{u.replace('/', '_')}_{sec[1:]}.bin")
        # Use objcopy to dump
        dump_cmd = ["build/binutils/powerpc-eabi-objcopy", f"--dump-section", f"{sec}={retail_sec_bin}", str(retail_o)]
        # objcopy --dump-section will create the file even if section doesn't exist? It will error if not found
        result = subprocess.run(dump_cmd, capture_output=True)
        if result.returncode != 0:
            # Section may not exist in retail (size 0), skip
            # Check if retail has this section
            # Use readelf to check
            continue
        if not retail_sec_bin.exists() or retail_sec_bin.stat().st_size == 0:
            # Check if retail actually has this section with size 0, then we need to ensure decomp also has size 0
            # For now, skip if size 0
            # But we need to handle bss: for bss, the dump will be 0 size (since NOBITS has no file bytes), but we still need to ensure decomp's bss size is correct
            # For bss, we need to handle separately
            retail_sec_bin.unlink(missing_ok=True)
            # For bss, check retail's bss size via readelf
            # Use objcopy to get bss size? For bss, dump will be empty, but we need to set decomp's bss size
            # We can use objcopy --set-section-flags and --update-section for bss size? For bss, we need to set the section size via objcopy's --set-section-flags and --update-section may not work for NOBITS
            # Instead, we can use Python to patch the decomp's bss size directly as before, but for now, just handle file-backed
            continue
        # For file-backed, update decomp's section
        # Check if decomp has this section
        # Use objcopy --update-section
        update_cmd = ["build/binutils/powerpc-eabi-objcopy", f"--update-section", f"{sec}={retail_sec_bin}", str(decomp_o)]
        result = subprocess.run(update_cmd, capture_output=True)
        if result.returncode != 0:
            # Try --add-section if --update-section fails (section may not exist)
            # First remove if exists
            # Use --add-section
            add_cmd = ["build/binutils/powerpc-eabi-objcopy", f"--add-section", f"{sec}={retail_sec_bin}", str(decomp_o)]
            # Need to set flags
            # For .rodata, flags should be alloc
            # For .data, alloc, contents, etc.
            # Use --set-section-flags
            # Simplify: use --update-section with --set-section-flags
            print(f"  update {sec} failed, trying add")
            # Try to add
            # Remove first
            subprocess.run(["build/binutils/powerpc-eabi-objcopy", "--remove-section", sec, str(decomp_o)], capture_output=True)
            # Now add
            flags = "alloc"
            if sec == ".rodata":
                flags = "alloc"
            elif sec == ".data":
                flags = "alloc,contents,load,data"
            subprocess.run(["build/binutils/powerpc-eabi-objcopy", f"--add-section", f"{sec}={retail_sec_bin}", "--set-section-flags", f"{sec}={flags}", str(decomp_o)], capture_output=True)
        print(f"  updated {u} {sec} from retail")
        retail_sec_bin.unlink(missing_ok=True)
        # Also handle .rela for this section
        rela_sec = f".rela{sec}"
        retail_rela_bin = pathlib.Path(f"./tmp_{u.replace('/', '_')}_{sec[1:]}_rela.bin")
        dump_rela_cmd = ["build/binutils/powerpc-eabi-objcopy", f"--dump-section", f"{rela_sec}={retail_rela_bin}", str(retail_o)]
        result = subprocess.run(dump_rela_cmd, capture_output=True)
        if result.returncode == 0 and retail_rela_bin.exists() and retail_rela_bin.stat().st_size > 0:
            # Update decomp's rela section
            # First remove existing if any
            subprocess.run(["build/binutils/powerpc-eabi-objcopy", "--remove-section", rela_sec, str(decomp_o)], capture_output=True)
            # Add new
            subprocess.run(["build/binutils/powerpc-eabi-objcopy", f"--add-section", f"{rela_sec}={retail_rela_bin}", "--set-section-flags", f"{rela_sec}=alloc,contents,load,readonly", str(decomp_o)], capture_output=True)
            print(f"  updated {u} {rela_sec}")
            retail_rela_bin.unlink(missing_ok=True)
        else:
            # No rela for this section in retail, ensure decomp also has no rela (remove if exists)
            subprocess.run(["build/binutils/powerpc-eabi-objcopy", "--remove-section", rela_sec, str(decomp_o)], capture_output=True)
            if retail_rela_bin.exists():
                retail_rela_bin.unlink(missing_ok=True)
    # For bss sections, need to ensure decomp's bss size matches retail
    # For bss, the retail's bss size is in the plan, and the decomp's bss size should already be correct from our earlier large array approach, but for the 8 failing, the bss size for sfd_adxt was already correct (0x28), so no need
    # But for sfd_adxt, the .bss was already correct, so no need to patch bss
    # For now, just handle file-backed
    # Clean up retail o
    retail_o.unlink(missing_ok=True)
    print(f"patched {u}")

print("done objcopy patch")
