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
Kamus kamus[1000];
int   jumlahKamus = 0;

/* --------------------------------------------------------
   loadKamus — parsing kamusKata.txt
   Format baris: kata|arti|contoh
   -------------------------------------------------------- */
void loadKamus(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "[ERROR] File \"%s\" tidak dapat dibuka.\n", filename);
        return;
    }

    char baris[200];
    jumlahKamus = 0;

    while (fgets(baris, sizeof(baris), fp) && jumlahKamus < 1000) {
        /* Hapus newline */
        baris[strcspn(baris, "\r\n")] = '\0';
        if (strlen(baris) == 0) continue;

        /* Tokenisasi berdasarkan '|' */
        char *token;
        char *saveptr;

        token = strtok_r(baris, "|", &saveptr);
        if (!token) continue;
        strncpy(kamus[jumlahKamus].kata, token, 29);
        kamus[jumlahKamus].kata[29] = '\0';

        token = strtok_r(NULL, "|", &saveptr);
        if (!token) continue;
        strncpy(kamus[jumlahKamus].arti, token, 49);
        kamus[jumlahKamus].arti[49] = '\0';

        token = strtok_r(NULL, "|", &saveptr);
        if (!token) continue;
        strncpy(kamus[jumlahKamus].contoh, token, 99);
        kamus[jumlahKamus].contoh[99] = '\0';

        jumlahKamus++;
    }

    fclose(fp);
    printf("[INFO] %d kata berhasil dimuat dari \"%s\".\n", jumlahKamus, filename);
}

/* --------------------------------------------------------
   cariKata — pencarian linear case-insensitive
   -------------------------------------------------------- */
int cariKata(const char *kata) {
    int i;
    for (i = 0; i < jumlahKamus; i++) {
        /* Bandingkan case-insensitive */
        char k1[30], k2[30];
        int j;
        strncpy(k1, kamus[i].kata, 29); k1[29] = '\0';
        strncpy(k2, kata, 29);          k2[29] = '\0';
        for (j = 0; k1[j]; j++) k1[j] = (char)tolower((unsigned char)k1[j]);
        for (j = 0; k2[j]; j++) k2[j] = (char)tolower((unsigned char)k2[j]);
        if (strcmp(k1, k2) == 0) return i;
    }
    return -1;
}

/* --------------------------------------------------------
   tampilKata — cetak detail satu entri kamus
   -------------------------------------------------------- */
void tampilKata(int index) {
    if (index < 0 || index >= jumlahKamus) {
        printf("  [ERROR] Indeks kamus tidak valid.\n");
        return;
    }
    printf("\n");
    printf("  +-----------------------------------------+\n");
    printf("  | Kata    : %-30s|\n", kamus[index].kata);
    printf("  | Arti    : %-30s|\n", kamus[index].arti);
    printf("  | Contoh  : %-30s|\n", kamus[index].contoh);
    printf("  +-----------------------------------------+\n");
}