#include "nodo.h"

Nodo::Nodo(Arbol *arbol,int id, int nivel, string nombre, Nodo* padre, vector<Nodo*>* hijos,bool dir, off_t tam,time_t date)
{
    this->arbol=arbol;
    this->id = id;
    this->nombre= nombre;
    this->padre=padre;
    this->hijos = hijos;
    this->dir=dir;
    this->tamano=tam;
    this->ultima_mod=date;
}

Nodo* Nodo::get_padre(){
    return this->padre;
}
string Nodo::get_nombre(){
    return this->nombre;
}
vector<Nodo*>*Nodo::get_hijos(){
    return this->hijos;
}
int Nodo::get_id(){
    return this->id;
}

int Nodo::get_nivel(){
    return this->nivel;
}

Arbol* Nodo::get_arbol(){
    return this->arbol;
}

bool Nodo::get_type(){
    return this->dir;
}

off_t Nodo::get_tam(){
    return this->tamano;
}

time_t Nodo::get_lastMod(){
    return this->ultima_mod;
}

//seters
void Nodo::set_name(string nombre){
    this->nombre = nombre;
}
void Nodo::set_tam(off_t tam){
    this->tamano = tam;
}

void Nodo::set_lastMod(time_t date){
    this->ultima_mod = date;
}

//otras
void Nodo::add_hijo(Nodo* hijo){
	if (this->hijos == NULL)
		hijos = new vector<Nodo*>;
	this->hijos->push_back(hijo);
		//insert(this->hijos->end(),hijos);
}
