#ifndef ARBOL_H
#define ARBOL_H
#include <string>
#include "nodo.h"
#include<iostream>
#include <vector>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

using namespace std;
class Nodo;
class Arbol
{
   vector<Nodo*>* listaNodos;
   int last_id;
   Nodo* root;
   vector<Nodo*>* pwd;
   bool recursive_dir(vector<Nodo*>* dir,vector<Nodo*>* hijos);

public:
    Arbol();
    void add_child(Nodo* padre,string new_nombre,bool dir);
    int move_to(vector<Nodo*>* dir);
    bool is_dir(vector<Nodo*>* dir);
    Nodo* find_child(Nodo* padre, string hijo);
    int update_child(Nodo* nodo,string nombre, time_t date,off_t tam);
    int delete_child(Nodo* nodo);

    Nodo* get_root();
	vector<Nodo*>* get_pwd();
	Nodo* get_nodo(string nombre);
    void pwd_tostring();
};
#endif // ARBOL_H
