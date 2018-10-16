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

			if(!strncmp("cd", comando, 2)){
                return CMD_CD;
            }
            if(!strncmp("mkdir", comando, 5)){
                return CMD_MKDIR;
            }
			if (!strncmp("pwd", comando, 3)) {
				return CMD_PWD;
			}
			if (!strncmp("ls", comando, 2)) {
				return CMD_LS;
			}
            if(!strncmp("exit", comando, 4)){
                return CMD_EXIT;
            }
        }

        return -1;
}

void Terminal::cd(comando_t * comm)
{
	int end = 0;
	char split[2] = "/";
	char* token;
	Nodo* nodo;
	vector<Nodo*>* path = new vector<Nodo*>;
	Nodo* pwd = arbol->get_pwd()->back();

	// comandos especiales 1
	if (!strncmp(comm->argumentos->at(0), "/", 1))
	{
		path->push_back(arbol->get_root());
		arbol->move_to(path);
	}
	// comandos especiales 2
	else if (!strncmp(comm->argumentos->at(0), "..",2)) {
		path=arbol->get_pwd();
		path->pop_back();
		if (path->size() > 0)
			end = arbol->move_to(path);
		else
			end = 2;
	}
	//caso normal
	else {
		token = strtok(comm->argumentos->at(0), split);
		nodo = arbol->get_nodo(token);
		// si la ruta empieza por root


		if (nodo != NULL) {
			if (nodo->get_id() != 0)// si el primero no es root tengo que comprobar que sea hijo del pwd
				path->push_back(arbol->get_pwd()->back());
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

	if(end<1)
		cout<<"La ruta indicada no es un directorio o no existe: "<< comm->argumentos->at(0) <<"\n";
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
		while (token != NULL )//&& nodo != NULL)
		{
			token = strtok(NULL, split);
			//cout << token << endl;
			if (token != NULL) {
				last_nodo = nodo;
				nodo = arbol->find_child(nodo, token);
				last_token = string(token);
			}
			else if (token == NULL && nodo == NULL)
				arbol->add_child(last_nodo, last_token,true);
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
	arbol->pwd_tostring();
	cout << endl;
}

void Terminal::rmdir(comando_t * comm)
{
}

void Terminal::rm(comando_t * comm)
{
}

void Terminal::shut_down()
{
	printf("Closing file system\n");
	//salvar datos
	getchar();
	exit(0);
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
        case CMD_EXIT:
			shut_down();
            break;
        default:
            printf("Commando not found\n");
            break;
    }
}

void Terminal::pintar_terminal() {

	cout << "sergioBMPN@SI_DI:";
	arbol->pwd_tostring();
	cout << "$ ";
}