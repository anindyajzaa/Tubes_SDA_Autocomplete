#ifndef KAMUS_H
#define KAMUS_H

/* ========================================================
   kamus.h — ADT Kamus (Interface / Header)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include "types.h"

/* Variabel data kamus — didefinisikan di kamus.c */
extern Kamus kamus[1000];
extern int   jumlahKamus;

/* Memuat data dari file kamusKata.txt ke array global kamus[] */
void loadKamus(const char *filename);

/* Pencarian linear; return indeks jika ditemukan, -1 jika tidak */
int cariKata(const char *kata);

/* Cetak detail kata (arti & contoh) berdasarkan indeks */
void tampilKata(int index);

#endif /* KAMUS_H */