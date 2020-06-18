#include "rpel.h"

rpel::rpel() {

}

//-----------------------------------------------------------------------------

bool rpel::execute(database* dbObjRef, std::string inBuffer, int clientSocket) {
  if(std::regex_match(inBuffer, exitRegex)) {
    return 0;
  }
  return evalOp(inBuffer, dbObjRef, clientSocket);
}

//-----------------------------------------------------------------------------

rpel::~rpel() {

}

//-----------------------------------------------------------------------------
