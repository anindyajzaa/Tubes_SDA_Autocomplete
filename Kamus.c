#define _POSIX_C_SOURCE 200809L

/* ========================================================
   kamus.c — ADT Kamus (Implementasi)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "kamus.h"

/* ---------- Definisi variabel data kamus ---------- */
Kamus kamus[115213];
int jumlahKamus = 0;


/* ========================================================
   LOAD KAMUS
   Membaca file:
   kata|kelas|arti|contoh|turunan|frasa
   ======================================================== */

void loadKamus(const char *filename)
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("[ERROR] File %s tidak ditemukan!\n", filename);
        return;
    }

    /* Buffer untuk membaca 1 baris file */
    char baris[10000];

    jumlahKamus = 0;

    while (fgets(baris, sizeof(baris), fp))
    {
        /* Hilangkan karakter newline */
        baris[strcspn(baris, "\r\n")] = '\0';

        /* Skip baris kosong */
        if (strlen(baris) == 0)
        {
            continue;
        }

        /* Skip header */
        if (strncmp(baris, "Kata", 4) == 0)
        {
            continue;
        }

        char *token;
        char *saveptr;

        /* ==========================
           KOLOM 1 : KATA
           ========================== */

        token = strtok_r(baris, "|", &saveptr);

        if (token == NULL)
        {
            continue;
        }

        trim(token);

        strcpy(kamus[jumlahKamus].kata, token);

        /* ==========================
           KOLOM 2 : KELAS KATA
           ========================== */

        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            trim(token);
            strcpy(kamus[jumlahKamus].kelas, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].kelas, "");
        }

        /* ==========================
           KOLOM 3 : ARTI
           ========================== */

        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            trim(token);
            strcpy(kamus[jumlahKamus].arti, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].arti, "");
        }

        /* ==========================
           KOLOM 4 : CONTOH
           ========================== */

        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            trim(token);
            strcpy(kamus[jumlahKamus].contoh, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].contoh, "");
        }

        /* ==========================
           KOLOM 5 : TURUNAN
           ========================== */

        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            trim(token);
            strcpy(kamus[jumlahKamus].turunan, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].turunan, "");
        }

        /* ==========================
           KOLOM 6 : FRASA
           ========================== */

        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            trim(token);
            strcpy(kamus[jumlahKamus].frasa, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].frasa, "");
        }

        jumlahKamus++;

        /* Batas maksimum data */
        if (jumlahKamus >= 115213)
        {
            break;
        }
    }

    fclose(fp);

    printf("\n[INFO] %d kata berhasil dimuat.\n",
           jumlahKamus);
}


/* ========================================================
   CARI KATA
   Pencarian linear (case-insensitive)
   ======================================================== */

int cariKata(const char *kata)
{
    int i;

    for (i = 0; i < jumlahKamus; i++)
    {
        char kataKamus[50];
        char kataInput[50];

        strcpy(kataKamus, kamus[i].kata);
        strcpy(kataInput, kata);

        /* Ubah menjadi huruf kecil */
        int j;

        for (j = 0; kataKamus[j] != '\0'; j++)
        {
            kataKamus[j] =
                (char)tolower((unsigned char)kataKamus[j]);
        }

        for (j = 0; kataInput[j] != '\0'; j++)
        {
            kataInput[j] =
                (char)tolower((unsigned char)kataInput[j]);
        }

        if (strcmp(kataKamus, kataInput) == 0)
        {
            return i;
        }
    }

    return -1;
}


/* ========================================================
   TAMPIL LIST
   Mengubah:

   arti1;arti2;arti3

   Menjadi:

   - arti1
   - arti2
   - arti3
   ======================================================== */

void tampilList(const char *text)
{
    if (text == NULL || strlen(text) == 0)
    {
        printf("-\n");
        return;
    }

    /* Salinan string karena strtok mengubah isi string */
    char temp[5000];

    strcpy(temp, text);

    char *token = strtok(temp, ";");

    while (token != NULL)
    {
        /* Hilangkan spasi di depan */
        while (*token == ' ')
        {
            token++;
        }

        printf("- %s\n", token);

        token = strtok(NULL, ";");
    }
}


/* ========================================================
   TAMPIL KATA
   Menampilkan detail satu kata
   ======================================================== */

void tampilKata(int index)
{
    if (index < 0 || index >= jumlahKamus)
    {
        printf("Data tidak ditemukan.\n");
        return;
    }

    printf("\n");
    printf("========================================================================\n");

    printf("Kata        : %s\n",
           kamus[index].kata);

    printf("Kelas Kata  : %s\n",
           kamus[index].kelas);

    printf("\nArti:\n");
    tampilList(kamus[index].arti);

    printf("\nContoh:\n");
    tampilContoh(kamus[index].contoh, kamus[index].kata);

    printf("\nTurunan:\n");
    tampilList(kamus[index].turunan);

    printf("\nFrasa:\n");
    tampilFrasa(kamus[index].frasa);

    printf("========================================================================\n");
}

/* ========================================================
   TRIM
   Menghapus spasi di depan dan belakang string
   ======================================================== */

static void trim(char *str) {
    int len = strlen(str);

    while (len > 0 &&
           isspace((unsigned char)str[len - 1]))
    {
        str[len - 1] = '\0';
        len--;
    }

    int start = 0;

    while (str[start] &&
           isspace((unsigned char)str[start]))
    {
        start++;
    }

    if (start > 0)
    {
        memmove(str,
                str + start,
                strlen(str + start) + 1);
    }
}

void tampilContoh(const char *contoh,
                  const char *kata)
{
    if(contoh == NULL ||
       strlen(contoh) == 0)
    {
        printf("-\n");
        return;
    }

    char temp[5000];
    strcpy(temp, contoh);

    char *token = strtok(temp, ";");

    while(token != NULL)
    {
        while(*token == ' ')
            token++;

        char hasil[5000];

        strcpy(hasil, token);

        char *pos = strstr(hasil, "--");

        while(pos != NULL)
        {
            char buffer[5000];

            *pos = '\0';

            sprintf(
                buffer,
                "%s%s%s",
                hasil,
                kata,
                pos + 2
            );

            strcpy(hasil, buffer);

            pos = strstr(hasil, "--");
        }

        printf("- %s\n", hasil);

        token = strtok(NULL, ";");
    }
}

/* ========================================================
   TAMPIL FRASA
   Format:
   - frasa
     -> arti frasa
   ======================================================== */

void tampilFrasa(const char *text)
{
    if (text == NULL || strlen(text) == 0)
    {
        printf("-\n");
        return;
    }

    char temp[2000];
    strcpy(temp, text);

    char *frasa = strtok(temp, ";");

    while (frasa != NULL)
    {
        while (*frasa == ' ')
        {
            frasa++;
        }

        char *bukaKurung = strchr(frasa, '(');

        if (bukaKurung != NULL)
        {
            *bukaKurung = '\0';

            char *arti = bukaKurung + 1;

            char *tutupKurung = strrchr(arti, ')');

            if (tutupKurung != NULL)
            {
                *tutupKurung = '\0';
            }

            printf("- %s\n", frasa);
            printf("  -> %s\n\n", arti);
        }
        else
        {
            printf("- %s\n", frasa);
        }

        frasa = strtok(NULL, ";");
    }
}