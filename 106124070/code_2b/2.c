#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX 300010

int nodes;
int depth[MAX];
int L, R;
int signals = 0;

// check if number is prime 
int prime(int n) {
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return 0;
    return 1;
}

// grab first need primes
void get_primes(int need, int p[]) {
    int num = 2, idx = 0;
    while (idx < need) {
        if (prime(num)) p[idx++] = num;
        num++;
    }
}

//dfs to place phones
int visit(int pos) {
    if (pos >= nodes || depth[pos] == 0) return 2;

    int a = visit(2 * pos + 1);
    int b = visit(2 * pos + 2);

    if (a == 0 || b == 0) {
        signals++;
        return 1;
    }
    if (a == 1 || b == 1) return 2;
    return 0;
}

int main() {
    scanf("%d", &nodes);
    for (int i = 0; i < nodes; i++)
        scanf("%d", &depth[i]);
    scanf("%d %d", &L, &R);

    signals = 0;
    if (visit(0) == 0) signals++;  // root needs signal

    printf("%d\n", signals + 1);   // +1 as per problem

    int plist[MAX];
    if (signals) get_primes(signals, plist);

    long long bad_pairs = 0;
    for (int i = 0; i < signals; i++) {
        for (int j = i + 1; j < signals; j++) {
            int xor = plist[i] ^ plist[j];
            if (xor < L || xor > R)
                bad_pairs++;
        }
    }

    printf("%lld\n", bad_pairs + 1);  // again +1

    return 0;
}