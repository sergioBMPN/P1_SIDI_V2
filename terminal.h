#pragma once

#include "arbol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <ctime>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <omp.h>
#include <fstream>
#include <pthread.h>
#include "utilities.h"
#include "harddisc.h"

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS



#define CMD_CD 0
#define CMD_MKDIR 1
#define CMD_EXIT 2
#define CMD_PWD 3
#define CMD_LS 4
#define CMD_RMDIR 5
#define CMD_RM 6
#define CMD_LPWD 7
#define CMD_LLS 8
#define CMD_LCD 9
#define CMD_UPL 10
#define CMD_MV 11
#define CMD_CP 12
#define CMD_DWL 13
#define CMD_FRM 14



using namespace std;

class Terminal
{
    const char* comandos[13]={
        "cd",
        "mkdir",
		"pwd",
		"rmdir",
		"ls",
		"upload",
        "download",
        "lpwd",
        "lls",
        "lcd",
        "mv",
        "cp",
        "exit"
    };
    HardDisc * hardDisc;
    Arbol *arbol;
    static void* comm_exe(void*args);
    static void* watch_dog(void* args);

public:    
    Terminal(long int discSize, int n_disc);// en Mb
    void run();
    int leer_comando(comando_t *comando);
    void ejecutar_comando(comando_t *comando);
    int get_tipo_comm(char * comando);

	//comandos
	void cd(comando_t* comm);
    void mv(comando_t* comm);
    void cp(comando_t* comm);
	void mkdir(comando_t* comm);
	void pwd();
	void rmdir(comando_t* comm);
	void rm(comando_t* comm);
	void ls();
	void upload(comando_t* comm);
	void shut_down();
	void lpwd();
    void lls();
    void lcd(comando_t* comm);
    void download(comando_t* comm);
    void format(comando_t* comm);

	//visual
	void pintar_terminal();
};



//TODO:
    //pruebas eliminar nodo y guardar comprobar last id?

