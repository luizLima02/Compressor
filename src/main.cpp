#include<iostream>
#include<stdlib.h>
#include"imagem.hpp"
#include"estruturas.hpp"
#include<string>
#include <bitset>
#include <chrono>
#include <thread>
#include <sys/stat.h>


std::string caminhoSalvo = "ImagensSalvas";

int writeBites(const char* path, string Hbits, string Sbits, string Vbits, pair<int, int>* VO, int FileSize, int width, int height){
    std::fstream f;
    f.open(path, std::ios::out | std::ios::binary);
    
    if(!f.is_open()){
        std::cout << "Erro ao Escrever\n";
        return -1;
    }
    //header 
    const int headerSize = 26;
    unsigned char Header[headerSize];
    //o Primeiro byte = L
    Header[0] = 'L';
    //o Segundo Byte = N
    Header[1] = 'N';
    //-----------------------------
    //3, 4, 5, 6 byte tamanho do arquivo original
    Header[2] = FileSize;
    Header[3] = FileSize >> 8;
    Header[4] = FileSize >> 16;
    Header[5] = FileSize >> 24;
    //7 , 8, 9, 10 byte width do arquivo original
    Header[6] = width;
    Header[7] = width >> 8;
    Header[8] = width >> 16;
    Header[9] = width >> 24;
    //11, 12, 13, 14 byte height do arquivo original
    Header[10] = height;
    Header[11] = height >> 8;
    Header[12] = height >> 16;
    Header[13] = height >> 24;
    //15, 16, 17, 18 byte size vetor H do arquivo
    Header[14] = Hbits.size();
    Header[15] = Hbits.size() >> 8;
    Header[16] = Hbits.size() >> 16;
    Header[17] = Hbits.size() >> 24;
    //19, 20, 21, 22 byte size vetor S do arquivo
    Header[18] = Sbits.size();
    Header[19] = Sbits.size() >> 8;
    Header[20] = Sbits.size() >> 16;
    Header[21] = Sbits.size() >> 24;
    //
    Header[22] = Vbits.size();
    Header[23] = Vbits.size() >> 8;
    Header[24] = Vbits.size() >> 16;
    Header[25] = Vbits.size() >> 24;

    f.write(reinterpret_cast<char*>(Header), headerSize);
    //info
    //vetor de frequencias
    //escreve o vetor de ocorrencias no arquivo no formato
    //[VO.first] = VO.second -> VO.first: numero/indice/valor,  VO.second: qnt de vezes que aparece, num da frequencia de 0 a 255;
    int frequencias[256] = {0};
    for(int i = 0; i < 256; i++){
        frequencias[VO[i].first] = VO[i].second;
    }

    for(int i = 0; i < 256; i++){
        char byte = char(frequencias[i]);
        f.write(&byte, sizeof(byte));
    }

    ///completa H
    int h = Hbits.size() % 8;
    for(int i = 0; i < (8-h); i++)
        Hbits.append("0");

    ///completa S
    int s = Sbits.size() % 8;
    for(int i = 0; i < (8-s); i++)
        Sbits.append("0");
    
    ///completa V
    int v = Vbits.size() % 8;
    for(int i = 0; i < (8-v); i++)
        Vbits.append("0");
    
    
    int k = 0;
    char c = 0;
    for(int i = 0; i < Hbits.size(); i++){
        c = c << 1;
        k++;
        if(Hbits[i] == '1'){
            c = c | 0b1;
        }else{
            c = c | 0b0;
        }
        if(k == 8){
            k = 0;
            c = c & 255;
            f.write(&c, sizeof(c));
            c = c & 0;
        }
    }
    char z = 0;
    k = 0;
    for(int i = 0; i < Sbits.size(); i++){
        z = z << 1;
        k++;
        if(Sbits[i] == '1'){
            z = z | 0b1;
        }else{
            z = z | 0b0;
        }
        if(k == 8){
            k = 0;
            z = z & 255;
            f.write(&z, sizeof(z));
            z = z & 0;
        }
    }

    char w = 0;
    k = 0;
    for(int i = 0; i < Vbits.size(); i++){
        w = w << 1;
        k++;
        if(Vbits[i] == '1'){
            w = w | 0b1;
        }else{
            w = w | 0b0;
        }
        if(k == 8){
            k = 0;
            w = int(w) & 255;
            f.write(&w, sizeof(w));
            w = w & 0;
        }
    }

    f.close();


    std::cout << "Escrito com sucesso para: " << path << "\n";
    return 0;
}

