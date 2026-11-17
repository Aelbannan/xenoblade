#include "monolib/nand/CNand.hpp"

#include <revolution/NAND.h>

// Retail constructs this object without registering its destructor.
#pragma no_static_dtors on
CNand nand;
#pragma no_static_dtors reset

CNand::~CNand() {
}

CNand::CNand() {
    mHead = 0;
    mTail = 0;
    mShare = false;
}

void CNand::reset() {
    mHead = 0;
    mTail = 0;
    mSuccess = true;
}

void CNand::update() {
    bool success;

    while (mRequests[mHead].mTask != NULL) {
        if (!mRequests[mHead].update(&success)) {
            break;
        }
        mSuccess = success;
        mHead++;
        mHead %= REQUEST_MAX;
    }
}

bool CNand::check(u32 neededBlocks, u32 neededFiles, u32* answer) {
    CNRequest* request = alloc();
    if (request != NULL) {
        return request->check(neededBlocks, neededFiles, answer);
    }
    return false;
}

bool CNand::save(const char* name, const void* buf, u32 size, CNBanner* banner, u8 perm) {
    if (mShare) {
        CNRequest* request = alloc();
        if (request == NULL) {
            return false;
        }
        if (!request->mkdir("share", NAND_PERM_RUSR | NAND_PERM_WUSR | NAND_PERM_RGRP, false)) {
            return false;
        }
    }

    CNRequest* request = alloc();
    if (request == NULL) {
        return false;
    }
    if (!request->save(name, buf, size, perm, mShare)) {
        return false;
    }

    if (banner != NULL) {
        CNRequest* request = alloc();
        if (request == NULL) {
            return false;
        }
        if (!request->saveBanner(banner, false)) {
            return false;
        }
    }
    return true;
}

bool CNand::load(const char* name, void* buf, u32 size) {
    CNRequest* request = alloc();
    if (request == NULL) {
        return false;
    }
    return request->load(name, buf, size, mShare);
}

bool CNand::remove(const char* name) {
    CNRequest* request = alloc();
    if (request == NULL) {
        return false;
    }
    return request->remove(name, mShare);
}

bool CNand::readdir(char* nameList, u32 size, u32* num) {
    CNRequest* request = alloc();
    if (request == NULL) {
        return false;
    }
    return request->readdir(nameList, size, num, mShare);
}

bool CNand::saveBanner(CNBanner* banner) {
    CNRequest* request = alloc();
    if (request == NULL) {
        return false;
    }
    return request->saveBanner(banner, false);
}

bool CNand::loadBanner(void* buf, u32 size) {
    CNRequest* request = alloc();
    if (request == NULL) {
        return false;
    }
    return request->load("banner.bin", buf, size, false);
}

CNRequest* CNand::alloc() {
    s16 next = mTail + 1;
    next %= REQUEST_MAX;
    if (next == mHead) {
        return NULL;
    }
    CNRequest* request = &mRequests[mTail];
    mTail = next;
    return request;
}
