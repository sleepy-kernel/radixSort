template <typename T> void quickSort(T *data, long long int element_count) {
    if (element_count <= 1) return;
    T pv = *(data + element_count - 1);
    T *b = data;
    T *a = data;
    int k = 0;
    for (int i = 0; i < element_count - 1; i++) {
        if (*b < pv) {
            k++;
            T temp = *a;
            *a = *b;
            *b = temp;
            a++;
        }
        b++;
    }
    *b = *a;
    *a = pv;
    quickSort<T>(data, k);
    quickSort<T>(data + k + 1, element_count - k - 1);
}