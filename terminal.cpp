#include "terminal.h"

Terminal::Terminal(long int discSize, int n_disc)
{
    struct dirent **namelist;
    int size=scandir(".",&namelist,NULL,alphasort);
    int find=0;
    for(int i=0;i<size;i++) {
        char* name=namelist[i]->d_name;
        //si existe
        if(!strncmp("arbol.dat",name,9)&&strlen(name)==9)
        {
            find++;
        }
        if(!strncmp("HD.dat",name,6)&&strlen(name)==6)
        {
            find++;
        }
        if(find==2)
        {
            break;
        }
    }
    this->hardDisc= new HardDisc(discSize*1024*1024,n_disc,1024);
    if(find>1)// si lo encuentra
    {
        printf("Cargando sistema de ficheros\n");
        if(hardDisc->loadHD()==-1)
        {
            cout<< "No se ha cargado el HD correctamente, archivo HD.dat corrupto"<<endl;
            exit(-1);
        }
        arbol=new Arbol(hardDisc,false);
        if(arbol->load_arbol()==-1)
        {
            cout<< "No se ha cargado el arbol correctamente, archivo arbol.dat corrupto"<<endl;
            exit(-1);
        }
    }
    else if(find==1)
    {
        printf("Cargando sistema de ficheros\n");
        arbol=new Arbol(hardDisc,false);
        if(arbol->load_arbol()==-1)
        {
            cout<< "No se ha cargado el arbol correctamente, archivo arbol.dat corrupto"<<endl;
            exit(-1);
        }
    }
    else
    {
        arbol=new Arbol(hardDisc,true);
    }


}


void * Terminal::comm_exe(void*args)
{
    comando_t comando;
    watchDog_t* wd=(watchDog_t*)args;
    bool *exit=wd->exit;

    while(!(*exit)){
        comando.argumentos = new vector<char*>();
        wd->terminal->pintar_terminal();
        if(wd->terminal->leer_comando(&comando)==0)
            wd->terminal->ejecutar_comando(&comando);
        free(comando.argumentos);
    }
    pthread_exit(NULL);
}
void* Terminal::watch_dog(void* args)
{
    watchDog_t* wd=(watchDog_t*)args;
    bool *exit=wd->exit;
    while(!(*exit)){
        usleep(10000*20);

        if(wd->arbol->is_mod()){
            wd->arbol->save_arbol();
            wd->arbol->set_mod(false);
        }
    }
    pthread_exit(NULL);
}
void Terminal::run()
{
    bool exit=false;
    pthread_t threads[2];
    watchDog_t *wd = new watchDog_t;
    wd->arbol=this->arbol;
    wd->exit=&exit;
    wd->terminal=this;
    /* int rank = 0;
     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

     if(rank==0){}
    /*/
    pthread_create(&threads[0],NULL,Terminal::comm_exe,wd);
    pthread_create(&threads[1],NULL,Terminal::watch_dog,wd);
    pthread_join(threads[0],NULL);
    pthread_join(threads[1],NULL);//*/
}
//leer y ejecutar comm
int Terminal::leer_comando(comando_t *comando){

    //string line;
    char* line=new char [1024];
    char split[2]=" ";
    char* token=NULL;

    //leer terminal
    fgets(line,1023,stdin);
    //std::getline(std::cin,line);
    //gets("%s",line);
    strtok(line, "\n");

    //gestion errores
        //comando no introducido
	if (comando != NULL)
	{
		//coger comando
        token = strtok(line, split);
		int type = get_tipo_comm(token);
		comando->tipo = type;

		if (type >= 0)
		{
			//subdividir arg
			while (token != NULL) {
				token = strtok(NULL, split);
				if (token != NULL)
					comando->argumentos->push_back(token);
            }
		}
        return 0;
	}
    return -1;
}

