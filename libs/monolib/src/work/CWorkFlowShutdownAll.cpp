#include "monolib/work.hpp"

CWorkFlowShutdownAll* CWorkFlowShutdownAll::spInstance;

CWorkFlowShutdownAll* CWorkFlowShutdownAll::getInstance(){
    return spInstance;
}
