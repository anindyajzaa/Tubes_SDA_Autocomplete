#define _POSIX_C_SOURCE 200809L
/* ========================================================
   sinonim.c — ADT Sinonim / Thesaurus (Implementasi)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include "sinonim.h"

/* --------------------------------------------------------
   loadSinonim — parsing synonym.txt
   Format baris: kata|sin1|sin2|sin3|...  (maks 5 sinonim)
   -------------------------------------------------------- */
void loadSinonim(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "[WARN] File \"%s\" tidak dapat dibuka. "
                        "Fitur sinonim dinonaktifkan.\n", filename);
        return;
    }

    char baris[300];
    jumlahSinonim = 0;

    while (fgets(baris, sizeof(baris), fp) && jumlahSinonim < 500) {
        baris[strcspn(baris, "\r\n")] = '\0';
        if (strlen(baris) == 0) continue;

        char *saveptr;
        char *token = strtok_r(baris, "|", &saveptr);
        if (!token) continue;

        strncpy(sinonimList[jumlahSinonim].kata, token, 29);
        sinonimList[jumlahSinonim].kata[29] = '\0';
        sinonimList[jumlahSinonim].count    = 0;

        int s = 0;
        while (s < 5) {
            token = strtok_r(NULL, "|", &saveptr);
            if (!token) break;
            strncpy(sinonimList[jumlahSinonim].sinonim[s], token, 29);
            sinonimList[jumlahSinonim].sinonim[s][29] = '\0';
            s++;
            sinonimList[jumlahSinonim].count++;
        }

        jumlahSinonim++;
    }

    fclose(fp);
    printf("[INFO] %d data sinonim berhasil dimuat dari \"%s\".\n",
           jumlahSinonim, filename);
}

/* --------------------------------------------------------
   cariSinonim — linear search case-insensitive
   -------------------------------------------------------- */
int cariSinonim(const char *kata) {
    int i;
    for (i = 0; i < jumlahSinonim; i++) {
        if (strcasecmp(sinonimList[i].kata, kata) == 0)
            return i;
    }
    return -1;
}

/* --------------------------------------------------------
   tampilSinonim — cetak daftar sinonim
   -------------------------------------------------------- */
void tampilSinonim(int index) {
    if (index < 0 || index >= jumlahSinonim) {
        printf("  Sinonim tidak tersedia.\n");
        return;
    }
    if (sinonimList[index].count == 0) {
        printf("  Sinonim tidak tersedia.\n");
        return;
    }
    printf("  Sinonim: ");
    int i;
    for (i = 0; i < sinonimList[index].count; i++) {
        if (i > 0) printf(", ");
        printf("%s", sinonimList[index].sinonim[i]);
    }
    printf("\n");
}