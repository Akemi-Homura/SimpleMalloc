#include <stdio.h>
#include "simple_free.h"

int main() {
    printf("%ld\n", sizeof(struct s_block));
    int *a = (int *) _malloc(10 * sizeof(int));
    for (int i = 0; i < 10; i++) {
        a[i] = i;
    }
    for (int i = 0; i < 10; i++) {
        printf("%d\n", a[i]);
    }
    _free(a);
    return 0;
}