#include "monolib/work.hpp"

// Retail places the singleton pointer in the .sbss data slice (the code TU
// split carries no data); declare it extern so this object defines no .sbss.
extern CWorkFlowWiiMenu* CWorkFlowWiiMenu::spInstance;

CWorkFlowWiiMenu* CWorkFlowWiiMenu::getInstance(){
    return spInstance;
}
