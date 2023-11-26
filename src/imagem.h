#pragma once
#ifndef IMAGE
#define IMAGE

#include<vector>
#include<iostream>
#include<fstream>

struct Color{
    int r, g, b;

    Color():r(0), g(0), b(0)
    {
    }

    Color(int nr, int ng, int nb):r(nr), g(ng), b(nb)
    {
    }

};


class Imagem
{
private:
    int m_width;
    int m_height;
    std::vector<Color> m_colors;
public:
    Imagem(int width, int height);
    ~Imagem();

    Color getColor(int x, int y) const;
    void setColor(const Color& cor, int x, int y);

    void ler(const char* path, int rk, int gk, int bk );
    void salvar(const char* path) const;
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

     for(int y = 0; y < m_height; y++){
        for (int x = 0; x < m_width; x++){
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);
            m_colors[y * m_width + x].g = static_cast<int>(color[gk]);
            m_colors[y * m_width + x].r = static_cast<int>(color[rk]);
            m_colors[y * m_width + x].b = static_cast<int>(color[bk]);
        }
        f.ignore(paddingAmount);
    }
    f.close();

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