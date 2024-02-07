#include <stdio.h>


void main(){


    int currentYear;
    int age;
    int birthYear;

    printf("Enter the current year: \n");
    scanf("%d", &currentYear);
    printf("Enter your age: \n");
    scanf("%d", &age);

    birthYear = currentYear - age;

    printf("Yow were born in %d\n", birthYear);


}