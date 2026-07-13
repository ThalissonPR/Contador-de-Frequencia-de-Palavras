#ifndef COMPARING_HPP
#define COMPARING_HPP
#include <unicode/coll.h>
#include <unicode/unistr.h>

// Esta struct define um comparador personalizado para palavras Unicode utilizando ICU.
// Garante que as comparações considerem regras linguísticas corretas, como acentos e caracteres especiais.
// Usado para ordenar palavras corretamente nas estruturas de dados do projeto.
struct comparing {
    // Ponteiro para o comparador da ICU.
    icu::Collator* col;

    // Construtor.
    comparing() {
        UErrorCode status = U_ZERO_ERROR;
        col = icu::Collator::createInstance(status);
    }

    // Destrutor.
    ~comparing() {
        delete col;
    }

    // Define o operador de comparação < para duas UnicodeString.
    // Retorna true se "a" deve vir antes de "b" na ordenação linguística. 
    bool operator()(const icu::UnicodeString& a, const icu::UnicodeString& b) const {
        UErrorCode status = U_ZERO_ERROR;
        return col->compare(a, b, status) < 0;
    }

};

#endif