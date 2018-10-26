#include "terminal.h"

Terminal::Terminal()
{
    arbol=new Arbol();
}

void Terminal::run()
{
    bool exit=false;
    comando_t comando;
    while(!exit){
		comando.argumentos = new vector<char*>();
		pintar_terminal();
        if(leer_comando(&comando)==0)
            ejecutar_comando(&comando);
		free(comando.argumentos);
    }
}
//leer y ejecutar comm
int Terminal::leer_comando(comando_t *comando){

    char* line=new char [1024];
    char split[2]=" ";
    char* token=NULL;

    //leer terminal
    fgets(line,1023,stdin);
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
	char* token;
	Nodo* nodo;
	vector<Nodo*>* path = new vector<Nodo*>;
   
	Nodo* pwd = arbol->get_pwd()->back();
	vector<Nodo*>* pwd_actual = new vector<Nodo*>;
	// comandos especiales 1
	if (!strncmp(comm->argumentos->at(0), "/", 1))
	{
		path->push_back(arbol->get_root());
		end = arbol->move_to(path);
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
	else {
		token = strtok(comm->argumentos->at(0), split);
		nodo = arbol->get_nodo(token);

		if (nodo != NULL) {
			if (nodo->get_id() != 0)// si el primero no es root tengo que comprobar que sea hijo del pwd
				for (int i = 0; i < arbol->get_pwd()->size(); i++)//añadimos toda la ruta hasta el nodo actual
					path->push_back(arbol->get_pwd()->at(i));

			path->push_back(nodo);

			while (end == 0)
			{
				token = strtok(NULL, split);
				if (token != NULL)
				{
					nodo = arbol->get_nodo(token);
					if (nodo != NULL)
						path->push_back(nodo);
					else
						end = -1;
				}
				else
					end = 1;
			}
		}
	}

	//cambiar el pwd
	if (end == 1)
		end = arbol->move_to(path);

	if (end<1)
		cout << "La ruta indicada no es un directorio o no existe: " << comm->argumentos->at(0) << "\n";
}

void Terminal::mkdir(comando_t * comm)
{
    char split[2] = "/";
    char* token;
    Nodo* nodo;
    string last_token;
    Nodo* last_nodo;

    //encontrar argumento en lista nodos
    //separar path
    //buscar los nodos

    token = strtok(comm->argumentos->at(0), split);
    nodo = arbol->get_nodo(token);

    if (nodo != NULL)
    {
        while (token != NULL)//&& nodo != NULL)
        {
            token = strtok(NULL, split);

            if (token != NULL) {
                last_nodo = nodo;
                nodo = arbol->find_child(nodo, token);
                last_token = string(token);
            }
            else if (token == NULL && nodo == NULL){
                if(arbol->find_child(last_nodo, last_token)==NULL)// si no tenemos un hijo con ese mismo nombre
                    arbol->add_dir(last_nodo, last_token,NULL);
            }
            else
                cout << "Error 1.0 la ruta no es un directorio o no existe" << endl;
        }
    }
    else {
        last_token = token;
        token = strtok(NULL, split);
        if (token == NULL)
        {
            last_nodo = arbol->get_pwd()->back();
            if(arbol->find_child(last_nodo, last_token)==NULL)
                arbol->add_dir(last_nodo, last_token,NULL);
        }
        else
            cout << "Error 1.2 la ruta no es un directorio o no existe" << endl;
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
	char split[2] = "/";
	char* token;
	Nodo* nodo;
	string last_token;
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
	char split[2] = "/";
	char* token;
	Nodo* nodo;
	string last_token;
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
				if (last_nodo->get_type())
					cout << "Error " << last_nodo->get_nombre() << " es un directorio, para eliminarlo utilice rmdir <directorio>" << endl;
				else
					arbol->delete_child(last_nodo->get_padre(), last_nodo);
			}

			else
				cout << "Error 1.0 la ruta no es un archivo o no existe" << endl;
		}
	}
	else {
		cout << "Error 1.2 la ruta no es un archivo o no existe" << endl;
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
    struct dirent **namelist;
    struct stat attrb;
    int size=scandir(".",&namelist,NULL,alphasort);
    int find=0;
    char* name = comm->argumentos->at(0);
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
                                //lcd()
                                //upload(&command);
                            }
                            else
                                arbol->add_file(padre,(string)nombre,(intmax_t)hijos_att.st_size);
                        }
                    }
                }
                else
                    arbol->add_file(padre,(string)name,(intmax_t)attrb.st_size);
                cout <<"El archivo " <<name<<" se ha subido correctamente"<<endl;
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
	getchar();
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
    char split[2] = "/";
    char* token;
    Nodo* nodo;
    string last_token;
    Nodo* last_nodo;

    token = strtok(comm->argumentos->at(0), split);
    nodo = arbol->get_nodo(token);
    last_nodo=nodo;

    if (nodo != NULL)
    {
        while (token != NULL)
        {
            token = strtok(NULL, split);
            if (token != NULL) {
                last_nodo = nodo;
                nodo = arbol->find_child(nodo, token);
                last_token = string(token);
            }
            else if (token == NULL && nodo!= NULL)
            {
                if(comm->argumentos->at(1) !=NULL)
                    last_nodo->set_name(comm->argumentos->at(1));
                else
                    cout << "Error de sentencia el segundo arg es NULL" << endl;
            }
            else
                cout << "Error 1.0 la ruta no es un directorio o no existe" << endl;
        }
    }
    else
        cout << "Error 1.2 la ruta no es un directorio o no existe" << endl;



}

