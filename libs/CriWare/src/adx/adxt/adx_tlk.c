#include <harness_catalog.h>
#include "libs/CriWare/src/adx/adxt/adx_tlk.h"

__declspec(section ".data") __attribute__((aligned(8))) const unsigned char adx_tlk_data[0x8] = {0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x01};
/* Retail .rodata layout: prefix + typed 2^52 magic at +0x8 + suffix.
 * Plain (float)(s32) sites keep FULL (fsubs); MWCC still pools a duplicate
 * @N at the tail - drop_data_tail + retarget cover that (+8). Typed local
 * D8 (value-numbering) alone does not kill the pool. */
__declspec(section ".rodata") __attribute__((aligned(8))) const unsigned char adx_tlk_rodata_pre[0x8] = {
    0x3F,0x59,0x99,0x9A,0x00,0x00,0x00,0x00,
};
__declspec(section ".rodata") __attribute__((aligned(8))) const f64 lbl_eu_805162D8 = 4503601774854144.0;
__declspec(section ".rodata") __attribute__((aligned(8))) const unsigned char adx_tlk_rodata_suf[0x7A8] = {
    0x00,0x00,0x00,0x00,0x44,0x7A,0x00,0x00,0x42,0x70,0x00,0x00,0xC2,0x70,0x00,0x00,
    0x42,0xC8,0x00,0x00,0xBF,0x80,0x00,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x30,
    0x34,0x20,0x61,0x64,0x78,0x74,0x5F,0x43,0x72,0x65,0x61,0x74,0x65,0x3A,0x20,0x70,
    0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,
    0x30,0x33,0x31,0x30,0x30,0x38,0x30,0x31,0x20,0x61,0x64,0x78,0x74,0x5F,0x43,0x72,
    0x65,0x61,0x74,0x65,0x3A,0x20,0x54,0x6F,0x6F,0x20,0x6D,0x61,0x6E,0x79,0x20,0x68,
    0x61,0x6E,0x64,0x6C,0x65,0x73,0x2E,0x00,0x45,0x30,0x36,0x30,0x32,0x32,0x32,0x30,
    0x31,0x20,0x61,0x64,0x78,0x74,0x5F,0x43,0x72,0x65,0x61,0x74,0x65,0x3A,0x20,0x57,
    0x6F,0x72,0x6B,0x20,0x73,0x69,0x7A,0x65,0x20,0x69,0x73,0x20,0x74,0x6F,0x6F,0x20,
    0x73,0x68,0x6F,0x72,0x74,0x2E,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x30,0x35,
    0x20,0x61,0x64,0x78,0x74,0x5F,0x44,0x65,0x73,0x74,0x72,0x6F,0x79,0x3A,0x20,0x70,
    0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,
    0x30,0x32,0x30,0x38,0x30,0x38,0x31,0x33,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x74,
    0x6F,0x70,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,
    0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x31,0x32,0x20,0x61,0x64,
    0x78,0x74,0x5F,0x53,0x74,0x61,0x72,0x74,0x53,0x6A,0x3A,0x20,0x70,0x61,0x72,0x61,
    0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,
    0x38,0x30,0x38,0x31,0x34,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x53,0x74,
    0x61,0x74,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,
    0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x37,0x20,0x61,0x64,
    0x78,0x74,0x5F,0x47,0x65,0x74,0x4E,0x75,0x6D,0x53,0x6D,0x70,0x6C,0x4F,0x62,0x75,
    0x66,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,
    0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x31,0x35,0x20,0x61,0x64,0x78,
    0x74,0x5F,0x47,0x65,0x74,0x54,0x69,0x6D,0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,
    0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,
    0x30,0x38,0x31,0x37,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x4E,0x75,0x6D,
    0x53,0x6D,0x70,0x6C,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,
    0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x31,0x39,0x20,
    0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x53,0x66,0x72,0x65,0x71,0x3A,0x20,0x70,
    0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,
    0x30,0x32,0x30,0x38,0x30,0x38,0x32,0x30,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,0x65,
    0x74,0x4E,0x75,0x6D,0x43,0x68,0x61,0x6E,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,
    0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,
    0x38,0x32,0x32,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x48,0x64,0x72,0x4C,
    0x65,0x6E,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,
    0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x32,0x31,0x20,0x61,0x64,
    0x78,0x74,0x5F,0x47,0x65,0x74,0x46,0x6D,0x74,0x42,0x70,0x73,0x3A,0x20,0x70,0x61,
    0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,
    0x32,0x30,0x38,0x30,0x38,0x32,0x35,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,0x74,
    0x4F,0x75,0x74,0x50,0x61,0x6E,0x3A,0x20,0x4E,0x75,0x6C,0x6C,0x20,0x70,0x6F,0x69,
    0x6E,0x74,0x65,0x72,0x20,0x69,0x73,0x20,0x70,0x61,0x73,0x73,0x65,0x64,0x00,0x45,
    0x30,0x37,0x30,0x34,0x31,0x38,0x30,0x31,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,
    0x74,0x4F,0x75,0x74,0x50,0x61,0x6E,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,
    0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x38,0x31,0x30,0x31,0x32,0x30,
    0x38,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,0x74,0x4F,0x75,0x74,0x50,0x61,0x6E,
    0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,
    0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x32,0x36,0x20,0x61,0x64,0x78,0x74,
    0x5F,0x47,0x65,0x74,0x4F,0x75,0x74,0x50,0x61,0x6E,0x3A,0x20,0x70,0x61,0x72,0x61,
    0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,
    0x38,0x30,0x38,0x37,0x30,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,0x74,0x4F,0x75,
    0x74,0x42,0x61,0x6C,0x61,0x6E,0x63,0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,
    0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,
    0x38,0x37,0x31,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x4F,0x75,0x74,0x42,
    0x61,0x6C,0x61,0x6E,0x63,0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,
    0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x32,
    0x33,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,0x74,0x4F,0x75,0x74,0x56,0x6F,0x6C,
    0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,
    0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x32,0x34,0x20,0x61,0x64,0x78,0x74,
    0x5F,0x47,0x65,0x74,0x4F,0x75,0x74,0x56,0x6F,0x6C,0x3A,0x20,0x70,0x61,0x72,0x61,
    0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,
    0x38,0x30,0x38,0x34,0x30,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,0x74,0x53,0x76,
    0x72,0x46,0x72,0x65,0x71,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,
    0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x38,
    0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,0x74,0x52,0x65,0x6C,0x6F,0x61,0x64,0x54,
    0x69,0x6D,0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,
    0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x33,0x31,0x31,0x31,0x35,0x30,0x31,0x20,0x61,
    0x64,0x78,0x74,0x5F,0x52,0x65,0x73,0x65,0x74,0x52,0x65,0x6C,0x6F,0x61,0x64,0x54,
    0x69,0x6D,0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,
    0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x39,0x20,0x61,
    0x64,0x78,0x74,0x5F,0x53,0x65,0x74,0x52,0x65,0x6C,0x6F,0x61,0x64,0x53,0x63,0x74,
    0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,
    0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x34,0x20,0x61,0x64,0x78,0x74,
    0x5F,0x47,0x65,0x74,0x4E,0x75,0x6D,0x53,0x63,0x74,0x49,0x62,0x75,0x66,0x3A,0x20,
    0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,
    0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x35,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,
    0x65,0x74,0x49,0x62,0x75,0x66,0x52,0x65,0x6D,0x61,0x69,0x6E,0x54,0x69,0x6D,0x65,
    0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,
    0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x36,0x20,0x41,0x44,0x58,0x54,
    0x5F,0x49,0x73,0x49,0x62,0x75,0x66,0x53,0x61,0x66,0x65,0x74,0x79,0x3A,0x20,0x70,
    0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,
    0x30,0x32,0x30,0x38,0x30,0x38,0x30,0x32,0x20,0x61,0x64,0x78,0x74,0x5F,0x49,0x73,
    0x43,0x6F,0x6D,0x70,0x6C,0x65,0x74,0x65,0x64,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,
    0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,
    0x30,0x38,0x34,0x33,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x45,0x72,0x72,
    0x43,0x6F,0x64,0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,
    0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x34,0x34,0x20,
    0x61,0x64,0x78,0x74,0x5F,0x43,0x6C,0x65,0x61,0x72,0x45,0x72,0x72,0x43,0x6F,0x64,
    0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,
    0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x32,0x39,0x20,0x61,0x64,0x78,
    0x74,0x5F,0x47,0x65,0x74,0x4C,0x70,0x43,0x6E,0x74,0x3A,0x20,0x70,0x61,0x72,0x61,
    0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,
    0x38,0x30,0x38,0x32,0x38,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,0x74,0x4C,0x70,
    0x46,0x6C,0x67,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,
    0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x33,0x20,0x61,
    0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x49,0x6E,0x70,0x75,0x74,0x53,0x6A,0x3A,0x20,
    0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,
    0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x30,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,
    0x65,0x74,0x57,0x61,0x69,0x74,0x50,0x6C,0x61,0x79,0x53,0x74,0x61,0x72,0x74,0x3A,
    0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,
    0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x33,0x31,0x20,0x61,0x64,0x78,0x74,0x5F,
    0x49,0x73,0x52,0x65,0x61,0x64,0x79,0x50,0x6C,0x61,0x79,0x53,0x74,0x61,0x72,0x74,
    0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,
    0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x34,0x36,0x20,0x61,0x64,0x78,0x74,
    0x5F,0x50,0x61,0x75,0x73,0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,
    0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x34,
    0x37,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x53,0x74,0x61,0x74,0x50,0x61,
    0x75,0x73,0x65,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,
    0x72,0x72,0x6F,0x72,0x00,0x45,0x32,0x30,0x30,0x35,0x31,0x31,0x31,0x36,0x30,0x31,
    0x20,0x3A,0x20,0x4E,0x55,0x4C,0x4C,0x20,0x70,0x6F,0x69,0x6E,0x74,0x65,0x72,0x20,
    0x69,0x73,0x20,0x70,0x61,0x73,0x73,0x65,0x64,0x2E,0x00,0x45,0x32,0x30,0x30,0x36,
    0x31,0x31,0x30,0x32,0x30,0x30,0x3A,0x20,0x49,0x6E,0x76,0x61,0x6C,0x69,0x64,0x20,
    0x76,0x61,0x6C,0x75,0x65,0x73,0x2E,0x00,0x45,0x30,0x32,0x30,0x38,0x30,0x38,0x31,
    0x38,0x20,0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x44,0x65,0x63,0x4E,0x75,0x6D,
    0x53,0x6D,0x70,0x6C,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,
    0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x34,0x30,0x34,0x31,0x39,0x30,0x31,0x20,
    0x61,0x64,0x78,0x74,0x5F,0x47,0x65,0x74,0x44,0x65,0x63,0x44,0x74,0x4C,0x65,0x6E,
    0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,
    0x72,0x00,0x45,0x30,0x34,0x30,0x34,0x31,0x39,0x30,0x32,0x20,0x41,0x44,0x58,0x54,
    0x5F,0x53,0x65,0x74,0x43,0x62,0x44,0x65,0x63,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,
    0x65,0x74,0x65,0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,0x45,0x30,0x32,0x30,0x38,
    0x30,0x38,0x36,0x30,0x20,0x61,0x64,0x78,0x74,0x5F,0x53,0x65,0x74,0x4B,0x65,0x79,
    0x53,0x74,0x72,0x69,0x6E,0x67,0x3A,0x20,0x70,0x61,0x72,0x61,0x6D,0x65,0x74,0x65,
    0x72,0x20,0x65,0x72,0x72,0x6F,0x72,0x00,
};



