#include <iostream>
#include "terminal.h"
#include "utilities.h"
#include "slave.h"
#include "mpi/mpi.h"
using namespace std;

int main(int argc, char *argv[])
{
    int rank = 0;
    int numProc = 0;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if(rank==0){
        cout<<"Hola soy el master"<<endl;
        Terminal *term = new Terminal(32,numProc-1);
        term->run();
    }
    else{
        cout<<"Hola soy un escalvo"<<endl;
        bool exit= false;
        Slave *slave=new Slave();
        while(!exit)
        {
            int msg=recv_msg();

            switch (msg) {
            case MSG_DWL:
                slave->read();
                break;
            case MSG_UPL:
                slave->write();
                break;
            case MSG_FRM:
                slave->format();
                break;
            case MSG_EXIT:
                exit=true;
                break;
            }
        }

    }
    MPI_Finalize();
    return 0;
}
