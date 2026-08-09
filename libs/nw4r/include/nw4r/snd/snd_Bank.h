#ifndef NW4R_SND_BANK_H
#define NW4R_SND_BANK_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_BankFile.h>
#include <nw4r/snd/snd_NoteOnCallback.h>

namespace nw4r {
namespace snd {
namespace detail {

// Forward declarations
class Channel;

class Bank {
public:
    explicit Bank(const void* pBankBin);
    ~Bank();

    Channel* NoteOn(const NoteOnInfo& rInfo) const;

    void SetWaveDataAddress(const void* pData) {
        mWaveDataAddress = static_cast<const u8*>(pData);
    }

private:
    BankFileReader mBankReader;   // at 0x0
    const u8* mWaveDataAddress;   // at 0xC
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
