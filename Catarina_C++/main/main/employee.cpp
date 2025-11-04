#include "employee.h"
#include <ctime>

// verifica se o ano e bissexto (ano com 29 dias em fevereiro)
bool leapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// valida se uma data (dia/mes/ano) e real e possivel
bool date_ok(int d, int m, int y) {
    // verifica limites basicos
    if (m < 1 || m > 12 || d < 1) return false;

    // tabela com dias de cada mes
    // fevereiro depende se e bissexto ou nao
    int days[] = { 31, leapYear(y) ? 29 : 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // verifica se o dia nao ultrapassa o limite do mes
    return d <= days[m - 1];
}

// verifica se a data escolhida e fim-de-semana
bool weekend(int d, int m, int y) {
    // cria estrutura de data para o C++ calcular o dia da semana
    std::tm t = { 0,0,0,d,m - 1,y - 1900 };
    std::mktime(&t);

    // retorna true se for DOMINGO (0) ou SABADO (6)
    return (t.tm_wday == 0 || t.tm_wday == 6);
}
