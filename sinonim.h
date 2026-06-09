#ifndef SINONIM_H
#define SINONIM_H

/* ========================================================
   sinonim.h — ADT Sinonim / Thesaurus (Interface / Header)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include "types.h"

/* Memuat relasi sinonim dari file synonym.txt ke sinonimList[] */
void loadSinonim(const char *filename);

/* Return indeks sinonim jika ditemukan, -1 jika tidak ada */
int cariSinonim(const char *kata);

/* Cetak semua sinonim berdasarkan indeks di sinonimList */
void tampilSinonim(int index);

#endif 