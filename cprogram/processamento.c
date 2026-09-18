#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unicsul.h"

int main(int argc, char *argv[])
{
    char dt_proc[30];
    char hr_proc[30];
    char prox_dt[30];

    char data_formatada[30];
    char hora_formatada[30];
    char credito_formatado[30];

    int codigo;
    int valor;
    int metodo;

    int valor_mdr;
    int valor_liquido;

    const char *nome_metodo;

    if (argc != 4)
    {
        printf("Quantidade de argumentos invalida.\n");
        return 1;
    }

    codigo = atoi(argv[1]);
    valor = atoi(argv[2]);
    metodo = atoi(argv[3]);

    /*
     * Meio de pagamento:
     *
     * 0 = PIX
     * 1 = DEBITO
     * 2 = CREDITO
     */

    switch (metodo)
    {
        case 0:
            nome_metodo = "PIX";
            valor_mdr = 0;
            break;

        case 1:
            nome_metodo = "DEBITO";
            valor_mdr = valor / 100;       /* 1% */
            break;

        case 2:
            nome_metodo = "CREDITO";
            valor_mdr = (valor * 5) / 100; /* 5% */
            break;

        default:
            printf("Metodo de pagamento invalido.\n");
            return 1;
    }

    /* Calcula o valor liquido */
    valor_liquido = valor - valor_mdr;

    /*
     * A biblioteca retorna:
     *
     * Data: YYYYMMDD
     * Hora: HHMMSS
     */

    get_data_hora(dt_proc, hr_proc);

    /*
     * Calcula a proxima data util.
     * Mantemos dt_proc sem formatacao porque
     * get_prox_data trabalha com YYYYMMDD.
     */

    get_prox_data(dt_proc, 1, prox_dt);

    /*
     * Formata a data:
     * YYYYMMDD -> YYYY-MM-DD
     */

    snprintf(
        data_formatada,
        sizeof(data_formatada),
        "%.4s-%.2s-%.2s",
        dt_proc,
        dt_proc + 4,
        dt_proc + 6
    );

    /*
     * Formata a hora:
     * HHMMSS -> HH:MM:SS
     */

    snprintf(
        hora_formatada,
        sizeof(hora_formatada),
        "%.2s:%.2s:%.2s",
        hr_proc,
        hr_proc + 2,
        hr_proc + 4
    );

    /*
     * Formata a data do credito:
     * YYYYMMDD -> YYYY-MM-DD
     */

    snprintf(
        credito_formatado,
        sizeof(credito_formatado),
        "%.4s-%.2s-%.2s",
        prox_dt,
        prox_dt + 4,
        prox_dt + 6
    );

    /* Exibe o resultado */

    printf("Data do processamento: %s\n", data_formatada);
    printf("Hora do processamento: %s\n", hora_formatada);
    printf("Codigo: %06d\n", codigo);
    printf("Meio de pagamento: %s\n", nome_metodo);
    printf("Valor: %d\n", valor);
    printf("MDR: %d\n", valor_mdr);
    printf("Valor liquido: %d\n", valor_liquido);
    printf("Data do credito: %s\n", credito_formatado);

    /*
     * Grava o JSON utilizando as datas
     * no formato solicitado pela atividade.
     */

    grava_log(
        data_formatada,
        hora_formatada,
        codigo,
        valor,
        valor_mdr,
        valor_liquido,
        credito_formatado,
        nome_metodo
    );

    return 0;
}