struct HSV_Compac{
    int width;
    int height;
    pair<int, int> frequencia[256];
    string h;
    string s;
    string v;
};

HSV_Compac* descompactar(const char* path){
    std::cout << "iniciado descompactacao!\n";
    std::fstream f;
    //seleciona o arquivo, modo de abertura (leitura, escrita, atualizacao) | como vai ler arquivo
    f.open(path, std::ios::in | std::ios::binary);

    if(!f.is_open()){
        std::cout << "error ao ler arquivo";
        return nullptr;
    }

    const int HeaderSize = 26;
    unsigned char Header[HeaderSize];
    //le o Header
    f.read(reinterpret_cast<char*>(Header), HeaderSize);

    if(Header[0] != 'L' && Header[1] != 'N'){
        std::cout << "Arquivo em formato nao reconhecido!\n";
        f.close();
        return nullptr;
    }
    //tamanho arquivo original
    int fileSize = Header[2] + (Header[3] << 8) + (Header[4] << 16) + (Header[5] << 24);
    //Width original
    int widthOriginal =  Header[6] + (Header[7] << 8) + (Header[8]  << 16) + (Header[9]   << 24);
    //Height original
    int HeightOriginal = Header[10] + (Header[11] << 8) + (Header[12] << 16) + (Header[13]  << 24);
    //hsv sizes
    const int h_size = Header[14] + (Header[15] << 8) + (Header[16] << 16) + (Header[17]  << 24);
    const int s_size = Header[18] + (Header[19] << 8) + (Header[20] << 16) + (Header[21]  << 24);
    const int v_size = Header[22] + (Header[23] << 8) + (Header[24] << 16) + (Header[25]  << 24);
    
    HSV_Compac* compac = new HSV_Compac;
    compac->h = "";
    compac->s = "";
    compac->v = "";
    compac->width  = widthOriginal;
    compac->height = HeightOriginal;
    //vetor de frequencia
    unsigned char Frequencias[256];
    f.read(reinterpret_cast<char*>(Frequencias), sizeof(Frequencias));

    for(int i = 0; i < 256; i++){
        compac->frequencia[i].first = i;
        compac->frequencia[i].second = Frequencias[i];
    }
    ////////////////////////////////////////////////////////
    int h_resto = h_size % 8;
    int s_resto = s_size % 8;
    int v_resto = v_size % 8;
    /////////////////////////////////////////////////////////
    /// H
    char c = 0;
    string outH = "";
    for(int i = 0; i < h_size + (8-h_resto); i+=8){
        f.read(&c, sizeof(c));
        outH += fromByte(c & 255);
    }
    compac->h += outH;
    /// S
    char w = 0;
    string outS = "";
    for(int i = 0; i < (s_size + (8-s_resto)); i+=8){
        f.read(&w, sizeof(w));
        outS += fromByte(w & 255);
    }
    compac->s += outS;
    /// V
    char z = 0;
    string outV = "";
    for(int i = 0; i < (v_size + (8-v_resto)); i+=8){
        f.read(&z, sizeof(z));
        outV += fromByte(z & 255);
    }
    compac->v += outV;

    for(int i = 0; i < (8-h_resto); i++)
        compac->h.pop_back();

    for(int i = 0; i < (8-s_resto); i++)
        compac->s.pop_back();

    for(int i = 0; i < (8-v_resto); i++)
        compac->v.pop_back();

    std::cout << "Arquivo Descompactado!\n";

    f.close();
    return compac;
}


