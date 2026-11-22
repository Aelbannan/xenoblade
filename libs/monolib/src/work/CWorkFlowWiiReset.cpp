#include "monolib/work.hpp"

CWorkFlowWiiReset* CWorkFlowWiiReset::spInstance;

CWorkFlowWiiReset* CWorkFlowWiiReset::getInstance(){
    return spInstance;
}