void Terminal::cp(comando_t* comm)
{
    char split[2] = "/";
    char* token;
    Nodo* nodo;
    string last_token;
    Nodo* last_nodo1, *last_nodo2;
    int correcto=2;

    /***SACAMOS EL ARG1***/
    token = strtok(comm->argumentos->at(0), split);
    nodo = arbol->get_nodo(token);
    last_nodo1=nodo;

    if (nodo != NULL)
    {
        while (token != NULL)
        {
            token = strtok(NULL, split);
            if (token != NULL) {
                last_nodo1 = nodo;
                nodo = arbol->find_child(nodo, token);
                last_token = string(token);
            }
            else if (nodo== NULL){
                correcto--;
                cout << "Error 1.0 la ruta no es un directorio o no existe" << endl;
            }
        }
    }
    else{
        correcto--;
        cout << "Error 1.2 la ruta no es un directorio o no existe" << endl;
    }

    /***SACAMOS EL ARG2***/
    token = strtok(comm->argumentos->at(1), split);
    nodo = arbol->get_nodo(token);
    last_nodo2=nodo;

    if (nodo != NULL && correcto==2)
    {
        while (token != NULL)
        {
            token = strtok(NULL, split);
            if (token != NULL) {
                last_nodo2 = nodo;
                nodo = arbol->find_child(nodo, token);
                last_token = string(token);
            }
            else if (nodo== NULL){
                correcto--;
                cout << "Error 1.0 la ruta no es un directorio o no existe" << endl;
            }
        }
    }
    else{
        correcto--;
        cout << "Error 1.2 la ruta no es un directorio o no existe" << endl;
    }

    if(correcto==2)
    {
        //FILE-> DIR
        if(!last_nodo1->get_type() && last_nodo2->get_type())
        {
            last_nodo2->add_hijo(last_nodo1);
        }
        //FILE->FILE
        else if(!last_nodo1->get_type() && !last_nodo2->get_type())
        {
            time_t t = time(0);
            last_nodo2->set_tam(last_nodo1->get_tam());
            last_nodo2->set_lastMod(t);
            last_nodo2->set_name(last_nodo1->get_nombre());
        }
        //DIR->DIR
        else if(last_nodo1->get_type() && last_nodo2->get_type()){
            vector<Nodo*> *hijos =last_nodo1->get_hijos();
            for(int i=0;i<hijos->size();i++)
                last_nodo2->add_hijo(hijos->at(i));
        }

    }

}

//visuales
void Terminal::pintar_terminal() {

	cout << "sergioBMPN@SI_DI:";
	arbol->pwd_tostring();
	cout << "$ ";
}
