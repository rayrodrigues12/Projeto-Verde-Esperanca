#ifndef UNICSUL_H
#define UNICSUL_H

#include <time.h>
#include <stdio.h>

void get_data_hora(char *dt, char *hr)
{
    time_t now;
    struct tm *local;

    now = time(NULL);
    local = localtime(&now);

    /* Date: YYYYMMDD */
    snprintf(
        dt,
        30,
        "%04d%02d%02d",
        local->tm_year + 1900,
        local->tm_mon + 1,
        local->tm_mday
    );

    /* Time: HHMMSS */
    snprintf(
        hr,
        30,
        "%02d%02d%02d",
        local->tm_hour,
        local->tm_min,
        local->tm_sec
    );
}


void get_prox_data(
    const char *data,
    int qty_days,
    char *prox_data
)
{
    int year, month, day;
    struct tm date = {0};

    /* Convert YYYYMMDD string to numbers */
    sscanf(
        data,
        "%4d%2d%2d",
        &year,
        &month,
        &day
    );

    date.tm_year = year - 1900;
    date.tm_mon  = month - 1;
    date.tm_mday = day;

    /* Calculate weekday */
    mktime(&date);

    while (qty_days > 0)
    {
        /* Move one calendar day forward */
        date.tm_mday++;

        /* Recalculate weekday */
        mktime(&date);

        /*
         * tm_wday:
         * 0 = Sunday
         * 1 = Monday
         * 2 = Tuesday
         * ...
         * 6 = Saturday
         */

        if (date.tm_wday >= 1 && date.tm_wday <= 5)
        {
            qty_days--;
        }
    }

    /* Return YYYYMMDD */
    snprintf(
        prox_data,
        30,
        "%04d%02d%02d",
        date.tm_year + 1900,
        date.tm_mon + 1,
        date.tm_mday
    );
}


int grava_log(
    const char *data_process,
    const char *processing_time,
    int code,
    float process_value,
    float mdr_value,
    float net_value,
    const char *data_credit,
    const char *metodo
)
{
    char filename[100];
    FILE *file;


    /*
     * Generate log filename.
     *
     * Example:
     * log/20260914_233015_123456.json
     */

    char data_arquivo[30];
char hora_arquivo[30];

snprintf(
    data_arquivo,
    sizeof(data_arquivo),
    "%.4s%.2s%.2s",
    data_process,
    data_process + 5,
    data_process + 8
);

snprintf(
    hora_arquivo,
    sizeof(hora_arquivo),
    "%.2s%.2s%.2s",
    processing_time,
    processing_time + 3,
    processing_time + 6
);

snprintf(
    filename,
    sizeof(filename),
    "log/%s_%s_%06d.json",
    data_arquivo,
    hora_arquivo,
    code
);

    printf("\n[%s]\n", filename);


    /*
     * Open log file.
     */

    file = fopen(filename, "w");

    if (file == NULL)
    {
        return 1;
    }


    /*
     * Write JSON.
     */

    fprintf(
        file,
        "{\n"
        "    \"data_proc\": \"%s\",\n"
        "    \"hora_proc\": \"%s\",\n"
        "    \"cliente\": %06d,\n"
        "    \"valor_proc\": %.2f,\n"
        "    \"valor_mdr\": %.2f,\n"
        "    \"valor_credito\": %.2f,\n"
        "    \"data_credito\": \"%s\",\n"
        "    \"metodo\": \"%s\"\n"
        "}\n",

        data_process,
        processing_time,
        code,
        process_value,
        mdr_value,
        net_value,
        data_credit,
        metodo
    );


    /*
     * Close file.
     */

    fclose(file);

    return 0;
}

#endif
