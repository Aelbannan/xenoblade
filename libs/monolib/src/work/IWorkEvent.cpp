#include "monolib/work/IWorkEvent.hpp"

// Default virtual bodies (WorkEvent1..31, OnFileEvent, OnPauseTrigger) live in
// kyoshin/CGame.cpp to match retail weak placement. Only the dtor stays here.
IWorkEvent::~IWorkEvent() {}
