#define _POSIX_C_SOURCE 200809L

/* ========================================================
   sinonim.c — ADT Sinonim / Thesaurus (Implementasi)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include <stdio.h>
#include <string.h>
#include "sinonim.h"

#ifdef _WIN32
#define strcasecmp _stricmp
#endif

/* Forward declaration untuk fungsi internal */
static void trim(char *str);

/* ---------- Definisi variabel data sinonim ---------- */
Sinonim sinonimList[MAX_SINONIM];
int jumlahSinonim = 0;

/* --------------------------------------------------------
   loadSinonim — parsing synonym.txt
   Format baris: kata|sin1|sin2|sin3|... (maks 5 sinonim)
   -------------------------------------------------------- */
void loadSinonim(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (!fp)
    {
        fprintf(stderr,
                "[WARN] File \"%s\" tidak dapat dibuka. "
                "Fitur sinonim dinonaktifkan.\n",
                filename);
        return;
    }

    /* Buffer untuk membaca satu baris file */
    char baris[1000];

    jumlahSinonim = 0;

    while (fgets(baris, sizeof(baris), fp) &&
           jumlahSinonim < MAX_SINONIM)
    {
        /* Hilangkan karakter newline */
        baris[strcspn(baris, "\r\n")] = '\0';

        /* Lewati baris kosong */
        if (strlen(baris) == 0)
        {
            continue;
        }

        char *saveptr;
        char *token;

        /* Ambil kata utama */
        token = strtok_r(baris, "|", &saveptr);

        if (token == NULL)
        {
            continue;
        }

        trim(token);

        strncpy(
            sinonimList[jumlahSinonim].kata,
            token,
            29
        );

        sinonimList[jumlahSinonim].kata[29] = '\0';
        sinonimList[jumlahSinonim].count = 0;

        /* Ambil daftar sinonim */
        int s = 0;
        if (saveptr != NULL)
        {
            char *syn_saveptr;
            char *syn_token = strtok_r(saveptr, ",", &syn_saveptr);

            while (syn_token != NULL && s < MAX_SINONIM_PER_KATA)
            {
                trim(syn_token);

                strncpy(
                    sinonimList[jumlahSinonim].sinonim[s],
                    syn_token,
                    29
                );

                sinonimList[jumlahSinonim].sinonim[s][29] = '\0';

                s++;
                syn_token = strtok_r(NULL, ",", &syn_saveptr);
            }
        }

        sinonimList[jumlahSinonim].count = s;

        jumlahSinonim++;
    }

    fclose(fp);

    printf(
        "[INFO] %d data sinonim berhasil dimuat dari \"%s\".\n",
        jumlahSinonim,
        filename
    );
}

/* --------------------------------------------------------
   cariSinonim — linear search case-insensitive
   -------------------------------------------------------- */
int cariSinonim(const char *kata)
{
    int i;

    for (i = 0; i < jumlahSinonim; i++)
    {
        if (strcasecmp(
                sinonimList[i].kata,
                kata
            ) == 0)
        {
            return i;
        }
    }

    return -1;
}

/* --------------------------------------------------------
   tampilSinonim — cetak daftar sinonim
   -------------------------------------------------------- */
void tampilSinonim(int index)
{
    if (index < 0 || index >= jumlahSinonim)
    {
        printf("Sinonim tidak tersedia.\n");
        return;
    }

    if (sinonimList[index].count == 0)
    {
        printf("Sinonim tidak tersedia.\n");
        return;
    }

    printf("Sinonim:\n");

    int i;

    for (i = 0; i < sinonimList[index].count; i++)
    {
        printf("- %s\n",
               sinonimList[index].sinonim[i]);
    }
}

/* --------------------------------------------------------
   trim
   Menghapus spasi di depan dan belakang string
   -------------------------------------------------------- */
static void trim(char *str)
{
    int len = strlen(str);

    while (len > 0 &&
           (str[len - 1] == ' ' ||
            str[len - 1] == '\t'))
    {
        str[len - 1] = '\0';
        len--;
    }

    int start = 0;

    while (str[start] == ' ' ||
           str[start] == '\t')
    {
        start++;
    }

    if (start > 0)
    {
        memmove(
            str,
            str + start,
            strlen(str + start) + 1
        );
    }
}