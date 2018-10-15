#ifndef NODO_H
#define NODO_H
#include <string>
#include <iostream>
#include <vector>
#include "arbol.h"

using namespace std;
class Arbol;
class Nodo
{
    Arbol* arbol; //arbol de directorios
    string nombre; //nombre del archivo o dir
    int id;
    int nivel; //profundidad en el arbol
    Nodo* padre;
    vector<Nodo*>* hijos;
    bool dir; // T->directorio F->archivo
    off_t tamano;// 4096bytes dir
    time_t ultima_mod;//ultima modificacion

public:
    Nodo(Arbol *arbol,int id, int nivel, string nombre, Nodo* padre, vector<Nodo*>* hijos,bool dir, off_t tam,time_t date);

    //geters
    Nodo* get_padre();
    vector<Nodo*>* get_hijos();
    int get_id();
    int get_nivel();
    Arbol* get_arbol();
    string get_nombre();
    bool get_type();
    off_t get_tam();
    time_t get_lastMod();

    //seters
    void set_name(string nombre);
    void set_tam(off_t tam);
    void set_lastMod(time_t date);

    //otras
    void add_hijo(Nodo* hijos);


};

#endif // NODO_H
