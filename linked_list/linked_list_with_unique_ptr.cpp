//
// Created by scott on 10/29/25.
//

#include <iostream>
#include <memory>

struct Node {
    int key;
    std::unique_ptr<Node> next;
    Node * prev = nullptr;        // raw pointer for circular link back
};

int main() {
    int N, M;
    std::cin >> N >> M;

    if (N <= 0) return 0;

    // create first node
    auto head = std::make_unique<Node>();
    head->key = 1;
    Node * tail = head.get();

    // Build circular linked list
    for (int i = 2; i <= N; ++i) {
        tail->next = std::make_unique<Node>();
        tail->next->key = i;
        tail->next->prev = tail;
        tail = tail->next.get();
    }
    tail->next = std::move(head);      // close the loop
    tail->next->prev = tail;           // back-link

    // Now tail->next points to first node (circular)
    Node* current = tail;  // start from tail so current->next is first node

    // Josephus elimination
    while (current != current->next.get()) {
        for (int i = 1; i <= M; ++i)
            current = current->next.get();

        std::cout << current->next->key << ' ';

        // Remove next node (unique_ptr magic)
        auto doomed = std::move(current->next);
        current->next = std::move(doomed->next);
        current->next->prev = current;
    }

    std::cout << current->key << '\n';
}