/* .bss objects are defined after the code; see the bottom of this file. */

typedef void (*ADX_VFN1)(void*);
typedef s32 (*ADX_VFN2)(void*, s32);

extern void ADXCRS_Enter(void);
extern void ADXCRS_Leave(void);
extern void ADXCRS_Lock(void);
extern void ADXCRS_Unlock(void);
extern void ADXERR_CallErrFunc1_(const char*);
extern char adxt_mes[];
extern u8 lbl_eu_805E26E8[];

extern void ADXSTM_ReleaseFileNw(void*);
extern void ADXSTM_SetBufSize(void*, u32, u32);
extern void ADXSTM_SetEos(void*, void*);
extern void ADXSTM_EntryEosFunc(void*, void*, void*);
extern void ADXSTM_Seek(void*, s32);
extern void ADXSTM_StopNw(void*);
extern void ADXSTM_BindFileNw(void*, void*, void*, void*, u32, u32);
extern void ADXSTM_Start(void*);
extern void LSC_Stop(void*);
extern void ADXAMP_Stop(void*);
extern void ADXAMP_Start(void*);
extern void ADXSJD_SetInSj(void*, void*);
extern void ADXSJD_Start(void*);
extern u32 ADXSJD_GetSfreq(void*);
extern u32 ADXSJD_GetTotalNumSmpl(void*);
extern u32 ADXSJD_GetNumChan(void*);
extern u32 ADXSJD_GetDecNumSmpl(void*);
extern u32 ADXSJD_GetOutBps(void*);
extern u32 ADXSJD_GetDecDtLen(void*);
extern u32 ADXSJD_GetLpStartOfst(void*);
extern u32 ADXSJD_GetLpEndOfst(void*);
extern u32 ADXSJD_GetBlkSmpl(void*);
extern u32 ADXSJD_GetNumLoop(void*);
extern u32 ADXSJD_GetStat(void*);
extern void ADXSJD_Stop(void*);
extern s16 ADXSJD_GetDefPan(void*, s32);
extern void ADXSJD_SetLnkSw(void*, u32);
extern void ADXSJD_SetMaxDecSmpl(void*, u32);
extern void ADXSJD_TermSupply(void*);
extern void ADXSJD_EntryFltFunc(void*, void*, void*);
extern void ADXSJD_ExecServer(void);
extern u32 ADXRNA_GetNumData(void*);
extern u32 ADXRNA_GetNumRoom(void*);
extern void ADXRNA_Destroy(void*);
extern void ADXSJD_Destroy(void*);
extern void ADXSTM_Destroy(void*);
extern s32 ADXSTM_GetFileSct(void*);
extern s32 ADXSTM_Tell(void*);
extern void LSC_Destroy(void*);
extern void ADXAMP_Destroy(void*);
extern u32 ADXRNA_DiscardData(void*, u32);
extern void ADXRNA_SetTransSw(void*, s32);
extern void ADXRNA_SetPlaySw(void*, s32);
extern void ADXRNA_SetOutVol(void*, s32);
extern void ADXRNA_SetOutPan(void*, s32, s32);
extern u32 lbl_eu_805E26D4;
extern void ADXRNA_ExecServer(void);
extern u32 ADXT_ExecHndl(void*);
extern void SJ_SplitChunk(void*, u32, void*, void*);
extern u32 __cvt_fp2unsigned(float);
extern u32 lbl_eu_805E26DC;
extern void (*lbl_eu_805E4F20)();
extern void (*ADXT_DestroyFunc)(void*);
extern u32 ADXT_TimeGuard[2];
extern u32 ADXT_DefSvrFreq;
extern u32 ADXT_SvrFreq;
typedef union ADXT_F32U32_ { u32 u; f32 f; } ADXT_F32U32_;
extern ADXT_F32U32_ ADXT_SvrFreqTable[0x12];
extern u32 ADXT_RecalcSw;
extern u32 ADXT_EosMode;
/* rodata constant pool used by the adxt time/sfreq helpers */
extern f32 lbl_eu_805162D0;
extern f32 lbl_eu_805162F0;
extern s32 ADX_DecodeInfo(const u8* info, s32 size, s16* outA, u8* outB, u8* outC,
                         u8* outD, u8* outE, u32* outF, u32* outG, u32* outH);
