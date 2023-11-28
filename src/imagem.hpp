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

using std::string;





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

    HSV(int h, int s, int v):h(h), s(s), v(v)
    {
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
    float s = si / 100;
    float v = vi /100;

    float hi = (h / 255.0) * 360.0;
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

    void ler(const char* path, int rk, int gk, int bk );
    void salvar(const char* path) const;
};



class ImagemCompactada
{
private:
    int m_width;
    int m_height;
    std::vector<HSV> m_colors;
public:
    ImagemCompactada(int width, int height):m_width(width), m_height(height), m_colors(std::vector<HSV>(width*height))
    {}
    
    

    ~ImagemCompactada(){}

    int getWidth(){return m_width;}

    int getHeight(){return m_height;}

    HSV getColor(int x, int y) const { return m_colors[y * m_width + x]; }

    void setColor(const HSV& cor, int x, int y)
    {
        m_colors[y * m_width + x].h = cor.h;
        m_colors[y * m_width + x].s = cor.s;
        m_colors[y * m_width + x].v = cor.v;
    }

    //void ler(const char* path, int rk, int gk, int bk ){}

    void salvar(const char* path, Imagem img) const
    {
        std::fstream f;
    f.open(path, std::ios::out | std::ios::binary);
    
    if(!f.is_open()){
        std::cout << "Erro ao salvar\n";
        return;
    }

    unsigned char bmpPad[3] = {0, 0, 0};
    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

    const int fileHeaderSize = 7;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_height;

    unsigned char fileHeader[fileHeaderSize];
    //file type
    fileHeader[0] = 'L';
    fileHeader[1] = 'N';
    //file size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //pixel data offset
    fileHeader[6] = fileHeaderSize + informationHeaderSize;


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
            unsigned char h = static_cast<unsigned char>(getColor(x, y).h);
            unsigned char s = static_cast<unsigned char>(getColor(x, y).s);
            unsigned char v = static_cast<unsigned char>(getColor(x, y).v);

            unsigned char color[] = {h, s, v};

            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }

    f.close();

    std::cout << "Salvo em " << path << "\n";
    }
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

void Imagem::ler(const char* path, int rk, int gk, int bk ){
   std::fstream f;
   //seleciona o arquivo, modo de abertura (leitura, escrita, atualizacao) | como vai ler arquivo
   f.open(path, std::ios::in | std::ios::binary);

   if(!f.is_open()){
    std::cout << "error ao ler arquivo";
    return;
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
    int hOcr[256] = {0};
    /*for (int i = 0; i < 256; i++){
        hOcr[std::to_string(i)] = 0;
    }*/
    //std::set<int> hs;
    //int ocorrenciasDeH[256] = {0};
    for(int y = 0; y < m_height; y++){
        for (int x = 0; x < m_width; x++){
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);
            m_colors[y * m_width + x].g = static_cast<int>(color[gk]);
            m_colors[y * m_width + x].r = static_cast<int>(color[rk]);
            m_colors[y * m_width + x].b = static_cast<int>(color[bk]);
            HSV hsvC(m_colors[y * m_width + x].r, m_colors[y * m_width + x].g, m_colors[y * m_width + x].b);
            //hs.insert(hsvC.h);
            if(hsvC.v > 5){
                hOcr[hsvC.h]++;
                hOcr[hsvC.s]++;
                hOcr[hsvC.v]++;
            }else{
                hOcr[hsvC.v]++;
            }
            
            //ocorrenciasDeH[hsvC.h]++;
        }
        f.ignore(paddingAmount);
    }
    f.close();
    
    /*for (auto& i : hOcr){
        std::cout << "Cor em hsv: "<< i.first << " ocorrencias: " << i.second << "\n";
    }*/
    for (int i = 0; i < 256; i++){
        std::cout << "Cor em hsv: "<< i << " ocorrencias: " << hOcr[i] << "\n";
    }

    /*for (const auto& valor : hOcr) {
        std::cout << "Cor em hsv: "<< valor << " ocorrencias: " << ocorrenciasDeH[valor] << "\n";
    }*/

    std::cout << "arquivo " << path << " lido\n";
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