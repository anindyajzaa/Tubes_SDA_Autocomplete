/* ========================================================
   trie.c — ADT Trie (Implementasi)
   Kamus Trie — Struktur Data dan Algoritma
   ======================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "trie.h"

/* ---------- Definisi variabel root Trie ---------- */
TrieNode *root = NULL;

/* --------------------------------------------------------
   buatTrieNode — alokasi & inisialisasi node baru
   -------------------------------------------------------- */
TrieNode* buatTrieNode(void) {
    TrieNode *node = (TrieNode*) malloc(sizeof(TrieNode));
    if (!node) {
        fprintf(stderr, "[ERROR] Gagal alokasi TrieNode.\n");
        exit(EXIT_FAILURE);
    }
    int i;
    for (i = 0; i < 26; i++)
        node->children[i] = NULL;
    node->isEnd     = 0;
    node->indexKamus = -1;
    return node;
}

/* --------------------------------------------------------
   insertTrie — masukkan satu kata ke pohon Trie
   -------------------------------------------------------- */
void insertTrie(TrieNode *root, const char *kata, int indexKamus) {
    TrieNode *current = root;
    int i;
    for (i = 0; kata[i] != '\0'; i++) {
        int ch = tolower((unsigned char)kata[i]) - 'a';
        if (ch < 0 || ch > 25) continue; /* lewati karakter non-alfabet */
        if (current->children[ch] == NULL)
            current->children[ch] = buatTrieNode();
        current = current->children[ch];
    }
    current->isEnd      = 1;
    current->indexKamus = indexKamus;
}

/* --------------------------------------------------------
   searchPrefix — kembalikan node ujung prefix, atau NULL
   -------------------------------------------------------- */
TrieNode* searchPrefix(TrieNode *root, const char *prefix) {
    TrieNode *current = root;
    int i;
    for (i = 0; prefix[i] != '\0'; i++) {
        int ch = tolower((unsigned char)prefix[i]) - 'a';
        if (ch < 0 || ch > 25) return NULL;
        if (current->children[ch] == NULL) return NULL;
        current = current->children[ch];
    }
    return current;
}

/* --------------------------------------------------------
   kumpulkanHasil — DFS rekursif, kumpulkan kata lengkap
   -------------------------------------------------------- */
void kumpulkanHasil(TrieNode *node, char *buffer, int depth,
                    char hasil[][30], int *count, int maxHasil) {
    if (*count >= maxHasil) return;

    if (node->isEnd) {
        buffer[depth] = '\0';
        strncpy(hasil[*count], buffer, 29);
        hasil[*count][29] = '\0';
        (*count)++;
    }

    int i;
    for (i = 0; i < 26 && *count < maxHasil; i++) {
        if (node->children[i]) {
            buffer[depth] = (char)('a' + i);
            kumpulkanHasil(node->children[i], buffer, depth + 1,
                           hasil, count, maxHasil);
        }
    }
}

/* --------------------------------------------------------
   autocomplete — cari & cetak rekomendasi berdasar prefix
   -------------------------------------------------------- */
int autocomplete(TrieNode *root, const char *prefix, char hasil[][30], int maxHasil) {
    TrieNode *node = searchPrefix(root, prefix);
    if (!node) {
        printf("  Tidak ada kata yang cocok dengan prefix \"%s\".\n", prefix);
        return 0;
    }

    /* buffer: prefix + perpanjangan DFS */
    char buffer[100];
    strncpy(buffer, prefix, 99);
    buffer[99] = '\0';
    int depth = (int)strlen(prefix);

    int count = 0;

    /* Jika prefix itu sendiri adalah kata yang valid, masukkan duluan */
    if (node->isEnd) {
        strncpy(hasil[count], prefix, 29);
        hasil[count][29] = '\0';
        count++;
    }

    /* Lanjutkan DFS pada anak-anak node prefix */
    int i;
    for (i = 0; i < 26 && count < maxHasil; i++) {
        if (node->children[i]) {
            buffer[depth] = (char)('a' + i);
            kumpulkanHasil(node->children[i], buffer, depth + 1,
                           hasil, &count, maxHasil);
        }
    }

    if (count == 0) {
        printf("  Tidak ada rekomendasi untuk prefix \"%s\".\n", prefix);
        return 0;
    }

    printf("\n  Rekomendasi autocomplete untuk \"%s\":\n", prefix);
    for (i = 0; i < count; i++)
        printf("  %2d. %s\n", i + 1, hasil[i]);

    return count;
}

/* --------------------------------------------------------
   bebaskanTrie — rekursif free seluruh node
   -------------------------------------------------------- */
void bebaskanTrie(TrieNode *node) {
    if (!node) return;
    int i;
    for (i = 0; i < 26; i++)
        bebaskanTrie(node->children[i]);
    free(node);
}