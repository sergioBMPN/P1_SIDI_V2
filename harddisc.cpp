#include "harddisc.h"

HardDisc::HardDisc(long int hdSize, int num_hd, int bSize)
{
    this->totalSize=hdSize*num_hd;
    this->freeSize=hdSize*num_hd;
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
    for(int i=0;i<num_hd+1;i++)
    {
        string fname="HDisc";
        fname+=to_string(i);
        fname+=".dat";
        hdisc->push_back(fname);
    }
}

int HardDisc::format(long int hdSize, int num_hd, int bSize){
    if(num_hd==0 || bSize==0)
    {
        num_hd=this->hdisc->size();
        bSize=this->blockSize;
    }
    this->totalSize=hdSize*num_hd;
    this->freeSize=hdSize*num_hd;
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
        remove(fname.c_str());
    }
    return 1;

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
    //enviar mensage de escritura
    send_msg(MSG_UPL,disc_num);

    //enviar posicion fseek
    int pos=(blockSize*(block->blockId/hdisc->size()));
    MPI_Send(&pos,sizeof(int),MPI_BYTE,disc_num,MSG_UPL,MPI_COMM_WORLD);
    //enviar tam block
    MPI_Send(&tam,sizeof(int),MPI_BYTE,disc_num,MSG_UPL,MPI_COMM_WORLD);
    //enviar block
    MPI_Send(block->info,sizeof(char)*tam,MPI_BYTE,disc_num,MSG_UPL,MPI_COMM_WORLD);
    return 0;
    /*
    FILE *disc = fopen(hdisc->at(disc_num).c_str(),"a");
    if(disc== NULL)
        return -1;
    else
    {
        fseek(disc,blockSize*(block->blockId/hdisc->size()),SEEK_SET);
        fwrite(block->info,sizeof(char),tam,disc);//MPI enviar por separado id y datos block
        fclose(disc);
        return 1;
    }*/
}
int HardDisc::readBlock(block_t* block, int disc_num,int tam)
{    
    //enviar mensage de lectura
    send_msg(MSG_DWL,disc_num);

    //enviar posicion fseek
    int pos=(blockSize*(block->blockId/hdisc->size()));
    MPI_Send(&pos,sizeof(int),MPI_BYTE,disc_num,MSG_DWL,MPI_COMM_WORLD);
    //enviar tam block
    MPI_Send(&tam,sizeof(int),MPI_BYTE,disc_num,MSG_DWL,MPI_COMM_WORLD);

    //recibir block
    MPI_Status status;
    MPI_Recv(block->info, sizeof(char)*tam, MPI_BYTE, disc_num, MSG_DWL,MPI_COMM_WORLD, &status);

    return 0;
    /*
    FILE *disc = fopen(hdisc->at(disc_num).c_str(),"r");
    if(disc== NULL)
        return -1;
    else
    {
        fseek(disc,blockSize*(block->blockId/hdisc->size()),SEEK_SET);
        fread(buffer,sizeof(char),tam,disc);
        block->info=buffer;
        fclose(disc);
        return 1;
    }*/
}

int HardDisc::writeFile(Nodo* file)// escribir en disco
{
    if(file->get_tam()<=freeSize)// si entra en el disco
    {
        int fileBlocks=file->get_tam()/blockSize;
        if(file->get_tam()%blockSize!=0) fileBlocks++;
        char* buffer;
        FILE *f=fopen(file->get_nombre().c_str(),"r");
        for(int i=0;i<fileBlocks;i++)
        {
            int disco=0;
            int readSize=blockSize;
            if(i==fileBlocks-1)
                readSize=file->get_tam()%blockSize;
            block_t* block = freeBlocks->at(0);

            buffer = NULL;
            buffer = (char*) malloc (sizeof(char)*blockSize);
            fseek(f,i*blockSize,SEEK_SET);//ponemos el puntero en el inicio del bloque
            fread(buffer,1,readSize,f);//leemos blocksize bytes del documento
            block->info = buffer;
            string b =buffer;

            disco=block->blockId%hdisc->size()+1;

            if(writeBlock(block,disco,blockSize)==-1)
                return -1;
            else
            {
                blockList->at(block->blockId)->status=USED;
                file->get_blocks()->push_back(block->blockId);

                freeBlocks->erase(freeBlocks->begin());
            }
        }
         fclose(f);//cerramos el fichero disco duro
        return 1;
    }
    return-1;
}
int HardDisc::readFile(Nodo* file)//leer de disco
{
    int fileBlocks, totalBlocks;
    totalBlocks=file->get_blocks()->size();//ocupando todos los block del ultimo sector ocupado
    fileBlocks=file->get_tam()/blockSize;
    if(file->get_tam()%blockSize!=0) fileBlocks++;
    for(int i=0;i<fileBlocks;i++)
    {
        int disco=0;
        int writeSize=blockSize;
        if(i==fileBlocks-1)
            writeSize=file->get_tam()%blockSize;
        block_t *block=new block_t;
        block->blockId = file->get_blocks()->at(i);
        block->status= USED;
        disco=block->blockId%hdisc->size()+1;
        if(readBlock(block,disco,writeSize)==-1)
            return -1;
        else
        {
            //guardamos el archivo que hemos leido con la extension .read
            string namef= file->get_nombre();
            namef+=".read";
            FILE *f=fopen(namef.c_str(),"a+");
            fseek(f,i*blockSize,SEEK_SET);
            fwrite(block->info,sizeof(char),writeSize,f);
            fclose(f);
        }
    }
}
