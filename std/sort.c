#include "../include/sort.h"
#include "../include/mm.h"

int *__sort_counting(int *data, int size) {
    int *res = (int *)malloc(size * 4);
    int m = data[0];
    int i = 1;
    while(i < size) {
        if(data[i] > m) m = data[i];
        i++;
    }
    int *count = (int *)malloc((m + 1) * 4);
    i = 0;
    while(i <= m) {
        count[i] = 0;
        ++i;
    }
    i = 0;
    while(i < size) {
        count[data[i]]++;
        i++;
    }
    i = 1;
    while(i <= m) {
        count[i] += count[i - 1];
        i++;
    }
    i = size - 1;
    while(i >= 0) {
        res[count[data[i]] - 1] = data[i];
        count[data[i]]--;
        i--;
    }
    return data;
}
int *_sort_bubble(int *data, int size) {
    int i, j;
    i = 0;
    while(i < size - 1) {
        j = 0;
        while(j < size - i - 1) {
            if(data[j] > data[j + 1]) {
                int t = *(&data[j]);
                *(&data[j]) = *(&data[j + 1]);
                *(&data[j + 1]) = t;
            }
            j++;
        }
        i++;
    }
    return data;
}