int Terminal::get_tipo_comm(char* comando){
    //Errores, si comando es null, las mayusculas estan permitidas

        if(comando != NULL){
			for (int i = 0; i < strlen(comando); i++)
				comando[i] = tolower(comando[i]);

            if(!strncmp("cp", comando, 2) && strlen(comando) == 2){
                return CMD_CP;
            }
			if(!strncmp("cd", comando, 2) && strlen(comando) == 2){
                return CMD_CD;
            }
            if(!strncmp("mv", comando, 2) && strlen(comando) == 2){
                return CMD_MV;
            }
			if (!strncmp("rm", comando, 2) && strlen(comando) == 2) {
				return CMD_RM;
			}
            if(!strncmp("mkdir", comando, 5) && strlen(comando) == 5){
                return CMD_MKDIR;
            }
			if (!strncmp("rmdir", comando, 5) && strlen(comando) == 5) {
				return CMD_RMDIR;
			}
			if (!strncmp("pwd", comando, 3) && strlen(comando) == 3) {
				return CMD_PWD;
			}
			if (!strncmp("ls", comando, 2) && strlen(comando) == 2) {
				return CMD_LS;
			}
            if (!strncmp("lls", comando, 3) && strlen(comando) == 3) {
                return CMD_LLS;
            }
            if (!strncmp("lpwd", comando, 4) && strlen(comando) == 4) {
                return CMD_LPWD;
            }
            if (!strncmp("lcd", comando, 3) && strlen(comando) == 3) {
                return CMD_LCD;
            }
            if (!strncmp("upload", comando, 6) && strlen(comando) == 6) {
                return CMD_UPL;
            }
            if (!strncmp("download", comando, 8) && strlen(comando) == 8) {
                return CMD_DWL;
            }
            if (!strncmp("format", comando, 6) && strlen(comando) == 6) {
                return CMD_FRM;
            }
            if(!strncmp("exit", comando, 4) && strlen(comando) == 4){
                return CMD_EXIT;
            }
        }

        return -1;
}

void Terminal::ejecutar_comando(comando_t *comando){

    switch(comando->tipo){
    case CMD_CD:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            cd(comando);
        break;
    case CMD_CP:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            cp(comando);
        break;
    case CMD_MV:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            mv(comando);
        break;
    case CMD_MKDIR:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            mkdir(comando);
        break;
    case CMD_PWD:
        pwd();
        break;
    case CMD_RMDIR:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            rmdir(comando);
        break;
    case CMD_RM:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            rm(comando);
        break;
    case CMD_LS:
        ls();
        break;
    case CMD_LPWD:
        lpwd();
        break;
    case CMD_LLS:
        lls();
        break;
    case CMD_LCD:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            lcd(comando);
        break;
    case CMD_UPL:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            upload(comando);
        break;
    case CMD_DWL:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            download(comando);
        break;
    case CMD_FRM:
        if(comando->argumentos->size()==0)
            cout <<"Debe introducir argumentos en la sentencia"<< endl;
        else
            format(comando);
        break;
    case CMD_EXIT:
        shut_down();
        break;
    default:
        printf("Commando not found\n");
        break;
    }
}


//comandos

void Terminal::cd(comando_t * comm)
{
	int end = 0;
	char split[2] = "/";
    string token;
	Nodo* nodo;
    Nodo *padre;
	vector<Nodo*>* path = new vector<Nodo*>;
    vector<string> *line;
	vector<Nodo*>* pwd_actual = new vector<Nodo*>;


	// comandos especiales 1
	if (!strncmp(comm->argumentos->at(0), "/", 1))
	{
		path->push_back(arbol->get_root());
        end = 1;
	}
	// comandos especiales 2
	else if (!strncmp(comm->argumentos->at(0), "..", 2)) {

		pwd_actual = arbol->get_pwd();
		for (int i = 0; i < pwd_actual->size() - 1; i++)
			path->push_back(pwd_actual->at(i));

		if (path->size() > 0)
			end = 1;
		else
			end = 2;
	}
	//caso normal
    else
    {
        end=1;
        line = get_elements(comm->argumentos->at(0), split);

        for(int i=0;i<line->size();i++)
        {
            token= line->at(i);
            if(i==0)//si el primero no es root tengo que comprobar que sea hijo del pwd
            {
                if(0!=strncmp(token.c_str(), arbol->get_root()->get_nombre().c_str(),4))//si no es root
                {
                   padre = arbol->get_pwd()->back();
                   nodo =arbol->find_child(padre,token);
                   if(nodo==NULL)//si no es hijo
                   {
                       cout <<"Error 1.0 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                       end=-1;
                       break;
                   }
                   else // si existe
                       for (int i = 0; i < arbol->get_pwd()->size(); i++)//añadimos toda la ruta hasta el nodo actual
                           path->push_back(arbol->get_pwd()->at(i));
                }
                // si el nodo es root la ruta a la que vamos a ir parte desde aqui
                nodo=arbol->get_nodo(token);
                path->push_back(nodo);
            }
            else// el resto de iteraciones
            {
                nodo=arbol->find_child(path->back(),token);
                if(nodo!=NULL)//si es hijo de su padre
                {
                    path->push_back(nodo);
                }
                else// si no es su hijo
                {
                    cout <<"Error 2.0 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                    end=-1;
                    break;
                }
            }
        }
    }

	//cambiar el pwd
	if (end == 1)
		end = arbol->move_to(path);

}

