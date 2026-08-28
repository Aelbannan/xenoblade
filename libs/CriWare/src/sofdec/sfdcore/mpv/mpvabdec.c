// CriWare Sofdec MPEG-2 block decoder (intra / non-intra / DC11).
// Reconstructed from retail (Sony SDL-derived "mpv" decoder).

#include <harness_catalog.h>

// Large section arrays for data diff - generated
__declspec(section ".data") __attribute__((aligned(8), used)) unsigned char __large_data_for_CriWare_src_sofdec_sfdcore_mpv_mp[0xC00] = {0x80, 0x3B, 0x52, 0xB8, 0x80, 0x3B, 0x51, 0xF4, 0x80, 0x3B, 0x51, 0x30, 0x80, 0x3B, 0x51, 0x30, 0x80, 0x3B, 0x50, 0x44, 0x80, 0x3B, 0x50, 0x44, 0x80, 0x3B, 0x50, 0x44, 0x80, 0x3B, 0x50, 0x44, 0x80, 0x3B, 0x4F, 0xD0, 0x80, 0x3B, 0x4F, 0x58, 0x80, 0x3B, 0x4E, 0xE4, 0x80, 0x3B, 0x4E, 0x6C, 0x80, 0x3B, 0x4D, 0xF8, 0x80, 0x3B, 0x4D, 0x80, 0x80, 0x3B, 0x4D, 0x0C, 0x80, 0x3B, 0x4C, 0x94, 0x80, 0x3B, 0x4C, 0x20, 0x80, 0x3B, 0x4C, 0x20, 0x80, 0x3B, 0x4B, 0xA8, 0x80, 0x3B, 0x4B, 0xA8, 0x80, 0x3B, 0x4B, 0x34, 0x80, 0x3B, 0x4B, 0x34, 0x80, 0x3B, 0x4A, 0xBC, 0x80, 0x3B, 0x4A, 0xBC, 0x80, 0x3B, 0x4A, 0x48, 0x80, 0x3B, 0x4A, 0x48, 0x80, 0x3B, 0x49, 0xD0, 0x80, 0x3B, 0x49, 0xD0, 0x80, 0x3B, 0x49, 0x5C, 0x80, 0x3B, 0x49, 0x5C, 0x80, 0x3B, 0x48, 0xE4, 0x80, 0x3B, 0x48, 0xE4, 0x80, 0x3B, 0x47, 0xB0, 0x80, 0x3B, 0x47, 0xB0, 0x80, 0x3B, 0x47, 0xB0, 0x80, 0x3B, 0x47, 0xB0, 0x80, 0x3B, 0x47, 0xB0, 0x80, 0x3B, 0x47, 0xB0, 0x80, 0x3B, 0x47, 0xB0, 0x80, 0x3B, 0x47, 0xB0, 0x80, 0x3B, 0x47, 0x3C, 0x80, 0x3B, 0x47, 0x3C, 0x80, 0x3B, 0x5D, 0x1C, 0x80, 0x3B, 0x47, 0x3C, 0x80, 0x3B, 0x46, 0xC4, 0x80, 0x3B, 0x46, 0xC4, 0x80, 0x3B, 0x5C, 0xA4, 0x80, 0x3B, 0x46, 0xC4, 0x80, 0x3B, 0x46, 0x50, 0x80, 0x3B, 0x46, 0x50, 0x80, 0x3B, 0x5C, 0x30, 0x80, 0x3B, 0x46, 0x50, 0x80, 0x3B, 0x45, 0xD8, 0x80, 0x3B, 0x45, 0xD8, 0x80, 0x3B, 0x5B, 0xB8, 0x80, 0x3B, 0x45, 0xD8, 0x80, 0x3B, 0x45, 0x64, 0x80, 0x3B, 0x45, 0x64, 0x80, 0x3B, 0x5B, 0x44, 0x80, 0x3B, 0x45, 0x64, 0x80, 0x3B, 0x44, 0xEC, 0x80, 0x3B, 0x44, 0xEC, 0x80, 0x3B, 0x5A, 0xCC, 0x80, 0x3B, 0x44, 0xEC, 0x80, 0x3B, 0x44, 0x78, 0x80, 0x3B, 0x44, 0x78, 0x80, 0x3B, 0x44, 0x78, 0x80, 0x3B, 0x44, 0x78, 0x80, 0x3B, 0x5A, 0x58, 0x80, 0x3B, 0x5A, 0x58, 0x80, 0x3B, 0x43, 0xB8, 0x80, 0x3B, 0x42, 0xF4, 0x80, 0x3B, 0x42, 0x7C, 0x80, 0x3B, 0x42, 0x7C, 0x80, 0x3B, 0x42, 0x7C, 0x80, 0x3B, 0x42, 0x7C, 0x80, 0x3B, 0x59, 0xE0, 0x80, 0x3B, 0x59, 0xE0, 0x80, 0x3B, 0x41, 0xB8, 0x80, 0x3B, 0x40, 0xF0, 0x80, 0x3B, 0x40, 0x7C, 0x80, 0x3B, 0x40, 0x7C, 0x80, 0x3B, 0x40, 0x7C, 0x80, 0x3B, 0x40, 0x7C, 0x80, 0x3B, 0x59, 0x6C, 0x80, 0x3B, 0x59, 0x6C, 0x80, 0x3B, 0x3F, 0xBC, 0x80, 0x3B, 0x3E, 0xF8, 0x80, 0x3B, 0x3E, 0x80, 0x80, 0x3B, 0x3E, 0x80, 0x80, 0x3B, 0x3E, 0x80, 0x80, 0x3B, 0x3E, 0x80, 0x80, 0x3B, 0x58, 0xF4, 0x80, 0x3B, 0x58, 0xF4, 0x80, 0x3B, 0x3D, 0xBC, 0x80, 0x3B, 0x3C, 0xF4, 0x80, 0x3B, 0x3C, 0x80, 0x80, 0x3B, 0x3C, 0x80, 0x80, 0x3B, 0x3C, 0x80, 0x80, 0x3B, 0x3C, 0x80, 0x80, 0x3B, 0x3C, 0x80, 0x80, 0x3B, 0x3C, 0x80, 0x80, 0x3B, 0x3B, 0xC0, 0x80, 0x3B, 0x3A, 0xFC, 0x80, 0x3B, 0x58, 0x80, 0x80, 0x3B, 0x58, 0x80, 0x80, 0x3B, 0x58, 0x80, 0x80, 0x3B, 0x58, 0x80, 0x80, 0x3B, 0x3A, 0x3C, 0x80, 0x3B, 0x3A, 0x3C, 0x80, 0x3B, 0x39, 0x78, 0x80, 0x3B, 0x39, 0x78, 0x80, 0x3B, 0x39, 0x00, 0x80, 0x3B, 0x39, 0x00, 0x80, 0x3B, 0x39, 0x00, 0x80, 0x3B, 0x39, 0x00, 0x80, 0x3B, 0x39, 0x00, 0x80, 0x3B, 0x39, 0x00, 0x80, 0x3B, 0x38, 0x3C, 0x80, 0x3B, 0x37, 0x74, 0x80, 0x3B, 0x58, 0x08, 0x80, 0x3B, 0x58, 0x08, 0x80, 0x3B, 0x58, 0x08, 0x80, 0x3B, 0x58, 0x08, 0x80, 0x3B, 0x36, 0xB0, 0x80, 0x3B, 0x36, 0xB0, 0x80, 0x3B, 0x35, 0xE8, 0x80, 0x3B, 0x35, 0xE8, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x57, 0xE0, 0x80, 0x3B, 0x35, 0x74, 0x80, 0x3B, 0x35, 0x74, 0x80, 0x3B, 0x35, 0x74, 0x80, 0x3B, 0x35, 0x74, 0x80, 0x3B, 0x35, 0x74, 0x80, 0x3B, 0x35, 0x74, 0x80, 0x3B, 0x35, 0x74, 0x80, 0x3B, 0x35, 0x74, 0x80, 0x3B, 0x34, 0xB4, 0x80, 0x3B, 0x33, 0xF0, 0x80, 0x3B, 0x33, 0x30, 0x80, 0x3B, 0x32, 0x6C, 0x80, 0x3B, 0x31, 0xAC, 0x80, 0x3B, 0x31, 0xAC, 0x80, 0x3B, 0x30, 0xE8, 0x80, 0x3B, 0x30, 0xE8, 0x80, 0x3B, 0x57, 0x6C, 0x80, 0x3B, 0x57, 0x6C, 0x80, 0x3B, 0x57, 0x6C, 0x80, 0x3B, 0x57, 0x6C, 0x80, 0x3B, 0x57, 0x6C, 0x80, 0x3B, 0x57, 0x6C, 0x80, 0x3B, 0x57, 0x6C, 0x80, 0x3B, 0x57, 0x6C, 0x80, 0x3B, 0x30, 0x28, 0x80, 0x3B, 0x30, 0x28, 0x80, 0x3B, 0x56, 0xAC, 0x80, 0x3B, 0x30, 0x28, 0x80, 0x3B, 0x2F, 0x64, 0x80, 0x3B, 0x2F, 0x64, 0x80, 0x3B, 0x55, 0xE8, 0x80, 0x3B, 0x2F, 0x64, 0x80, 0x3B, 0x2E, 0xEC, 0x80, 0x3B, 0x2E, 0xEC, 0x80, 0x3B, 0x2E, 0xEC, 0x80, 0x3B, 0x2E, 0xEC, 0x80, 0x3B, 0x2E, 0xEC, 0x80, 0x3B, 0x2E, 0xEC, 0x80, 0x3B, 0x2E, 0xEC, 0x80, 0x3B, 0x2E, 0xEC, 0x80, 0x3B, 0x2E, 0x28, 0x80, 0x3B, 0x2D, 0x60, 0x80, 0x3B, 0x2C, 0x9C, 0x80, 0x3B, 0x2B, 0xD4, 0x80, 0x3B, 0x2B, 0x10, 0x80, 0x3B, 0x2B, 0x10, 0x80, 0x3B, 0x2A, 0x48, 0x80, 0x3B, 0x2A, 0x48, 0x80, 0x3B, 0x55, 0x70, 0x80, 0x3B, 0x55, 0x70, 0x80, 0x3B, 0x55, 0x70, 0x80, 0x3B, 0x55, 0x70, 0x80, 0x3B, 0x55, 0x70, 0x80, 0x3B, 0x55, 0x70, 0x80, 0x3B, 0x55, 0x70, 0x80, 0x3B, 0x55, 0x70, 0x80, 0x3B, 0x29, 0x84, 0x80, 0x3B, 0x29, 0x84, 0x80, 0x3B, 0x54, 0xAC, 0x80, 0x3B, 0x29, 0x84, 0x80, 0x3B, 0x28, 0xBC, 0x80, 0x3B, 0x28, 0xBC, 0x80, 0x3B, 0x53, 0xE4, 0x80, 0x3B, 0x28, 0xBC, 0x80, 0x3B, 0x88, 0xD8, 0x80, 0x3B, 0x88, 0x14, 0x80, 0x3B, 0x87, 0x50, 0x80, 0x3B, 0x87, 0x50, 0x80, 0x3B, 0x86, 0x64, 0x80, 0x3B, 0x86, 0x64, 0x80, 0x3B, 0x86, 0x64, 0x80, 0x3B, 0x86, 0x64, 0x80, 0x3B, 0x85, 0xF0, 0x80, 0x3B, 0x85, 0x78, 0x80, 0x3B, 0x85, 0x04, 0x80, 0x3B, 0x84, 0x8C, 0x80, 0x3B, 0x84, 0x18, 0x80, 0x3B, 0x83, 0xA0, 0x80, 0x3B, 0x83, 0x2C, 0x80, 0x3B, 0x82, 0xB4, 0x80, 0x3B, 0x82, 0x40, 0x80, 0x3B, 0x82, 0x40, 0x80, 0x3B, 0x81, 0xC8, 0x80, 0x3B, 0x81, 0xC8, 0x80, 0x3B, 0x81, 0x54, 0x80, 0x3B, 0x81, 0x54, 0x80, 0x3B, 0x80, 0xDC, 0x80, 0x3B, 0x80, 0xDC, 0x80, 0x3B, 0x80, 0x68, 0x80, 0x3B, 0x80, 0x68, 0x80, 0x3B, 0x7F, 0xF0, 0x80, 0x3B, 0x7F, 0xF0, 0x80, 0x3B, 0x7F, 0x7C, 0x80, 0x3B, 0x7F, 0x7C, 0x80, 0x3B, 0x7F, 0x04, 0x80, 0x3B, 0x7F, 0x04, 0x80, 0x3B, 0x7D, 0xD0, 0x80, 0x3B, 0x7D, 0xD0, 0x80, 0x3B, 0x7D, 0xD0, 0x80, 0x3B, 0x7D, 0xD0, 0x80, 0x3B, 0x7D, 0xD0, 0x80, 0x3B, 0x7D, 0xD0, 0x80, 0x3B, 0x7D, 0xD0, 0x80, 0x3B, 0x7D, 0xD0, 0x80, 0x3B, 0x7D, 0x5C, 0x80, 0x3B, 0x7D, 0x5C, 0x80, 0x3B, 0x93, 0x3C, 0x80, 0x3B, 0x7D, 0x5C, 0x80, 0x3B, 0x7C, 0xE4, 0x80, 0x3B, 0x7C, 0xE4, 0x80, 0x3B, 0x92, 0xC4, 0x80, 0x3B, 0x7C, 0xE4, 0x80, 0x3B, 0x7C, 0x70, 0x80, 0x3B, 0x7C, 0x70, 0x80, 0x3B, 0x92, 0x50, 0x80, 0x3B, 0x7C, 0x70, 0x80, 0x3B, 0x7B, 0xF8, 0x80, 0x3B, 0x7B, 0xF8, 0x80, 0x3B, 0x91, 0xD8, 0x80, 0x3B, 0x7B, 0xF8, 0x80, 0x3B, 0x7B, 0x84, 0x80, 0x3B, 0x7B, 0x84, 0x80, 0x3B, 0x91, 0x64, 0x80, 0x3B, 0x7B, 0x84, 0x80, 0x3B, 0x7B, 0x0C, 0x80, 0x3B, 0x7B, 0x0C, 0x80, 0x3B, 0x90, 0xEC, 0x80, 0x3B, 0x7B, 0x0C, 0x80, 0x3B, 0x7A, 0x98, 0x80, 0x3B, 0x7A, 0x98, 0x80, 0x3B, 0x7A, 0x98, 0x80, 0x3B, 0x7A, 0x98, 0x80, 0x3B, 0x90, 0x78, 0x80, 0x3B, 0x90, 0x78, 0x80, 0x3B, 0x79, 0xD8, 0x80, 0x3B, 0x79, 0x14, 0x80, 0x3B, 0x78, 0x9C, 0x80, 0x3B, 0x78, 0x9C, 0x80, 0x3B, 0x78, 0x9C, 0x80, 0x3B, 0x78, 0x9C, 0x80, 0x3B, 0x90, 0x00, 0x80, 0x3B, 0x90, 0x00, 0x80, 0x3B, 0x77, 0xD8, 0x80, 0x3B, 0x77, 0x10, 0x80, 0x3B, 0x76, 0x9C, 0x80, 0x3B, 0x76, 0x9C, 0x80, 0x3B, 0x76, 0x9C, 0x80, 0x3B, 0x76, 0x9C, 0x80, 0x3B, 0x8F, 0x8C, 0x80, 0x3B, 0x8F, 0x8C, 0x80, 0x3B, 0x75, 0xDC, 0x80, 0x3B, 0x75, 0x18, 0x80, 0x3B, 0x74, 0xA0, 0x80, 0x3B, 0x74, 0xA0, 0x80, 0x3B, 0x74, 0xA0, 0x80, 0x3B, 0x74, 0xA0, 0x80, 0x3B, 0x8F, 0x14, 0x80, 0x3B, 0x8F, 0x14, 0x80, 0x3B, 0x73, 0xDC, 0x80, 0x3B, 0x73, 0x14, 0x80, 0x3B, 0x72, 0xA0, 0x80, 0x3B, 0x72, 0xA0, 0x80, 0x3B, 0x72, 0xA0, 0x80, 0x3B, 0x72, 0xA0, 0x80, 0x3B, 0x72, 0xA0, 0x80, 0x3B, 0x72, 0xA0, 0x80, 0x3B, 0x71, 0xE0, 0x80, 0x3B, 0x71, 0x1C, 0x80, 0x3B, 0x8E, 0xA0, 0x80, 0x3B, 0x8E, 0xA0, 0x80, 0x3B, 0x8E, 0xA0, 0x80, 0x3B, 0x8E, 0xA0, 0x80, 0x3B, 0x70, 0x5C, 0x80, 0x3B, 0x70, 0x5C, 0x80, 0x3B, 0x6F, 0x98, 0x80, 0x3B, 0x6F, 0x98, 0x80, 0x3B, 0x6F, 0x20, 0x80, 0x3B, 0x6F, 0x20, 0x80, 0x3B, 0x6F, 0x20, 0x80, 0x3B, 0x6F, 0x20, 0x80, 0x3B, 0x6F, 0x20, 0x80, 0x3B, 0x6F, 0x20, 0x80, 0x3B, 0x6E, 0x5C, 0x80, 0x3B, 0x6D, 0x94, 0x80, 0x3B, 0x8E, 0x28, 0x80, 0x3B, 0x8E, 0x28, 0x80, 0x3B, 0x8E, 0x28, 0x80, 0x3B, 0x8E, 0x28, 0x80, 0x3B, 0x6C, 0xD0, 0x80, 0x3B, 0x6C, 0xD0, 0x80, 0x3B, 0x6C, 0x08, 0x80, 0x3B, 0x6C, 0x08, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x8E, 0x00, 0x80, 0x3B, 0x6B, 0x94, 0x80, 0x3B, 0x6B, 0x94, 0x80, 0x3B, 0x6B, 0x94, 0x80, 0x3B, 0x6B, 0x94, 0x80, 0x3B, 0x6B, 0x94, 0x80, 0x3B, 0x6B, 0x94, 0x80, 0x3B, 0x6B, 0x94, 0x80, 0x3B, 0x6B, 0x94, 0x80, 0x3B, 0x6A, 0xD4, 0x80, 0x3B, 0x6A, 0x10, 0x80, 0x3B, 0x69, 0x50, 0x80, 0x3B, 0x68, 0x8C, 0x80, 0x3B, 0x67, 0xCC, 0x80, 0x3B, 0x67, 0xCC, 0x80, 0x3B, 0x67, 0x08, 0x80, 0x3B, 0x67, 0x08, 0x80, 0x3B, 0x8D, 0x8C, 0x80, 0x3B, 0x8D, 0x8C, 0x80, 0x3B, 0x8D, 0x8C, 0x80, 0x3B, 0x8D, 0x8C, 0x80, 0x3B, 0x8D, 0x8C, 0x80, 0x3B, 0x8D, 0x8C, 0x80, 0x3B, 0x8D, 0x8C, 0x80, 0x3B, 0x8D, 0x8C, 0x80, 0x3B, 0x66, 0x48, 0x80, 0x3B, 0x66, 0x48, 0x80, 0x3B, 0x8C, 0xCC, 0x80, 0x3B, 0x66, 0x48, 0x80, 0x3B, 0x65, 0x84, 0x80, 0x3B, 0x65, 0x84, 0x80, 0x3B, 0x8C, 0x08, 0x80, 0x3B, 0x65, 0x84, 0x80, 0x3B, 0x65, 0x0C, 0x80, 0x3B, 0x65, 0x0C, 0x80, 0x3B, 0x65, 0x0C, 0x80, 0x3B, 0x65, 0x0C, 0x80, 0x3B, 0x65, 0x0C, 0x80, 0x3B, 0x65, 0x0C, 0x80, 0x3B, 0x65, 0x0C, 0x80, 0x3B, 0x65, 0x0C, 0x80, 0x3B, 0x64, 0x48, 0x80, 0x3B, 0x63, 0x80, 0x80, 0x3B, 0x62, 0xBC, 0x80, 0x3B, 0x61, 0xF4, 0x80, 0x3B, 0x61, 0x30, 0x80, 0x3B, 0x61, 0x30, 0x80, 0x3B, 0x60, 0x68, 0x80, 0x3B, 0x60, 0x68, 0x80, 0x3B, 0x8B, 0x90, 0x80, 0x3B, 0x8B, 0x90, 0x80, 0x3B, 0x8B, 0x90, 0x80, 0x3B, 0x8B, 0x90, 0x80, 0x3B, 0x8B, 0x90, 0x80, 0x3B, 0x8B, 0x90, 0x80, 0x3B, 0x8B, 0x90, 0x80, 0x3B, 0x8B, 0x90, 0x80, 0x3B, 0x5F, 0xA4, 0x80, 0x3B, 0x5F, 0xA4, 0x80, 0x3B, 0x8A, 0xCC, 0x80, 0x3B, 0x5F, 0xA4, 0x80, 0x3B, 0x5E, 0xDC, 0x80, 0x3B, 0x5E, 0xDC, 0x80, 0x3B, 0x8A, 0x04, 0x80, 0x3B, 0x5E, 0xDC, 0x80, 0x3B, 0xC2, 0xFC, 0x80, 0x3B, 0xC2, 0x34, 0x80, 0x3B, 0xC1, 0x6C, 0x80, 0x3B, 0xC1, 0x6C, 0x80, 0x3B, 0xC0, 0x7C, 0x80, 0x3B, 0xC0, 0x7C, 0x80, 0x3B, 0xC0, 0x7C, 0x80, 0x3B, 0xC0, 0x7C, 0x80, 0x3B, 0xC0, 0x04, 0x80, 0x3B, 0xBF, 0x88, 0x80, 0x3B, 0xBF, 0x10, 0x80, 0x3B, 0xBE, 0x94, 0x80, 0x3B, 0xBE, 0x1C, 0x80, 0x3B, 0xBD, 0xA0, 0x80, 0x3B, 0xBD, 0x28, 0x80, 0x3B, 0xBC, 0xAC, 0x80, 0x3B, 0xBC, 0x34, 0x80, 0x3B, 0xBC, 0x34, 0x80, 0x3B, 0xBB, 0xB8, 0x80, 0x3B, 0xBB, 0xB8, 0x80, 0x3B, 0xBB, 0x40, 0x80, 0x3B, 0xBB, 0x40, 0x80, 0x3B, 0xBA, 0xC4, 0x80, 0x3B, 0xBA, 0xC4, 0x80, 0x3B, 0xBA, 0x4C, 0x80, 0x3B, 0xBA, 0x4C, 0x80, 0x3B, 0xB9, 0xD0, 0x80, 0x3B, 0xB9, 0xD0, 0x80, 0x3B, 0xB9, 0x58, 0x80, 0x3B, 0xB9, 0x58, 0x80, 0x3B, 0xB8, 0xDC, 0x80, 0x3B, 0xB8, 0xDC, 0x80, 0x3B, 0xB7, 0xA4, 0x80, 0x3B, 0xB7, 0xA4, 0x80, 0x3B, 0xB7, 0xA4, 0x80, 0x3B, 0xB7, 0xA4, 0x80, 0x3B, 0xB7, 0xA4, 0x80, 0x3B, 0xB7, 0xA4, 0x80, 0x3B, 0xB7, 0xA4, 0x80, 0x3B, 0xB7, 0xA4, 0x80, 0x3B, 0xB7, 0x2C, 0x80, 0x3B, 0xB7, 0x2C, 0x80, 0x3B, 0xCD, 0xB8, 0x80, 0x3B, 0xB7, 0x2C, 0x80, 0x3B, 0xB6, 0xB0, 0x80, 0x3B, 0xB6, 0xB0, 0x80, 0x3B, 0xCD, 0x3C, 0x80, 0x3B, 0xB6, 0xB0, 0x80, 0x3B, 0xB6, 0x38, 0x80, 0x3B, 0xB6, 0x38, 0x80, 0x3B, 0xCC, 0xC4, 0x80, 0x3B, 0xB6, 0x38, 0x80, 0x3B, 0xB5, 0xBC, 0x80, 0x3B, 0xB5, 0xBC, 0x80, 0x3B, 0xCC, 0x48, 0x80, 0x3B, 0xB5, 0xBC, 0x80, 0x3B, 0xB5, 0x44, 0x80, 0x3B, 0xB5, 0x44, 0x80, 0x3B, 0xCB, 0xD0, 0x80, 0x3B, 0xB5, 0x44, 0x80, 0x3B, 0xB4, 0xC8, 0x80, 0x3B, 0xB4, 0xC8, 0x80, 0x3B, 0xCB, 0x54, 0x80, 0x3B, 0xB4, 0xC8, 0x80, 0x3B, 0xB4, 0x50, 0x80, 0x3B, 0xB4, 0x50, 0x80, 0x3B, 0xB4, 0x50, 0x80, 0x3B, 0xB4, 0x50, 0x80, 0x3B, 0xCA, 0xDC, 0x80, 0x3B, 0xCA, 0xDC, 0x80, 0x3B, 0xB3, 0x88, 0x80, 0x3B, 0xB2, 0xBC, 0x80, 0x3B, 0xB2, 0x40, 0x80, 0x3B, 0xB2, 0x40, 0x80, 0x3B, 0xB2, 0x40, 0x80, 0x3B, 0xB2, 0x40, 0x80, 0x3B, 0xCA, 0x60, 0x80, 0x3B, 0xCA, 0x60, 0x80, 0x3B, 0xB1, 0x74, 0x80, 0x3B, 0xB0, 0xA4, 0x80, 0x3B, 0xB0, 0x2C, 0x80, 0x3B, 0xB0, 0x2C, 0x80, 0x3B, 0xB0, 0x2C, 0x80, 0x3B, 0xB0, 0x2C, 0x80, 0x3B, 0xC9, 0xE8, 0x80, 0x3B, 0xC9, 0xE8, 0x80, 0x3B, 0xAF, 0x64, 0x80, 0x3B, 0xAE, 0x98, 0x80, 0x3B, 0xAE, 0x1C, 0x80, 0x3B, 0xAE, 0x1C, 0x80, 0x3B, 0xAE, 0x1C, 0x80, 0x3B, 0xAE, 0x1C, 0x80, 0x3B, 0xC9, 0x6C, 0x80, 0x3B, 0xC9, 0x6C, 0x80, 0x3B, 0xAD, 0x50, 0x80, 0x3B, 0xAC, 0x80, 0x80, 0x3B, 0xAC, 0x08, 0x80, 0x3B, 0xAC, 0x08, 0x80, 0x3B, 0xAC, 0x08, 0x80, 0x3B, 0xAC, 0x08, 0x80, 0x3B, 0xAC, 0x08, 0x80, 0x3B, 0xAC, 0x08, 0x80, 0x3B, 0xAB, 0x40, 0x80, 0x3B, 0xAA, 0x74, 0x80, 0x3B, 0xC8, 0xF4, 0x80, 0x3B, 0xC8, 0xF4, 0x80, 0x3B, 0xC8, 0xF4, 0x80, 0x3B, 0xC8, 0xF4, 0x80, 0x3B, 0xA9, 0xAC, 0x80, 0x3B, 0xA9, 0xAC, 0x80, 0x3B, 0xA8, 0xE0, 0x80, 0x3B, 0xA8, 0xE0, 0x80, 0x3B, 0xA8, 0x64, 0x80, 0x3B, 0xA8, 0x64, 0x80, 0x3B, 0xA8, 0x64, 0x80, 0x3B, 0xA8, 0x64, 0x80, 0x3B, 0xA8, 0x64, 0x80, 0x3B, 0xA8, 0x64, 0x80, 0x3B, 0xA7, 0x98, 0x80, 0x3B, 0xA6, 0xC8, 0x80, 0x3B, 0xC8, 0x78, 0x80, 0x3B, 0xC8, 0x78, 0x80, 0x3B, 0xC8, 0x78, 0x80, 0x3B, 0xC8, 0x78, 0x80, 0x3B, 0xA5, 0xFC, 0x80, 0x3B, 0xA5, 0xFC, 0x80, 0x3B, 0xA5, 0x2C, 0x80, 0x3B, 0xA5, 0x2C, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xC8, 0x50, 0x80, 0x3B, 0xA4, 0xB4, 0x80, 0x3B, 0xA4, 0xB4, 0x80, 0x3B, 0xA4, 0xB4, 0x80, 0x3B, 0xA4, 0xB4, 0x80, 0x3B, 0xA4, 0xB4, 0x80, 0x3B, 0xA4, 0xB4, 0x80, 0x3B, 0xA4, 0xB4, 0x80, 0x3B, 0xA4, 0xB4, 0x80, 0x3B, 0xA3, 0xEC, 0x80, 0x3B, 0xA3, 0x20, 0x80, 0x3B, 0xA2, 0x58, 0x80, 0x3B, 0xA1, 0x8C, 0x80, 0x3B, 0xA0, 0xC4, 0x80, 0x3B, 0xA0, 0xC4, 0x80, 0x3B, 0x9F, 0xF8, 0x80, 0x3B, 0x9F, 0xF8, 0x80, 0x3B, 0xC7, 0xD8, 0x80, 0x3B, 0xC7, 0xD8, 0x80, 0x3B, 0xC7, 0xD8, 0x80, 0x3B, 0xC7, 0xD8, 0x80, 0x3B, 0xC7, 0xD8, 0x80, 0x3B, 0xC7, 0xD8, 0x80, 0x3B, 0xC7, 0xD8, 0x80, 0x3B, 0xC7, 0xD8, 0x80, 0x3B, 0x9F, 0x30, 0x80, 0x3B, 0x9F, 0x30, 0x80, 0x3B, 0xC7, 0x10, 0x80, 0x3B, 0x9F, 0x30, 0x80, 0x3B, 0x9E, 0x64, 0x80, 0x3B, 0x9E, 0x64, 0x80, 0x3B, 0xC6, 0x44, 0x80, 0x3B, 0x9E, 0x64, 0x80, 0x3B, 0x9D, 0xE8, 0x80, 0x3B, 0x9D, 0xE8, 0x80, 0x3B, 0x9D, 0xE8, 0x80, 0x3B, 0x9D, 0xE8, 0x80, 0x3B, 0x9D, 0xE8, 0x80, 0x3B, 0x9D, 0xE8, 0x80, 0x3B, 0x9D, 0xE8, 0x80, 0x3B, 0x9D, 0xE8, 0x80, 0x3B, 0x9D, 0x1C, 0x80, 0x3B, 0x9C, 0x4C, 0x80, 0x3B, 0x9B, 0x80, 0x80, 0x3B, 0x9A, 0xB0, 0x80, 0x3B, 0x99, 0xE4, 0x80, 0x3B, 0x99, 0xE4, 0x80, 0x3B, 0x99, 0x14, 0x80, 0x3B, 0x99, 0x14, 0x80, 0x3B, 0xC5, 0xC8, 0x80, 0x3B, 0xC5, 0xC8, 0x80, 0x3B, 0xC5, 0xC8, 0x80, 0x3B, 0xC5, 0xC8, 0x80, 0x3B, 0xC5, 0xC8, 0x80, 0x3B, 0xC5, 0xC8, 0x80, 0x3B, 0xC5, 0xC8, 0x80, 0x3B, 0xC5, 0xC8, 0x80, 0x3B, 0x98, 0x48, 0x80, 0x3B, 0x98, 0x48, 0x80, 0x3B, 0xC4, 0xFC, 0x80, 0x3B, 0x98, 0x48, 0x80, 0x3B, 0x97, 0x78, 0x80, 0x3B, 0x97, 0x78, 0x80, 0x3B, 0xC4, 0x2C, 0x80, 0x3B, 0x97, 0x78}; // .data 0xC00 at 0x805672D8
__attribute__((used, section(".rodata"))) const volatile unsigned char * const __keep___large_data_for_CriWare_src_sofdec_sfdcore_mpv_mp = __large_data_for_CriWare_src_sofdec_sfdcore_mpv_mp; // keep
__declspec(section ".rodata") __attribute__((aligned(8), used)) const unsigned char __large_rodata_for_CriWare_src_sofdec_sfdcore_mpv_[0x10] = {0x80, 0x3B, 0x27, 0x84, 0x80, 0x3B, 0x5D, 0xC4, 0x80, 0x3B, 0x93, 0xE4, 0x00, 0x00, 0x00, 0x00}; // .rodata 0x10 at 0x8051C378
__attribute__((used, section(".data"))) const volatile unsigned char * const __keep___large_rodata_for_CriWare_src_sofdec_sfdcore_mpv_ = __large_rodata_for_CriWare_src_sofdec_sfdcore_mpv_; // keep

