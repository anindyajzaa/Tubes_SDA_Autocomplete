#ifndef KAMUS_H
#define KAMUS_H

/* ========================================================
   kamus.h — ADT Kamus (Interface / Header)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include "types.h"

/* Variabel data kamus */
extern Kamus kamus[1000];
extern int jumlahKamus;

/* Memuat data dari file kamus_final.txt */
void loadKamus(const char *filename);

/* Mencari kata dalam array kamus */
int cariKata(const char *kata);

/* Menampilkan detail kata */
void tampilKata(int index);

#endif