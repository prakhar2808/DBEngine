#include "rpel.h"

int main() {
  database* dbObject = new database();
  std::string inBuffer;
  printWelcome();
  while(true) {
    std::cout<<">> ";
    std::getline(std::cin, inBuffer);
    if(std::regex_match(inBuffer, exitRegex)) {
      printExit();
      break;
    }
    evalOp(inBuffer, dbObject);
  }
  return 0;
}

//*****************************************************************************

void printWelcome() {
  std::cout<<"*************************************************************\n"; 
  std::cout<<"*************************************************************\n"; 
  std::cout<<"*                Welcome to myDB Engine.                    *\n"; 
  std::cout<<"*              Created by : Prakhar Sinha                   *\n"; 
  std::cout<<"*************************************************************\n"; 
  std::cout<<"*********** Type EXIT to terminate the engine ***************\n"; 
  std::cout<<"*************************************************************\n"; 
}

//*****************************************************************************

void printExit() {
  std::cout<<"*************************************************************\n"; 
  std::cout<<"*                      Good Bye!                            *\n";
  std::cout<<"*************************************************************\n";
}

//*****************************************************************************