void Terminal::mkdir(comando_t * comm)
{
    arbol->set_mod(true);
    char split[2] = "/";
    string token;
    Nodo* nodo;
    Nodo* padre;

    vector<string> *line = get_elements(comm->argumentos->at(0),split);
    if(line->size()>0)
    {
        for(int i=0;i<line->size();i++)
        {
            token = line->at(i);
            nodo = arbol->get_nodo(token);

            if(!strncmp("..", token.c_str(), 2) && strlen(token.c_str()) == 2
                    || !strncmp(".", token.c_str(), 1) && strlen(token.c_str()) == 1
                    || !strncmp("/", token.c_str(), 1) && strlen(token.c_str()) == 1)
            {
                cout << "Error los nombres '.', '/' y '..' son comandos reservados "<< endl;
                break;
            }
            else if(nodo != NULL)// si existe
            {
                if(i==line->size()-1)//si es el ultimo
                {
                   cout << "Error 1.0 ya existe un directorio llamado "<<token << endl;
                   break;
                }
            }
            else// si no existe
            {
                if(i==line->size()-1)//si es el ultimo
                {
                    if(i!=0)
                        padre = arbol->get_nodo(line->at(i-1));
                    else
                        padre = arbol->get_pwd()->back();

                    if(arbol->find_child(padre, token)==NULL)// si no tenemos un hijo con ese mismo nombre
                        arbol->add_dir(padre, token,NULL);
                    else
                    {
                        cout << "Error 1.1 ya existe un directorio llamado "<<token << endl;
                        break;
                    }
                }
                else// si no es el ultimo
                {
                    cout << "Error 1.1 el directorio "<<token <<" no existe" <<endl;
                    break;
                }
            }
        }
    }

    else
    {
        cout << "Error 2.0 argumento invalido" << endl;
    }


}

void Terminal::pwd()
{
	vector<Nodo*>*ruta=arbol->get_pwd();

	for (int i = 0; i < ruta->size(); i++) {
		cout << ruta->at(i)->get_nombre();
		if (!(i == ruta->size() - 1))
			cout << "/";
	}
	cout << endl;
}

void Terminal::rmdir(comando_t * comm)
{
    arbol->set_mod(true);
	char split[2] = "/";
	char* token;
	Nodo* nodo;

    Nodo* last_nodo;

	//encontrar argumento en lista nodos
	//separar path
	//buscar los nodos

	token = strtok(comm->argumentos->at(0), split);
	nodo = arbol->get_nodo(token);
	last_nodo = nodo;

    if (nodo != NULL)
	{
		while (token != NULL)//&& nodo != NULL)
		{
			token = strtok(NULL, split);

            if (token != NULL) {
				last_nodo = nodo;
				nodo = arbol->find_child(nodo, token);
			}
			else if (token == NULL && nodo != NULL) //cuando ya es el ultimo de la sentencia (el nodo a borrar)
			{
				if (!last_nodo->get_type())
					cout << "Error " << last_nodo->get_nombre() << " es un archivo, para eliminarlo utilice rm <directorio>" << endl;
				else if(last_nodo->get_hijos()!=NULL)
					cout << "Error " << last_nodo->get_nombre() << " no esta vacio" << endl;
				else
					arbol->delete_child(last_nodo->get_padre(), last_nodo);
			}

			else
				cout << "Error 1.0 la ruta no es un directorio o no existe" << endl;
		}
	}
	else {
		cout << "Error 1.2 la ruta no es un directorio o no existe" << endl;
	}
}

