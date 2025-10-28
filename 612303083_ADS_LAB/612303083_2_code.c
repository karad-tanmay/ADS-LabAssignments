#include <stdio.h>
#include <stdlib.h>

void swap(int *a, int *b) {
    int temp = *a; *a = *b; *b = temp;
}

void display_arr(int *arr, int start, int end) {
    for (int i = start; i <= end; i++) printf("%d ", arr[i]);
}

static void print_half(int *arr, int l, int r) {
    for (int i = l; i <= r; ++i) {
        printf("%d", arr[i]);
        if (i < r) printf(" ");
    }
}

static void print_division(int *arr, int low, int mid, int high) {
    printf("divide: ");
    print_half(arr, low, mid);
    printf("        ");
    print_half(arr, mid + 1, high);
    printf("\n");
}

void merge(int *arr, int low, int mid, int high) {
    int temp[100];
    int i = low, j = mid + 1, k = 0;
    while (i <= mid && j <= high) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid) temp[k++] = arr[i++];
    while (j <= high) temp[k++] = arr[j++];

    for (i = low, j = 0; i <= high; i++) arr[i] = temp[j++];

    printf("merge: ");
    print_half(arr, low, high);
    printf("\n");
}

void mS(int *arr, int low, int high, int len) {
    if (low >= high) return;
    int mid = (low + high) / 2;

    print_division(arr, low, mid, high);

    mS(arr, low, mid, len);
    mS(arr, mid + 1, high, len);
    merge(arr, low, mid, high);
}

void merge_sort(int *arr, int len) {
    mS(arr, 0, len - 1, len);
}

int main() {
    int arr[] = {1, 2, 9, 5, 6, 0, 4, -1, 2, 1, 15, -10, 7, 3, 8, 11, 14, 13, 12, 10};
    display_arr(arr, 0, 19);
    printf("\n");

    merge_sort(arr, 20);
    return 0;
}