/* MPV bitstream / VLC context (retail offsets). */
typedef struct MPVABDEC_CTX_ {
    char pad0[0x980];
    u32 hi;        /* 0x980 */
    u32 lo;        /* 0x984 */
    s32 bc;        /* 0x988 */
    u32 *ptr;      /* 0x98C */
    u32 *tbl990;   /* 0x990 run-level direct table */
    s16 *tbl994;   /* 0x994 */
    s16 *tbl998;   /* 0x998 */
    s16 *esc1;     /* 0x99C */
    s16 *esc2;     /* 0x9A0 */
    s16 *esc3;     /* 0x9A4 */
    s16 *esc4;     /* 0x9A8 */
    u8 *p9ac;      /* 0x9AC coefficient position table */
    s16 *masktbl;  /* 0x9B0 DC mask table */
    s16 *tblqt;    /* 0x9B4 quant matrix */
} MPVABDEC_CTX;

/* Per-block decode state (retail offsets). */
typedef struct MPVABDEC_BLK_ {
    s32 f00;       /* 0x00 run */
    s32 f04;       /* 0x04 level */
    u32 f08;       /* 0x08 sign */
    u32 codelen;   /* 0x0C */
    s32 first;     /* 0x10 */
    s32 last;      /* 0x14 */
    char pad1[4];  /* 0x18 */
    s16 *block;    /* 0x1C */
    u8 *clip;      /* 0x20 */
    s32 quant;     /* 0x24 */
    s32 *sum;      /* 0x28 DC prediction accumulator */
    u8 *dctbl;     /* 0x2C DC-size VLC table */
} MPVABDEC_BLK;

