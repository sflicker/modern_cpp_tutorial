#include <iostream>
//
// Created by scott on 10/25/25.
//
const int N = 1000;

int main() {
    int i, j, a[N+1];
    int count = 0;
    for (a[1] = 0, i = 2; i <= N; i++) {
        a[i] = 1;
    }
    for (i = 2; i <= N/2; i++) {
        for (j = 2; j <= N/i; j++) {
            a[i*j] = 0;
        }
    }
    for (i=1; i<=N; i++) {
        if (a[i] == 1) {
            count++;
            std::cout << i << ' ' ;
        }
    }
    std::cout << std::endl;
    std::cout << "number of primes up to " << N << " is " << count << std::endl;
    return 0;
}