extern void* memset(void*, int, u32);
extern void* SJRBF_Create(void* pool_mem, u32 buf_size, u32 xtr_size);
extern void* ADXSTM_Create(void* fileName, int type);
extern void* ADXSJD_Create(void* stream, s32 numChan, void* sjArray);
extern void* ADXRNA_Create(void* sjArray, s32 numChan, void* work);
extern void* LSC_Create(void* handle);
extern void LSC_SetStmHndl(void* entry, void* stm);

void* adxt_Create(s32 numChan, void* work, s32 workEnd);

typedef struct ADXT_ {
    u8 _pad0[0x38];
    s32 sfreq; /* 0x38 */
    u8 _pad1[0x48 - 0x3C];
    s32 maxDec; /* 0x48 */
} ADXT_;

/* Overlay over the read-only literal pool at lbl_eu_805162D0 */
typedef struct ADXT_CST_ {
    f32 sectorScale; /* 0x00 */
    f32 _pad1;       /* 0x04 */
    f64 conv;        /* 0x08: 2^52 bias for the int->double conversion trick */
    f32 zero;        /* 0x10 */
    f32 thousand;    /* 0x14 */
    f32 limitHi;     /* 0x18 */
    f32 limitLo;     /* 0x1C */
} ADXT_CST_;

/* Materialize the 0x43300000_<lo> bias double from explicit halfwords;
 * MWCC lowers this to the retail lis/stw + lfd stack-double shape instead of
 * pooling a local 2^52 constant. */
static inline f64 ADXT_BiasDouble(u32 lo) {
    union {
        u32 w[2];
        f64 d;
    } t;
    t.w[0] = 0x43300000u;
    t.w[1] = lo;
    return t.d;
}

/* Signed int -> float via the 0x4330-bias double trick: flip the sign bit,
 * build 2^52 + flipped bits as a raw double, then subtract the shared
 * hi-magic constant at lbl_eu_805162D8 (== cst->conv) to recover the value.
 * (cst is kept for call-site compatibility; the constant is read through its
 * retail symbol so MWCC uses absolute lis/addi addressing like retail.) */
#define ADXT_S2F(cst, v) \
    ((f32)(ADXT_BiasDouble((u32)(v) ^ 0x80000000u) - lbl_eu_805162D8))

/* Full adxt handle (0xC4 bytes, table at lbl_eu_805E26E8) */

typedef struct SJ_CHUNK_ {
    u8* ptr;
    s32 size;
} SJ_CHUNK_;

void adxt_SetLpFlg(ADXT_OBJ_* self, u32 flg);
void adxt_start_sj(ADXT_OBJ_* self, void* sj);
void adxt_start_stm(ADXT_OBJ_* self, void* fname, void* sctOfst, void* sctLen, s32 numChan);
void adxt_StopWithoutLsc(void* self);
void adxt_Destroy(void* self);
void adxt_GetTime(void* self, u32* time, u32* sfreq);
void adxt_SetOutPan(ADXT_OBJ_* self, s32 ch, s32 pan);
void adxt_ExecServer(void);

