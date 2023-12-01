#include<iostream>
#include<stdlib.h>
#include"imagem.hpp"
#include"estruturas.hpp"
#include<string>


std::string caminhoSalvo = "ImagensSalvas";


int main(){
	std::cout << "start conversor!\n";
    /*Noh e(10);
    Noh d(11);
    Arvore arv(&e, &d, e.valor+d.valor);

    Noh e2(20);
    Noh d2(21);
    Arvore arv2(&e2, &d2, e2.valor+d2.valor);

    Arvore arv3(&arv, &arv2, arv.raiz->valor + arv2.raiz->valor);
    PrintArvore(arv3.raiz, 0);*/
	//std::string nomeArquivo = "copia2.bmp";

	Imagem img(0, 0);
	vecRetorno *v = img.ler("benchmark.bmp", 1,0,2);
  
    auto arvore = huffman(v->frequencia, 256);
    for(int i = 0; i < 256; i++){
        auto repres = geRepresentacao(arvore, i);
        std::cout <<"Nova representacao de " << i << " R: " << repres <<"\n";
    }
    
    //std::cout << (s == "ach") << "\n-------------------\n";
    //std::cout <<"Valor de 254 arvore " << repres <<"\n-------------------\n";
    //lista.printLista();
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
	//system("pause");
	return 0;
}