void Terminal::rm(comando_t * comm)
{
    arbol->set_mod(true);

    int end = 0;
    char split[2] = "/";
    string token;
    Nodo* nodo;
    Nodo *padre;
    vector<Nodo*>* path = new vector<Nodo*>;
    vector<string> *line;
    vector<Nodo*>* pwd_actual = new vector<Nodo*>;

    end=1;
    line = get_elements(comm->argumentos->at(0), split);

    for(int i=0;i<line->size();i++)
    {
        token= line->at(i);
        if(i==0)//si el primero no es root tengo que comprobar que sea hijo del pwd
        {
            if(0!=strncmp(token.c_str(), arbol->get_root()->get_nombre().c_str(),4))//si no es root
            {
               padre = arbol->get_pwd()->back();
               nodo =arbol->find_child(padre,token);
               if(nodo==NULL)//si no es hijo
               {
                   cout <<"Error 1.0 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                   end=-1;
                   break;
               }
               else // si existe
                   for (int i = 0; i < arbol->get_pwd()->size(); i++)//añadimos toda la ruta hasta el nodo actual
                       path->push_back(arbol->get_pwd()->at(i));
            }
            // si el nodo es root la ruta a la que vamos a ir parte desde aqui
            nodo=arbol->get_nodo(token);
            path->push_back(nodo);
        }
        else// el resto de iteraciones
        {
            nodo=arbol->find_child(path->back(),token);
            if(nodo!=NULL)//si es hijo de su padre
            {
                path->push_back(nodo);
            }
            else// si no es su hijo
            {
                cout <<"Error 2.0 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                end=-1;
                break;
            }
        }
        if(i==line->size()-1)//si es el ultimo comprobar ademas que es un archivo
        {
            nodo=path->back();
            if(nodo!=NULL)//si es hijo de su padre
            {
                if(nodo->get_type())// si es un dir
                {
                    cout <<"Error 3.0 el nodo "<<token <<" es un directorio"<<endl;
                    end=-1;
                    break;
                }
            }
            else
            {
                cout <<"Error 3.1 el nodo "<<token <<" no existe"<<endl;
                end=-1;
                break;
            }
        }

    }

    if(end==1)
    {
        arbol->delete_child(path->back()->get_padre(), path->back());
    }


}

void Terminal::ls()
{
	Nodo* nodo = arbol->get_pwd()->back();
	vector<Nodo*>* hijos = nodo->get_hijos();
	int tam_total = 0;
	if (hijos != NULL)
	{
		for (int i = 0; i < hijos->size(); i++) {
			nodo = hijos->at(i);
			const time_t t = nodo->get_lastMod();
			if (nodo->get_type())
                cout << "DIR ";
            else
                cout << "- ";
            cout << nodo->get_nombre() << " " << (int)nodo->get_tam() << " " << asctime(gmtime(&t));
		}
	}
}

