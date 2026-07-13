#ifndef DICTIONARY_HPP
#define DICTIONARY_HPP
#include <unicode/ucnv.h>
#include <unicode/ustream.h>
#include <unicode/unistr.h>
#include <string>
#include <iostream>
#include <chrono>
#include "helpers.hpp"

// O template será utilizado para dizer qual estrutura vai armazenar as palavras do arquivo.
template <typename T>
class dictionary {
private:
    T dicionario;

public:

    // Construtor.
    dictionary() {}

    // Inserção: Insere uma palavra no dictionary.
    // Resumindo, ele tenta buscar a palavra no dictionary.
    // Caso a palavra já exista, aumenta a sua frequência.
    // Caso contrário, insere a palavra com frequência 1.
    void insert(const icu::UnicodeString& palavra) {
        try {
            auto& frequencia = dicionario.search(palavra);
            frequencia++;
        } catch(std::out_of_range& error) {
            dicionario.insert(palavra, 1);
        }
    }

    // Remoção: Remove uma palavra do dictionary.
    void remove(const icu::UnicodeString& palavra) {
        dicionario.remove(palavra);
    }

    // Verificação de existência: Verifica se uma palavra está presente no dictionary.
    bool contains(const icu::UnicodeString& palavra) const {
        return dicionario.contains(palavra);
    }

    // Atualização: Atualiza a quantidade de vezes que uma mesma palavra aparece no dictionary.
    void update(const icu::UnicodeString& palavra, int up) {
        dicionario.update(palavra, up);
    }

    // Acesso: Retorna a quantidade de vezes que uma palavra aparece no dictionary.
    // Acesso == Busca
    int search(const icu::UnicodeString& palavra) const {
        return dicionario.search(palavra);
    }

    // Tamanho: Retorna a quantidade de palavras presentes no dictionary.
    int getSize() const {
        return dicionario.getSize();
    }

    // Retorna a quantidade de comparações do dictionary.
    int getComparisons() const {
        return dicionario.getComparisons();
    }

    // Faz uma verificação para saber se o dictionary está vazio.
    bool isEmpty() const {
        return dicionario.isEmpty();
    }

    // Limpeza: Limpa o dictionary.
    void clear() {
        dicionario.clear();
    }

    // Gera uma tabela formatada contendo todas as palavras registradas no dictionary e suas frequências.
    icu::UnicodeString gerarTabela() {
        icu::UnicodeString lista;

        // Cabeçalho da tabela.
        lista += "|------------------------------|\n";
        lista += "| Palavra         | Frequência |\n";
        lista += "|-----------------|------------|\n";

        // Percorre o dictionary (palavra e frequência).
        for(const auto& p : dicionario) {
            icu::UnicodeString palavra = p.first;

            // Converte a frequência de int para string e depois converte para UnicodeString.
            icu::UnicodeString frequencia = icu::UnicodeString::fromUTF8(std::to_string(p.second));

            // Preenche com espaços até atingir o tamanho da coluna.
            // Útil para alinhamento (questão estética).
            while(palavra.length() < 15) {
                palavra += " ";
            }

            while(frequencia.length() < 10) {
                frequencia += " ";
            }

            lista += "| " + palavra + " | " + frequencia + " |\n";
        }

        lista += "|------------------------------|\n";
        return lista;
    }

    // Salva o conteúdo do dictionary em um arquivo de saída.
    // O arquivo contém: quantidade de comparações, quantidade de rotações (para as árvores balanceadas),
    // tamanho do dictionary, tempo de execução e a tabela formatada com as palavras e suas frequências.
    void salvarConteudo(const std::string& arquivo, std::chrono::milliseconds duracao, const std::string& estrutura) {
        // Gera as estatísticas de saída da estrutura.
        std::string saida;
        saida += "Estrutura Utilizada: " + estrutura + "\n";
        saida += "Tamanho da tabela: " + std::to_string(this->getSize()) + " palavras\n";
        saida += "Tempo para finalizar a tabela (em milissegundos): " + std::to_string(duracao.count()) + " milissegundos\n";
        saida += "Quantidade de comparações: " + std::to_string(dicionario.getComparisons()) + "\n";

        // Verifica se o tipo T (estrutura utilizada para criar a tabela) possui o método getRotations().
        if constexpr(has_getRotations<decltype(dicionario)>::value) {
            saida += "Quantidade de rotações: " + std::to_string(dicionario.getRotations()) + "\n";
        }

        saida += "\nDicionário de Palavras:\n\n";
        
        // Converte a tabela formatada de UnicodeString para std::string.
        std::string tabela;
        this->gerarTabela().toUTF8String(tabela);
        saida += tabela;

        // Salva no arquivo.
        helpers::registrarArquivo(arquivo, saida);
    }

};

#endif