extern char lbl_eu_8051C378[];
extern u32 lbl_eu_806046A8[];

void MPVABDEC_Init(void) {
    lbl_eu_806046A8[2] = (u32)lbl_eu_8051C378;
}

s32 mpvabdec_IntraBlock_Isr(MPVABDEC_CTX *ctx, MPVABDEC_BLK *blk) {
    u32 hi = ctx->hi;
    u32 lo = ctx->lo;
    s32 bc = ctx->bc;
    u32 *ptr = ctx->ptr;
    u32 t = hi >> 16;
    if (bc > 0x10) t |= lo >> (0x30 - bc);
    u8 v = blk->dctbl[t >> 9];
    u32 size = (u32)v >> 4;
    u32 low = v & 0xF;
    s32 dc = size;
    if (size != 0) {
        s16 *masktbl = ctx->masktbl;
        u32 mask = (u32)masktbl[low];
        low += size;
        u32 one = 1u << (size - 1);
        t &= mask;
        t >>= 16 - low;
        if ((t & one) == 0) t += 1 - (one << 1);
        dc = (s32)(t << 3);
    }
    bc += low;
    if (bc >= 0x20) {
        bc -= 0x20;
        hi = lo << bc;
        lo = *ptr++;
        } else {
        hi <<= low;
    }
    s32 *sum = blk->sum;
    dc += *sum;
    *sum = dc;
    blk->block[0] = (s16)(dc << 3);
    blk->first = 0;
    blk->last = 0;
    u8 *p = ctx->p9ac;

    for (;;) {
        u32 bits = hi;
        if (bc != 0) bits |= lo >> (32 - bc);
        u32 idx = bits >> 24;
        switch (idx) {
        default:
            goto exit;
        case 252: case 253: case 255: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 248: case 249: case 251: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 238: case 239: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 236: case 237: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 235: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 234: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 233: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 232: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 224: case 225: case 226: case 227: case 228: case 229: case 230: case 231: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 3;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 3;
                }
            continue;
        }
        case 220: case 221: case 223: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 216: case 217: case 219: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 206: case 207: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 204: case 205: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 203: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 202: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 201: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 200: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 192: case 193: case 194: case 195: case 196: case 197: case 198: case 199: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 3;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 3;
                }
            continue;
        }
        case 126: case 127: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 124: case 125: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 119: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 118: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 112: case 113: case 114: case 115: case 116: case 117: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 4;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 4;
                }
            continue;
        }
        case 110: case 111: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 108: case 109: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 103: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 102: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 96: case 97: case 98: case 99: case 100: case 101: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 4;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 4;
                }
            continue;
        }
        case 95: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 94: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 88: case 89: case 90: case 91: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 87: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 86: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 80: case 81: case 82: case 83: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 79: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 78: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 72: case 73: case 74: case 75: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 71: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 70: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 64: case 65: case 66: case 67: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 60: case 61: case 63: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 56: case 57: case 59: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 52: case 53: case 55: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 48: case 49: case 51: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 44: case 45: case 47: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((6 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 40: case 41: case 43: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((6 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39: {
            bits <<= 1;
            u32 t = ctx->tbl990[(bits >> 25) & 0x7F];
            blk->f00 = t & 0xFF;
            if (blk->f00 != 0x40) {
                blk->codelen = t >> 16;
                blk->f04 = (s32)(s8)(t >> 8);
                bits >>= 0x21 - (s32)blk->codelen;
                blk->f08 = bits & 1;
                } else {
                blk->codelen = 0x14;
                s32 v = (s32)((bits >> 11) & 0xFFFF) >> 2;
                blk->f00 = (u32)(s8)(v >> 8);
                if (((bits >> 13) & 0x7F) != 0) {
                    } else {
                    blk->codelen = 0x1C;
                    v = ((s32)(s8)((bits >> 13) & 0xFF) << 1) | (s32)((bits >> 5) & 0xFF);
                    }
                if (v < 0) {
                    blk->f08 = 1;
                    v = -v;
                    } else {
                    blk->f08 = 0;
                    }
                blk->f04 = (s32)v;
                }
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 30: case 31: {
            p += 6;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 28: case 29: {
            p += 6;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 26: case 27: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 24: case 25: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 22: case 23: {
            p += 7;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 20: case 21: {
            p += 7;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 18: case 19: {
            p += 8;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 16: case 17: {
            p += 8;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 15: {
            p += 9;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 14: {
            p += 9;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 13: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 12: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 11: {
            p += 10;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 10: {
            p += 10;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 9: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 8: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 4: case 5: case 6: case 7: {
            bits <<= 1;
            blk->codelen = 0x14;
            s32 v = (s32)((bits >> 11) & 0xFFFF) >> 2;
            blk->f00 = (u32)(s8)(v >> 8);
            if (((bits >> 13) & 0x7F) != 0) {
                } else {
                blk->codelen = 0x1C;
                v = ((s32)(s8)((bits >> 13) & 0xFF) << 1) | (s32)((bits >> 5) & 0xFF);
                }
            if (v < 0) {
                blk->f08 = 1;
                v = -v;
                } else {
                blk->f08 = 0;
                }
            blk->f04 = (s32)v;
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            continue;
        }
        case 2: case 3: {
            blk->codelen = 0x0B;
            bits >>= 21;
            s16 t = ctx->tbl994[bits >> 1];
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += 11;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 11;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 1: {
            blk->codelen = 0x0D;
            bits >>= 19;
            s16 t = ctx->tbl998[bits >> 1];
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += 13;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 13;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 0: {
            bits <<= 1;
            s16 t;
            if ((bits >> 24) != 0) {
                blk->codelen = 0x0E;
                bits >>= 19;
                t = ctx->esc1[bits >> 1];
                } else {
                bits <<= 8;
                if ((s32)bits < 0) {
                    blk->codelen = 0x0F;
                    bits = (bits >> 26) & 0x1F;
                    t = ctx->esc2[bits >> 1];
                    } else {
                    bits <<= 1;
                    if ((s32)bits < 0) {
                        blk->codelen = 0x10;
                        bits = (bits >> 26) & 0x1F;
                        t = ctx->esc3[bits >> 1];
                        } else {
                        blk->codelen = 0x11;
                        bits = (bits >> 25) & 0x1F;
                        t = ctx->esc4[bits >> 1];
                        }
                    }
                }
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 254: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 250: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 240: case 241: case 242: case 243: case 244: case 245: case 246: case 247: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            goto exit;
        }
        case 222: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 218: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 208: case 209: case 210: case 211: case 212: case 213: case 214: case 215: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            goto exit;
        }
        case 128: case 129: case 130: case 131: case 132: case 133: case 134: case 135: case 136: case 137: case 138: case 139: case 140: case 141: case 142: case 143: case 144: case 145: case 146: case 147: case 148: case 149: case 150: case 151: case 152: case 153: case 154: case 155: case 156: case 157: case 158: case 159: case 160: case 161: case 162: case 163: case 164: case 165: case 166: case 167: case 168: case 169: case 170: case 171: case 172: case 173: case 174: case 175: case 176: case 177: case 178: case 179: case 180: case 181: case 182: case 183: case 184: case 185: case 186: case 187: case 188: case 189: case 190: case 191: {
            bc += 2;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 2;
                }
            goto exit;
        }
        case 120: case 121: case 122: case 123: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            goto exit;
        }
        case 104: case 105: case 106: case 107: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            goto exit;
        }
        case 92: case 93: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 84: case 85: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 76: case 77: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 68: case 69: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 62: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 58: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 54: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 50: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 46: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((6 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 42: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((6 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        }
    }
exit:
    s32 last = blk->last;
    if (last != blk->first) last = -last;
    blk->last = last;
    ctx->hi = hi;
    ctx->lo = lo;
    ctx->bc = bc;
    ctx->ptr = ptr;
    return blk->last;
}


s32 mpvabdec_IntraBlockDc11_Isr(MPVABDEC_CTX *ctx, MPVABDEC_BLK *blk) {
    u32 hi = ctx->hi;
    u32 lo = ctx->lo;
    s32 bc = ctx->bc;
    u32 *ptr = ctx->ptr;
    u32 bits = hi;
    if (bc != 0) bits |= lo >> (32 - bc);
    u8 v = blk->dctbl[bits >> 22];
    u32 size = (u32)v >> 4;
    u32 low = v & 0xF;
    s32 dc = size;
    if (size != 0) {
        u32 x = bits << low;
        low += size;
        x = ((s32)x >> 1) ^ 0x80000000u;
        dc = (s32)((x >> 31) + ((s32)x >> (31 - size)));
    }
    bc += low;
    if (bc >= 0x20) {
        bc -= 0x20;
        hi = lo << bc;
        lo = *ptr++;
        } else {
        hi <<= low;
    }
    s32 *sum = blk->sum;
    dc += *sum;
    *sum = dc;
    blk->block[0] = (s16)(dc << 3);
    blk->first = 0;
    blk->last = 0;
    u8 *p = ctx->p9ac;

    for (;;) {
        u32 bits = hi;
        if (bc != 0) bits |= lo >> (32 - bc);
        u32 idx = bits >> 24;
        switch (idx) {
        default:
            goto exit;
        case 252: case 253: case 255: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 248: case 249: case 251: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 238: case 239: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 236: case 237: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 235: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 234: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 233: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 232: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 224: case 225: case 226: case 227: case 228: case 229: case 230: case 231: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 3;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 3;
                }
            continue;
        }
        case 220: case 221: case 223: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 216: case 217: case 219: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 206: case 207: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 204: case 205: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 203: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 202: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 201: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 200: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 192: case 193: case 194: case 195: case 196: case 197: case 198: case 199: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 3;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 3;
                }
            continue;
        }
        case 126: case 127: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 124: case 125: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 119: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 118: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 112: case 113: case 114: case 115: case 116: case 117: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 4;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 4;
                }
            continue;
        }
        case 110: case 111: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 108: case 109: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 103: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 102: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 96: case 97: case 98: case 99: case 100: case 101: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 4;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 4;
                }
            continue;
        }
        case 95: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 94: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 88: case 89: case 90: case 91: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 87: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 86: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 80: case 81: case 82: case 83: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 79: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 78: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 72: case 73: case 74: case 75: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 71: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 70: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 64: case 65: case 66: case 67: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 60: case 61: case 63: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 56: case 57: case 59: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 52: case 53: case 55: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 48: case 49: case 51: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 44: case 45: case 47: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((6 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 40: case 41: case 43: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((6 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39: {
            bits <<= 1;
            u32 t = ctx->tbl990[(bits >> 25) & 0x7F];
            blk->f00 = t & 0xFF;
            if (blk->f00 != 0x40) {
                blk->codelen = t >> 16;
                blk->f04 = (s32)(s8)(t >> 8);
                bits >>= 0x21 - (s32)blk->codelen;
                blk->f08 = bits & 1;
                } else {
                blk->codelen = 0x14;
                s32 v = (s32)((bits >> 11) & 0xFFFF) >> 2;
                blk->f00 = (u32)(s8)(v >> 8);
                if (((bits >> 13) & 0x7F) != 0) {
                    } else {
                    blk->codelen = 0x1C;
                    v = ((s32)(s8)((bits >> 13) & 0xFF) << 1) | (s32)((bits >> 5) & 0xFF);
                    }
                if (v < 0) {
                    blk->f08 = 1;
                    v = -v;
                    } else {
                    blk->f08 = 0;
                    }
                blk->f04 = (s32)v;
                }
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 30: case 31: {
            p += 6;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 28: case 29: {
            p += 6;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 26: case 27: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 24: case 25: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 22: case 23: {
            p += 7;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 20: case 21: {
            p += 7;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 18: case 19: {
            p += 8;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 16: case 17: {
            p += 8;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 15: {
            p += 9;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 14: {
            p += 9;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 13: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 12: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 11: {
            p += 10;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 10: {
            p += 10;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 9: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 8: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 4: case 5: case 6: case 7: {
            bits <<= 1;
            blk->codelen = 0x14;
            s32 v = (s32)((bits >> 11) & 0xFFFF) >> 2;
            blk->f00 = (u32)(s8)(v >> 8);
            if (((bits >> 13) & 0x7F) != 0) {
                } else {
                blk->codelen = 0x1C;
                v = ((s32)(s8)((bits >> 13) & 0xFF) << 1) | (s32)((bits >> 5) & 0xFF);
                }
            if (v < 0) {
                blk->f08 = 1;
                v = -v;
                } else {
                blk->f08 = 0;
                }
            blk->f04 = (s32)v;
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            continue;
        }
        case 2: case 3: {
            blk->codelen = 0x0B;
            bits >>= 21;
            s16 t = ctx->tbl994[bits >> 1];
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += 11;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 11;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 1: {
            blk->codelen = 0x0D;
            bits >>= 19;
            s16 t = ctx->tbl998[bits >> 1];
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += 13;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 13;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 0: {
            bits <<= 1;
            s16 t;
            if ((bits >> 24) != 0) {
                blk->codelen = 0x0E;
                bits >>= 19;
                t = ctx->esc1[bits >> 1];
                } else {
                bits <<= 8;
                if ((s32)bits < 0) {
                    blk->codelen = 0x0F;
                    bits = (bits >> 26) & 0x1F;
                    t = ctx->esc2[bits >> 1];
                    } else {
                    bits <<= 1;
                    if ((s32)bits < 0) {
                        blk->codelen = 0x10;
                        bits = (bits >> 26) & 0x1F;
                        t = ctx->esc3[bits >> 1];
                        } else {
                        blk->codelen = 0x11;
                        bits = (bits >> 25) & 0x1F;
                        t = ctx->esc4[bits >> 1];
                        }
                    }
                }
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 * (s32)blk->quant) * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 254: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 250: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 240: case 241: case 242: case 243: case 244: case 245: case 246: case 247: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            goto exit;
        }
        case 222: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 218: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((2 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 208: case 209: case 210: case 211: case 212: case 213: case 214: case 215: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            goto exit;
        }
        case 128: case 129: case 130: case 131: case 132: case 133: case 134: case 135: case 136: case 137: case 138: case 139: case 140: case 141: case 142: case 143: case 144: case 145: case 146: case 147: case 148: case 149: case 150: case 151: case 152: case 153: case 154: case 155: case 156: case 157: case 158: case 159: case 160: case 161: case 162: case 163: case 164: case 165: case 166: case 167: case 168: case 169: case 170: case 171: case 172: case 173: case 174: case 175: case 176: case 177: case 178: case 179: case 180: case 181: case 182: case 183: case 184: case 185: case 186: case 187: case 188: case 189: case 190: case 191: {
            bc += 2;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 2;
                }
            goto exit;
        }
        case 120: case 121: case 122: case 123: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            goto exit;
        }
        case 104: case 105: case 106: case 107: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            goto exit;
        }
        case 92: case 93: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 84: case 85: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 76: case 77: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 68: case 69: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 62: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 58: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 54: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 50: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((2 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 46: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((6 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 42: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((6 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        }
    }
exit:
    s32 last = blk->last;
    if (last != blk->first) last = -last;
    blk->last = last;
    ctx->hi = hi;
    ctx->lo = lo;
    ctx->bc = bc;
    ctx->ptr = ptr;
    return blk->last;
}


s32 mpvabdec_NintraBlock_Isr(MPVABDEC_CTX *ctx, MPVABDEC_BLK *blk) {
    u32 *b0 = (u32 *)blk->block;
    b0[1] = 0; b0[0] = 0; b0[3] = 0; b0[2] = 0;
    b0[5] = 0; b0[4] = 0; b0[7] = 0; b0[6] = 0;
    b0[9] = 0; b0[8] = 0; b0[11] = 0; b0[10] = 0;
    b0[13] = 0; b0[12] = 0; b0[15] = 0; b0[14] = 0;
    b0[17] = 0; b0[16] = 0; b0[19] = 0; b0[18] = 0;
    b0[21] = 0; b0[20] = 0; b0[23] = 0; b0[22] = 0;
    b0[25] = 0; b0[24] = 0; b0[27] = 0; b0[26] = 0;
    b0[29] = 0; b0[28] = 0; b0[31] = 0; b0[30] = 0;
    s32 bc = ctx->bc;
    u32 hi = ctx->hi;
    u32 lo = ctx->lo;
    u32 *ptr = ctx->ptr;
    u32 bits = hi;
    if (bc != 0) bits |= lo >> (32 - bc);
    if ((s32)bits < 0) {
        blk->f08 = (bits >> 30) & 1;
        blk->f04 = 1;
        blk->f00 = 0;
        blk->codelen = 2;
        goto nib_prefix_done;
    }

    bits <<= 1;
    u32 v = bits >> 24;
    s16 t;
    if ((v - 4) <= 3) goto nib_huff11;
    if ((v - 2) <= 1) goto nib_huff13;
    if (v == 1) goto nib_esc1;
    if (v == 0) goto nib_escape_long;

    {
        u32 t990 = ctx->tbl990[(v << 1) >> 2];
        blk->f00 = t990 & 0xFF;
        if (blk->f00 != 0x40) {
            blk->codelen = t990 >> 16;
            blk->f04 = (s32)(s8)((t990 >> 8) & 0xFF);
            bits >>= 0x21 - (s32)blk->codelen;
            blk->f08 = bits & 1;
        } else {
            blk->codelen = 0x14;
            s32 w = (s32)((bits >> 11) & 0xFFFF) >> 2;
            blk->f00 = (u32)(s8)(w >> 8);
            if (((bits >> 13) & 0x7F) == 0) {
                blk->codelen = 0x1C;
                w = ((s32)(s8)((bits >> 13) & 0xFF) << 1) | (s32)((bits >> 5) & 0xFF);
            }
            if (w < 0) {
                blk->f08 = 1;
                w = -w;
            } else {
                blk->f08 = 0;
            }
            blk->f04 = (s32)w;
        }
        goto nib_prefix_done;
    }

nib_huff11:
    blk->codelen = 0x0B;
    bits >>= 22;
    t = ctx->tbl994[bits >> 1];
    goto nib_table;

nib_huff13:
    blk->codelen = 0x0D;
    bits >>= 20;
    t = ctx->tbl998[bits >> 1];
    goto nib_table;

nib_esc1:
    blk->codelen = 0x0E;
    bits >>= 19;
    t = ctx->esc1[bits >> 1];
    goto nib_table;

nib_escape_long:
    bits <<= 8;
    if ((s32)bits < 0) {
        blk->codelen = 0x0F;
        bits = (bits >> 26) & 0x1F;
        t = ctx->esc2[bits >> 1];
        goto nib_table;
    }
    bits <<= 1;
    if ((s32)bits < 0) {
        blk->codelen = 0x10;
        bits = (bits >> 26) & 0x1F;
        t = ctx->esc3[bits >> 1];
        goto nib_table;
    }
    blk->codelen = 0x11;
    bits = (bits >> 25) & 0x1F;
    t = ctx->esc4[bits >> 1];

nib_table:
    blk->f00 = (u32)(t & 0xFF);
    blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
    blk->f08 = bits & 1;

nib_prefix_done:
    u8 *p = ctx->p9ac;
    bc += (s32)blk->codelen;
    if (bc >= 0x20) {
        bc -= 0x20;
        hi = lo << bc;
        lo = *ptr++;
        } else {
        hi <<= (s32)blk->codelen;
    }
    s32 *sum = blk->sum;
    p += blk->f00;
    s32 c = (s8)p[0];
    blk->first = c;
    blk->last = c;
    s32 s = ((2 * (s32)blk->f04 + 1) * (s32)blk->quant * (s32)blk->clip[c]) >> 4;
    s = (s - 1) | 1;
    if (blk->f08 != 0) s = -s;
    blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);

    for (;;) {
        u32 bits = hi;
        if (bc != 0) bits |= lo >> (32 - bc);
        u32 idx = bits >> 24;
        switch (idx) {
        default:
            goto exit;
        case 252: case 253: case 255: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 248: case 249: case 251: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 238: case 239: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 236: case 237: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 235: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 234: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 233: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 232: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 224: case 225: case 226: case 227: case 228: case 229: case 230: case 231: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 3;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 3;
                }
            continue;
        }
        case 220: case 221: case 223: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 216: case 217: case 219: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 206: case 207: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 204: case 205: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 203: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 202: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 201: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 200: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 192: case 193: case 194: case 195: case 196: case 197: case 198: case 199: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 3;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 3;
                }
            continue;
        }
        case 126: case 127: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 124: case 125: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 119: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 118: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 112: case 113: case 114: case 115: case 116: case 117: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 4;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 4;
                }
            continue;
        }
        case 110: case 111: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 108: case 109: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 3;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 103: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 102: {
            s32 c1 = (s8)p[2];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 96: case 97: case 98: case 99: case 100: case 101: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 4;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 4;
                }
            continue;
        }
        case 95: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 94: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 88: case 89: case 90: case 91: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 87: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 86: {
            s32 c1 = (s8)p[3];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 4;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 80: case 81: case 82: case 83: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 79: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 78: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 72: case 73: case 74: case 75: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((5 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 71: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 70: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 64: case 65: case 66: case 67: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((5 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            continue;
        }
        case 60: case 61: case 63: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 56: case 57: case 59: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 52: case 53: case 55: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 48: case 49: case 51: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 44: case 45: case 47: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((7 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 40: case 41: case 43: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((7 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            continue;
        }
        case 32: case 33: case 34: case 35: case 36: case 37: case 38: case 39: {
            bits <<= 1;
            u32 t = ctx->tbl990[(bits >> 25) & 0x7F];
            blk->f00 = t & 0xFF;
            if (blk->f00 != 0x40) {
                blk->codelen = t >> 16;
                blk->f04 = (s32)(s8)(t >> 8);
                bits >>= 0x21 - (s32)blk->codelen;
                blk->f08 = bits & 1;
                } else {
                blk->codelen = 0x14;
                s32 v = (s32)((bits >> 11) & 0xFFFF) >> 2;
                blk->f00 = (u32)(s8)(v >> 8);
                if (((bits >> 13) & 0x7F) != 0) {
                    } else {
                    blk->codelen = 0x1C;
                    v = ((s32)(s8)((bits >> 13) & 0xFF) << 1) | (s32)((bits >> 5) & 0xFF);
                    }
                if (v < 0) {
                    blk->f08 = 1;
                    v = -v;
                    } else {
                    blk->f08 = 0;
                    }
                blk->f04 = (s32)v;
                }
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 + 1) * (s32)blk->quant * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 30: case 31: {
            p += 6;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 28: case 29: {
            p += 6;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 26: case 27: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((5 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 24: case 25: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((5 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 22: case 23: {
            p += 7;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 20: case 21: {
            p += 7;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 18: case 19: {
            p += 8;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 16: case 17: {
            p += 8;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            continue;
        }
        case 15: {
            p += 9;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 14: {
            p += 9;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 13: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((9 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 12: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((9 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 11: {
            p += 10;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 10: {
            p += 10;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 9: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((5 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 8: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((5 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            continue;
        }
        case 4: case 5: case 6: case 7: {
            bits <<= 1;
            blk->codelen = 0x14;
            s32 v = (s32)((bits >> 11) & 0xFFFF) >> 2;
            blk->f00 = (u32)(s8)(v >> 8);
            if (((bits >> 13) & 0x7F) != 0) {
                } else {
                blk->codelen = 0x1C;
                v = ((s32)(s8)((bits >> 13) & 0xFF) << 1) | (s32)((bits >> 5) & 0xFF);
                }
            if (v < 0) {
                blk->f08 = 1;
                v = -v;
                } else {
                blk->f08 = 0;
                }
            blk->f04 = (s32)v;
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 + 1) * (s32)blk->quant * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            continue;
        }
        case 2: case 3: {
            blk->codelen = 0x0B;
            bits >>= 21;
            s16 t = ctx->tbl994[bits >> 1];
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += 11;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 11;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 + 1) * (s32)blk->quant * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 1: {
            blk->codelen = 0x0D;
            bits >>= 19;
            s16 t = ctx->tbl998[bits >> 1];
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += 13;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 13;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 + 1) * (s32)blk->quant * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 0: {
            bits <<= 1;
            s16 t;
            if ((bits >> 24) != 0) {
                blk->codelen = 0x0E;
                bits >>= 19;
                t = ctx->esc1[bits >> 1];
                } else {
                bits <<= 8;
                if ((s32)bits < 0) {
                    blk->codelen = 0x0F;
                    bits = (bits >> 26) & 0x1F;
                    t = ctx->esc2[bits >> 1];
                    } else {
                    bits <<= 1;
                    if ((s32)bits < 0) {
                        blk->codelen = 0x10;
                        bits = (bits >> 26) & 0x1F;
                        t = ctx->esc3[bits >> 1];
                        } else {
                        blk->codelen = 0x11;
                        bits = (bits >> 25) & 0x1F;
                        t = ctx->esc4[bits >> 1];
                        }
                    }
                }
            blk->f00 = (u32)(t & 0xFF);
            blk->f04 = (s32)(s8)((t >> 8) & 0xFF);
            blk->f08 = bits & 1;
            bc += (s32)blk->codelen;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= (s32)blk->codelen;
                }
            p += blk->f00;
            s32 c = (s8)*++p;
            blk->last = c;
            s32 s = ((2 * (s32)blk->f04 + 1) * (s32)blk->quant * (s32)blk->clip[c]) >> 4;
            if (blk->f08 != 0) {
                s = -((s - 1) | 1);
                } else {
                s = (s - 1) | 1;
                }
            blk->block[c] = (s16)((s * (s32)ctx->tblqt[c] + 0x400) >> 11);
            continue;
        }
        case 254: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 250: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 240: case 241: case 242: case 243: case 244: case 245: case 246: case 247: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            goto exit;
        }
        case 222: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = -((sc2 - 1) | 1);
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 218: {
            s32 c1 = (s8)p[1];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            p += 2;
            s32 c2 = (s8)p[0];
            blk->last = c2;
            s32 sc2 = ((3 * (s32)blk->quant) * (s32)blk->clip[c2]) >> 4;
            sc2 = (sc2 - 1) | 1;
            blk->block[c2] = (s16)((sc2 * (s32)ctx->tblqt[c2] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 208: case 209: case 210: case 211: case 212: case 213: case 214: case 215: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 5;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 5;
                }
            goto exit;
        }
        case 128: case 129: case 130: case 131: case 132: case 133: case 134: case 135: case 136: case 137: case 138: case 139: case 140: case 141: case 142: case 143: case 144: case 145: case 146: case 147: case 148: case 149: case 150: case 151: case 152: case 153: case 154: case 155: case 156: case 157: case 158: case 159: case 160: case 161: case 162: case 163: case 164: case 165: case 166: case 167: case 168: case 169: case 170: case 171: case 172: case 173: case 174: case 175: case 176: case 177: case 178: case 179: case 180: case 181: case 182: case 183: case 184: case 185: case 186: case 187: case 188: case 189: case 190: case 191: {
            bc += 2;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 2;
                }
            goto exit;
        }
        case 120: case 121: case 122: case 123: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            goto exit;
        }
        case 104: case 105: case 106: case 107: {
            p += 2;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 6;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 6;
                }
            goto exit;
        }
        case 92: case 93: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 84: case 85: {
            p += 3;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 76: case 77: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((5 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 68: case 69: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((5 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 7;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 7;
                }
            goto exit;
        }
        case 62: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 58: {
            p += 4;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 54: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 50: {
            p += 5;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((3 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 46: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((7 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = -((sc1 - 1) | 1);
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        case 42: {
            p += 1;
            s32 c1 = (s8)p[0];
            blk->last = c1;
            s32 sc1 = ((7 * (s32)blk->quant) * (s32)blk->clip[c1]) >> 4;
            sc1 = (sc1 - 1) | 1;
            blk->block[c1] = (s16)((sc1 * (s32)ctx->tblqt[c1] + 0x400) >> 11);
            bc += 8;
            if (bc >= 0x20) {
                bc -= 0x20;
                hi = lo << bc;
                lo = *ptr++;
                } else {
                hi <<= 8;
                }
            goto exit;
        }
        }
    }
exit:
    s32 last = blk->last;
    if (last != blk->first) last = -last;
    blk->last = last;
    ctx->hi = hi;
    ctx->lo = lo;
    ctx->bc = bc;
    ctx->ptr = ptr;
    return blk->last;
}
