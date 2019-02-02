#include "slave.h"

Slave::Slave()
{

}

void Slave::write(){
    MPI_Status status;
    //recibir posicion fseek
    int f_pos=0;
    MPI_Recv(&f_pos, 1, MPI_INT, 0, MSG_UPL,MPI_COMM_WORLD, &status);
    //recibir tam block
    int b_size=0;
    MPI_Recv(&b_size, 1, MPI_INT, 0, MSG_UPL,MPI_COMM_WORLD, &status);
    //recibir block
    char* buff=(char*)malloc(sizeof(char)*b_size+1);
    MPI_Recv(buff, b_size, MPI_CHAR, 0, MSG_UPL,MPI_COMM_WORLD, &status);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    string fname="HDisc";
    fname+=to_string(rank);
    fname+=".dat";

    FILE *disc = fopen(fname.c_str(),"a");
    if(disc!= NULL)
    {
        fseek(disc,f_pos,SEEK_SET);
        fwrite(buff,sizeof(char),b_size,disc);
        fclose(disc);
    }
}

void Slave::read(){
    MPI_Status status;
    //recibir posicion fseek
    int f_pos=0;
    MPI_Recv(&f_pos, 1, MPI_INT, 0, MSG_DWL,MPI_COMM_WORLD, &status);
    //recibir tam block
    int b_size=0;
    MPI_Recv(&b_size, 1, MPI_INT, 0, MSG_DWL,MPI_COMM_WORLD, &status);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    string fname="HDisc";
    fname+=to_string(rank);
    fname+=".dat";

    FILE *disc = fopen(fname.c_str(),"r");
    if(disc!= NULL)
    {
        fseek(disc,f_pos,SEEK_SET);
        char* buff=(char*)malloc(sizeof(char)*b_size+1);
        fread(buff,sizeof(char),b_size,disc);
        MPI_Send(buff,b_size,MPI_CHAR,0,MSG_DWL,MPI_COMM_WORLD);
        fclose(disc);
    }
}

void Slave::format(){

}
