#include<iostream>
#include<stdlib.h>
#include"imagem.hpp"
#include"estruturas.hpp"
#include<string>


std::string caminhoSalvo = "ImagensSalvas";


int main(){
	system("cls");
	system("cls");
	std::cout << "start conversor!\n";
	//std::string nomeArquivo = "copia2.bmp";

	Imagem img(0, 0);
	img.ler("benchmark.bmp", 1,0,2);

	//std::string saida = caminhoSalvo + "/" + nomeArquivo;

	/*std::ifstream file("benchmark.bmp", std::ios::binary);

    if (!file) {
        std::cerr << "Não foi possível abrir o arquivo.\n";
        return 1;
    }

    char byte;
    while (file.read(&byte, sizeof(byte))) {
        // Processa o byte
        std::cout << "Byte lido: " << static_cast<int>(byte) << '\n';
    }

    file.close();
	//img.salvar(saida.c_str());
	*/
	system("pause");
	return 0;
}
