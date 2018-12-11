#ifndef UTILITIES_H
#define UTILITIES_H
#include<vector>
#include<string>
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
#endif // UTILITIES_H
