#define _POSIX_C_SOURCE 200809L
/* ========================================================
   turunan.c — ADT Turunan Kata (Implementasi)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include "turunan.h"
#include "kamus.h"

/* --------------------------------------------------------
   tampilTurunan — cari kata majemuk/frasa ber-prefix sama
   Strategi : strncmp pada array kamus, skip kata itu sendiri
   -------------------------------------------------------- */
void tampilTurunan(const char *kata_dasar) {
    int panjang = (int)strlen(kata_dasar);
    int ditemukan = 0;
    int i;

    printf("\n  Kata turunan / majemuk dari \"%s\":\n", kata_dasar);

    for (i = 0; i < jumlahKamus; i++) {
        /* Lewati kata dasar itu sendiri */
        if (strcasecmp(kamus[i].kata, kata_dasar) == 0) continue;

        /* Cek apakah kata_dasar adalah awalan dari kamus[i].kata */
        if (strncasecmp(kamus[i].kata, kata_dasar, (size_t)panjang) == 0) {
            /* Pastikan karakter berikutnya adalah spasi atau huruf lanjutan */
            char next = kamus[i].kata[panjang];
            if (next == ' ' || next == '\0' || isalpha((unsigned char)next)) {
                printf("    - %s\n", kamus[i].kata);
                ditemukan++;
            }
        }
    }

    if (ditemukan == 0)
        printf("    (Tidak ada kata turunan yang terdaftar)\n");
}