// Build a playback handle in the first free slot of the 16-entry handle table.
// Layout: main SJ ring buffer, per-channel SJ ring buffers, SJD decoder,
// RNA transfer manager and LSC link, all carved out of the caller's work area.
void* adxt_Create(s32 numChan, void* work, s32 workEnd) {
    ADXT_OBJ_* h;
    s32 idx;
    s32 i;
    u8* p;
    u8* aligned = (u8*)(((u32)work + 0x3F) & ~0x3Fu);
    s32 pad = (s32)((u8*)aligned - (u8*)work);
    s32 end = workEnd - pad;

    if (numChan < 0 || work == NULL || workEnd < 0) {
        ADXERR_CallErrFunc1_(adxt_mes);
        return NULL;
    }
    p = lbl_eu_805E26E8;
    for (idx = 0; idx < 16; idx++) {
        if ((s8)*p == 0) {
            break;
        }
        p += 0xC4;
    }
    if (idx == 16) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x27);
        return NULL;
    }
    h = (ADXT_OBJ_*)p;
    memset(h, 0, 0xC4);
    /* carve the work area: per-channel blocks then the shared region */
    h->workBase = aligned + numChan * 0x60C0;
    h->workSize = (u32)(((end - numChan * 0x60C0) - 0x124) / 0x800) << 11;
    if ((s32)h->workSize < 0) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x50);
        return NULL;
    }
    h->f28 = 0x24;
    h->sjTail = h->workBase + h->workSize + 0x24;
    h->alignedWork = aligned;
    h->f30 = 0x2000;
    h->f34 = 0x2060;
    h->inSj = NULL;
    h->mainSj = SJRBF_Create(h->workBase, h->workSize, 0x24);
    if (h->mainSj == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    h->stm = ADXSTM_Create(NULL, 0);
    if (h->stm == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    for (i = 0; i < numChan; i++) {
        h->chans[i] = SJRBF_Create(h->alignedWork + h->f34 * 2 * i, h->f30 * 2,
                                   (h->f34 - h->f30) * 2);
        if (h->chans[i] == NULL) {
            adxt_Destroy(h);
            return NULL;
        }
    }
    h->sjd = ADXSJD_Create(h->mainSj, numChan, h->chans);
    if (h->sjd == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    h->rna = ADXRNA_Create(h->chans, numChan, aligned + numChan * 0x40C0);
    if (h->rna == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    h->lsc = LSC_Create(h->mainSj);
    if (h->lsc == NULL) {
        adxt_Destroy(h);
        return NULL;
    }
    LSC_SetStmHndl(h->lsc, h->stm);
    ADXCRS_Lock();
    h->svrFreq = ADXT_DefSvrFreq;
    h->numSector = (s16)((s32)h->workSize / 0x800);
    h->sectorSize = (s16)(lbl_eu_805162D0 *
                          (f32)(ADXT_BiasDouble((u32)(s32)h->numSector ^
                                                0x80000000u) -
                                lbl_eu_805162D8));
    h->f40 = 0;
    for (i = 0; i < numChan; i++) {
        h->pan[i] = -128;
    }
    h->f46 = 0;
    h->lpFlg = 1;
    h->f54 = 0;
    h->f58 = 0;
    h->f5C = 0;
    h->f60 = 0;
    h->f64 = 0;
    h->f68 = 0;
    h->f6A = 0;
    h->f6D = 1;
    h->pauseFlg = 0;
    h->timeOfst = 0;
    h->f98 = 0;
    if (h->sjd != NULL) {
        ADXSJD_SetLnkSw(h->sjd, 0);
    }
    h->volSw = 1;
    h->active = 1;
    ADXCRS_Unlock();
    return h;
}

void* ADXT_Create(void* a, void* b, s32 c) {
    void* ret;
    ADXCRS_Enter();
    ret = adxt_Create((s32)a, b, c);
    ADXCRS_Leave();
    return ret;
}

// Tear down a handle: optional user callbacks, stop + release children,
// per-channel object cleanup, then wipe the whole 0xC4-byte handle.
// The channel loop walks the handle word-by-word (slots 0x18/0x78/0x80 are
// parallel per-channel arrays), like retail.
void adxt_Destroy(void* self) {
    ADXT_OBJ_* h = (ADXT_OBJ_*)self;
    void** slot;
    s32 i;
    void* p;
    void* obj;
    s32 act;
    s32 mode;

    if (h == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x7F);
        return;
    }
    {
        /* u32-local copy keeps the @ha base out of r3 like retail */
        u32 fn = *(u32*)&lbl_eu_805E4F20;
        if (fn != 0) {
            ((void (*)(void))fn)();
        }
    }
    if (ADXT_DestroyFunc != NULL) {
        ADXT_DestroyFunc(h);
    }
    /* signed-int temps keep retail's lbz+cmpi shape (no extsb/cmpli) */
    act = h->active;
    if (act == 1) {
        if (h == NULL) {
            ADXERR_CallErrFunc1_(adxt_mes + 0xA7);
        } else {
            obj = h->stm;
            if (obj != NULL) {
                ADXSTM_ReleaseFileNw(obj);
            }
            ADXCRS_Lock();
            mode = h->mode;
            if (mode == 4) {
                LSC_Stop(h->lsc);
                obj = h->inSj;
                if (obj != NULL) {
                    ((ADX_VFN1*)*(void**)obj)[5](obj);
                }
            }
            adxt_StopWithoutLsc(h);
            ADXCRS_Unlock();
        }
    }
    /* p doubles as each child temp and the per-channel word-walker, like retail */
    p = h->rna;
    if (p != NULL) {
        h->rna = NULL;
        ADXRNA_Destroy(p);
    }
    p = h->sjd;
    if (p != NULL) {
        h->sjd = NULL;
        ADXSJD_Destroy(p);
    }
    p = h->stm;
    if (p != NULL) {
        h->stm = NULL;
        ADXSTM_EntryEosFunc(p, NULL, NULL);
        ADXSTM_Destroy(p);
    }
    p = h->lsc;
    if (p != NULL) {
        h->lsc = NULL;
        LSC_Destroy(p);
    }
    ADXCRS_Lock();
    obj = h->mainSj;
    if (obj != NULL) {
        h->mainSj = NULL;
        ((ADX_VFN1*)*(void**)obj)[3](obj);
    }
    /* per-channel arrays at 0x18/0x78/0x80, walked with one advancing pointer */
    p = (void*)h;
    for (i = 0; i < (s32)(s8)h->numChan; i++) {
        obj = ((void**)p)[6];
        if (obj != NULL) {
            ((void**)p)[6] = NULL;
            ((ADX_VFN1*)*(void**)obj)[3](obj);
        }
        obj = ((void**)p)[0x1E];
        if (obj != NULL) {
            ((void**)p)[0x1E] = NULL;
            ((ADX_VFN1*)*(void**)obj)[3](obj);
        }
        obj = ((void**)p)[0x20];
        if (obj != NULL) {
            ((void**)p)[0x20] = NULL;
            ((ADX_VFN1*)*(void**)obj)[3](obj);
        }
        p = (void**)((u8*)p + 4);
    }
    p = h->f74_amp;
    if (p != NULL) {
        h->f74_amp = NULL;
        ADXAMP_Destroy(p);
    }
    memset(h, 0, 0xC4);
    h->active = 0;
    ADXCRS_Unlock();
}

void ADXT_Destroy(void* self) {
    ADXCRS_Enter();
    adxt_Destroy(self);
    ADXCRS_Leave();
}

void ADXT_DestroyAll() {
    s32 i;
    u8* p;
    ADXCRS_Enter();
    p = lbl_eu_805E26E8;
    for (i = 0; i < 16; i++) {
        if ((s8)*p == 1) {
            adxt_Destroy(p);
        }
        p += 0xC4;
    }
    ADXCRS_Leave();
}

void adxt_start_sj(ADXT_OBJ_* self, void* sj) {
    u8* p;
    s32 i;
    p = (u8*)self;
    for (i = 0; i < (s32)(s8)((u8*)self)[3]; i++) {
        void* obuf = *(void**)(p + 0x18);
        void** vtbl = *(void***)obuf;
        ((void (*)(void*))vtbl[5])(obuf);
        p += 4;
    }
    ADXSJD_SetInSj(*(void**)((u8*)self + 4), sj);
    self->inSj = sj;
    ADXSJD_Start(self->sjd);
    ((u8*)self)[1] = 1;
    self->f4C = 0;
    ((u8*)self)[0x71] = 0;
    self->f8C = 0x7FFFFFFF;
    self->f90 = (u32)-1;
    self->time = 0;
    self->fA4 = 0;
    self->tick = lbl_eu_805E26DC;
    self->lpStartOfs = 0;
    if ((s8)((u8*)self)[2] == 4) {
        ADXCRS_Enter();
        adxt_SetLpFlg(self, 0);
        ADXCRS_Leave();
    }
    if (self->f74_amp != NULL) {
        ADXAMP_Start(self->f74_amp);
    }
}

void adxt_start_stm(ADXT_OBJ_* self, void* fname, void* sctOfst, void* sctLen, s32 numChan) {
    u32 signExt;
    u32 shiftVal;
    ADXSTM_SetBufSize(self->stm,
                      (u32)self->sectorSize << 11,
                      (u32)self->numSector << 11);
    ADXSTM_SetEos(*(void**)((u8*)self + 8), (void*)ADXT_EosMode);
    ADXSTM_EntryEosFunc(*(void**)((u8*)self + 8), NULL, NULL);
    ADXSTM_Seek(*(void**)((u8*)self + 8), 0);
    ADXSTM_StopNw(*(void**)((u8*)self + 8));
    ADXSTM_ReleaseFileNw(*(void**)((u8*)self + 8));
    signExt = (u32)(numChan >> 31) << 11;
    shiftVal = __rlwimi(signExt, numChan, 11, 21, 31);
    ADXSTM_BindFileNw(*(void**)((u8*)self + 8), fname, sctOfst, sctLen,
                      shiftVal, numChan << 11);
    ADXSTM_Start(*(void**)((u8*)self + 8));
    adxt_start_sj(self, self->mainSj);
}

void adxt_Destroy(void* self);
void adxt_StopWithoutLsc(void* self);
void ADXT_StartSj(void* self, void* sj) {
    ADXCRS_Enter();
    if (self == NULL || sj == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0xCC);
    } else {
        if (self == NULL) {
            ADXERR_CallErrFunc1_(adxt_mes + 0xA7);
        } else {
            if (((void**)self)[2] != NULL) {
                ADXSTM_ReleaseFileNw(((void**)self)[2]);
            }
            ADXCRS_Lock();
            if ((s8)((u8*)self)[2] == 4) {
                LSC_Stop(((void**)self)[0x25]);
                if (((void**)self)[5] != NULL) {
                    ADX_VFN1 fn = ((ADX_VFN1*)*(void**)((void**)self)[5])[5];
                    fn(((void**)self)[5]);
                }
            }
            adxt_StopWithoutLsc(self);
            ADXCRS_Unlock();
        }
        ADXCRS_Lock();
        ((u8*)self)[2] = 3;
        adxt_start_sj(self, sj);
        ((u8*)self)[0x98] = 1;
        if (((void**)self)[1] != NULL) {
            ADXSJD_SetLnkSw(((void**)self)[1], 1);
        }
        ADXCRS_Unlock();
    }
    ADXCRS_Leave();
}

void adxt_StopWithoutLsc(void* self) {
    ADXCRS_Lock();
    ADXRNA_SetTransSw(((void**)self)[3], 0);
    ADXRNA_SetPlaySw(((void**)self)[3], 0);
    ADXSJD_Stop(((void**)self)[1]);
    if ((s8)((u8*)self)[2] == 2) {
        if (((void**)self)[5] != NULL) {
            void* obj = ((void**)self)[5];
            ((void**)self)[5] = NULL;
            ((ADX_VFN1*)*(void**)obj)[3](obj);
        }
    }
    if (((void**)self)[0x1D] != NULL) {
        ADXAMP_Stop(((void**)self)[0x1D]);
    }
    ((void**)self)[5] = NULL;
    ((u8*)self)[1] = 0;
    ((u8*)self)[0xA8] = 0;
    ADXCRS_Unlock();
}

void ADXT_Stop(void* self) {
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0xA7);
    } else {
        if (((void**)self)[2] != NULL) {
            ADXSTM_ReleaseFileNw(((void**)self)[2]);
        }
        ADXCRS_Lock();
        if ((s8)((u8*)self)[2] == 4) {
            LSC_Stop(((void**)self)[0x25]);
            if (((void**)self)[5] != NULL) {
                ADX_VFN1 fn = ((ADX_VFN1*)*(void**)((void**)self)[5])[5];
                fn(((void**)self)[5]);
            }
        }
        adxt_StopWithoutLsc(self);
        ADXCRS_Unlock();
    }
    ADXCRS_Leave();
}

