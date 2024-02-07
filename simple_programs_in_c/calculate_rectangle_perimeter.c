#include <stdio.h>

void main(){

    double height, width;
    double perimeter;

    printf("Enter height: \n");
    scanf("%lf", &height);

    printf("Enter width: \n");
    scanf("%lf", &width);

    perimeter = 2*(height+width);
    printf("The perimeter is: %.2lf\n", perimeter);

}