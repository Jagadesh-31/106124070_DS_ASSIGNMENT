#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

// create node, 0 means no node
Node* create(int val) {
    if (!val) return NULL;
    Node* node = malloc(sizeof(Node));
    node->data = val;
    node->left = node->right = NULL;
    return node;
}

// build tree from level-order list
Node* make_tree(int arr[], int size) {
    if (size == 0 || arr[0] == 0) return NULL;

    Node** nodes = malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        nodes[i] = arr[i] ? create(arr[i]) : NULL;
    }

    int child_idx = 1;
    for (int i = 0; child_idx < size; i++) {
        if (!nodes[i]) continue;
        if (child_idx < size) nodes[i]->left = nodes[child_idx++];
        if (child_idx < size) nodes[i]->right = nodes[child_idx++];
    }

    Node* root = nodes[0];
    free(nodes);
    return root;
}

int cameras = 0;

// return: 0 = covered, 1 = need camera, 2 = has camera
int place(Node* node) {
    if (!node) return 0;

    int l = place(node->left);
    int r = place(node->right);

    if (l == 1 || r == 1) {
        cameras++;
        return 2;  // put camera here
    }
    if (l == 2 || r == 2) return 0;  // covered by child
    return 1;  // ask parent to cover
}

// get first n prime numbers
int* get_primes(int n) {
    if (n <= 0) return NULL;

    int max_n = n < 5 ? 20 : (int)(n * (log(n) + log(log(n + 1))) + 15);
    char* prime = calloc(max_n + 1, sizeof(char));
    for (int i = 2; i <= max_n; i++) prime[i] = 1;

    for (int i = 2; i * i <= max_n; i++) {
        if (prime[i]) {
            for (int j = i * i; j <= max_n; j += i)
                prime[j] = 0;
        }
    }

    int* list = malloc(n * sizeof(int));
    int pos = 0;
    for (int i = 2; pos < n && i <= max_n; i++)
        if (prime[i]) list[pos++] = i;

    free(prime);
    return list;
}

typedef struct TrieNode {
    struct TrieNode* next[2];
    int count;
} TrieNode;

TrieNode* new_node() {
    TrieNode* t = malloc(sizeof(TrieNode));
    t->next[0] = t->next[1] = NULL;
    t->count = 0;
    return t;
}

// insert number (MSB to LSB)
void add(TrieNode* root, int num) {
    TrieNode* cur = root;
    for (int i = 20; i >= 0; i--) {
        int bit = (num >> i) & 1;
        if (!cur->next[bit]) cur->next[bit] = new_node();
        cur = cur->next[bit];
        cur->count++;
    }
}

// count how many numbers give XOR <= k
int find_xor(TrieNode* root, int num, int k) {
    if (!root) return 0;
    TrieNode* cur = root;
    int ans = 0;

    for (int i = 20; i >= 0; i--) {
        int nbit = (num >> i) & 1;
        int kbit = (k >> i) & 1;

        if (kbit) {
            if (cur->next[nbit]) ans += cur->next[nbit]->count;
            cur = cur->next[1 - nbit];
        } else {
            cur = cur->next[nbit];
        }
        if (!cur) break;
    }
    return ans;
}

long long count_xor_leq(int nums[], int n, int limit) {
    TrieNode* trie = new_node();
    long long result = 0;

    for (int i = 0; i < n; i++) {
        result += find_xor(trie, nums[i], limit);
        add(trie, nums[i]);
    }
    return result;
}

long long count_in_range(int nums[], int n, int low, int high) {
    if (n < 2) return 0;
    long long total_up_to_high = count_xor_leq(nums, n, high);
    long long total_up_to_low = (low == 0) ? 0 : count_xor_leq(nums, n, low - 1);
    return total_up_to_high - total_up_to_low;
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    int* input = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &input[i]) != 1) {
            free(input);
            return 0;
        }
    }

    Node* root = make_tree(input, n);
    int status = place(root);
    if (status == 1) cameras++;  // root needs camera

    printf("Number of phones needed: %d\n", cameras);

    int L, R;
    if (scanf("%d %d", &L, &R) == 2) {
        int* primes = get_primes(cameras);
        long long total_pairs = 1LL * cameras * (cameras - 1) / 2;
        long long good_pairs = count_in_range(primes, cameras, L, R);
        printf("number of xor pairs outside the range: %lld\n", total_pairs - good_pairs);
        free(primes);
    }

    free(input);
    return 0;
}