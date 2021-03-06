#pragma once
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include "nodo.h"
#include "utilities.h"
/*File *hdd;
BlockId=5;
BlockSize=1024;
fseek(hdd,blockSize*blockId,SEEK_SET)
fread/fwrite(hdd, buffer,....)
using namespace std;*/


class HardDisc
{
    vector<string>* hdisc;
    int totalSize;
    int freeSize;
    int blockSize;
    int numblocks;
    vector<block_t*>* freeBlocks;
    vector<block_t*>* blockList;

    int writeBlock(block_t* block, int disc_num,int tam);
    int readBlock(block_t* block, int disc_num,int tam);

public:
    HardDisc(long int hdSize, int num_hd, int bSize);
    int get_totalSize();
    int get_blockSize();
    int get_numBlocks();
    int findBlock(int id);
    int loadHD();
    int saveHD(vector<Nodo*> *nodos);
    int format(long int hdSize, int num_hd=0, int bSize=0);

    vector<block_t*>* get_freeBlocks();
    vector<block_t*>* get_blockList();

    int writeFile(Nodo* file);
    int readFile(Nodo* file);


};
