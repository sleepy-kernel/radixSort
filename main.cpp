#include <cstdio>
#include <vector>
#include <cstdint>
#include <time.h>
#include <cstdlib>
#include <radix_sort.h>
#include <quick_sort.h>

// 1 = fail, 0 = success
inline int sort_test(void) {
    const int count = 5;
    std::vector<int32_t> data(count);
    srand(time(NULL));
    for (int i = 0; i < count; i++) {
        data[i] = -(rand() % 65536) * (rand() & 1) + (rand() % 65536) * (!(rand() & 1));
    }
    // for (int i = 0; i < count; i++) { printf("%d\t", data[i]); }
    // printf("\n");
    std::vector<int32_t> test = data;
    radixSort<int32_t>(data.data(), count); ///////////////////////////////////// sort call here
    // for (int i = 0; i < count; i++) { printf("%d\t", data[i]); }
    // printf("\n");
    for (int i = 1; i < count; i++) {
        if (data[i - 1] > data[i]) {
            printf("\033[31mFailed\033[0m\n");
            return 1;
        }
    }
    std::vector<int> temp;
    temp.reserve(count);
    for (int i = 0; i < count; i++) {
        bool flag = false;
        for (int j = 0; j < data.size(); j++) {
            if (test[i] == data[j]) {
                flag = true;
                data.erase(data.begin() + j);
                break;
            }
        }
        if (!flag) {
            printf("\033[31mFailed\033[0m\n");
            return 1;
        }
    }
    printf("\033[32mSuccess\033[0m\n");
    return 0;
}

int main(void) {
    for (int i = 0; i < 10; i++) sort_test();
    return 0;
}