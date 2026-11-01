#include <stdio.h>

typedef long long ll;
typedef unsigned long long ull;

#define MAXN 1000002

// Each node stores:
//   - next index in upper 20 bits
//   - value in lower 44 bits
static ull node[MAXN];

// Pointers for the simulated list
static int prev_idx, cur_idx, free_head;

// Masks for extracting fields
static const ull NEXT_MASK = ((1ULL << 20) - 1) << 44;
static const ull VAL_MASK  = (1ULL << 44) - 1;

// Extract the "next" pointer from a packed node
static inline int get_next(int i) {
    return (int)((node[i] & NEXT_MASK) >> 44);
}

// Replace the "next" pointer of node i
static inline void set_next(int i, int nxt) {
    node[i] = (node[i] & ~NEXT_MASK) | ((ull)nxt << 44);
}

// Replace the stored value of node i
static inline void set_value(int i, ll v) {
    node[i] = (node[i] & ~VAL_MASK) | (ull)v;
}

// Read a signed integer and encode sign in bit 43
static ll read_encoded_value() {
    ll x;
    scanf("%lld", &x);

    if (x < 0)
        return ((-x) | (1LL << 43));  // encode negative

    return x;
}

// Move right: >
static void op_next() {
    int nxt = get_next(cur_idx);
    set_next(cur_idx, prev_idx);
    prev_idx = cur_idx;
    cur_idx = nxt;
}

// Move left: <
static void op_prev() {
    int nxt = get_next(prev_idx);
    set_next(prev_idx, cur_idx);
    cur_idx = prev_idx;
    prev_idx = nxt;
}

// Update current value: =
static void op_update() {
    set_value(cur_idx, read_encoded_value());
}

// Insert new node after current: +
static void op_insert() {
    int new_node = free_head;
    free_head = get_next(free_head);

    set_value(new_node, read_encoded_value());
    set_next(new_node, cur_idx);
    cur_idx = new_node;
}

// Delete current node: -
static void op_delete() {
    int dead = cur_idx;
    cur_idx = get_next(cur_idx);

    set_next(dead, free_head);
    free_head = dead;
}

// Print current value: !
static void op_print() {
    ll x = node[cur_idx] & VAL_MASK;

    if (x & (1LL << 43)) {
        x ^= (1LL << 43);
        printf("-%lld\n", x);
    } else {
        printf("%lld\n", x);
    }
}

int main() {
    // Initial state:
    //   prev = 0 (dummy)
    //   cur  = 1 (first real node)
    //   free = 2 (start of free list)
    prev_idx = 0;
    cur_idx = 1;
    free_head = 2;

    // Build free list: node[i].next = i+1
    for (int i = 1; i < MAXN; i++)
        node[i] = (ull)(i + 1) << 44;

    int m;
    scanf("%d", &m);

    while (m--) {
        char op;
        scanf(" %c", &op);

        switch (op) {
            case '>': op_next();   break;
            case '<': op_prev();   break;
            case '=': op_update(); break;
            case '+': op_insert(); break;
            case '-': op_delete(); break;
            case '!': op_print();  break;
        }
    }

    return 0;
}