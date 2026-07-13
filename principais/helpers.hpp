#ifndef HELPERS_HPP
#define HELPERS_HPP
#include <unicode/unistr.h>
#include <unicode/uchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <type_traits>

// Usado para permitir que o UnicodeString funcione como chave nas tabelas hash.
// A ICU define hashCode() para UnicodeString, mas o C++ exige um functor
// explícito com operator() para usá-la como chave em estruturas de hash.
struct hashUni {
    size_t operator()(const icu::UnicodeString& str) const {
        return str.hashCode();
    }
};

// Trait has_getRotations: Verifica em tempo de compilação se um tipo T possui o método getRotations().
// Isso é útil para que o código possa incluir a quantidade de rotações apenas para estruturas como árvores, e ignorar esse detalhe nas tabelas hash.

// A ideia central é usar SFINAE (Substitution Failure Is Not An Error) com std::void_t para detectar se a expressão T().getRotations() é valida.
// Caso ela seja válida, herda de std::true_type -> has_getRotations<T>::value == true.
// Se ela não for válida, herda de std::false_type -> has_getRotations<T>::value == false.

// Isso permite utilizar if constexpr (has_getRotations<T>::value) para condicionar ao código a existência ou não do método.

template <typename T, typename = void>
struct has_getRotations : std::false_type {};

template <typename T>
struct has_getRotations<T, std::void_t<decltype(std::declval<T>().getRotations())>> : std::true_type {};

// Classe utilitária com métodos auxiliares para o funcionamento deste projeto.
class helpers {

public:

    // Funções utilitárias sem template.
    // Serão implementadas no helpers.cpp.

    static icu::UnicodeString lerEFormatarArquivo(const std::string& arquivo);

    static void registrarArquivo(const std::string& arquivo, const std::string& conteudo);

    // Funções com template.
    // Implementadas diretamente neste arquivo.

    // Separa um texto já formatado em palavras, com base em espaços em branco,
    // e insere cada palavra no dictionary fornecido.
    template <typename T>
    static void inserirTexto(T& dictionary, icu::UnicodeString texto) {
        icu::UnicodeString palavra;

        // Adiciona um espaço em branco no final para garantir que a última palavra seja processada.
        texto.append(" ");

        int i = 0;
        while(i < texto.length()) {
            // Obtém o caractere Unicode no índice i, tratado corretamente para suportar acentos (code point 32 bits).
            UChar32 caractere = texto.char32At(i);
            ++i;

            // Se o caractere for um espaço em branco, insere no dictionary e limpa a palavra acumulada.
            if(u_isWhitespace(caractere)) {
                if(!palavra.isEmpty()) {
                    dictionary.insert(palavra);
                    palavra.remove();
                }
            } else {
                // Caso contrário, adiciona o caractere na palavra atual.
                palavra.append(caractere);
            }
        }
    }

    // Executa uma função passada como parâmetro e retorna o tempo de execução dessa função.
    template <typename F>
    static std::chrono::milliseconds medirTempo(F&& f) {
        auto inicio = std::chrono::high_resolution_clock::now();

        // Chama a função que será medida.
        f();

        auto fim = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(fim - inicio);
    }

    // Lê um arquivo, insere seu conteúdo no dictionary, mede o tempo de execução
    // e salva os resultados formatados em um novo arquivo de saída.
    template <typename T>
    static void processarArquivo(T& dictionary, const std::string& arquivo, const std::string& estrutura) {
        // Lê o conteúdo do arquivo na pasta e converte o conteúdo em UnicodeString.
        icu::UnicodeString arq = lerEFormatarArquivo("testesFinais/entrada/" + arquivo);

        // Mede quanto tempo leva para executar o método inserirTexto,
        // que insere as palavras do texto no dictionary.
        auto duracao = medirTempo([&]() {
            inserirTexto(dictionary, arq);
        });

        // Gera o nome do arquivo de saída.
        std::string saida = "testesFinais/saida/" + estrutura + "--" + arquivo;
        
        // Salva o dictionary.
        dictionary.salvarConteudo(saida, duracao, estrutura);  
    }

};

#endif