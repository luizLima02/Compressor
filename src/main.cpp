#include<iostream>
#include<stdlib.h>
#include"imagem.hpp"
#include"estruturas.hpp"
#include<string>
#include <bitset>


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
    cout << "H bits: " << Hbits.size() % 8 << std::endl;
    int h = Hbits.size() % 8;
    for(int i = 0; i < (8-h); i++)
        Hbits.append("0");

    ///completa S
    cout << "S bits: " << Sbits.size() % 8<< std::endl;
    int s = Sbits.size() % 8;
    for(int i = 0; i < (8-s); i++)
        Sbits.append("0");
    
    ///completa V
    cout << "V bits: " << Vbits.size() % 8<< std::endl;
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
            if(i == 7){cout << "primeiro byte escrito: " << int(c) << std::endl;}
            if(i == Hbits.size()-1){cout << "ultimo byte escrito: " << int(c) << std::endl;}
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
            if(i == 7){cout << "primeiro byte escrito S: " << int(z) << std::endl;}
            if(i == Sbits.size()-1){cout << "ultimo byte escrito S: " << (int(z) & 255) << std::endl;}
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
            if(i == 7){cout << "primeiro byte escrito V: " << (int(w) & 255) << std::endl;}
            if(i == Vbits.size()-1){cout << "ultimo byte escrito V: " << (int(w) & 255) << std::endl;}
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
/*

void SalvarBmp(const char* path, std::vector<int> h, std::vector<int> s, std::vector<int> v, int width, int height){
    /*std::fstream f;
    f.open(path, std::ios::out | std::ios::binary);
    
    if(!f.is_open()){
        std::cout << "Erro ao salvar\n";
        return;
    }*/

    //std::vector<Color> m_colors;
    //m_colors.reserve(width * height);
    /*unsigned char bmpPad[3] = {0, 0, 0};
    const int paddingAmount = ((4 - (width * 3) % 4) % 4);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + width * height * 3 + paddingAmount * height;

    unsigned char fileHeader[fileHeaderSize];
    //file type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    //file size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //reserved 1
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    //reservado 2
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    //pixel data offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;


    unsigned char informationHeader[informationHeaderSize];
    //header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    //image width
    informationHeader[4] = width;
    informationHeader[5] = width >> 8;
    informationHeader[6] = width >> 16;
    informationHeader[7] = width >> 24;
    //image height
    informationHeader[8] = height;
    informationHeader[9] = height >> 8;
    informationHeader[10] = height >> 16;
    informationHeader[11] = height >> 24;
    //planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    //bits per pixel(rgb)
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    //compression (no compression) 
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    //image size (no compression) 
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    //X pixels per meter
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    //Y pixels per meter
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    //total colors
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    //important colors
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);


    Imagem img(width, height);
    //make rgb mat from hsv
    for(int y = 0; y < height; y+=1){
        for (int x = 0; x < width; x+=4){
           Color cor1 = hsvToRgb(h[x], s[x], v[y * width + x]);
           Color cor2 = hsvToRgb(h[x], s[x], v[y * width + x+1]);
           Color cor3 = hsvToRgb(h[x], s[x], v[(y) * width + x+2]);
           Color cor4 = hsvToRgb(h[x], s[x], v[(y) * width + x+3]);
           img.setColor(cor1, x, y);
           img.setColor(cor2, x+1, y);
           img.setColor(cor3, x+2, y);
           img.setColor(cor4, x+3, y);
        }
    }
    img.salvar(path);
    std::cout << "Imagem salva para: " << path <<"\n";
}
*/

int main(){
	std::cout << "start conversor!\n";

	Imagem img(0, 0);
	vecRetorno *v = img.ler("benchmark.bmp", 1,0,2, 2);
  
    
    //HSV_Compac* compac = ;

    auto arvore = huffman(v->frequencia, 256);

    //std::cout <<"posicao 0 " << v->hs_values[0].second <<"\n";

    std::cout << "Convertendo hue para string" <<"\n";
    auto hue         = stringFromH(v->hs_values, arvore);
 
    std::cout << "Convertendo values para string" <<"\n";
    auto values      = stringFromV(v->v_values, arvore);

    std::cout << "Convertendo saturation para string" <<"\n";
    auto saturation  = stringFromS(v->hs_values, arvore);

    std::cout <<values.size() % 8<<"\n";

    writeBites("Comprimido.ln", hue, saturation, values, v->frequencia, v->FileSize, v->width, v->height);

    std::cout <<values.size() % 8<<"\n";
    auto desc = descompactar("Comprimido.ln");

    auto hueDescompac = getNumeroFromLista(desc->h, arvore);
    auto satDescompac = getNumeroFromLista(desc->s, arvore);
    auto valDescompac = getNumeroFromLista(desc->v, arvore);

    std::cout << "h: " << hueDescompac[0] <<"\n";
    std::cout << "s: " << satDescompac[0] <<"\n";
    std::cout << "v: " << valDescompac[0] <<"\n";

    //SalvarBmp("ImagensSalvas/salva1.bmp", hueDescompac, satDescompac, valDescompac, desc->width, desc->height);

    //std::cout << hue.size()/*hueDescompac.size()*/ <<"\n";
    //std::cout << desc->h.size()/*hueDescompac.size()*/ <<"\n";
    std::cout << (hue == desc->h)/*hueDescompac.size()*/ <<"\n";
    std::cout << (saturation == desc->s)/*hueDescompac.size()*/ <<"\n";
    std::cout << (values == desc->v)/*hueDescompac.size()*/ <<"\n";
   // std::cout << values.size()/*hueDescompac.size()*/ <<"\n";
    //std::cout << desc->v.size()/*hueDescompac.size()*/ <<"\n";
    //std::cout << (hue == desc->h)/*hueDescompac.size()*/ <<"\n";
    //std::cout << desc->s.substr(0,8)/*hueDescompac.size()*/ <<"\n";
    
    //std::cout << satDescompac[0]/*satDescompac.size()*/ <<"\n";
    //std::cout << valDescompac[0]/*valDescompac.size()*/ <<"\n";

    //std::cout << saturation.size()/*values.substr(284168,8)*/ <<"\n";
	//std::cout << desc->s.size()/*desc->v.substr(284168,8)*/ <<"\n";


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

