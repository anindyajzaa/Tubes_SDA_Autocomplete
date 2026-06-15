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

/* VARIABEL GLOBAL*/
Kamus kamus[1000];
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

    char baris[5000];

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

        // KOLOM 1 : KATA
        token = strtok_r(baris, "|", &saveptr);

        if (token == NULL)
        {
            continue;
        }

        strcpy(kamus[jumlahKamus].kata, token);

        // KOLOM 2 : KELAS KATA
        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            strcpy(kamus[jumlahKamus].kelas, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].kelas, "");
        }

        // KOLOM 3 : ARTI
        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            strcpy(kamus[jumlahKamus].arti, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].arti, "");
        }

        // KOLOM 4 : CONTOH
        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            strcpy(kamus[jumlahKamus].contoh, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].contoh, "");
        }

        // KOLOM 5 : TURUNAN
        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            strcpy(kamus[jumlahKamus].turunan, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].turunan, "");
        }

        // KOLOM 6 : FRASA 
        token = strtok_r(NULL, "|", &saveptr);

        if (token != NULL)
        {
            strcpy(kamus[jumlahKamus].frasa, token);
        }
        else
        {
            strcpy(kamus[jumlahKamus].frasa, "");
        }

        jumlahKamus++;

        if (jumlahKamus >= 1000)
        {
            break;
        }
    }

    fclose(fp);

    printf("\n[INFO] %d kata berhasil dimuat.\n", jumlahKamus);
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
   Dalam format:
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

    char temp[2000];

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
    tampilList(kamus[index].contoh);

    printf("\nTurunan:\n");
    tampilList(kamus[index].turunan);

    printf("\nFrasa:\n");
    tampilList(kamus[index].frasa);

    printf("========================================================================\n");
}