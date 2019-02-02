#ifndef SLAVE_H
#define SLAVE_H
#include "mpi/mpi.h"
#include "utilities.h"
#include <iostream>

class Slave
{
public:
    Slave();
    void write();
    void read();
    void format();
};

#endif // SLAVE_H