s32 ADXT_GetStat(void* self) {
    s32 stat;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0xF4);
        stat = -1;
    } else {
        stat = *(s8*)((u8*)self + 1);
    }
    ADXCRS_Leave();
    return stat;
}

void adxt_GetTimeSfreq2(void* self_, u32* time, u32* sfreq) {
    ADXT_OBJ_* self = (ADXT_OBJ_*)self_;
    u8 stat;
    u32 decNumSmpl;
    u32 obufNumSmpl;
    stat = self->stat;
    if ((u8)(stat - 3) <= 1) {
        /* playing: extrapolate the decode position */
        *sfreq = ADXSJD_GetSfreq(self->sjd);
        decNumSmpl = ADXSJD_GetDecNumSmpl(self->sjd);
        if (self == NULL) {
            ADXERR_CallErrFunc1_(adxt_mes + 0x11C);
            obufNumSmpl = (u32)-1;
        } else if (self->chans[0] != NULL) {
            void* ob = self->chans[0];
            s32 v = ((ADX_VFN2*)*(void**)ob)[9](ob, 1);
            obufNumSmpl = (u32)(((v < 0) + v) >> 1);
        } else {
            obufNumSmpl = 0;
        }
        /* NOTE: retail emits `add r3, r3, r28`; every source shape tried
         * (anon call left/right, named temps, compound +=, signedness,
         * declaration order) leaves this single commutative operand pair
         * swapped, or shifts the whole r0/r3 chain - known fixed-codegen
         * cap (cf. MWCC_CASES sfadxt_SetAdxtHd). */
        *time = self->fA4 + (decNumSmpl - (ADXRNA_GetNumData(self->rna) + obufNumSmpl));
    } else if ((s8)stat == 5) {
        /* finished: total samples scaled from output bit width */
        *time = ADXSJD_GetTotalNumSmpl(self->sjd);
        *sfreq = ADXSJD_GetSfreq(self->sjd);
        *time = *time * (16 / (s32)ADXSJD_GetOutBps(self->sjd));
        *time += self->fA4;
    } else {
        *time = 0;
        *sfreq = 1;
    }
    *time = *time + self->timeOfst;
    if ((s32)*time < 0) {
        *time = 0;
    }
}

