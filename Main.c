/* ========================================================
   Main.c — Program Utama
   Aplikasi Kamus dengan Autocomplete berbasis Trie
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "Types.h"
#include "Trie.h"
#include "Kamus.h"
#include "sinonim.h"

#define FILE_KAMUS      "kamusKata.txt"
#define FILE_SINONIM    "synonym.txt"
#define MAX_REKOMENDASI 10

/* --------------------------------------------------------
   bacaInputString — baca baris, strip newline
   -------------------------------------------------------- */
static void bacaInputString(char *buf, int maks) {
    if (fgets(buf, maks, stdin)) {
        buf[strcspn(buf, "\r\n")] = '\0';
    }
}

/* --------------------------------------------------------
   hanyaHuruf — return 1 jika semua karakter adalah huruf
   -------------------------------------------------------- */
static int hanyaHuruf(const char *str) {
    int i;
    if (strlen(str) == 0) return 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i]))
            return 0;
    }
    return 1;
}

/* --------------------------------------------------------
   tampilSubMenu — sub-menu setelah rekomendasi ditampilkan
   Return: 1 = Pilih Kata, 2 = Input Prefix Lagi
   -------------------------------------------------------- */
static int tampilSubMenu(void) {
    char inputSubMenu[10];
    int  opsiSub;

    do {
        printf("\n");
        printf("  +-------------------------------------+\n");
        printf("  |  1. Pilih Kata                      |\n");
        printf("  |  2. Input Prefix Lagi               |\n");
        printf("  +-------------------------------------+\n");
        printf("  Pilihan: ");
        bacaInputString(inputSubMenu, sizeof(inputSubMenu));

        /* Validasi: harus digit */
        int valid = 1, j;
        if (strlen(inputSubMenu) == 0) valid = 0;
        for (j = 0; inputSubMenu[j] != '\0'; j++) {
            if (!isdigit((unsigned char)inputSubMenu[j])) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("  [ERROR] Pilihan tidak valid. Masukkan 1 atau 2.\n");
            continue;
        }

        opsiSub = atoi(inputSubMenu);
        if (opsiSub != 1 && opsiSub != 2) {
            printf("  [ERROR] Pilihan tidak valid. Masukkan 1 atau 2.\n");
            continue;
        }
        break;
    } while (1);

    return opsiSub;
}

/* --------------------------------------------------------
   fiturAutocomplete — Opsi 1
   -------------------------------------------------------- */
