#include "utilities.h"

vector<string>* get_elements(string line,string split)
{
     vector<string>* out = new vector<string>();

     char * c_line = new char[line.size() + 1];
     std::copy(line.begin(), line.end(), c_line);
     c_line[line.size()] = '\0';

     char *token = (strtok(c_line, split.c_str()));
     if(token != NULL)
     {
         out->push_back(string(token));
         while (token != NULL) {
             token = (strtok(NULL, split.c_str()));
             if (token != NULL)
                 out->push_back(string(token));
         }
     }
     return out;

}

int findId(vector<string>* list,int id){
    for(int i=0; i<list->size();i++)
    {
        if(id==atoi(list->at(i).c_str()))
        {
            return i;
        }
    }
    return -1;
}

void send_msg(int type, int dest)
{
    MPI_Send(&type,sizeof(int),MPI_BYTE,dest,0,MPI_COMM_WORLD);
}
int recv_msg(){
    int type;
    MPI_Status status;
    MPI_Recv(&type, sizeof(int),MPI_BYTE, 0, MPI_ANY_TAG,MPI_COMM_WORLD, &status);
    return type;
}