void SalvarBmp(const char* path, std::vector<int> h, std::vector<int> s, std::vector<int> v, int width, int height){

    std::vector<HSV> coresHSV;
    coresHSV.resize(width * height);
    for(int i = 0; i < (width * height); i++){
        coresHSV[i].v = v[i];
    }
    
    for(int y = 0, y2 = 0; y < (height/2); y++, y2+=2){
        for(int x = 0, x2 = 0; x < (width/2); x++, x2+=2){
            coresHSV[y2 * width + x2].h = h[y * width/2 + x];
            coresHSV[y2 * width + (x2+1)].h = h[y * width/2 + x];
            coresHSV[(y2+1) * width + x2].h = h[y * width/2 + x];
            coresHSV[(y2+1) * width + (x2+1)].h = h[y * width/2 + x];

            coresHSV[y2 * width + x2].s = s[y * width/2 + x];
            coresHSV[y2 * width + (x2+1)].s = s[y * width/2 + x];
            coresHSV[(y2+1) * width + x2].s = s[y * width/2 + x];
            coresHSV[(y2+1) * width + (x2+1)].s = s[y * width/2 + x];
        }   
    }

    Imagem imgag(width, height);
    for(int i = 0; i < (width * height); i++){
        if(coresHSV[i].v <= 5){
            auto c = hsvToRgb(0, 0, 0);
            imgag.m_colors[i] = c;
        }else{
            auto c = hsvToRgb(coresHSV[i].h, coresHSV[i].s, coresHSV[i].v);
            imgag.m_colors[i] = c;
        }
        
    }


    imgag.salvar(path);
    std::cout << "Imagem salva para: " << path <<"\n";
}


