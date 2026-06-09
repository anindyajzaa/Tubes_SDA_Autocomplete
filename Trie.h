#ifndef TRIE_H
#define TRIE_H

/* ========================================================
   trie.h — ADT Trie (Interface / Header)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include "types.h"

/* Variabel root Trie — didefinisikan di trie.c */
extern TrieNode *root;

/* Alokasi node Trie baru, semua children NULL, isEnd=0 */
TrieNode* buatTrieNode(void);

/* Memasukkan kata ke dalam Trie beserta referensi indeks kamus */
void insertTrie(TrieNode *root, const char *kata, int indexKamus);

/* Mencari node terakhir dari prefix; return NULL jika tidak ada */
TrieNode* searchPrefix(TrieNode *root, const char *prefix);

/* Menelusuri sub-pohon dan kumpulkan rekomendasi (maks maxHasil) */
void kumpulkanHasil(TrieNode *node, char *buffer, int depth,
                    char hasil[][30], int *count, int maxHasil);

/* Tampilkan hingga maxHasil rekomendasi autocomplete ke layar */
void autocomplete(TrieNode *root, const char *prefix, int maxHasil);

/* Bebaskan seluruh memori Trie secara rekursif */
void bebaskanTrie(TrieNode *node);

#endif /* TRIE_H */