void adxt_GetTime(void* self_, u32* time, u32* sfreq) {
    u8* self = (u8*)self_;
    u32 time2;
    u32 sfreq2;
    ADXT_CST_* cst = (ADXT_CST_*)&lbl_eu_805162D0;

    if (self == NULL || time == NULL || sfreq == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x14B);
        return;
    }
    if (ADXT_TimeGuard[0] == 0) {
        adxt_GetTimeSfreq2(self_, time, sfreq);
        return;
    }
    ADXT_SvrFreqTable[2].f = cst->zero;
    if ((u8)(((u8*)self)[1] - 3) <= 1) {
        /* decode-ahead extrapolation against the server clock */
        if ((s8)self[0x72] == 0) {
            *time = *(u32*)(self + 0x9C) +
                    (lbl_eu_805E26DC - *(u32*)(self + 0xA0)) * 100;
        } else {
            *time = *(u32*)(self + 0x9C);
        }
        adxt_GetTimeSfreq2(self_, &time2, &sfreq2);
        ADXT_SvrFreqTable[2].f = cst->thousand *
                   (ADXT_S2F(cst, time2) / ADXT_S2F(cst, sfreq2) -
                    ADXT_S2F(cst, *time) / ADXT_S2F(cst, ADXT_SvrFreqTable[0].u));
        if (ADXT_SvrFreqTable[2].f > cst->limitHi || ADXT_SvrFreqTable[2].f < cst->limitLo) {
            /* drift too large: resync the sample counter via a clean re-query */
            if (ADXT_RecalcSw == 1) {
                u32 savedActive = ADXT_TimeGuard[0];
                ADXT_TimeGuard[0] = 0;
                adxt_GetTime(self_, &time2, &sfreq2);
                ADXT_SvrFreqTable[1].u = ADXT_SvrFreqTable[1].u + 1;
                ADXT_TimeGuard[0] = savedActive;
            }
            *(u32*)(self + 0x9C) = __cvt_fp2unsigned(
                ADXT_S2F(cst, ADXT_SvrFreqTable[0].u) *
                (ADXT_S2F(cst, time2) / ADXT_S2F(cst, sfreq2)));
            *(u32*)(self + 0xA0) = lbl_eu_805E26DC;
        }
    } else if ((s8)((u8*)self)[1] == 5) {
        /* note: locals here are intentionally spilled to stack in retail */
        u32 totalSmpl;
        u32 sf;
        u32 tmp;
        totalSmpl = ADXSJD_GetTotalNumSmpl(*(void**)(self + 4));
        sf = ADXSJD_GetSfreq(*(void**)(self + 4));
        tmp = totalSmpl * (16 / (s32)ADXSJD_GetOutBps(*(void**)(self + 4)));
        *time = (u32)(s32)(ADXT_S2F(cst, ADXT_SvrFreqTable[0].u) *
                           (ADXT_S2F(cst, tmp) / ADXT_S2F(cst, sf)));
        *time = *time + 1 + *(u32*)(self + 0x9C);
    } else {
        *time = 0;
    }
    *time += *(u32*)(self + 0x88);
    *sfreq = ADXT_SvrFreqTable[0].u;
}

void ADXT_GetTime(void* a, void* b, void* c) {
    ADXCRS_Enter();
    adxt_GetTime(a, b, c);
    ADXCRS_Leave();
}

s32 ADXT_GetTimeReal(void* self) {
    u32 t2, t1;
    s32 ret;
    ADXCRS_Enter();
    adxt_GetTime(self, &t1, &t2);
    /* Builtin (float)(s32) keeps retail fsubs; ADXT_S2F emits fsub and loses FULL. */
    ret = (s32)((float)(s32)t1 / (float)(s32)t2 * lbl_eu_805162F0);
    ADXCRS_Leave();
    return ret;
}

s32 ADXT_GetNumSmpl(void* self) {
    s32 ret;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x173);
        ret = -1;
    } else if ((s8)((u8*)self)[1] >= 2) {
        ret = (s32)ADXSJD_GetTotalNumSmpl(((void**)self)[1]);
    } else {
        ret = 0;
    }
    ADXCRS_Leave();
    return ret;
}

s32 ADXT_GetSfreq(void* self) {
    s32 ret;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x19E);
        ret = -1;
    } else if ((s8)((u8*)self)[1] >= 2) {
        ret = (s32)ADXSJD_GetSfreq(((void**)self)[1]);
    } else {
        ret = 0;
    }
    ADXCRS_Leave();
    return ret;
}

s32 ADXT_GetNumChan(void* self) {
    s32 ret;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x1C7);
        ret = -1;
    } else if ((s8)((u8*)self)[1] >= 2) {
        ret = (s32)ADXSJD_GetNumChan(((void**)self)[1]);
    } else {
        ret = 0;
    }
    ADXCRS_Leave();
    return ret;
}

void adxt_SetOutPan(ADXT_OBJ_* self, s32 ch, s32 pan) {
    s32 defpan;
    s32 outpan;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x246);
        return;
    }
    if (ch != 0 && ch != 1) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x277);
        return;
    }
    if (pan != -128) {
        if (pan < -15) {
            pan = -15;
        } else if (pan > 15) {
            pan = 15;
        }
    }
    if ((s8)((u8*)self)[0xA9] == 1) {
        defpan = (s16)ADXSJD_GetDefPan(*(void**)((u8*)self + 4), ch);
        if (defpan == -128) {
            defpan = 0;
        }
    } else {
        defpan = 0;
    }
    if (lbl_eu_805E26D4 == 0) {
        if (pan == -128) {
            if ((s32)ADXSJD_GetNumChan(*(void**)((u8*)self + 4)) == 2) {
                outpan = (ch == 0) ? -15 : 15;
            } else {
                outpan = 0;
            }
            outpan += defpan;
        } else {
            outpan = pan + defpan;
        }
    } else {
        outpan = 0;
    }
    self->pan[ch] = (s16)pan;
    if (ch < (s32)(s8)((u8*)self)[3]) {
        ADXRNA_SetOutPan(self->rna, ch, outpan);
    } else {
        ADXERR_CallErrFunc1_(adxt_mes + 0x2A1);
    }
}

void ADXT_SetOutPan(void* self, s32 ch, s32 pan) {
    ADXCRS_Enter();
    adxt_SetOutPan(self, ch, pan);
    ADXCRS_Leave();
}

s32 ADXT_GetOutPan(ADXT_OBJ_* self, s32 ch) {
    s32 ret;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x2CA);
        ret = 0;
    } else {
        ret = self->pan[ch];
    }
    ADXCRS_Leave();
    return ret;
}

extern s16 ADXSJD_GetDefOutVol(void*);

void ADXT_SetOutVol(ADXT_OBJ_* self, s32 vol) {
    s32 defvol;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x350);
    } else {
        self->f40 = (s16)vol;
        if ((s8)((u8*)self)[0xA9] == 1) {
            defvol = ADXSJD_GetDefOutVol(self->sjd);
        } else {
            defvol = 0;
        }
        ADXRNA_SetOutVol(self->rna, self->f40 + defvol);
    }
    ADXCRS_Leave();
}

s32 ADXT_GetOutVol(ADXT_OBJ_* self) {
    s32 vol;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x37A);
        vol = 0;
    } else {
        vol = self->f40;
    }
    ADXCRS_Leave();
    return vol;
}

extern u32 ADXT_DefSvrFreq;
extern u32 ADXT_SvrFreq;

void ADXT_SetDefSvrFreq(void* self) {
    ADXCRS_Enter();
    ADXT_DefSvrFreq = (u32)self;
    ADXT_SvrFreq = (u32)self;
    ADXCRS_Leave();
}

