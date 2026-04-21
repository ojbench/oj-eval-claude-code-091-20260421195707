#include "src.hpp"
#include <iostream>
#include <cstdio>

int main() {
    ESet<long long> s[25];
    ESet<long long>::iterator it;
    int op, lst = 0, it_a = -1, valid = 0;

    while (scanf("%d", &op) != EOF) {
        long long a, b, c;
        switch (op) {
            case 0: {
                scanf("%lld %lld", &a, &b);
                auto p = s[a].emplace(b);
                if (p.second) {
                    it_a = a;
                    it = p.first;
                    valid = 1;
                }
                break;
            }
            case 1: {
                scanf("%lld %lld", &a, &b);
                if (valid && it_a == a && *it == b) valid = 0;
                s[a].erase(b);
                break;
            }
            case 2: {
                scanf("%lld", &a);
                s[++lst] = s[a];
                break;
            }
            case 3: {
                scanf("%lld %lld", &a, &b);
                auto it2 = s[a].find(b);
                if (it2 != s[a].end()) {
                    printf("true\n");
                    it_a = a;
                    it = it2;
                    valid = 1;
                } else {
                    printf("false\n");
                }
                break;
            }
            case 4: {
                scanf("%lld %lld %lld", &a, &b, &c);
                printf("%d\n", s[a].range(b, c));
                break;
            }
            case 5: {
                if (valid) {
                    auto it2 = it;
                    // Check if it's the first element
                    // The original 1.cpp used a trick: if (it == --it2) valid = 0;
                    // This implies it2 becomes the element before it, and if it was already first, it2 might become end or stay same depending on implementation.
                    // In my implementation, --it on first element will likely stay at first or go to a state we can check.
                    // Let's refine operator-- and operator++ to handle begin/end properly.
                    // For simplicity, let's just try to emulate the 1.cpp logic.
                    auto it_copy = it;
                    ESet<long long>::iterator it_begin = s[it_a].find(*it); // Not quite right, but find will give us it
                    // Actually, let's just use the logic from 1.cpp
                    auto it_prev = it;
                    --it_prev;
                    if (it == it_prev) {
                        valid = 0;
                    }
                    if (valid) {
                        --it;
                        printf("%lld\n", *it);
                    } else {
                        printf("-1\n");
                    }
                } else {
                    printf("-1\n");
                }
                break;
            }
            case 6: {
                if (valid) {
                    auto it_next = it;
                    ++it_next;
                    if (it_next == s[it_a].end()) {
                        valid = 0;
                    } else {
                        it = it_next;
                        printf("%lld\n", *it);
                    }
                }
                if (!valid) {
                    printf("-1\n");
                }
                break;
            }
        }
    }
    return 0;
}
