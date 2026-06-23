#ifndef KAMUS_H
#define KAMUS_H

/* ========================================================
   kamus.h — ADT Kamus (Interface / Header)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include "types.h"

/* Variabel data kamus */
#define MAX_KAMUS 115213
extern Kamus kamus[MAX_KAMUS];
extern int jumlahKamus;

/* Memuat data dari file kamus_final.txt */
void loadKamus(const char *filename);

/* Mencari kata dalam array kamus */
int cariKata(const char *kata);

/* Menampilkan detail kata */
void tampilKata(int index);

/* Memunculkan kata saat menampilkan contoh kalimat suatu kata */
void tampilContoh(const char *contoh, const char *kata);

/* Merapihkan Output frasa */
void tampilFrasa(const char *text);

/* Menampilkan daftar terpisah titik koma */
void tampilList(const char *text);

#endif