void Terminal::upload(comando_t * comm)
{
    arbol->set_mod(true);
    struct dirent **namelist;
    struct stat attrb;
    int size=scandir(".",&namelist,NULL,alphasort);
    int find=0;
    char* name = comm->argumentos->at(0);
    int end=1;
    for(int i=size-1;i>=0;i--){
        if(!strcmp(namelist[i]->d_name,name))
        {
            find=1;
            stat(name,&attrb);
            Nodo* padre= arbol->get_pwd()->back();
            if(arbol->find_child(padre, name)==NULL){
                if(namelist[i]->d_type==DT_DIR){// si es un directorio añado sus hijos ademas

                    struct dirent **hijos;
                    struct stat hijos_att;

                    int tam=scandir(name,&hijos,NULL,alphasort);
                    padre=arbol->add_dir(padre,(string)name,NULL);

                    for(int j=0;j<tam;j++) {
                        char* nombre=hijos[j]->d_name;
                        stat(nombre,&hijos_att);
                        if(strcmp(nombre,".") && strcmp(nombre,"..")){
                            if(hijos[j]->d_type==DT_DIR  )// si un hijo es un directorio
                            {
                                comando_t command;
                                command.argumentos = new vector<char*>();
                                command.argumentos->push_back(nombre);
                                arbol->add_dir(padre,(string)nombre,NULL);
                                string dir=name;
                                if(chdir(dir.c_str())==-1)
                                {
                                   cout << "Se ha producido un error"<< endl;
                                   end=-1;
                                }
                                else
                                {
                                    lpwd();
                                    upload(&command);
                                }
                            }
                            else
                                arbol->add_file(padre,(string)nombre,(intmax_t)hijos_att.st_size);
                        }
                    }
                }
                else
                    arbol->add_file(padre,(string)name,(intmax_t)attrb.st_size);

                if(end==1)
                    cout <<"El archivo " <<name<<" se ha subido correctamente"<<endl;
                else
                    cout <<"Error al subir el archivo"<<endl;
            }
            else
            {
                cout <<"El archivo " <<name<<" ya existe"<<endl;
                find=2;
            }
            break;
        }
        free(namelist[i]);
    }
    free(namelist);
    if(find==0)
        cout <<"Ha habido un error subiendo el archivo "<<name<<endl;

}

void Terminal::shut_down()
{
	printf("Closing file system\n");

	//salvar datos
    if(arbol->is_mod())
        arbol->save_arbol();
    int numProc = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &numProc);
    for(int i=1;i<=numProc;i++)
        send_msg(MSG_EXIT,i);
	exit(0);
}

void Terminal::lpwd()
{
    char path[PATH_MAX];
    if(getcwd(path,sizeof(path))!=NULL)
        cout << path << endl;
}

void Terminal::lls()
{
    struct dirent **namelist;
    struct stat attrb;
    int size=scandir(".",&namelist,NULL,alphasort);

    for(int i=0;i<size;i++) {
        char* name=namelist[i]->d_name;

       stat(name,&attrb);

        if(namelist[i]->d_type==DT_DIR)
            cout <<"DIR ";
        else
            cout <<"- ";
        cout << name <<"\t"<<(intmax_t)attrb.st_size<<"\t"<<ctime(&attrb.st_mtime);

    }
    /*
    while (size--) {
        free(namelist[size]);
    }*/
    free(namelist);

}

void Terminal::lcd(comando_t* comm)
{
   string dir=comm->argumentos->at(0);
   if(chdir(dir.c_str())==-1)
      cout << "Se ha producido un error"<< endl;
}

void Terminal::mv(comando_t* comm)
{
    arbol->set_mod(true);
    char split[2] = "/";
    string token;
    Nodo* nodo;
    Nodo* padre;
    string n_name;
    vector<string> *line;
    int end=0;

    line= get_elements(comm->argumentos->at(0), split);

    for(int i=0;i<line->size();i++)
    {
        token= line->at(i);
        if(i==0)//si el primero no es root tengo que comprobar que sea hijo del pwd
        {
            if(0!=strncmp(token.c_str(), arbol->get_root()->get_nombre().c_str(),4))//si no es root
            {
               padre = arbol->get_pwd()->back();
               nodo =arbol->find_child(padre,token);
               if(nodo==NULL)//si no es hijo
               {
                   cout <<"Error 1.0 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                   end=-1;
                   break;
               }
               else
                   padre=nodo;
            }
            // si el nodo es root la ruta a la que vamos a ir parte desde aqui
            else
                padre=arbol->get_root();
        }
        else //resto de iteraciones
        {
            nodo=arbol->find_child(padre,token);
            if(nodo==NULL)
            {
                cout <<"Error 1.1 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                end=-1;
                break;
            }
            else
                padre=nodo;

        }
        if(i==line->size()-1)//si es el ultimo
        {
           n_name=comm->argumentos->at(1);
           if(arbol->find_child(nodo->get_padre(),n_name))
           {
               cout <<"Error 1.2 ya existe un nodo con el nombre "<<token <<" en el directorio "<<nodo->get_padre()->get_nombre()<<endl;
               end=-1;
               break;
           }
           else
           {
               if(!strncmp("..", token.c_str(), 2) && strlen(token.c_str()) == 2
                       || !strncmp(".", token.c_str(), 1) && strlen(token.c_str()) == 1
                       || !strncmp("/", token.c_str(), 1) && strlen(token.c_str()) == 1)
               {
                   cout << "Error los nombres '.', '/' y '..' son comandos reservados "<< endl;
                   end=-1;
                   break;
               }
               else
                   nodo->set_name(n_name);
           }
        }
    }



}

