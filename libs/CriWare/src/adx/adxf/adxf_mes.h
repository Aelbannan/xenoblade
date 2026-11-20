#pragma once

/* CriWare ADX-F error and status message table.  Each member is one NUL-
 * terminated string; the table is emitted as one .rodata object and every
 * call site takes the address of the relevant message. */
typedef struct ADXF_MES {
    char e9040801[0x37];  /* 0x000 E9040801:partition ID is range outside.(adxf_ChkPrmPt) */
    char e9040802[0x2a];  /* 0x037 E9040802:'ptinfo' is NULL.(adxf_ChkPrmPt) */
    char e20040917[0x3c];  /* 0x061 E20040917 : It is impossible to load AFS files in parallel. */
    char e2005121504[0x26];  /* 0x09d E2005121504 : NULL pointer is passed. */
    char e1110701[0x2b];  /* 0x0c3 E1110701:'tmpbuf' is null.(load partition) */
    char e1110702[0x28];  /* 0x0ee E1110702:'tbsize' <= 0.(load partition) */
    char e0042401[0x2f];  /* 0x116 E0042401:multi-load partition.(load partition) */
    char e9040804[0x2a];  /* 0x145 E9040804:can't open file.(load partition) */
    char e9040832[0x27];  /* 0x16f E9040832:'adxf' is NULL.(adxf_GetStat) */
    char e0041303[0x33];  /* 0x196 E0041303:illigal parameter 'ptid'.(adxf_GetPtStat) */
    char e0120401[0x34];  /* 0x1c9 E0120401:'buf' isn't 32byte alignment.(adxf_ReadNw) */
    char afs[0x4];  /* 0x1fd AFS */
    char e0040701[0x37];  /* 0x201 E0040701:Unacceptable format(not AFS).(adxf_GetPtStat) */
    char e0040702[0x36];  /* 0x238 E0040702:Unacceptable number of file.(adxf_GetPtStat) */
    char e2122501[0x44];  /* 0x26e E2122501:AFS file has 128MB or more of inside file.(adxf_GetPtStat) */
    char e04041201[0x34];  /* 0x2b2 E04041201:not enough ADXF handle (adxf_CreateAdxFs) */
    char e02111001[0x35];  /* 0x2e6 E02111001:can't create stm handle (adxf_CreateAdxFs) */
    char e9081901[0x3b];  /* 0x31b E9081901:illigal parameter fname=null.(adxf_SetFileInfoEx) */
    char e9040828[0x2b];  /* 0x356 E9040828:'adxf' is NULL.(adxf_GetFsizeSct) */
    char e2005121501[0x26];  /* 0x381 E2005121501 : NULL pointer is passed. */
    char e2005121502[0x26];  /* 0x3a7 E2005121502 : NULL pointer is passed. */
    char e2005121503[0x26];  /* 0x3cd E2005121503 : NULL pointer is passed. */
    char e9040811[0x28];  /* 0x3f3 E9040811:'adxf' is NULL.(adxf_ReadSj32) */
    char e9040812[0x2b];  /* 0x41b E9040812:'nsct'is negative.(adxf_ReadSj32) */
    char e9040813[0x25];  /* 0x446 E9040813:'sj'is NULL.(adxf_ReadSj32) */
    char e9040816[0x28];  /* 0x46b E9040816:'adxf' is NULL.(adxf_ReadNw32) */
    char e9040817[0x2c];  /* 0x493 E9040817:'nsct' is negative.(adxf_ReadNw32) */
    char e2006080701[0x2f];  /* 0x4bf E2006080701:Buffer size must be less than 2GB. */
    char e9040818[0x27];  /* 0x4ee E9040818:'buf' is NULL.(adxf_ReadNw32) */
    char e9040821[0x2b];  /* 0x515 E9040821:'sj' must be NULL.(adxf_ReadNw32) */
    char e9040822[0x24];  /* 0x540 E9040822:'adxf' is NULL.(adxf_Stop) */
    char e9040823[0x29];  /* 0x564 E9040823:'adxf->stm' is NULL.(adxf_Stop) */
    char e2092301[0x26];  /* 0x58d E2092301:'adxf' is NULL.(adxf_StopNw) */
    char e2092302[0x2b];  /* 0x5b3 E2092302:'adxf->stm' is NULL.(adxf_StopNw) */
    char e9040825[0x24];  /* 0x5de E9040825:'adxf' is NULL.(adxf_Seek) */
    char e9040826[0x27];  /* 0x602 E9040826:'type' is illigal.(adxf_Seek) */
    char e9040827[0x24];  /* 0x629 E9040827:'adxf' is NULL.(ADXF_Tell) */
    char e2005101910[0x2f];  /* 0x64d E2005101910:'adxf' is NULL.(adxf_GetFsizeByte) */
    char e2005101911[0x31];  /* 0x67c E2005101911:'adxf' is NULL.(adxf_GetFsizeByte64) */
    char e9040830[0x2c];  /* 0x6ad E9040830:'adxf' is NULL.(adxf_GetNumReqSct) */
    char e9040831[0x2d];  /* 0x6d9 E9040831:'adxf' is NULL.(adxf_GetNumReadSct) */
    char e9040828_2[0x27];  /* 0x706 E9040828:Partition ID is out of range. */
    char e2008110501[0x33];  /* 0x72d E2008110501:Loading of partition is not completed. */
    char e9040828_3[0x22];  /* 0x760 E9040828:File ID is out of range. */
    char e0041201[0x2c];  /* 0x782 E0041201:state is reading(ADXF_SetReqRdSct) */
    char e2005072901[0x25];  /* 0x7ae E2005072901: NULL pointer is passed. */
} ADXF_MES;
