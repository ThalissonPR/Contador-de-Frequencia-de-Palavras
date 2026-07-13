#include "helpers.hpp"
#include <unicode/ucnv.h>
#include <unicode/uchar.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <cstdlib>

// Lê o conteúdo de um arquivo de texto, converte para UnicodeString e faz a formatação.
// Essa formatação inclui: transformar as letras em minúsculas e remover símbolos e pontuação, com exceção dos hífens entre letras.
// Retorna o texto pronto para ser processado pelo dictionary.
icu::UnicodeString helpers::lerEFormatarArquivo(const std::string& arquivo) {
    // Abre o código em modo binário para evitar problemas de codificação.
    std::ifstream entrada(arquivo, std::ios::binary);

    // Verifica se o arquivo foi aberto corretamente.
    if(!entrada.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo!\n";
        std::exit(1);
    }

    // Lê todo o conteúdo do arquivo em uma string.
    // Eficiente para uso com iteradores (meu caso).
    std::string conteudoBruto((std::istreambuf_iterator<char>(entrada)), std::istreambuf_iterator<char>());
    
    // Converte de UTF-8 para UnicodeString.
    // Lida corretamente com acentos e caracteres especiais.
    icu::UnicodeString texto = icu::UnicodeString::fromUTF8(icu::StringPiece(conteudoBruto));

    // Converte o texto para letras minúsculas.
    texto.toLower();

    int i = 0;
    while(i < texto.length()) {
        UChar caractere = texto[i];

        // Verifica se o caractere não é uma letra.
        if(!u_isalpha(caractere)) {
            // Verifica se o caractere atual é um hífen entre duas letras.
            if(caractere == '-' && i > 0 && u_isalpha(texto[i - 1]) && i < texto.length() - 1 && u_isalpha(texto[i + 1])) {
                // Não faz nada.
                // Mantém o hífen entre as letras.
            } else {
                // Caso contrário, substitui por um espaço.
                texto.replace(i, 1, u' ');
            }
        }
        
        ++i;
    }

    // Retorna o conteúdo formatado.
    return texto;
}

// Salva o conteúdo de uma string em um arquivo no disco.
// Se não for possível abrir o arquivo, encerra o programa com erro.
void helpers::registrarArquivo(const std::string& arquivo, const std::string& dados) {
    // Abre o arquivo de saída.
    std::ofstream saida(arquivo);

    // Verifica se o arquivo foi aberto corretamente.
    if(!saida.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo para escrita!\n";
        std::exit(1);
    }

    // Grava os dados no arquivo e fecha o arquivo.
    saida << dados;
    saida.close();
}