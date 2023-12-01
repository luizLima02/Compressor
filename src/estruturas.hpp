#pragma once


#include <iostream>

using std::cin;  using std::cout;

#include<string>
#include <queue>
#include<vector>
#include <new>

using std::priority_queue;
using std::vector;
using std::pair;
using std::string;
using std::nothrow;

struct Noh{
    pair<int,int> valor;
    Noh* fEsquerdo;
    Noh* fDireito;
    
    Noh(pair<int,int> valor):valor(valor),fEsquerdo(nullptr),fDireito(nullptr)
    {
    }

    Noh(Noh* fE, Noh* fD)
    {
        fEsquerdo = fE;
        fDireito =  fD;
        valor.first = -1;
        valor.second = fEsquerdo->valor.second + fDireito->valor.second;
    }

    Noh()
    {
    }
};
/*
struct ListaDeNohs{
    Noh* lista;
    int size;
    int last;

    ListaDeNohs(pair<int, int>* ocorrencias, int sizeOcor){
        lista = new(std::nothrow)Noh[sizeOcor];
        size = sizeOcor;
        last = size - 1;
        for(int i = 0; i < sizeOcor; i++)
        {
            Noh n(ocorrencias[i]);
            lista[i] = n;
        }
    }

    void printLista(){
        for(int i = 0; i <= last; i++)
        {
            cout << "valor: " << lista[i].valor.first << " Ocorrencias:" << lista[i].valor.second << "\n";
        }
    }


    void moverParaFrente(int inicio){
        for(int i = last+1; i > inicio; i--){
            lista[i] = lista[i-1];
        }
    }
    //retorna true se a insercao for bem sucedida
    bool Inserir(Noh valor){
        for(int i = 0; i <=last; i++){
            if(valor.valor.second < lista[i].valor.second){
                moverParaFrente(i+1);
                lista[i] = valor;
                break;
            }else if(valor.valor.second >= lista[i].valor.second && i == last){
                lista[i+1] = valor;
                break;
            }
        }
        last++;
        return true;
    }

    bool Inserir(Noh *valor){
        for(int i = 0; i <=last; i++){
            if(valor->valor.second < lista[i].valor.second){
                moverParaFrente(i+1);
                lista[i] = *valor;
                break;
            }else if(valor->valor.second >= lista[i].valor.second && i == last){
                lista[i+1] = *valor;
                break;
            }
        }
        last++;
        return true;
    }


    Noh* RetirarMenor(){
        if(last >= 0){ 
            Noh* noh = new Noh();
            noh->valor = this->lista[0].valor;
            noh->fEsquerdo = this->lista[0].fEsquerdo;
            noh->fDireito = this->lista[0].fDireito;
            for(int i = 0; i <= last; i++){
                this->lista[i] = this->lista[i+1];
            }
            this->last--;
            return noh;
        }else{
            Noh* noh = new Noh(std::make_pair(-1,-1));
            return noh;
        }
    }

    int Tamanho(){
        return last+1;
    }

};
*/

class Compare {
public:
    bool operator()(Noh* below, Noh* above)
    {
        if (below->valor.second > above->valor.second) {
            return true;
        }
        return false;
    }
};


void printNoh(Noh* n, int alt){
        if(n == nullptr){return;}
        std::cout <<"Valor: f: " << n->valor.first <<" s: " << n->valor.second << " altura: " << alt << "\n";
        printNoh(n->fEsquerdo, alt+1);
        printNoh(n->fDireito, alt+1);
        return;
}

string geRepresentacao(Noh* n, int valorProcurado){
    if(n->valor.first == valorProcurado && (n->fDireito==nullptr && n->fEsquerdo == nullptr)){
        return string("achei");
    }
    if(n->fEsquerdo != nullptr){
        string r = geRepresentacao(n->fEsquerdo, valorProcurado);
        if(r == "achei"){
            return string("E");
        }else if(r != "nao"){
            return string("E") + r;
        }
    }

    if(n->fEsquerdo !=nullptr ){
        string r = geRepresentacao(n->fDireito, valorProcurado);
        if(r == "achei"){
            return string("D");
        }else if(r != "nao"){
            return string("D") + r;
        }
    }

    return string("nao");
}



//Passa uma lista de nohs ordenados da menor frequencia para a maior
/*priority_queue<Noh*, vector<Noh*>, Compare>*/Noh* huffman(pair<int, int>* ocorrencias, int sizeOcor){
    priority_queue<Noh*, vector<Noh*>, Compare> freq;
    for (int i = 0; i < sizeOcor; i++)
    {
        Noh* n = new Noh(ocorrencias[i]);
        freq.push(n);
    }
    //return freq;
    while(freq.size() > 1){
        Noh *s0 = freq.top();
        freq.pop();
        Noh *s1 = freq.top();
        freq.pop();
        Noh *xyz = new Noh(s0, s1);
        freq.push(xyz);
    }
    Noh* x;
    x = freq.top();
    freq.pop();
    return x;
}






////Codigo Legado
/*
struct Arvore{
    Noh* raiz;

    /*Cria a arvore passando dois noh`s Esquerdo, Direito respectivamente
    Arvore(Noh* filhoE, Noh* filhoD, int valor){
        raiz = new(nothrow)Noh;
        raiz->fDireito = filhoD;
        raiz->fEsquerdo = filhoE;
        raiz->valor = valor;
    }

    /*Cria a arvore passando somente um valor
    Arvore(int valor){
        raiz = new(nothrow)Noh;
        raiz->fDireito = nullptr;
        raiz->fEsquerdo = nullptr;
        raiz->valor = valor;
    }

    /*Cria a arvore passando um noh que sera a raiz da arvore
    Arvore(Noh* nova_raiz){
        raiz = nova_raiz;
    }

    /*Cria a arvore passando duas arvores e um valor para a raiz
    Arvore(Arvore* ArvoreE, Arvore* ArvoreD, int valor){
        raiz = new(nothrow)Noh;
        raiz->fDireito = ArvoreD->raiz;
        raiz->fEsquerdo = ArvoreE->raiz;
        raiz->valor = valor;
    }

    /*Cria a arvore passando a arvore para o filho esquerdo e um noh para o filho direito
    Arvore(Arvore* ArvoreE, Noh* noh, int valor){
        raiz = new(nothrow)Noh;
        raiz->fDireito = noh;
        raiz->fEsquerdo = ArvoreE->raiz;
        raiz->valor = valor;
    }

    /*Cria a arvore passando a arvore para o filho direito e um noh para o filho esquerdo
    Arvore(Noh* noh, Arvore* ArvoreD,  int valor){
        raiz = new(nothrow)Noh;
        raiz->fEsquerdo = noh;
        raiz->fDireito = ArvoreD->raiz;
        raiz->valor = valor;
    }

};*/
