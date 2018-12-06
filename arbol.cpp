#include "arbol.h"


 Arbol::Arbol(HardDisc* disc,bool first_init=true)
 {
    listaNodos = new vector<Nodo*>();
	pwd = new vector<Nodo*>();
    last_id=0;
    mod=false;
    hardDisc=disc;
    if(first_init)
    {
        time_t t = time(0);
        root = new Nodo(this,0,0,"Root",NULL,NULL,true,4096,t,NULL);
        listaNodos->push_back(root);
        pwd->push_back(root);
    }
 }

 Nodo* Arbol::add_dir(Nodo* padre,string new_nombre,vector<Nodo*>* hijos)
 {
    time_t t=time(0);
    last_id++;
    int level=(padre->get_nivel()+1);
    Nodo *n_nodo= new Nodo(this,last_id,level,new_nombre,padre,hijos,true,4096,t,NULL);

    padre->add_hijo(n_nodo);
    listaNodos->push_back(n_nodo);
    return n_nodo;
 }

 Nodo* Arbol::add_recursive_dir(Nodo* org,Nodo* dest)
 {
    if(org->get_type() && dest->get_type())//deben ser dos dir
    {
        if(org->get_hijos()!=NULL)
        {
            Nodo* n_nodo, *nodo;
            for(int i=0;i<org->get_hijos()->size();i++)
            {
                nodo =org->get_hijos()->at(i);
                if(nodo->get_type())// si es un directorio
                {
                    //creamos el nuevo nodo
                    n_nodo=add_dir(dest,nodo->get_nombre(),NULL);

                    //recusive
                    add_recursive_dir(nodo,n_nodo);
                }
                else //si es un archivo
                    n_nodo= add_file(dest,nodo->get_nombre(),nodo->get_tam());
            }
        }
        return dest;
    }
 }

 Nodo* Arbol::add_file(Nodo* padre,string new_nombre,off_t size)
 {
     time_t t=time(0);
     last_id++;
     Nodo *n_nodo= new Nodo(this,last_id,(padre->get_nivel()+1),new_nombre,padre,NULL,false,size,t,NULL);

     //añadir el nod al hd
     hardDisc->writeFile(n_nodo);

     padre->add_hijo(n_nodo);
     listaNodos->push_back(n_nodo);
     return n_nodo;

 }

 int Arbol::move_to(vector<Nodo*>* dir)
 {
     if (is_dir(dir))
        pwd = dir;
     else
        return -1;

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

 int Arbol::delete_child(Nodo* padre ,Nodo* hijo)
 {
	 vector<Nodo*> *hijos = padre->get_hijos();
	 int id_borrar=-1;
	 
	 if (hijos != NULL)
		 for (int i = 0; i < hijos->size(); i++)
			 if (hijos->at(i)->get_id() == hijo->get_id())
				 id_borrar = i;
	 if (id_borrar != -1)
	 {
		 hijos->erase(hijos->begin() + id_borrar);
		 listaNodos->erase(listaNodos->begin() + hijo->get_id());
	 }

	 return id_borrar;
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
	 if (nombre.compare("root") == 0)
		 return root;

	 Nodo* nodo_activo = get_pwd()->back();	 
	 vector<Nodo*>*hijos = nodo_activo->get_hijos();
     //cout << hijos->size()<<endl;
	 if(hijos!=NULL)
		 for (int i = 0; i<nodo_activo->get_hijos()->size(); i++)
             if (hijos->at(i)->get_nombre().compare(nombre)==0)		//buscar en hijos
                 return hijos->at(i);

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

 void Arbol::save_arbol(){
     string file="arbol.dat";
     Nodo* nodo;
     Nodo* padre;
     vector<Nodo*>* hijos;
     string line;

     remove(file.c_str());
     ofstream f(file.c_str(),ios::app | ios::binary);

     line = to_string(last_id);
     line+=",";
     line += to_string(listaNodos->size());
     line += "\n";
     f.write(line.c_str(),line.size());

    for(int i=0;i<listaNodos->size();i++)
    {
        nodo=listaNodos->at(i);
        padre=nodo->get_padre();
        hijos=nodo->get_hijos();

        line = to_string(nodo->get_id());
        line+=",";
        line+=to_string(nodo->get_nivel());
        line+=",";
        line+=nodo->get_nombre();
        line+=",";
        if(padre!=NULL)
            line+=to_string(padre->get_id());
        else
            line+="-1";
        line+=",(";
        if(hijos!=NULL) {
            for(int j=0;j<hijos->size();j++)
            {
                if(j>0)
                    line+="/";
                line+=to_string(nodo->get_hijos()->at(j)->get_id());
            }
        }
        line+="),";
        line+=to_string(nodo->get_type());
        line+=",";
        line+=to_string(nodo->get_tam());
        line+=",";
        line+=to_string(nodo->get_lastMod());
        line+=",(";
        if(nodo->get_blocks()!=NULL) {
            for(int i=0;i<nodo->get_blocks()->size();i++)
            {
                if(i>0)
                    line+="/";
                line+=to_string(nodo->get_blocks()->at(i));
            }
        }
        line+=")";
        line+="\n";
        //cout<<line.length()<< " "<<line.size()<<endl;

        f.write(line.c_str(),line.size());
     }
     f.close();
 }

int Arbol::load_arbol()
{
    //añadir comprobacion del tamaño del arbol
    string file="arbol.dat";
    string line;
    ifstream f(file.c_str());
    vector<string>* vectorHijos= new vector<string>;
    int result=-1;
    try{
        if(f.is_open())
        {
            vector<string>* line_info;
            getline(f,line);
            line_info = get_elements(line,",");
            last_id = atoi(line_info->at(0).c_str());
            int numNodos = atoi(line_info->at(1).c_str());


            while(getline(f,line))
            {
                //variables de nodo
                Nodo *newNodo;
                int  id,nivel;
                string nombre;
                Nodo* padre;
                bool dir;
                off_t tam;
                time_t date;
                vector<int>* blocks=NULL;


                //coger elemento linea
                line_info = get_elements(line,",");
                if(line_info->size()==0)
                    break;
                //id
                id = atoi(line_info->at(0).c_str());
                //nivel
                nivel = atoi(line_info->at(1).c_str());
                //nombre
                nombre = line_info->at(2);
                //padre
                int idPadre=atoi(line_info->at(3).c_str());
                if(idPadre ==-1)
                    padre=NULL;
                else
                    padre=listaNodos->at(idPadre);

                //hijos
                    // los hijos se añaden desde ellos mismos, no desde el padre
                vectorHijos->push_back(line_info->at(4));
                //is_dir
                if( atoi(line_info->at(5).c_str())==0)
                    dir=false;
                else
                    dir=true;
                //tamano
                tam= atoi(line_info->at(6).c_str());
                //fecha
                date= atoi(line_info->at(7).c_str());
                //bloques de datos en hd
                if(!dir)
                {
                    vector<string>* aux=get_elements(line_info->at(8).c_str(),"(/)");
                    for(int i=0;i<aux->size();i++)
                        blocks->push_back(atoi(aux->at(i).c_str()));
                }

                newNodo= new Nodo(this,id,nivel,nombre,padre,NULL,dir,tam,date,blocks);
                listaNodos->push_back(newNodo);
                if(id==0)
                {
                    pwd->push_back(newNodo);
                    root=newNodo;
                }
                if(padre!=NULL)
                {
                    padre->add_hijo(newNodo);
                }


            }
            if(numNodos==listaNodos->size())
                result=1;
            for(int i=0;i<listaNodos->size();i++){
                if(listaNodos->at(i)->get_padre()==NULL && listaNodos->at(i)->get_id()!=0)
                    result=-1;
            }
        }
    }
    catch(int e)
    {
        cout<<"ha sudedido un error "<< e<<endl;
    }
    return result;

}

vector<string>* Arbol::get_elements(string line,string split)
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

bool Arbol::is_mod()
{
#pragma omp flush(mod)
    return mod;
}

void Arbol::set_mod(bool mod)
{
    this->mod=mod;
#pragma omp flush(mod)
}


