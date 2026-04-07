#pragma once
#include <cstdlib>
#include <cstring>
#include <type_traits>

/*
return false; only when malloc fail

Works for both signed and unsigned int8 int16 int32 int64...

Does not work on float
*/
template <typename T> bool radixSort(T* data, unsigned long long int element_count) {
    constexpr int bucket_size = 512; // bucket type + 256, example: for 16 bit as bucket then bucket size will be 2^16 + 256
    signed char* _data = reinterpret_cast<signed char*>(data);
    signed char* temp = (signed char*)std::malloc(element_count * sizeof(T));
    if (temp == NULL) return false;
    signed char* container[2];
    container[0] = _data;
    container[1] = temp;
    unsigned long long* freq = (unsigned long long*)malloc(bucket_size * sizeof(*freq));
    if (freq == NULL) return false;
    freq += (bucket_size >> 1); // singed x-bit int range from -2^x -> 2^x - 1
    int k = 0;
    for (; k + 1 < sizeof(T); k++) {
        unsigned char* in = reinterpret_cast<unsigned char*>(container[k & 1] + k);
        std::memset(freq - (bucket_size >> 1), 0, bucket_size * sizeof(*freq));
        for (unsigned long long int i = 0; i < element_count * sizeof(T); i += sizeof(T)) freq[in[i]]++;
        for (int i = -(bucket_size >> 1) + 1; i < (bucket_size >> 1); i++) freq[i] += freq[i - 1];
        T* _in = reinterpret_cast<T*>(container[k & 1]);
        T* _out = reinterpret_cast<T*>(container[!(k & 1)]);
        for (unsigned long long int i = element_count - 1; i < (unsigned long long int)(-1); i--) _out[--freq[in[i * sizeof(T)]]] = _in[i];
    }
    if (std::is_signed<T>()) {
        signed char* in = container[k & 1] + k;
        std::memset(freq - (bucket_size >> 1), 0, bucket_size * sizeof(*freq));
        for (unsigned long long int i = 0; i < element_count * sizeof(T); i += sizeof(T)) freq[in[i]]++;
        for (int i = -(bucket_size >> 1) + 1; i < (bucket_size >> 1); i++) freq[i] += freq[i - 1];
        T* _in = reinterpret_cast<T*>(container[k & 1]);
        T* _out = reinterpret_cast<T*>(container[!(k & 1)]);
        for (unsigned long long int i = element_count - 1; i < (unsigned long long int)(-1); i--) _out[--freq[in[i * sizeof(T)]]] = _in[i];
    }
    else {
        unsigned char* in = reinterpret_cast<unsigned char*>(container[k & 1] + k);
        std::memset(freq - (bucket_size >> 1), 0, bucket_size * sizeof(*freq));
        for (unsigned long long int i = 0; i < element_count * sizeof(T); i += sizeof(T)) freq[in[i]]++;
        for (int i = -(bucket_size >> 1) + 1; i < (bucket_size >> 1); i++) freq[i] += freq[i - 1];
        T* _in = reinterpret_cast<T*>(container[k & 1]);
        T* _out = reinterpret_cast<T*>(container[!(k & 1)]);
        for (unsigned long long int i = element_count - 1; i < (unsigned long long int)(-1); i--) _out[--freq[in[i * sizeof(T)]]] = _in[i];
    }
    if (!(k & 1)) std::memcpy(_data, temp, element_count * sizeof(T));
    free(temp);
    return true;
}