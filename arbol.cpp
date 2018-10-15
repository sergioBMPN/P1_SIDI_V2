#include "arbol.h"

 Arbol::Arbol()
 {
    time_t t = time(0);
	//cout << asctime(gmtime(&t)) << endl;
    root = new Nodo(this,0,0,"Root",NULL,NULL,true,4096,t);
    listaNodos = new vector<Nodo*>();
	pwd = new vector<Nodo*>();

    listaNodos->insert(listaNodos->begin(),root);
    last_id=0;
	pwd->insert(pwd->begin(),root);
 }

 void Arbol::add_child(Nodo* padre,string new_nombre,bool dir)
 {
    time_t t=time(0);
    last_id++;
    Nodo *n_nodo= new Nodo(this,last_id,(padre->get_nivel()+1),new_nombre,padre,NULL,dir,4096,t);

    padre->add_hijo(n_nodo);
    listaNodos->push_back(n_nodo);
 }

 int Arbol::move_to(vector<Nodo*>* dir)
 {
     if (is_dir(dir))
        pwd = dir;
     else
        return 0;

    return 1;
 }

 bool Arbol::recursive_dir(vector<Nodo*>* dir, vector<Nodo*>* hijos)
 {
     //TODO Check is_dir o archivo

     if(dir->size()==0)
         return true;

	 bool is_dir = false;
	 Nodo * root = dir->at(0);
	 Nodo* hijo;

     for(int i=0;i<hijos->size();i++)
     {
         hijo=hijos->at(i);
         if(root->get_id() == hijo->get_id())
         {
            vector<Nodo*> n_dir(dir->begin()+1,dir->end());
            is_dir=recursive_dir(&n_dir,root->get_hijos());
         }
     }
     return is_dir;
 }

 bool Arbol::is_dir(vector<Nodo*>* dir)
 {
     bool is_dir=false;
     Nodo* root=dir->at(0);

     if(dir->back()->get_type()==false)
         return is_dir;

     if(root->get_id()==0)
     {
         vector<Nodo*> n_dir(dir->begin()+1,dir->end());
         is_dir=recursive_dir(&n_dir,root->get_hijos());
     }
     else
         is_dir=recursive_dir(dir,root->get_hijos());
	 return is_dir;
 }

 Nodo* Arbol::find_child(Nodo* padre, string hijo){
	 if(padre->get_hijos()!=NULL)
		 for(int i=0;i<padre->get_hijos()->size();i++)
			 if(padre->get_hijos()->at(i)->get_nombre()==hijo)
				 return padre->get_hijos()->at(i);
     return NULL;
 }

 int Arbol::update_child(Nodo* nodo,string nombre="", time_t date=(time_t)(-1),off_t tam=0){

     try{
         if(nombre!="")
             nodo->set_name(nombre);

         if(date!=(time_t)(-1))
             nodo->set_lastMod(date);

         if(tam!=0)
             nodo->set_tam(tam);

     }
     catch(int e)
     {
         cout << "An exception occurred. Exception Nr. " << e << '\n';
         return 0;
     }
     return 1;


 }

 int Arbol::delete_child(Nodo* nodo)
 {
     if(nodo->get_hijos()!=NULL)
        return 0;

     int pos=-1;
     for(int i=0;i<listaNodos->size();i++)
        if(listaNodos->at(i)->get_id()==nodo->get_id())
            pos=i;
     if(pos<0)
         return 0;

     listaNodos->erase(listaNodos->begin()+pos);
     return 1;
 }

 Nodo * Arbol::get_root()
 {
	 return this->root;
 }

 vector<Nodo*>* Arbol::get_pwd()
 {
	 return this->pwd;
 }

 Nodo * Arbol::get_nodo(string nombre)
 {
	 if(!nombre.empty())
		 for (int i = 0; i<listaNodos->size(); i++)
			 if (listaNodos->at(i)->get_nombre() == nombre)			 
				 return listaNodos->at(i);

	 return NULL;
 }

 void Arbol::pwd_tostring()
 {
	 vector<Nodo*>* nodos = get_pwd();
	 for (int i = 0; i < nodos->size(); i++) {
		 if (nodos->at(i)->get_id() == 0)
			 cout << "~";
		 else
			cout << nodos->at(i)->get_nombre();
		 if(!(i == nodos->size()-1))
			cout << "/";
	 }

 }



