#ifndef RPEL_H
#define RPEL_H

#include "db.h"
#include "dbops.h"

class rpel {
  public:
    // Constructor
    rpel();
    // Start program
    opStatus execute(database* dbObjRef, 
                     std::string& inBuffer, 
                     int clientSocket);
    // Destructor
    ~rpel();
};

#endif
