#include<iostream>
#include<stdlib.h>
#include"imagem.hpp"
#include"estruturas.hpp"
#include<string>
#include <set>

std::string caminhoSalvo = "ImagensSalvas";


int main(){
	system("cls");
	std::cout << "start conversor!\n";
	std::set<int> coresInImg;
	//std::string nomeArquivo = "copia2.bmp";

	//Imagem img(0, 0);
	//img.ler("benchmark.bmp", 1,0,2);
	Color rgb(125, 100, 10);
	HSV hsv(rgb);
	rgb.Print();
	hsv.Print();
	std::cout << "Cor em int: " << color_int(rgb) <<  "\n";

	//std::string saida = caminhoSalvo + "/" + nomeArquivo;

	//img.salvar(saida.c_str());

	system("pause");
	return 0;
}
