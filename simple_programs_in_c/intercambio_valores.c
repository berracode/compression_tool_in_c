#include <stdio.h>


void swap_without_pointers(){
    int a = 10;
    int b = 20;
    int temp;

    printf("a = %d\n", a);
    printf("b = %d\n", b);

    temp = a;
    a = b;
    b = temp;

    printf("a = %d\n", a);
    printf("b = %d\n", b);

}

void swap_with_floats(){
    float a = 10.5;
    float b = 20.0;
    float temp;

    printf("a = %f\n", a);
    printf("b = %f\n", b);

    temp = a;
    a = b;
    b = temp;

    printf("a = %f\n", a);
    printf("b = %f\n", b);

}

void swap_with_pointers(int *a, int *b) {
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;

}


int main(){
    swap_with_floats();
    swap_without_pointers();

    int a = 11;
    int b = 22;
    printf("a = %d\n", a);
    printf("b = %d\n", b);

    swap_with_pointers(&a, &b);

    printf("a = %d\n", a);
    printf("b = %d\n", b);


    return 0;
}

