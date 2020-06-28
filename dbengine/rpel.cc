#include "rpel.h"

rpel::rpel() {

}

//-----------------------------------------------------------------------------

opStatus rpel::execute(database* dbObjRef, 
                       std::string& inBuffer, 
                       int clientSocket) {
  if(std::regex_match(inBuffer, exitRegex)) {
    return opStatus::opExit;
  }
  return evalOp(inBuffer, dbObjRef, clientSocket);
}

//-----------------------------------------------------------------------------

rpel::~rpel() {

}

//-----------------------------------------------------------------------------
