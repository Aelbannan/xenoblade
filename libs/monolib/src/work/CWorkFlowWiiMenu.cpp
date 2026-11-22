#include "monolib/work.hpp"

CWorkFlowWiiMenu* CWorkFlowWiiMenu::spInstance;

CWorkFlowWiiMenu* CWorkFlowWiiMenu::getInstance(){
    return spInstance;
}
