#include "harddisc.h"

HardDisc::HardDisc(long int hdSize, int num_hd, int bSize)//,bool first_init=true)
{
   // if(first_init){
        this->totalSize=hdSize*num_hd;
        this->blockSize=bSize;
        this->numblocks= hdSize/bSize;
        int resto = hdSize%bSize;
        if (resto!=0)
            numblocks+=1;
        this->freeBlocks=new vector<block_t*>;
        this->blockList=new vector<block_t*>;
        this->hdisc= new vector<string>;

        //crear bloques
        for(int i=0;i<numblocks*num_hd;i++)
        {
            block_t* block= new block_t;
            block->blockId=i;
            block->status=FREE;
            freeBlocks->push_back(block);
            blockList->push_back(block);
        }
        //crear discos duros
        for(int i=0;i<num_hd;i++)
        {
            string fname="HDisc";
            fname+=to_string(i);
            fname+=".dat";
            hdisc->push_back(fname);
        }
    //}
}
int HardDisc::saveHD(vector<Nodo*>* nodos){


        string file="HD.dat";

        remove(file.c_str());
        ofstream f(file.c_str(),ios::app | ios::binary);

        string line = to_string(totalSize);
        line+=",";
        line += to_string(blockSize);
        line+=",";
        line += to_string(numblocks);
        line+=",";
        line += to_string(hdisc->size());
        line+=",";
        line+="(";
        for(int j=0;j<nodos->size();j++){
            vector<int>* actual_block=nodos->at(j)->get_blocks();
            for(int i=0;i<actual_block->size();i++)
            {
                line += to_string(actual_block->at(i));
                line+="/";
            }
        }
        line+=")";
        line += "\n";
        f.write(line.c_str(),line.size());

        f.close();
        return 1;
}

int HardDisc::loadHD(){

    string file="HD.dat";
    string line;
    ifstream f(file.c_str());

    int result=-1;
    try{
        if(f.is_open())
        {
            int num_hd;
            this->freeBlocks=new vector<block_t*>;
            this->blockList=new vector<block_t*>;
            this->hdisc= new vector<string>;

            vector<string>* line_info, *line_f;
            getline(f,line);
            line_info = get_elements(line,",");

            //coger elemento linea
            if(line_info->size()==0)
                throw;
            //Totalsize
            this->totalSize = atoi(line_info->at(0).c_str());
            //blockSize
            this->blockSize = atoi(line_info->at(1).c_str());
            //num_blocks
            this->numblocks = atoi(line_info->at(2).c_str());
            //num_hd
            num_hd = atoi(line_info->at(3).c_str());
            //block_id libres
            line_f = get_elements(line_info->at(4),"(/)");

            for(int i=0;i<this->numblocks*num_hd;i++)
            {
                //crear lista blocks
                block_t *b = new block_t;
                b->blockId=i;
                b->info=NULL;
                b->status=FREE;
                if(findId(line_f,i)!=-1)
                    b->status=USED;
                else
                    this->freeBlocks->push_back(b);

                this->blockList->push_back(b);
            }

            for(int i=0;i<num_hd;i++)
            {
                string fname="HDisc";
                fname+=to_string(i);
                fname+=".dat";
                hdisc->push_back(fname);
            }
            result = 1;
       }
    }
    catch(int e)
    {
        cout<<"Ha sudedido un error "<< e<<endl;
    }
    return result;

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
int HardDisc::findBlock(int id){
    for(int i=0; i<freeBlocks->size();i++)
    {
        if(id==freeBlocks->at(i)->blockId)
        {
            return i;
        }
    }
    return -1;
}

int HardDisc::writeBlock(block_t* block, int disc_num, int tam)
{
    FILE *disc = fopen(hdisc->at(disc_num).c_str(),"a");
    if(disc== NULL)
        return -1;
    else
    {
        fseek(disc,blockSize*(block->blockId/hdisc->size()),SEEK_SET);
        fwrite(block->info,sizeof(char),tam,disc);//MPI enviar por separad id y datos block
        fclose(disc);
        return 1;
    }


}
int HardDisc::readBlock(block_t* block, int disc_num,int tam)
{
    FILE *disc = fopen(hdisc->at(disc_num).c_str(),"r+");
    if(disc== NULL)
        return -1;
    else
    {
        fseek(disc,blockSize*(block->blockId/hdisc->size()),SEEK_SET);
        char* buffer = (char*) malloc (sizeof(char)*tam);
        fread(buffer,sizeof(char),tam,disc);
        block->info=buffer;
        fclose(disc);
        return 1;
    }

}

//TODO: Hay que quitar el resto de bloques de un sector
int HardDisc::writeFile(Nodo* file)// escribir en disco
{
    long int tamActual=blockSize*freeBlocks->size();
    if(file->get_tam()<=tamActual)// si entra en el disco
    {
        int fileBlocks=file->get_tam()/blockSize;
        if(file->get_tam()%blockSize!=0) fileBlocks++;

        FILE *f=fopen(file->get_nombre().c_str(),"r");
        for(int i=0;i<fileBlocks;i++)
        {
            int disco=0;
            int readSize=blockSize;
            if(i==fileBlocks-1)
                readSize=file->get_tam()%blockSize;
//            int pos=findBlock(i);
//            if(pos==-1)
//                return-1;
//            block_t* block = freeBlocks->at(pos);//buscar la posicion
            block_t* block = freeBlocks->at(0);
            char* buffer = (char*) malloc (sizeof(char)*readSize);
            fseek(f,i*blockSize,SEEK_SET);//ponemos el puntero en el inicio del bloque
            fread(buffer,1,readSize,f);//leemos blocksize bytes del documento
            block->info = buffer;
            disco=block->blockId%hdisc->size();
            if(writeBlock(block,disco,readSize)==-1)
                return -1;
            else
            {
                blockList->at(block->blockId)->status=USED;
                file->get_blocks()->push_back(block->blockId);
//                int pos=findBlock(block->blockId);
//                if(pos>=0)
                freeBlocks->erase(freeBlocks->begin());// hay que hacer una funcion para liberar buscando el id
            }
        }
         fclose(f);//cerramos el fichero disco duro
        tamActual=tamActual-file->get_tam();
        return 1;
    }
    return-1;
}
int HardDisc::readFile(Nodo* file)//leer de disco
{
    int fileBlocks=file->get_blocks()->size();
    for(int i=0;i<fileBlocks;i++)
    {
        int disco=0;
        int writeSize=blockSize;
        if(i==fileBlocks-1)
            writeSize=file->get_tam()%blockSize;
        block_t *block=new block_t;
        block->blockId = file->get_blocks()->at(i);
        block->status= blockList->at(block->blockId);//hacer funcion find
        disco=block->blockId%hdisc->size();
        if(readBlock(block,disco,writeSize)==-1)
            return -1;
        else
        {
            //guardamos el archivo que hemos leido con la extension .read
            string namef= file->get_nombre();
            namef+=".read";

            FILE *f=fopen(namef.c_str(),"a");
            fseek(f,i*blockSize,SEEK_SET);

            //if i == fin escribir resto
            if(i==fileBlocks-1)
                writeSize=file->get_tam()%blockSize;

            fwrite(block->info,sizeof(char),writeSize,f);
            fclose(f);

        }
    }
}
