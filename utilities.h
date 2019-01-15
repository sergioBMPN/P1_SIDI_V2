#ifndef UTILITIES_H
#define UTILITIES_H
#include<vector>
#define FREE true
#define USED false

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

#endif // UTILITIES_H
