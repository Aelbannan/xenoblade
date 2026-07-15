from __future__ import annotations


# Special-purpose registers present in retail main.dol that are not represented
# by dedicated MachineState fields.  Names follow GekkoDisassembler.cpp and the
# MetroTRK ppc_reg.h definitions bundled with this repository.
AUX_SPR_NAMES: dict[int, str] = {
    18: "dsisr", 19: "dar", 22: "dec", 25: "sdr1",
    272: "sprg0", 273: "sprg1", 274: "sprg2", 275: "sprg3",
    282: "ear", 287: "pvr",
    528: "ibat0u", 529: "ibat0l", 530: "ibat1u", 531: "ibat1l",
    532: "ibat2u", 533: "ibat2l", 534: "ibat3u", 535: "ibat3l",
    536: "dbat0u", 537: "dbat0l", 538: "dbat1u", 539: "dbat1l",
    540: "dbat2u", 541: "dbat2l", 542: "dbat3u", 543: "dbat3l",
    560: "ibat4u", 561: "ibat4l", 562: "ibat5u", 563: "ibat5l",
    564: "ibat6u", 565: "ibat6l", 566: "ibat7u", 567: "ibat7l",
    568: "dbat4u", 569: "dbat4l", 570: "dbat5u", 571: "dbat5l",
    572: "dbat6u", 573: "dbat6l", 574: "dbat7u", 575: "dbat7l",
    920: "hid2", 921: "wpar", 922: "dmau", 923: "dmal",
    936: "ummcr0", 937: "upmc1", 938: "upmc2", 939: "usia",
    940: "ummcr1", 941: "upmc3", 942: "upmc4",
    952: "mmcr0", 953: "pmc1", 954: "pmc2", 955: "sia",
    956: "mmcr1", 957: "pmc3", 958: "pmc4",
    1008: "hid0", 1009: "hid1", 1010: "iabr", 1011: "hid4",
    1013: "dabr", 1017: "l2cr", 1019: "ictc",
}

AUX_SPR_NUMBERS = tuple(AUX_SPR_NAMES)
AUX_SPR_INDEX = {number: index for index, number in enumerate(AUX_SPR_NUMBERS)}
AUX_SPR_NAME_INDEX = {
    AUX_SPR_NAMES[number]: index for index, number in enumerate(AUX_SPR_NUMBERS)
}
AUX_SPR_OBSERVABLES = tuple(AUX_SPR_NAMES[number] for number in AUX_SPR_NUMBERS)

DIRECT_SPRS = frozenset({1, 8, 9, 26, 27, *range(912, 920)})
TIME_BASE_WRITE_SPRS = frozenset({284, 285})
READABLE_SPRS = frozenset({*DIRECT_SPRS, *AUX_SPR_NUMBERS})
WRITABLE_SPRS = frozenset({*READABLE_SPRS, *TIME_BASE_WRITE_SPRS})

SPR_HID2 = 920
SPR_HID0 = 1008
HID2_LCE = 0x10000000
HID0_DCE = 0x00004000


def auxiliary_spr_name(number: int) -> str:
    return AUX_SPR_NAMES[number]


def auxiliary_spr_index(number: int) -> int:
    return AUX_SPR_INDEX[number]
