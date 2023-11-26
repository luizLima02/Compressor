#include<iostream>
#include<stdlib.h>
#include"imagem.h"
#include<string>


std::string caminhoSalvo = "ImagensSalvas";


int main(){
	system("cls");
	std::cout << "start conversor!\n";

	std::string nomeArquivo = "copia2.bmp";

	Imagem img(640, 480);
	img.ler("benchmark.bmp", 1,0,2);

	std::string saida = caminhoSalvo + "/" + nomeArquivo;

	img.salvar(saida.c_str());

	system("pause");
	return 0;
}
