#include <stdio.h>

void main(){

    double height, width;
    double area;

    printf("Enter height: \n");
    scanf("%lf", &height);

    printf("Enter width: \n");
    scanf("%lf", &width);

    area = height*width;
    printf("The area is: %.2lf\n", area);

}