void Terminal::cp(comando_t* comm)
{
    arbol->set_mod(true);
    char split[2] = "/";
    string token;
    Nodo* nodo;
    Nodo* padre;
    Nodo* last_nodos[2];
    int end=2;
    vector<string> *line;

    /***SACAMOS EL ARG1 Y ARG2***/ //TODO: No acepta rutas relativas error 2.1 (cp a/nodo.o c/p; pwd:root)
    for(int i=0;i<2;i++)
    {
        line= get_elements(comm->argumentos->at(i), split);
        if(line->size()>0)
        {
            for(int j=0;j<line->size();j++)
            {
                token = line->at(j);
                if(j==0)//si el primero no es root tengo que comprobar que sea hijo del pwd
                {
                    if(0!=strncmp(token.c_str(), arbol->get_root()->get_nombre().c_str(),4))//si no es root
                    {
                       padre = arbol->get_pwd()->back();
                       nodo =arbol->find_child(padre,token);
                       if(nodo==NULL)//si no es hijo
                       {
                           cout <<"Error 2.0 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                           end=-1;
                           break;
                       }
                       else//?????
                           padre=nodo;
                    }
                    else// si el nodo es root la ruta a la que vamos a ir parte desde aqui
                        padre=arbol->get_root();
                }
                else //resto de iteraciones
                {
                    nodo=arbol->find_child(padre,token);
                    if(nodo==NULL)
                    {
                        cout <<"Error 2.1 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                        end=-1;
                        break;
                    }
                    else
                        padre=nodo;

                }
                if(j==line->size()-1)//si es el ultimo
                {

                   if(arbol->find_child(nodo->get_padre(),token)==NULL)//si no existe
                   {
                       cout <<"Error 2.2 el nodo "<<token<<" no existe"<<endl;
                       end=-1;
                       break;
                   }
                   else
                       last_nodos[i]=nodo;

                }
            }
        }
        else
        {
            cout << "Error 1."<<i<<"argumentos invalidos" << endl;
            break;
        }
    }

    if(end==2)
    {
        Nodo *n_nodo;
        //FILE-> DIR
        if(!last_nodos[0]->get_type() && last_nodos[1]->get_type())
        {
            //si no existe un hijo con ese nombre
            if(arbol->find_child(last_nodos[1],last_nodos[0]->get_nombre())==NULL)
                n_nodo = arbol->add_file(last_nodos[1],last_nodos[0]->get_nombre(),last_nodos[0]->get_tam());
            else
            {
                cout <<"Error 3.0 el nodo "<<last_nodos[0]->get_nombre()<<" ya existe en el dir "<< last_nodos[1]->get_nombre()<<endl;
            }
        }
        //FILE->FILE
        else if(!last_nodos[0]->get_type() && !last_nodos[1]->get_type())
        {
            time_t t = time(0);
            last_nodos[1]->set_tam(last_nodos[0]->get_tam());
            last_nodos[1]->set_lastMod(t);
            last_nodos[1]->set_name(last_nodos[0]->get_nombre());
        }
        //DIR->DIR
        else //if(last_nodos[0]->get_type() && last_nodos[1]->get_type())
        {
            vector<Nodo*> *hijos =last_nodos[0]->get_hijos();
            if(hijos!=NULL)
            {
                vector<Nodo*> repetidos;
                for(int i=0;i<hijos->size();i++)
                {
                    Nodo* nodo=hijos->at(i);
                    if(arbol->find_child(last_nodos[1],nodo->get_nombre())==NULL)//si el hijo no existe en el dir destino
                    {
                        if(!nodo->get_type())// si es un fichero
                            arbol->add_file(last_nodos[1],nodo->get_nombre(),nodo->get_tam());
                        else //si es un dir
                        {
                            n_nodo=arbol->add_dir(last_nodos[1],nodo->get_nombre(),NULL);
                            if(nodo->get_hijos()!=NULL)
                                arbol->add_recursive_dir(nodo,n_nodo);
                        }
                    }
                    else
                        repetidos.push_back(hijos->at(i));// Peta aqui
                }

                if(!repetidos.empty())
                {
                    cout <<"En el directorio "<<last_nodos[1]->get_nombre()<<" ya existen archivos llamados: (";
                    for(int i=0;i<repetidos.size();i++)
                    {
                        cout <<repetidos.at(i)->get_nombre();
                        if(i<repetidos.size()-1) cout <<", ";
                    }
                     cout <<") y no se han podido copiar"<<endl;
                }

            }

        }

    }

}