int main(){
	std::cout << "start conversor!\n";

    string local;
    string nomeArquivo;
    string formatoAr = ".bmp";
    string fullPath;
    std::cout << "Digite 1: para compactar arquivo\n";
    std::cout << "Digite 2: para descompactar arquivo\n";
    std::cout << "Opcao: ";
    int modo;
    cin >> modo;
    if(modo == 1){
        //pega o nome da Folder
        /*std::cout << "Digite local do arquivo: ";
        cin >> local;
        struct stat sb;
        //verifica se a Folder existe
        if (stat(local.c_str(), &sb) == 1){
            cout << "Folder is invalid!";
            using namespace std::this_thread; // sleep_for, sleep_until
            using namespace std::chrono; // nanoseconds, system_clock, seconds

            //sleep_for(nanoseconds(10));
            sleep_until(system_clock::now() + seconds(3));
            return 1;
        }
        local.append("/");*/

        //pega o nome da Folder
        std::cout << "Digite nome do arquivo: ";
        cin >> nomeArquivo;


        fullPath =  "Imagens/" + nomeArquivo + formatoAr;

        //abre a imagem e salva
        Imagem img(0, 0);
        vecRetorno *v = img.ler(fullPath.c_str() , 2,1,0, 2);

        auto arvore = huffman(v->frequencia, 256);

        auto hue         = stringFromH(v->hs_values, arvore);
     
        auto values      = stringFromV(v->v_values, arvore);

        auto saturation  = stringFromS(v->hs_values, arvore);


        string saidaS = "Imagens/" + nomeArquivo + ".ln";

        writeBites(saidaS.c_str(), hue, saturation, values, v->frequencia, v->FileSize, v->width, v->height);

    }else if(modo == 2){
         //pega o nome da Folder
        /*std::cout << "Digite local do arquivo: ";
        cin >> local;
        struct stat sb;
        //verifica se a Folder existe
        if (stat(local.c_str(), &sb) == 1){
            cout << "Folder is invalid!";
            using namespace std::this_thread; // sleep_for, sleep_until
            using namespace std::chrono; // nanoseconds, system_clock, seconds

            //sleep_for(nanoseconds(10));
            sleep_until(system_clock::now() + seconds(3));
            return 1;
        }
        local.append("/");*/
        //pega o nome da Folder
        std::cout << "Digite nome do arquivo: ";
        cin >> nomeArquivo;


        fullPath = "Imagens/" + nomeArquivo + ".ln";

        //abre a imagem e salva
        auto desc = descompactar(fullPath.c_str());

        insertionSort(desc->frequencia, 256);
        for (int i = 0; i < 256; i++){
            desc->frequencia[i].second = i;
        }

        auto arvore = huffman(desc->frequencia, 256);

        auto hueDescompac = getNumeroFromLista(desc->h, arvore);
        auto satDescompac = getNumeroFromLista(desc->s, arvore);
        auto valDescompac = getNumeroFromLista(desc->v, arvore);

        string saida =  "Imagens/" + nomeArquivo + "LN.bmp";
        SalvarBmp(saida.c_str(), hueDescompac, satDescompac, valDescompac, desc->width, desc->height);
        

    }else{
        std::cout << "Opcao nao listada fechando..";
        using namespace std::this_thread; // sleep_for, sleep_until
        using namespace std::chrono; // nanoseconds, system_clock, seconds

        //sleep_for(nanoseconds(10));
        sleep_until(system_clock::now() + seconds(3));
        return 1;
    }
	//Imagem img(0, 0);
    //imagem3.bmp
    //benchmark.bmp
	//vecRetorno *v = img.ler("benchmark.bmp", 2,1,0, 2);
  
    /*for(int i = 0; i < 256; i++)
        std::cout << v->frequencia[i].first << " " << v->frequencia[i].second << std::endl;

    //HSV_Compac* compac = ;
/**
    auto arvore = huffman(v->frequencia, 256);

    //std::cout <<"posicao 0 " << v->hs_values[0].second <<"\n";

   // std::cout << "Convertendo hue para string" <<"\n";
    auto hue         = stringFromH(v->hs_values, arvore);
 
    //std::cout << "Convertendo values para string" <<"\n";
    auto values      = stringFromV(v->v_values, arvore);

    //std::cout << "Convertendo saturation para string" <<"\n";
    auto saturation  = stringFromS(v->hs_values, arvore);

    std::cout <<values.size() % 8 <<"\n";

    writeBites("Comprimido.ln", hue, saturation, values, v->frequencia, v->FileSize, v->width, v->height);

    

    auto desc = descompactar("Comprimido.ln");

    insertionSort(desc->frequencia, 256);
    for (int i = 0; i < 256; i++){
        desc->frequencia[i].second = i;
    }

    auto arvore = huffman(desc->frequencia, 256);

    auto hueDescompac = getNumeroFromLista(desc->h, arvore);
    auto satDescompac = getNumeroFromLista(desc->s, arvore);
    auto valDescompac = getNumeroFromLista(desc->v, arvore);

   // for(int i = 0; i < 256; i++)
      //  std::cout << desc->frequencia[i].first << " " <<desc->frequencia[i].second << std::endl;
    
    //img.salvar("ImagensSalvas/Originalsalva1.bmp");

    SalvarBmp("ImagensSalvas/salva1.bmp", hueDescompac, satDescompac, valDescompac, desc->width, desc->height);

    //std::cout << hue.size()/*hueDescompac.size() <<"\n";*/
    //std::cout << desc->h.size()/*hueDescompac.size()*/ <<"\n";
    //std::cout << (hue == desc->h)/*hueDescompac.size()*/ <<"\n";
    //std::cout << (saturation == desc->s)/*hueDescompac.size()*/ <<"\n";
    //std::cout << (values == desc->v)/*hueDescompac.size()*/ <<"\n";
   // std::cout << values.size()/*hueDescompac.size()*/ <<"\n";
    //std::cout << desc->v.size()/*hueDescompac.size()*/ <<"\n";
    //std::cout << (hue == desc->h)/*hueDescompac.size()*/ <<"\n";
    //std::cout << desc->s.substr(0,8)/*hueDescompac.size()*/ <<"\n";
    
    //std::cout << satDescompac[0]/*satDescompac.size()*/ <<"\n";
    //std::cout << valDescompac[0]/*valDescompac.size()*/ <<"\n";

    //std::cout << saturation.size()/*values.substr(284168,8)*/ <<"\n";
	//std::cout << desc->s.size()/*desc->v.substr(284168,8)*/ <<"\n";


    std::cout << std::endl;
	system("pause");
	return 0;
}


/*
Legado



for(std::string::size_type i = 0 ; i < Vbits.size() ; i++){
        auto cAtual = Vbits[i];
        k++;
        if(cAtual == 'E'){
            esc = esc << 1;
            esc = esc | 0b000000000;
        }else{
            esc = esc << 1;
            esc = esc | 0b000000001;
        }

        if(k == 8 || i == Vbits.size()-1){
            f.write(reinterpret_cast<char*>(esc), 1);
            k = 0;
            //std::cout << esc << "\n";
            esc = 0;
        }
            
    }
    //std::cout << "\n";
    f.close();
    return 0;
    /*
    
    f.write(values.c_str(), values.size());
    f.write(hue.c_str(), hue.size());
    f.write(saturation.c_str(), saturation.size());

    f.close();*/

