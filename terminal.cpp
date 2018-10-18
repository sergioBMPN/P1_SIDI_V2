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
        leer_comando(&comando);
        ejecutar_comando(&comando);
		free(comando.argumentos);
    }
}
//leer y ejecutar comm
void Terminal::leer_comando(comando_t *comando){

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
				//cout << token << endl;
			}
		}
	}
}

int Terminal::get_tipo_comm(char* comando){
    //Errores, si comando es null, las mayusculas estan permitidas

        if(comando != NULL){
			for (int i = 0; i < strlen(comando); i++)
				comando[i] = tolower(comando[i]);

			if(!strncmp("cd", comando, 2) && strlen(comando) == 2){
                return CMD_CD;
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
            if(!strncmp("exit", comando, 4) && strlen(comando) == 4){
                return CMD_EXIT;
            }
        }

        return -1;
}

void Terminal::ejecutar_comando(comando_t *comando){

    switch(comando->tipo){
        case CMD_CD:
			cd(comando);
            break;
        case CMD_MKDIR:
			mkdir(comando);
            break;
		case CMD_PWD:
			pwd();
			break;
		case CMD_RMDIR:
			rmdir(comando);
			break;
		case CMD_RM:
			rm(comando);
			break;
		case CMD_LS:
			ls();
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
	//cout << comm->argumentos->at(0) << endl;
	token = strtok(comm->argumentos->at(0), split);
	nodo = arbol->get_nodo(token);
	//cout << token << endl;
	if (nodo != NULL)
	{
		while (token != NULL)//&& nodo != NULL)
		{
			token = strtok(NULL, split);
			//cout << token << endl;
			if (token != NULL) {
				last_nodo = nodo;
				nodo = arbol->find_child(nodo, token);
				last_token = string(token);
			}
			else if (token == NULL && nodo == NULL)
				arbol->add_child(last_nodo, last_token, true);
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
			arbol->add_child(last_nodo, last_token, true);
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
	//cout << comm->argumentos->at(0) << endl;
	token = strtok(comm->argumentos->at(0), split);
	nodo = arbol->get_nodo(token);
	last_nodo = nodo;
	//cout << token << endl;
	if (nodo != NULL)
	{
		while (token != NULL)//&& nodo != NULL)
		{
			token = strtok(NULL, split);
			//cout << token << endl;
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
	//cout << comm->argumentos->at(0) << endl;
	token = strtok(comm->argumentos->at(0), split);
	nodo = arbol->get_nodo(token);
	last_nodo = nodo;
	//cout << token << endl;
	if (nodo != NULL)
	{
		while (token != NULL)//&& nodo != NULL)
		{
			token = strtok(NULL, split);
			//cout << token << endl;
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
				cout << "DIR " << nodo->get_nombre() << " " << (int)nodo->get_tam() << " " << asctime(gmtime(&t));
			tam_total = (int)nodo->get_tam();
		}
		cout << hijos->size() <<" elemento(s) en directorio remoto " << nodo->get_nombre() << " ocupando un total de " << tam_total <<" bytes " << endl;
	}
}

void Terminal::upload(comando_t * comm)
{

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
}



//visuales
void Terminal::pintar_terminal() {

	cout << "sergioBMPN@SI_DI:";
	arbol->pwd_tostring();
	cout << "$ ";
}