void Terminal::download(comando_t* comm)
{
    int end = 0;
    char split[2] = "/";
    string token;
    Nodo* nodo;
    Nodo *padre;
    vector<Nodo*>* path = new vector<Nodo*>;
    vector<string> *line;
    vector<Nodo*>* pwd_actual = new vector<Nodo*>;

    end=1;
    line = get_elements(comm->argumentos->at(0), split);

    for(int i=0;i<line->size();i++)
    {
        token= line->at(i);
        if(i==0)//si el primero no es root tengo que comprobar que sea hijo del pwd
        {
            if(0!=strncmp(token.c_str(), arbol->get_root()->get_nombre().c_str(),4))//si no es root
            {
               padre = arbol->get_pwd()->back();
               nodo =arbol->find_child(padre,token);
               if(nodo==NULL)//si no es hijo
               {
                   cout <<"Error 1.0 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                   end=-1;
                   break;
               }
               else // si existe
                   for (int i = 0; i < arbol->get_pwd()->size(); i++)//añadimos toda la ruta hasta el nodo actual
                       path->push_back(arbol->get_pwd()->at(i));
            }
            // si el nodo es root la ruta a la que vamos a ir parte desde aqui
            nodo=arbol->get_nodo(token);
            path->push_back(nodo);
        }
        else// el resto de iteraciones
        {
            nodo=arbol->find_child(path->back(),token);
            if(nodo!=NULL)//si es hijo de su padre
            {
                path->push_back(nodo);
            }
            else// si no es su hijo
            {
                cout <<"Error 2.0 el nodo "<<token <<" no existe o no es un directorio"<<endl;
                end=-1;
                break;
            }
        }
        if(i==line->size()-1)//si es el ultimo comprobar ademas que es un archivo
        {
            if(nodo!=NULL)//si es hijo de su padre
            {
                if(!nodo->get_type())// si es un archivo
                    path->push_back(nodo);
                else
                {
                    cout <<"Error 3.0 el nodo "<<token <<" es un directorio"<<endl;
                    end=-1;
                    break;
                }
            }
            else
            {
                cout <<"Error 3.1 el nodo "<<token <<" no existe"<<endl;
                end=-1;
                break;
            }
        }
    }

    if(end==1)
    {
        hardDisc->readFile(path->back());
    }


}

void Terminal::format(comando_t* comm)
{
    int tam =atoi(comm->argumentos->at(0));
    this->arbol->format(tam);
}
//visuales
void Terminal::pintar_terminal() {
	cout << "sergioBMPN@SI_DI:";
	arbol->pwd_tostring();
	cout << "$ ";
}
