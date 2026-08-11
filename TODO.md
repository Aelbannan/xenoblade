- [ ] Move all classes and structs into headers
- [ ] Get rid of all extern "C" imports
- [ ] Get rid of void *
- [ ] Convert possible member functions (symbols.txt)
- [ ] Convert possible static functions
- [ ] Get rid of pointer math
- [ ] Fix fake array accessors
- [ ] Extract repeated code into helper functions
- [ ] Combine duplicate classes/structs
- [ ] Get rid of vtable dispatch wrappers (or fix them?)
- [ ] Start renaming functions and labels + params + variables


NOTES
- harness needs better reporting
- web view sucks
- ideally a always on harness that can have runs launched from web view could be nice
- run ids should be stored in logs to filter by run
- Generic agents that tackle specific subsets of targets + ordering (EG. CODE_MATCH, smallest to largest)
- Auto promotion of units that match text/data/size
- Integrate promotion/diffing/etc into the harness
- maybe a header prepass?
