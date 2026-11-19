#ifndef NW4R_SND_MML_SEQ_TRACK_H
#define NW4R_SND_MML_SEQ_TRACK_H
#include <nw4r/types_nw4r.h>

#include <nw4r/snd/snd_MmlParser.h>
#include <nw4r/snd/snd_SeqTrack.h>

namespace nw4r {
namespace snd {
namespace detail {

class MmlSeqTrack : public SeqTrack {
public:
    // The MML interpreter state lives in the track's parser parameters, so the
    // accessor simply hands out that sub-object.
    typedef SeqTrack::MmlParserParam MmlParserParam;

public:
    MmlSeqTrack();
    virtual ~MmlSeqTrack();
    virtual ParseResult Parse(bool doNoteOn); // at 0xC

    void SetMmlParser(const MmlParser* pParser) {
        mParser = pParser;
    }

    MmlParserParam& GetMmlParserParam() {
        return GetParserTrackParam().mmlParserParam;
    }

private:
    const MmlParser* mParser; // at 0xC8
};

} // namespace detail
} // namespace snd
} // namespace nw4r

#endif
