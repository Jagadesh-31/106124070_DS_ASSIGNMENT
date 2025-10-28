#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX 300005

int total_nodes;
int height[MAX];
int low, high;
int phones = 0;


bool prime(int x) {
    if (x < 2) return false;
    for (int i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

//fill array with first cnt primes 
void make_primes(int cnt, int list[]) {
    int val = 2, pos = 0;
    while (pos < cnt) {
        if (prime(val)) list[pos++] = val;
        ++val;
    }
}


int walk(int idx) {
    if (idx >= total_nodes || height[idx] == 0) return 2;   // nothing here
    int left  = walk(2 * idx + 1);
    int right = walk(2 * idx + 2);
    if (left == 0 || right == 0) {        // a child asks for help
        ++phones;
        return 1;                         // we place a phone
    }
    if (left == 1 || right == 1)          // already covered by a child
        return 2;
    return 0;                             // need coverage from parent
}

int main() {
    scanf("%d", &total_nodes);
    for (int i = 0; i < total_nodes; ++i)
        scanf("%d", &height[i]);
    scanf("%d %d", &low, &high);
    phones = 0;
    if (walk(0) == 0) ++phones;           // root not covered yet
    printf("%d\n", phones + 1);           // +1 is part of the task
    int plist[MAX];
    if (phones) make_primes(phones, plist);
    long long bad = 0;
    for (int a = 0; a < phones; ++a)
        for (int b = a + 1; b < phones; ++b) {
            int xor_val = plist[a] ^ plist[b];
            if (xor_val < low || xor_val > high)
                ++bad;
        }
    printf("%lld\n", bad + 1);            // +1 again as required
    return 0;
}