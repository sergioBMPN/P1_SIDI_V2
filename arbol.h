#pragma once
#include <string>
#include "nodo.h"
#include<iostream>
#include <vector>
#include <sys/stat.h>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include "harddisc.h"
#include "utilities.h"
using namespace std;

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

class Nodo;
class HardDisc;
class Arbol
{
   vector<Nodo*>* listaNodos;
   int last_id;
   bool mod;
   Nodo* root;
   vector<Nodo*>* pwd;
   HardDisc* hardDisc;

   bool recursive_dir(vector<Nodo*>* dir,vector<Nodo*>* hijos);

public:
    Arbol(HardDisc *disc, bool first_init);
    Nodo* add_dir(Nodo* padre,string new_nombre,vector<Nodo*>* hijos);
    Nodo* add_recursive_dir(Nodo* org,Nodo* dest);
    Nodo* add_file(Nodo* padre,string new_nombre,off_t size);
    int move_to(vector<Nodo*>* dir);
    bool is_dir(vector<Nodo*>* dir);
    Nodo* find_child(Nodo* padre, string hijo);
    int update_child(Nodo* nodo,string nombre, time_t date,off_t tam);
    int delete_child(Nodo* padre,Nodo* nodo);
    void save_arbol();
    int load_arbol();

    Nodo* get_root();
	vector<Nodo*>* get_pwd();
	Nodo* get_nodo(string nombre);
    bool is_mod();
    void set_mod(bool mod);
    void pwd_tostring();
};
// ARBOL_H
