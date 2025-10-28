#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int val;
    struct Node *left, *right;
} Node;

// make a node (0 = null)
Node* node(int v) {
    if (!v) return NULL;
    Node *n = malloc(sizeof(Node));
    n->val = v;
    n->left = n->right = NULL;
    return n;
}

// build tree using level order array
Node* tree_from_array(int a[], int n) {
    if (n == 0 || a[0] == 0) return NULL;

    Node** all = malloc(n * sizeof(Node*));
    for (int i = 0; i < n; i++) all[i] = a[i] ? node(a[i]) : NULL;

    for (int i = 0, j = 1; j < n; i++) {
        if (!all[i]) continue;
        all[i]->left  = (j < n) ? all[j++] : NULL;
        all[i]->right = (j < n) ? all[j++] : NULL;
    }

    Node* root = all[0];
    free(all);
    return root;
}

int phones = 0;

// 0 = safe, 1 = needs cover, 2 = has camera
int cover(Node* cur) {
    if (!cur) return 0;

    int l = cover(cur->left);
    int r = cover(cur->right);

    if (l == 1 || r == 1) {
        phones++;
        return 2;
    }
    if (l == 2 || r == 2) return 0;
    return 1;
}

// first n primes
int* primes(int n) {
    if (n <= 0) return NULL;

    int bound = n < 6 ? 18 : (int)(n * log(n) + n * log(log(n + 2)) + 12);
    char* is_prime = calloc(bound + 1, 1);

    for (int i = 2; i <= bound; i++) is_prime[i] = 1;
    for (int i = 2; i * i <= bound; i++)
        if (is_prime[i])
            for (int j = i * i; j <= bound; j += i)
                is_prime[j] = 0;

    int* res = malloc(n * sizeof(int));
    int k = 0;
    for (int i = 2; k < n && i <= bound; i++)
        if (is_prime[i]) res[k++] = i;

    free(is_prime);
    return res;
}

typedef struct Trie {
    struct Trie* child[2];
    int size;
} Trie;

Trie* new() {
    Trie* t = malloc(sizeof(Trie));
    t->child[0] = t->child[1] = NULL;
    t->size = 0;
    return t;
}

// insert number (LSB to MSB)
void put(Trie* root, int x) {
    Trie* cur = root;
    for (int i = 0; i < 21; i++) {
        int b = x & 1;
        if (!cur->child[b]) cur->child[b] = new();
        cur = cur->child[b];
        cur->size++;
        x >>= 1;
    }
}

// count numbers y where (x ^ y) <= k
int get(Trie* root, int x, int k) {
    if (!root) return 0;
    Trie* cur = root;
    int ans = 0;

    for (int i = 0; i < 21; i++) {
        int xb = x & 1;
        int kb = k & 1;

        if (kb) {
            if (cur->child[xb]) ans += cur->child[xb]->size;
            cur = cur->child[1 - xb];
        } else {
            cur = cur->child[xb];
        }
        if (!cur) return ans;
        x >>= 1;
        k >>= 1;
    }
    return ans;
}

long long pairs_leq(int a[], int n, int cap) {
    Trie* trie = new();
    long long cnt = 0;
    for (int i = 0; i < n; i++) {
        cnt += get(trie, a[i], cap);
        put(trie, a[i]);
    }
    return cnt;
}

long long pairs_range(int a[], int n, int l, int r) {
    if (n < 2) return 0;
    return pairs_leq(a, n, r) - (l ? pairs_leq(a, n, l - 1) : 0);
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int* level = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        if (scanf("%d", &level[i]) != 1) { free(level); return 0; }

    Node* root = tree_from_array(level, n);
    int need = cover(root);
    if (need == 1) phones++;

    printf("Number of phones needed: %d\n", phones);

    int L, R;
    if (scanf("%d %d", &L, &R) == 2) {
        int* p = primes(phones);
        long long total = 1LL * phones * (phones - 1) / 2;
        long long inside = pairs_range(p, phones, L, R);
        printf("number of xor pairs outside the range: %lld\n", total - inside);
        free(p);
    }

    free(level);
    return 0;
}