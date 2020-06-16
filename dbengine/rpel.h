#ifndef RPEL_H
#define RPEL_H

#include "db.h"
#include "dbops.h"

class rpel {
  public:
    // Constructor
    rpel();
    // Start program
    std::string execute(database* dbObjRef, std::string inBuffer);
    // Destructor
    ~rpel();
};

#endif
