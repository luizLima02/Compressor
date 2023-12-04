#pragma once
#ifndef IMAGE
#define IMAGE

#include<vector>
#include<iostream>
#include <algorithm>
#include <cmath>
#include<fstream>
#include<map>
#include<set>
#include<string>
#include<new>

using std::string;

using std::pair;

void insertionSort(pair<int,int> arr[], int size)
{
    int i, j;
    pair<int,int> key;

    for(i = 1; i < size; i++)
    {
        key = arr[i];
        j = i - 1;
        while( j >= 0 && arr[j].second > key.second){
            arr[j+1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}



struct Color{
    int r, g, b;

    Color():r(0), g(0), b(0)
    {
    }

    Color(int nr, int ng, int nb):r(nr), g(ng), b(nb)
    {
    }

    void Print(){
        std::cout << "Cor em rgb: " << this->r << " " << this->g << " " << this->b << "\n";
    }

};

struct HSV{
    int h, s, v;

    HSV():h(0), s(0), v(0)
    {
    }

    HSV(int r, int g, int b)
    {
        float ht = 0;
        float R = r / 255.0f;
        float G = g / 255.0f;
        float B = b / 255.0f;

        float max = std::max(R, std::max(G, B));
        float min = std::min(R, std::min(G, B));
        float diff = max - min;

        if (max == min) {
            ht = 0;
        } else if (max == R) {
            ht = fmod((60 * ((G - B) / diff) + 360), 360);
        } else if (max == G) {
            ht = fmod((60 * ((B - R) / diff) + 120), 360);
        } else if (max == B) {
            ht = fmod((60 * ((R - G) / diff) + 240), 360);
        }

        if (max == 0) {
            s = 0;
        } else {
            s = round((diff / max) * 100);
        }

        v = round(max * 100);

        h = round((ht / 360.0) * 255);
    }

    HSV(Color cor)
    {
        float ht = 0;
        float R = cor.r / 255.0f;
        float G = cor.g / 255.0f;
        float B = cor.b / 255.0f;

        float max = std::max(R, std::max(G, B));
        float min = std::min(R, std::min(G, B));
        float diff = max - min;

        if (max == min) {
            ht = 0;
        } else if (max == R) {
            ht = fmod((60 * ((G - B) / diff) + 360), 360);
        } else if (max == G) {
            ht = fmod((60 * ((B - R) / diff) + 120), 360);
        } else if (max == B) {
            ht = fmod((60 * ((R - G) / diff) + 240), 360);
        }

        if (max == 0) {
            s = 0;
        } else {
            s = round((diff / max) * 100);
        }

        v = round(max * 100);

        h = round((ht / 360.0) * 255);
    }

    void Print(){
        std::cout << "Cor em hsv: "<< (this->h / 255.0)*360.0 << " " <<  this->s << " " <<  this->v << "\n";
    }
};

//retorna um int na forma 0xBBGGRR
int color_int(Color cor)
{
    int val;
    val = cor.b; val = val << 8;
    val = val | cor.g; val = val << 8;
    val = val | cor.r;
    return val;
}

//recebe um int na forma 0xBBGGRR
Color int_color(int cor)
{
    int r = cor & 0xFF; cor = cor >> 8;
    int g = cor & 0xFF; cor = cor >> 8;
    int b = cor & 0xFF;
    return Color(r,g,b);
}

Color hsvToRgb(int h, int si, int vi) {
    float s = float(si) / 100.0;
    float v = float(vi) /100.0;

    float hi = (float(h) / 255.0) * 360.0;
    int r;
    int g;
    int b;

    float C = v * s;
    float X = C * (1 - std::abs(fmod(hi / 60.0, 2) - 1));
    float m = v - C;

    float Rs, Gs, Bs;

    if(hi >= 0 && hi < 60) {
        Rs = C;
        Gs = X;
        Bs = 0;
    } else if(hi >= 60 && hi < 120) {
        Rs = X;
        Gs = C;
        Bs = 0;
    } else if(hi >= 120 && hi < 180) {
        Rs = 0;
        Gs = C;
        Bs = X;
    } else if(hi >= 180 && hi < 240) {
        Rs = 0;
        Gs = X;
        Bs = C;
    } else if(hi >= 240 && hi < 300) {
        Rs = X;
        Gs = 0;
        Bs = C;
    } else {
        Rs = C;
        Gs = 0;
        Bs = X;
    }

    r = (Rs + m) * 255;
    g = (Gs + m) * 255;
    b = (Bs + m) * 255;

    return Color(r, g, b);
}

std::vector<Color> FromHSV(std::vector<int> h, std::vector<int> s, std::vector<int> v, int width, int height){
    std::vector<Color> colRet;
    colRet.reserve(width * height);
    for(int y = 0; y < width; y+=2){
       for(int x = 0; x < height; x+=2){
           
           
        }
    }
}


struct vecRetorno{
    int width;
    int height;
    int FileSize;
    pair<int, int> frequencia[256];
    std::vector<int> v_values;
    std::vector<pair<int, int>> hs_values;
};



class Imagem
{
private:
    int m_width;
    int m_height;
    
public:
    std::vector<Color> m_colors;

    Imagem(int width, int height);
    ~Imagem();

    int getWidth(){return m_width;}

    int getHeight(){return m_height;}

    Color getColor(int x, int y) const;
    void setColor(const Color& cor, int x, int y);

    vecRetorno* ler(const char* path, int rk, int gk, int bk, int window );
    void salvar(const char* path) const;

    //void descompactar(const char* path);
};


Imagem::Imagem(int width, int height)
    :m_width(width), m_height(height), m_colors(std::vector<Color>(width*height))
{
}

Imagem::~Imagem()
{
}

Color Imagem::getColor(int x, int y) const{
    return m_colors[y * m_width + x];
}

void Imagem::setColor(const Color& color, int x, int y){
    m_colors[y * m_width + x].r = color.r;
    m_colors[y * m_width + x].g = color.g;
    m_colors[y * m_width + x].b = color.b;
}


vecRetorno* Imagem::ler(const char* path, int rk, int gk, int bk, int window){
   std::fstream f;
   //seleciona o arquivo, modo de abertura (leitura, escrita, atualizacao) | como vai ler arquivo
   f.open(path, std::ios::in | std::ios::binary);

   if(!f.is_open()){
    std::cout << "error ao ler arquivo";
    return nullptr;
   }

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    //onde armazenar, quantos bytes ler
    f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    unsigned char informationHeader[informationHeaderSize];
    f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    m_width =  informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6]  << 16) + (informationHeader[7]   << 24);
    m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11]  << 24);

    m_colors.reserve(m_width * m_height);

    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);
    
    //criacao do vetor de frequencia e dos tamanhos dos vetores hs e v
    pair<int, int> Ocorrencias[256];
    for (int i = 0; i < 256; i++){
        Ocorrencias[i].first  = i;
        Ocorrencias[i].second = 0;
    }
    vecRetorno* ret = new(std::nothrow)vecRetorno;
    
    for(int y = 0; y < m_height; y++){
        for (int x = 0; x < m_width; x++){
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);
            m_colors[y * m_width + x].g = static_cast<int>(color[gk]);
            m_colors[y * m_width + x].r = static_cast<int>(color[rk]);
            m_colors[y * m_width + x].b = static_cast<int>(color[bk]);
            HSV hsvC(m_colors[y * m_width + x].r, m_colors[y * m_width + x].g, m_colors[y * m_width + x].b);
            //ocorrencias
            Ocorrencias[hsvC.h].second++;
            Ocorrencias[hsvC.s].second++;
            Ocorrencias[hsvC.v].second++;
            //vetores HSV
            ret->hs_values.push_back(std::make_pair(hsvC.h, hsvC.s));
            ret->v_values.push_back(hsvC.v);
        }
        f.ignore(paddingAmount);
        ret->height = m_height;
        ret->width = m_width;
        ret->FileSize = fileSize;
    }
    f.close();
    
    std::vector<pair<int, int>> hs_Medias;
    for(int y = 0; y < m_height; y+=2){
       for(int x = 0; x < m_width; x+=2){
            //media de h
            int h1 = ret->hs_values[y * m_width + x].first;
            int h2 = ret->hs_values[y * m_width + (x+1)].first;
            int h3 = ret->hs_values[(y+1) * m_width + x].first;
            int h4 = ret->hs_values[(y+1) * m_width + (x+1)].first;
            int Media_H = (h1+h2+h3+h4)/4;
            //media de s
            int s1 = ret->hs_values[y * m_width + x].second;
            int s2 = ret->hs_values[y * m_width + (x+1)].second;
            int s3 = ret->hs_values[(y+1) * m_width + x].second;
            int s4 = ret->hs_values[(y+1) * m_width + (x+1)].second;
            int Media_S = (s1+s2+s3+s4)/4;
            hs_Medias.push_back(std::make_pair(Media_H,  Media_S));
        }
    }
    ret->hs_values.clear();
    for(auto val : hs_Medias){
        ret->hs_values.push_back(val);
    }

    //para fazer o vetor de frequencias
    insertionSort(Ocorrencias, 256);
    for (int i = 0; i < 256; i++){
        Ocorrencias[i].second = i;
    }

    for (int i = 0; i < 256; i++){
        ret->frequencia[i].first  =  Ocorrencias[i].first;
        ret->frequencia[i].second = Ocorrencias[i].second;
    }
    

    std::cout << "h " << ret->hs_values[10].first << "\n";
    std::cout << "s " << ret->hs_values[10].second << "\n";
    std::cout << "v " << ret->v_values[10] << "\n";
    
    std::cout << "arquivo " << path << " lido\n";

    return ret;
}


void Imagem::salvar(const char* path) const{
    std::fstream f;
    f.open(path, std::ios::out | std::ios::binary);
    
    if(!f.is_open()){
        std::cout << "Erro ao salvar\n";
        return;
    }

    unsigned char bmpPad[3] = {0, 0, 0};
    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_height;

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
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;
    //image height
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;
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

    for(int y = 0; y < m_height; y++){
        for (int x = 0; x < m_width; x++){
            unsigned char r = static_cast<unsigned char>(getColor(x, y).r);
            unsigned char g = static_cast<unsigned char>(getColor(x, y).g);
            unsigned char b = static_cast<unsigned char>(getColor(x, y).b);

            unsigned char color[] = {b, g, r};

            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }

    f.close();

    std::cout << "Salvo em " << path << "\n";
}




#endif