static void fiturAutocomplete(void) {
    char prefix[50];
    char hasilTemp[10][30];
    int  count;

    /* Loop utama: ulangi selama user memilih "Input Prefix Lagi" */
    while (1) {

        /* --- Validasi prefix: hanya huruf, minimal 2 karakter --- */
        do {
            printf("\n  Masukkan prefix: ");
            bacaInputString(prefix, sizeof(prefix));

            if (!hanyaHuruf(prefix)) {
                printf("  [ERROR] Tidak boleh mengandung angka atau simbol!\n");
                continue;
            }
            if ((int)strlen(prefix) < 2) {
                printf("  [ERROR] Minimal 2 karakter!\n");
                continue;
            }
            break;
        } while (1);

        /* --- Proses Autocomplete: tampilkan 10 rekomendasi --- */
        autocomplete(root, prefix, MAX_REKOMENDASI);

        /* --- Kumpulkan hasil ke hasilTemp untuk validasi pilihan user --- */
        count = 0;

        TrieNode *node = searchPrefix(root, prefix);
        if (node) {
            char buffer[100];
            int  depth = (int)strlen(prefix);
            int  i;

            strncpy(buffer, prefix, 99);
            buffer[99] = '\0';

            if (node->isEnd) {
                strncpy(hasilTemp[count], prefix, 29);
                hasilTemp[count][29] = '\0';
                count++;
            }

            for (i = 0; i < 26 && count < MAX_REKOMENDASI; i++) {
                if (node->children[i]) {
                    buffer[depth] = (char)('a' + i);
                    kumpulkanHasil(node->children[i], buffer, depth + 1,
                                   hasilTemp, &count, MAX_REKOMENDASI);
                }
            }
        }

        /* --------------------------------------------------------
           Sub-menu muncul SETELAH rekomendasi ditampilkan
           (sesuai flowchart revisi)
           -------------------------------------------------------- */
        fflush(stdout);
        int opsiSub = tampilSubMenu();

        if (opsiSub == 2) {
            /* Pilihan 2: ulangi dari input prefix */
            continue;
        }

        /* Pilihan 1: lanjut ke pemilihan nomor kata */

        if (count == 0) {
            printf("  [WARN] Tidak ada kata yang dapat dipilih.\n");
            return;
        }

        /* --- Input Nomor Kata: validasi harus angka dan 1-count --- */
        char inputPilihan[20];
        int  pilihan;

        do {
            printf("\n  Pilih nomor kata (1-%d): ", count);
            bacaInputString(inputPilihan, sizeof(inputPilihan));

            /* Cek apakah semua karakter digit */
            int valid = 1, j;
            if (strlen(inputPilihan) == 0) valid = 0;
            for (j = 0; inputPilihan[j] != '\0'; j++) {
                if (!isdigit((unsigned char)inputPilihan[j])) {
                    valid = 0;
                    break;
                }
            }

            if (!valid) {
                printf("  [ERROR] Input harus berupa angka!\n");
                continue;
            }

            pilihan = atoi(inputPilihan);
            if (pilihan < 1 || pilihan > count) {
                printf("  [ERROR] Pilihan tidak valid. Masukkan angka 1-%d.\n", count);
                continue;
            }
            break;
        } while (1);

        /* --- Ambil dan tampilkan data kata --- */
        int idxKamus = cariKata(hasilTemp[pilihan - 1]);
        if (idxKamus == -1) {
            printf("  [WARN] Data detail untuk kata ini tidak tersedia.\n");
            return;
        }

        /* Tampilkan: Kata, Kelas Kata, Definisi, Contoh, Turunan, Frasa */
        tampilKata(idxKamus);

        /* Tampilkan: Sinonim */
        int idxSin = cariSinonim(kamus[idxKamus].kata);
        tampilSinonim(idxSin);

        /* Selesai, keluar dari fiturAutocomplete dan kembali ke menu utama */
        break;

    } /* end while(1) */
}

/* --------------------------------------------------------
   tampilMenu — tampilkan antarmuka menu utama
   -------------------------------------------------------- */
static void tampilMenu(void) {
    printf("\n");
    printf("  +=========================================+\n");
    printf("  |     APLIKASI KAMUS DENGAN TRIE          |\n");
    printf("  +=========================================+\n");
    printf("  |  1. Input Prefix (Autocomplete)         |\n");
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
    char inputOpsi[10];
    int  opsi;

    do {
        tampilMenu();
        bacaInputString(inputOpsi, sizeof(inputOpsi));

        /* Validasi input menu: harus satu digit angka */
        int valid = 1, j;
        if (strlen(inputOpsi) == 0) valid = 0;
        for (j = 0; inputOpsi[j] != '\0'; j++) {
            if (!isdigit((unsigned char)inputOpsi[j])) {
                valid = 0;
                break;
            }
        }

        if (!valid) {
            printf("  [ERROR] Pilihan tidak valid. Masukkan 0 atau 1.\n");
            opsi = -1;
            continue;
        }

        opsi = atoi(inputOpsi);

        switch (opsi) {
            case 1:
                fiturAutocomplete();
                break;
            case 0:
                printf("\n  Terima kasih. Program selesai.\n\n");
                break;
            default:
                printf("  [ERROR] Pilihan tidak valid. Masukkan 0 atau 1.\n");
        }
    } while (opsi != 0);

    /* 5. Bebaskan memori Trie */
    bebaskanTrie(root);
    return 0;
}