void ADXT_SetSvrFreq(void* self, s32 freq) {
    ADXT_* adxt;
    void* sjd;
    s32 sfreq;
    s32 numLoop;
    s32 blkSmpl;
    s32 val;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x3A4);
    } else {
        adxt = (ADXT_*)self;
        adxt->sfreq = freq;
        ADXT_SvrFreq = (u32)freq;
        sjd = *(void**)((u8*)self + 4);
        if ((s32)ADXSJD_GetStat(sjd) == 2) {
            sfreq = (s32)ADXSJD_GetSfreq(sjd);
            numLoop = (s32)ADXSJD_GetNumLoop(sjd);
            if (numLoop > 0) {
                val = sfreq / adxt->sfreq;
                adxt->maxDec = val * 4 - val;
            } else {
                val = sfreq / adxt->sfreq;
                {
                    s32 x = val * 4 - val;
                    adxt->maxDec = ((x < 0) + x) >> 1;
                }
            }
            blkSmpl = (s32)ADXSJD_GetBlkSmpl(sjd);
            val = blkSmpl * 2;
            {
                s32 cur = adxt->maxDec;
                cur = (cur + val) / val * val;
                adxt->maxDec = cur;
            }
            ADXSJD_SetMaxDecSmpl(sjd, (u32)adxt->maxDec);
        }
    }
    ADXCRS_Leave();
}

s32 ADXT_GetNumSctIbuf(void* self) {
    s32 ret;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x45A);
        ret = -1;
    } else if (((void**)self)[5] != NULL) {
        ADX_VFN2 fn = ((ADX_VFN2*)*(void**)((void**)self)[5])[9];
        ret = fn(((void**)self)[5], 1) / 2048;
    } else {
        ret = 0;
    }
    return ret;
}

s32 ADXT_GetNumSmplObuf(void* self, s32 ch) {
    void* n;
    if (self == NULL || ch < 0) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x11C);
        return -1;
    }
    n = ((void**)self)[6 + ch];
    if (n != 0) {
        ADX_VFN2 fn = ((ADX_VFN2*)*(void**)n)[9];
        s32 v = fn((void*)n, 1);
        return ((v < 0) + v) >> 1;
    }
    return 0;
}

void ADXT_SetAutoRcvr(ADXT_OBJ_* self, u32 val) {
    ADXCRS_Enter();
    self->f6D = (u8)val;
    ADXCRS_Leave();
}

void ADXT_ExecServer(void) {
    ADXCRS_Enter();
    adxt_ExecServer();
    ADXCRS_Leave();
}

void adxt_ExecServer(void) {
    ADXT_SVR_* srv = &ADXT_ServerWork;
    u8* p;
    s32 i;
    ADXCRS_Lock();
    if (ADXT_TimeGuard[1] != 0) {
        ADXCRS_Unlock();
        return;
    }
    ADXT_TimeGuard[1] = 1;
    ADXCRS_Unlock();
    if (srv->callback0 != NULL) {
        srv->callback0(srv->callback0Arg);
    }
    ADXSJD_ExecServer();
    ADXT_TimeGuard[1] = 2;
    p = lbl_eu_805E26E8;
    for (i = 0; i < 16; i++) {
        if ((s8)*p == 1) {
            ADXT_ExecHndl(p);
        }
        p += 0xC4;
    }
    /* state transitions happen between the NULL checks and the calls;
     * volatile store pins the stw before the cmpwi like retail */
    {
        void (*fn)(void*) = srv->callback2;
        *(volatile u32*)&ADXT_TimeGuard[1] = 3;
        if (fn != NULL) {
            fn(srv->callback2Arg);
        }
    }
    ADXRNA_ExecServer();
    {
        void (*fn)(void*) = srv->callback3;
        if (fn != NULL) {
            fn(srv->callback3Arg);
        }
    }
    {
        void (*fn)(void*) = srv->callback1;
        *(volatile u32*)&ADXT_TimeGuard[1] = 0;
        if (fn != NULL) {
            fn(srv->callback1Arg);
        }
    }
}

s32 ADXT_GetErrCode(ADXT_OBJ_* self) {
    s32 err;
    ADXCRS_Enter();
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x513);
        err = -1;
    } else {
        err = self->f60;
    }
    ADXCRS_Leave();
    return err;
}

void adxt_SetLpFlg(ADXT_OBJ_* self, u32 flg) {
    s32 numData;
    s32 lpStart, lpEnd, lpSize;
    s32 decDtLen;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x594);
        return;
    }
    if (self->inSj == NULL) {
        numData = 0;
    } else {
        void** vtbl = *(void***)self->inSj;
        numData = ((s32 (*)(void*, s32))vtbl[9])(self->inSj, 1);
    }
    if ((s8)((u8*)self)[2] != 4 && (s8)((u8*)self)[0x6C] == 1 && flg == 0) {
        decDtLen = (s32)ADXSJD_GetDecDtLen(self->sjd);
        numData += decDtLen;
        lpStart = (s32)ADXSJD_GetLpStartOfst(self->sjd);
        {
            s32 rounded = (lpStart + 0x7FF) / 0x800;
            lpStart = rounded << 11;
        }
        lpEnd = (s32)ADXSJD_GetLpEndOfst(self->sjd);
        {
            s32 rounded = (lpEnd + 0x7FF) / 0x800;
            lpEnd = rounded << 11;
        }
        lpSize = lpEnd - lpStart;
        if (lpSize <= 0) {
            self->lpStartOfs = 0;
        } else {
            self->lpStartOfs = (u32)(lpStart + ((numData - lpStart) / lpSize) * lpSize);
        }
    }
    ((u8*)self)[0x6C] = (u8)flg;
}

void adxt_Pause(ADXT_OBJ_* self, int pause) {
    s32 stat;
    s32 sctRemain;
    s32 numData;
    u32 savedSvr;
    u32 time, sfreq;
    if (self == NULL) {
        ADXERR_CallErrFunc1_(adxt_mes + 0x64A);
        return;
    }
    stat = (s32)(s8)((u8*)self)[1];
    if (pause == (s32)(s8)((u8*)self)[0x72]) {
        return;
    }
    ADXCRS_Lock();
    ((u8*)self)[0x72] = (u8)pause;
    if ((u32)(stat - 3) > 1) {
        ADXCRS_Unlock();
        return;
    }
    if (pause == 1) {
        ADXRNA_SetPlaySw(self->rna, 0);
    } else if (((u8*)self)[0x73] == 1) {
        void* stm = self->stm;
        void* rna = self->rna;
        void* sjd = self->sjd;
        sctRemain = ADXSTM_GetFileSct(stm) - ADXSTM_Tell(stm);
        if (sctRemain > (s32)self->sectorSize) {
            sctRemain = (s32)self->sectorSize;
        }
        /* inline of ADXT_GetNumSctIbuf: sectors left in the input buffer */
        if (self == NULL) {
            ADXERR_CallErrFunc1_(adxt_mes + 0x45A);
            numData = -1;
        } else if (self->inSj != NULL) {
            void** vtbl = *(void***)self->inSj;
            numData = ((s32 (*)(void*, s32))vtbl[9])(self->inSj, 1) / 2048;
        } else {
            numData = 0;
        }
        if (numData > sctRemain) {
            /* inline of ADXT_GetNumSmplObuf for ch 0 */
            if (self == NULL) {
                ADXERR_CallErrFunc1_(adxt_mes + 0x11C);
                numData = -1;
            } else if (self->chans[0] != NULL) {
                void* ob = self->chans[0];
                numData = ((ADX_VFN2*)*(void**)ob)[9](ob, 1) / 2;
            } else {
                numData = 0;
            }
            if (numData > 0) {
                /* decode-ahead cap: min(maxDecSmpl, 0x2000) */
                s32 maxDec = self->maxDec;
                s32 limit = 0x2000;
                u32 blkSmpl;
                s32 numRoom;
                if (maxDec < 0x2000) {
                    limit = maxDec;
                }
                numData = ADXRNA_GetNumData(rna);
                if (numData < limit) {
                    blkSmpl = ADXSJD_GetBlkSmpl(sjd);
                    numRoom = (s32)ADXRNA_GetNumRoom(rna);
                    if (numRoom > (s32)blkSmpl && ADXSJD_GetStat(sjd) != 3) {
                        goto update_time; /* decoder not idle: skip resume */
                    }
                }
            }
        }
        if ((s8)((u8*)self)[0x72] == 0) {
            ADXRNA_SetPlaySw(rna, 1);
            ((u8*)self)[0x73] = 0;
            self->tick = lbl_eu_805E26DC;
        }
    } else {
        ADXRNA_SetPlaySw(self->rna, 1);
        self->tick = lbl_eu_805E26DC;
    }
update_time:
    /* refresh the sample-position estimate against the server clock */
    savedSvr = ADXT_TimeGuard[0];
    ADXT_TimeGuard[0] = 0;
    adxt_GetTime(self, &time, &sfreq);
    self->time =
        __cvt_fp2unsigned((float)(s32)ADXT_SvrFreqTable[0].u *
                          ((float)(s32)time / (float)(s32)sfreq));
    ADXT_TimeGuard[0] = savedSvr;
    ADXCRS_Unlock();
}

