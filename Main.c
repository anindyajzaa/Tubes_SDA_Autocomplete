/* ========================================================
   main.c — Program Utama
   Aplikasi Kamus dengan Autocomplete berbasis Trie
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "types.h"
#include "trie.h"
#include "kamus.h"
#include "turunan.h"
#include "sinonim.h"

#define FILE_KAMUS   "kamusKata.txt"
#define FILE_SINONIM "synonym.txt"
#define MAX_REKOMENDASI 10

/* --------------------------------------------------------
   clearBuffer — bersihkan sisa karakter di stdin
   -------------------------------------------------------- */
static void clearBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* --------------------------------------------------------
   bacaInputString — baca baris, strip newline
   -------------------------------------------------------- */
static void bacaInputString(char *buf, int maks) {
    if (fgets(buf, maks, stdin)) {
        buf[strcspn(buf, "\r\n")] = '\0';
    }
}

/* --------------------------------------------------------
   subProsesVisualisasi — tampil detail + turunan + sinonim
   -------------------------------------------------------- */
static void subProsesVisualisasi(int indexKamus) {
    tampilKata(indexKamus);

    /* --- Turunan --- */
    char pil;
    printf("\n  Tampilkan kata turunan? (y/n): ");
    scanf(" %c", &pil);
    clearBuffer();
    if (tolower((unsigned char)pil) == 'y')
        tampilTurunan(kamus[indexKamus].kata);

    /* --- Sinonim --- */
    printf("\n  Tampilkan sinonim? (y/n): ");
    scanf(" %c", &pil);
    clearBuffer();
    if (tolower((unsigned char)pil) == 'y') {
        int idxSin = cariSinonim(kamus[indexKamus].kata);
        tampilSinonim(idxSin);
    }
}

/* --------------------------------------------------------
   fiturPencarianKataDasar — Opsi 1
   -------------------------------------------------------- */
static void fiturPencarianKataDasar(void) {
    char input[30];
    printf("\n  Masukkan kata yang ingin dicari: ");
    bacaInputString(input, sizeof(input));

    int idx = cariKata(input);
    if (idx == -1) {
        printf("  [RESULT] Kata \"%s\" tidak ditemukan.\n", input);
        return;
    }
    subProsesVisualisasi(idx);
}

/* --------------------------------------------------------
   fiturAutocomplete — Opsi 2
   -------------------------------------------------------- */
static void fiturAutocomplete(void) {
    char prefix[50];

    /* Validasi minimal 2 huruf */
    do {
        printf("\n  Masukkan prefix (min. 2 huruf): ");
        bacaInputString(prefix, sizeof(prefix));
        if (strlen(prefix) < 2)
            printf("  [WARN] Minimal 2 huruf!\n");
    } while (strlen(prefix) < 2);

    /* Tampilkan rekomendasi */
    autocomplete(root, prefix, MAX_REKOMENDASI);

    /* Hitung ulang jumlah hasil (untuk validasi pilihan) */
    TrieNode *node = searchPrefix(root, prefix);
    if (!node) return;

    char hasilTemp[10][30];
    int count = 0;
    char buffer[100];
    strncpy(buffer, prefix, 99);

    if (node->isEnd) {
        strncpy(hasilTemp[count++], prefix, 29);
    }
    int i;
    int depth = (int)strlen(prefix);
    for (i = 0; i < 26 && count < MAX_REKOMENDASI; i++) {
        if (node->children[i]) {
            buffer[depth] = (char)('a' + i);
            kumpulkanHasil(node->children[i], buffer, depth + 1,
                           hasilTemp, &count, MAX_REKOMENDASI);
        }
    }

    if (count == 0) return;

    int pilihan;
    printf("\n  Pilih nomor kata (1-%d): ", count);
    if (scanf("%d", &pilihan) != 1) { clearBuffer(); return; }
    clearBuffer();

    if (pilihan < 1 || pilihan > count) {
        printf("  [WARN] Pilihan tidak valid.\n");
        return;
    }

    int idxKamus = cariKata(hasilTemp[pilihan - 1]);
    if (idxKamus == -1) {
        printf("  [WARN] Data detail untuk kata ini tidak tersedia.\n");
        return;
    }
    subProsesVisualisasi(idxKamus);
}

/* --------------------------------------------------------
   tampilMenu — tampilkan antarmuka menu utama
   -------------------------------------------------------- */
static void tampilMenu(void) {
    printf("\n");
    printf("  +=========================================+\n");
    printf("  |     APLIKASI KAMUS DENGAN TRIE          |\n");
    printf("  +=========================================+\n");
    printf("  |  1. Pencarian Kata Dasar                |\n");
    printf("  |  2. Autocomplete Berbasis Prefix        |\n");
    printf("  |  0. Keluar                              |\n");
    printf("  +-----------------------------------------+\n");
    printf("  Pilihan: ");
}

/* --------------------------------------------------------
   main — entry point, inisialisasi & loop menu
   -------------------------------------------------------- */
int main(void) {
    printf("\n  Memuat data kamus...\n");

    /* 1. Inisialisasi root Trie */
    root = buatTrieNode();

    /* 2. Muat kamus & bangun Trie */
    loadKamus(FILE_KAMUS);
    int i;
    for (i = 0; i < jumlahKamus; i++)
        insertTrie(root, kamus[i].kata, i);

    /* 3. Muat sinonim */
    loadSinonim(FILE_SINONIM);

    /* 4. Loop menu utama */
    int opsi;
    do {
        tampilMenu();
        if (scanf("%d", &opsi) != 1) {
            clearBuffer();
            opsi = -1;
        } else {
            clearBuffer();
        }

        switch (opsi) {
            case 1:
                fiturPencarianKataDasar();
                break;
            case 2:
                fiturAutocomplete();
                break;
            case 0:
                printf("\n  Terima kasih. Program selesai.\n\n");
                break;
            default:
                printf("  [WARN] Opsi tidak valid. Silakan pilih 0, 1, atau 2.\n");
        }
    } while (opsi != 0);

    /* 5. Bebaskan memori Trie */
    bebaskanTrie(root);
    return 0;
}