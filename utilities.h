#pragma once
#include<vector>
#include<string>
#include <string.h>
#include"mpi/mpi.h"

#define FREE true
#define USED false

#define MSG_EXIT 0
#define MSG_UPL 1
#define MSG_DWL 2
#define MSG_FRM 3

using namespace std;
typedef struct comando_t
{
    int tipo;
    vector<char*>* argumentos;
    int num_argumentos;
}comando_t;

class Terminal;
class Arbol;
class Nodo;
typedef struct watchDog_t
{
    bool* exit;
    Arbol* arbol;
    Terminal* terminal;
}watchDog_t;


typedef struct block_t{
    int blockId;//identificador de bloque
    bool status;//false: ocupado; true: libre
    char* info;//info a guardar
}block_t;

vector<string>* get_elements(string line,string split);
int findId(vector<string> *list,int id);

void send_msg(int type, int dest);
int recv_msg();

// UTILITIES_H
