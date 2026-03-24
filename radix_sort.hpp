#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>
#include <type_traits>
#include <vector>


// return false only if malloc failed. Sorting works for both signed and unsigned data types
template <typename T> bool radixSort(T* data, unsigned long long element_count) {
    constexpr int bucket_size = 512;
    signed char* _data = reinterpret_cast<signed char*>(data);
    signed char* temp = (signed char*)std::malloc(element_count * sizeof(T));
    if (temp == NULL) { return false; }
    signed char* container[2];
    container[0] = _data;
    container[1] = temp;
    unsigned long long* freq = (unsigned long long*)malloc(bucket_size * sizeof(*freq));
    if (freq == NULL) { return false; }
    freq += (bucket_size >> 1); // singed x-bit int range from -2^x -> 2^x - 1
    int k = 0;
    for (; k + 1 < sizeof(T); k++) {
        unsigned char* in = reinterpret_cast<unsigned char*>(container[k & 1] + k);
        std::memset(freq - (bucket_size >> 1), 0, bucket_size * sizeof(*freq));
        for (unsigned long long int i = 0; i < element_count * sizeof(T); i += sizeof(T)) {
            freq[in[i]]++;
        }
        for (int i = -(bucket_size >> 1) + 1; i < (bucket_size >> 1); i++) {
            freq[i] += freq[i - 1];
        }
        T* _in = reinterpret_cast<T*>(container[k & 1]);
        T* _out = reinterpret_cast<T*>(container[!(k & 1)]);
        for (unsigned long long int i = element_count - 1; i < (unsigned long long int)(-1); i--) {
            _out[--freq[in[i * sizeof(T)]]] = _in[i];
        }
    }
    if (std::is_signed<T>()) {
        signed char* in = container[k & 1] + k;
        std::memset(freq - (bucket_size >> 1), 0, bucket_size * sizeof(*freq));
        for (unsigned long long int i = 0; i < element_count * sizeof(T); i += sizeof(T)) {
            freq[in[i]]++;
        }
        for (int i = -(bucket_size >> 1) + 1; i < (bucket_size >> 1); i++) {
            freq[i] += freq[i - 1];
        }
        T* _in = reinterpret_cast<T*>(container[k & 1]);
        T* _out = reinterpret_cast<T*>(container[!(k & 1)]);
        for (unsigned long long int i = element_count - 1; i < (unsigned long long int)(-1); i--) {
            _out[--freq[in[i * sizeof(T)]]] = _in[i];
        }
    } else {
        unsigned char* in = reinterpret_cast<unsigned char*>(container[k & 1] + k);
        std::memset(freq - (bucket_size >> 1), 0, bucket_size * sizeof(*freq));
        for (unsigned long long int i = 0; i < element_count * sizeof(T); i += sizeof(T)) {
            freq[in[i]]++;
        }
        for (int i = -(bucket_size >> 1) + 1; i < (bucket_size >> 1); i++) {
            freq[i] += freq[i - 1];
        }
        T* _in = reinterpret_cast<T*>(container[k & 1]);
        T* _out = reinterpret_cast<T*>(container[!(k & 1)]);
        for (unsigned long long int i = element_count - 1; i < (unsigned long long int)(-1); i--) {
            _out[--freq[in[i * sizeof(T)]]] = _in[i];
        }
    }
    if (!(k & 1)) { std::memcpy(_data, temp, element_count * sizeof(T)); }
    free(temp);
    return true;
}


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