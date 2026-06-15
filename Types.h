#ifndef TYPES_H
#define TYPES_H

/* ========================================================
   types.h — Deklarasi Tipe Data Global
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

/* ---------- Kamus Utama ---------- */
typedef struct {
    char kata[50];
    char kelas[30];
    char arti[1000];
    char contoh[1000];
    char turunan[1000];
    char frasa[2000];
} Kamus;

/* ---------- Trie Node ---------- */
typedef struct TrieNode {
    struct TrieNode *children[26]; /* a-z */
    int isEnd;                     /* 1 = akhir kata */
    int indexKamus;                /* indeks ke array Kamus */
} TrieNode;

/* ---------- Sinonim ---------- */
typedef struct {
    char kata[30];
    char sinonim[5][30]; /* maks 5 sinonim per kata */
    int count;
} Sinonim;

#endif /* TYPES_H */