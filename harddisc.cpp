#include "harddisc.h"

HardDisc::HardDisc(long int hdSize, int num_hd, int bSize)
{
    this->totalSize=hdSize;
    this->blockSize=bSize;
    this->numblocks= hdSize/bSize;
    int resto = hdSize%bSize;
    if (resto!=0)
        numblocks+=1;
    this->freeBlocks=new vector<block_t*>;
    this->blockList=new vector<block_t*>;
    this->hdisc= new vector<string>;

    for(int i=0;i<numblocks;i++)
    {
        block_t* block= new block_t;
        block->blockId=i;
        block->status=FREE;
        freeBlocks->push_back(block);
        blockList->push_back(block);
    }
    for(int i=0;i<num_hd;i++)
    {
        string fname="HDisc";
        fname+=to_string(i);
        fname+=".dat";
        hdisc->push_back(fname);
    }
}

int HardDisc::get_totalSize()
{
    return totalSize;
}
int HardDisc::get_blockSize()
{
    return blockSize;
}
int HardDisc::get_numBlocks()
{
    return numblocks;
}

vector<block_t*>* HardDisc::get_freeBlocks()
{
    return freeBlocks;
}
vector<block_t*>* HardDisc::get_blockList()
{
    return blockList;
}


int HardDisc::writeBlock(block_t* block, int disc_num)
{
    FILE *disc = fopen(hdisc->at(disc_num).c_str(),"w");
    if(disc== NULL)
        return -1;
    else
    {
        fseek(disc,blockSize*block->blockId,SEEK_SET);
        fputs(block->info,disc);
        fclose(disc);
        return 1;
    }


}
int HardDisc::readBlock(block_t* block, int disc_num)
{
    FILE *disc = fopen(hdisc->at(disc_num).c_str(),"r+");
    if(disc== NULL)
        return -1;
    else
    {
        fseek(disc,blockSize*block->blockId,SEEK_SET);
        char* buffer = (char*) malloc (sizeof(char)*blockSize);
        block->info=buffer;
        fclose(disc);
        return 1;
    }

}

int HardDisc::writeFile(Nodo* file)
{
    long int tamActual=blockSize*freeBlocks->size();
    if(file->get_tam()<=tamActual)// si entra en el disco
    {
        int fileBlocks=file->get_tam()/blockSize;
        if(file->get_tam()%blockSize!=0) fileBlocks++;
        for(int j=0;j<hdisc->size();j++)
        {
            FILE *f=fopen(file->get_nombre().c_str(),"r+");
            for(int i=0;i<fileBlocks;i++)
            {
                block_t* block = freeBlocks->at(i);
                char* buffer = (char*) malloc (sizeof(char)*blockSize);
                fseek(f,i*blockSize,SEEK_SET);//ponemos el puntero en el inicio del bloque
                fread(buffer,1,blockSize,f);//leemos blocksize bytes del documento
                block->info = buffer;
                if(writeBlock(block,j)==-1)
                    return -1;
                else
                {
                 blockList->at(block->blockId)->status=USED;
                 file->get_blocks()->push_back(block->blockId);
                 freeBlocks->erase(freeBlocks->begin()+i);
                }
            }
            fclose(f);//cerramos el fichero disco duro
        }
        return 1;
    }
}
int HardDisc::readFile(Nodo* file)
{
    int fileBlocks=file->get_blocks()->size();
    for(int j=0;j<hdisc->size();j++)
    {
        for(int i=0;i<fileBlocks;i++)
        {
            block_t *block=new block_t;
            block->blockId = file->get_blocks()->at(j);
            block->status= blockList->at(block->blockId);

            if(readBlock(block,j)==-1)
                return -1;
            else
            {
                //guardamos el archivo que hemos leido con la extension .read
                string namef= file->get_nombre();
                namef+=".read";

                FILE *f=fopen(namef.c_str(),"w+");
                fseek(f,i*blockSize,SEEK_SET);
                fwrite(block->info,f);
                fclose(f);
            }
        }
    }
}

