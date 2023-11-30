#pragma once


#include <iostream>

using std::cin;  using std::cout;

#include<string>

#include <new>

using std::string;
using std::nothrow;

struct Noh{
    int valor;
    Noh* fEsquerdo;
    Noh* fDireito;
    
    Noh(int valor):valor(valor),fEsquerdo(nullptr),fDireito(nullptr)
    {
    }

    Noh()
    {
    }
};

struct Arvore{
    Noh* raiz;

    /*Cria a arvore passando dois noh`s Esquerdo, Direito respectivamente*/
    Arvore(Noh* filhoE, Noh* filhoD, int valor){
        raiz = new(nothrow)Noh;
        raiz->fDireito = filhoD;
        raiz->fEsquerdo = filhoE;
        raiz->valor = valor;
    }

    /*Cria a arvore passando somente um valor*/
    Arvore(int valor){
        raiz = new(nothrow)Noh;
        raiz->fDireito = nullptr;
        raiz->fEsquerdo = nullptr;
        raiz->valor = valor;
    }

    /*Cria a arvore passando um noh que sera a raiz da arvore*/
    Arvore(Noh* nova_raiz){
        raiz = nova_raiz;
    }

    /*Cria a arvore passando duas arvores e um valor para a raiz*/
    Arvore(Arvore* ArvoreE, Arvore* ArvoreD, int valor){
        raiz = new(nothrow)Noh;
        raiz->fDireito = ArvoreD->raiz;
        raiz->fEsquerdo = ArvoreE->raiz;
        raiz->valor = valor;
    }

    /*Cria a arvore passando a arvore para o filho esquerdo e um noh para o filho direito*/
    Arvore(Arvore* ArvoreE, Noh* noh, int valor){
        raiz = new(nothrow)Noh;
        raiz->fDireito = noh;
        raiz->fEsquerdo = ArvoreE->raiz;
        raiz->valor = valor;
    }

    /*Cria a arvore passando a arvore para o filho direito e um noh para o filho esquerdo*/
    Arvore(Noh* noh, Arvore* ArvoreD,  int valor){
        raiz = new(nothrow)Noh;
        raiz->fEsquerdo = noh;
        raiz->fDireito = ArvoreD->raiz;
        raiz->valor = valor;
    }

};

void PrintArvore(Noh* raiz, int altura){
    //printo o valor de valor
    cout << "-------------" << "\n";
    cout << "valor: " << raiz->valor << " altura: " << altura << "\n";
    //percorro ate o ultimo filho esquerdo
    if(raiz->fEsquerdo != nullptr){
        PrintArvore(raiz->fEsquerdo, altura+1);
    }
    //percorro ate o ultimo filho direiro
    if(raiz->fDireito != nullptr){
        PrintArvore(raiz->fDireito, altura+1);
    }
    return;
    //retorno
}
