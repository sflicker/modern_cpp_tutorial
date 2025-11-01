//
// Created by scott on 10/29/25.
//
// Linked List demo implementation from
// Algorithms in c++ by R. Sedgewick
#include <iostream>

struct node {
    int key;
    struct node* next;
};

int main() {
    int i, N, M;
    struct node *t, *x;
    std::cin >> N >> M;
    t = new node;
    t->key = 1;
    x = t;
    for (i = 2; i <= N; i++) {
        t->next = new node;
        t = t->next;
        t->key = i;
    }

    t->next = x;

    while (t != t->next) {
        for (i = 1; i <= M; i++) {
            t = t->next;
        }
        std::cout << t->next->key << ' ';
        x = t->next;
        t->next = x->next;
        delete x;
    }
    std::cout << t->key << '\n';

}