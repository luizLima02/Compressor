#pragma once


#include <iostream>

using std::cin;  using std::cout;


#include <new>

using std::nothrow;

/**
template <typename TC, typename TV>
struct Dicionario
  {
   struct Par { TC chave;  TV valor; };

   Par *v;  int tam_v, n;

   bool iniciar ()
     {
      // TODO: Esta função deve deixar o dicionário num estado utilizável e
      // vazio, e deve retornar "false" sse houver falha de alocação de memória.
     }

   bool redimensionar (int novo_tam)  // Pré-condição: n <= novo_tam.
     {
      // TODO: Esta função deve substituir o vetor atual por outro de tamanho
      //       "novo_tam", retornando "false" sse houver falha de alocação.
     }

   bool inserir (TC c, TV valor)  // Pré-condição: "c" não está no dicionário.
     {
      // TODO: Esta função deve inserir a chave "c" e valor "valor" no
      //       dicionário, já partindo da pré-condição de que "c" é uma chave
      //       nova, e deve retornar "false" sse houver falha de alocação.
     }

//   TConsulta { bool existe; TV valor; };
//
//   TConsulta consulta (TC c)  // A consulta poderia ser implementada assim.
//     {
//     }

   struct Iterador  // A implementação de iterador já foi feita pelo professor.
     {
      Par *p;

      void operator ++ () { ++p; }

      bool operator != (Iterador it) { return p != it.p; }

      bool operator == (Iterador it) { return p == it.p; }

      Par operator * () { return *p; }

      TV valor () { return p->valor; }  // (*p).valor

      TC chave () { return p->chave; }  // (*p).chave

     };  // Iterador

   Iterador inicio () { Iterador it; it.p = v; return it; }

   Iterador fim () { Iterador it; it.p = v + n; return it; }

   Iterador consultar (TC c)
     {
      // TODO: Esta função deve retornar um iterador apontando para a chave "c",
      //       caso ela esteja presente no dicionário, ou então um iterador para
      //       o "fim", em caso contrário.
     }

   Iterador alterar (TC c, TV novo_valor)
     {
      // TODO: Esta função deve alterar para "v" o valor associado à chave "c",
      //       caso ela exista (ie., esteja presente no dicionário). Caso "c"
      //       não exista, então o dicionário deve ser mantido inalterado.
      //       A função também deve retornar um iterador para a chave "c", no
      //       primeiro caso, ou então um iterador para o "fim", no 2º caso.
     }

   bool remover (TC c)  // Pré-condição: "c" está no dicionário.
     {
      // TODO: Esta função deve remover a chave "c", já partindo da pré-condição
      //       de que "c" está no dicionário. Caso o vetor passe a estar
      //       exatamente 25% preenchido, ele deve ser substituído por outro com
      //       a metade do tamanho. A função deve retornar "false" sse houver
      //       falha de alocação.
     }

  };  // Dicionario
*/