void ADXT_Pause(void* self, int pause) {
    ADXCRS_Enter();
    adxt_Pause(self, pause);
    ADXCRS_Leave();
}

void ADXT_SetTranspose(void* self) {
    ADXCRS_Enter();
    ADXCRS_Leave();
}

void ADXT_GetTranspose(void* self) {
    ADXCRS_Enter();
    ADXCRS_Leave();
}

void ADXT_TermSupply(void* self) {
    ADXCRS_Enter();
    ADXSJD_TermSupply(*(void**)((u8*)self + 4));
    ADXCRS_Leave();
}

void adxt_GetTime(void* self, u32* time, u32* sfreq);

s32 ADXT_DiscardSmpl(ADXT_OBJ_* self, u32 num) {
    u32 savedLock;
    u32 result;
    u32 sfreq, time;
    ADXCRS_Enter();
    if ((s8)((u8*)self)[0x72] == 0) {
        result = 0;
    } else {
        result = ADXRNA_DiscardData(self->rna, num);
        ADXCRS_Enter();
        adxt_ExecServer();
        ADXCRS_Leave();
        savedLock = ADXT_TimeGuard[0];
        ADXT_TimeGuard[0] = 0;
        adxt_GetTime(self, &time, &sfreq);
        ADXT_TimeGuard[0] = savedLock;
        self->time = __cvt_fp2unsigned(
            (float)(s32)time / (float)(s32)sfreq * (float)(s32)ADXT_SvrFreqTable[0].u);
        self->tick = lbl_eu_805E26DC;
    }
    ADXCRS_Leave();
    return result;
}

void ADXT_SetTimeOfst(ADXT_OBJ_* self, u32 val) {
    ADXCRS_Enter();
    self->timeOfst = val;
    ADXCRS_Leave();
}

void ADXT_SetLnkSw(ADXT_OBJ_* self, u32 val) {
    ADXCRS_Enter();
    self->f98 = (u8)val;
    if (self->sjd != NULL) {
        ADXSJD_SetLnkSw(self->sjd, val);
    }
    ADXCRS_Leave();
}

void ADXT_EntryFltFunc(void* this_, void* arg1, void* arg2) {
    ADXCRS_Enter();
    ADXSJD_EntryFltFunc(*(void**)((char*)this_ + 4), arg1, arg2);
    ADXCRS_Leave();
}

s32 ADXT_IsHeader(void* data, s32 size, s32* out) {
    s16 outA;
    u8 outB, outC, outD, outE;
    u32 outF, outG, outH;
    s16* pA;
    u8* pB;
    u8* pC;
    u8* pD;
    u8* pE;
    u32* pF;
    u32* pG;
    u32* pH;
    if (size < 2) return 0;
    if (*(u16*)data != 0x8000) return 0;
    pA = &outA;
    pB = &outB;
    pC = &outC;
    pD = &outD;
    pE = &outE;
    pF = &outF;
    pG = &outG;
    pH = &outH;
    if (ADX_DecodeInfo(data, size, pA, pB, pC, pD, pE, pF, pG, pH) < 0) {
        return 0;
    }
    *out = outA;
    return 1;
}

s32 ADXT_IsEndcode(void* self, s32 idx, u32* out) {
    if (idx < 2) return 0;
    if (*(u16*)self != 0x8001) return 0;
    *out = idx;
    return 1;
}

s32 adxt_InsertSilence(ADXT_OBJ_* self, s32 ch, s32 smpl) {
    s32 numBytes;
    void* sj;
    s32 chunkSize;
    s32 numBytes2;
    SJ_CHUNK_ data;
    SJ_CHUNK_ rest;
    sj = self->inSj;
    if (sj == NULL) {
        return 0;
    }
    chunkSize = ch * 0x12;
    {
        /* temp forces the divide pseudo to be created before chunkSize */
        s32 q = smpl / 32;
        numBytes = q * chunkSize;
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, s32, void**))vtbl[6])(sj, 0, numBytes, (void**)&data);
    }
    numBytes2 = (data.size / chunkSize) * chunkSize;
    memset(data.ptr, 0, numBytes2);
    SJ_SplitChunk(&data, numBytes2, &data, &rest);
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[8])(sj, 1, (void**)&data);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[7])(sj, 0, (void**)&rest);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, s32, void**))vtbl[6])(sj, 0, numBytes - numBytes2, (void**)&data);
    }
    numBytes = (data.size / chunkSize) * chunkSize;
    memset(data.ptr, 0, numBytes);
    SJ_SplitChunk(&data, numBytes, &data, &rest);
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[8])(sj, 1, (void**)&data);
    }
    {
        void** vtbl = *(void***)sj;
        ((void (*)(void*, s32, void**))vtbl[7])(sj, 0, (void**)&rest);
    }
    return ((numBytes2 + numBytes) / chunkSize) << 5;
}

s32 ADXT_InsertSilence(void* self, s32 ch, s32 smpl) {
    s32 ret;
    ADXCRS_Enter();
    ret = adxt_InsertSilence(self, ch, smpl);
    ADXCRS_Leave();
    return ret;
}

/* .bss: reverse retail order for MWCC emission. */
ADXT_F32U32_ ADXT_SvrFreqTable[0x12];
void (*ADXT_DestroyFunc)(void*);
void (*lbl_eu_805E4F20)();
u32 ADXT_SvrFreq;
u32 ADXT_DefSvrFreq;
u32 ADXT_TimeGuard[2];
ADXT_SVR_ ADXT_ServerWork;
