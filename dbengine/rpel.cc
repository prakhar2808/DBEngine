#include "rpel.h"

rpel::rpel() {

}

//-----------------------------------------------------------------------------

std::string rpel::execute(database* dbObjRef, std::string inBuffer) {
  if(std::regex_match(inBuffer, exitRegex)) {
    return "exit";
  }
  return evalOp(inBuffer, dbObjRef);
}

//-----------------------------------------------------------------------------

rpel::~rpel() {

}

//-----------------------------------------------------------------------------
