#include <stdio.h>

/*
para imprimir variables usando la función printf debemos considerar los diferentes tipos de datos y el formato como se pueden
imprimir.

|    FORMATO    |        DEFINICION           |      USO      |          EJEMPLO          |
    %d               entero con signo              %d o %i      printf("num is %d", 10);
    %ld           entero largo con signo            %ld         printf("num is %ld", 1000000000L);
    %f            numero flotante                   %f          printf("float %f", 3.14);
    %lf           double float                      %lf         printf("d.float %lf", 3.8181818);
    %.nf          floante con n precision        %.1f o %.nf    printf("precision %.2f", 4.789); //muestra 4.78
    %c            caracter                          %c          printf("caracter: %c", 'A');
    %s            string (cadena de caracteres)     %s          printf("nombre: %s", "Pedro Pablo");
    %p            puntero                           %p          printf("dirección: %p", )



*/

int main(){

    int a = 2;
    //*b es un puntero, es decir, va a guardar una dirección de memoria.
    int *b = &a; //con & obtenemos la referencia o la dirección de memoria de 'a'. Una referencia a  'a'

    printf("valor de a: %d\n", a);
    printf("dirección de a: %p\n", &a);

    printf("valor de b: %p\n", b);
    printf("valor de la dirección a la que apunta b: %d\n", *b);// * es el operador de desreferenciación, y obtiene el valor almacenado
    //en la dirección de memoria a la que apunta el puntero b
    printf("direccion de b: %p\n", &b);

    printf("precision: %.2f\n", 3.12345);

    int num;
    scanf("%d", &num);
    printf("num is %d\n", num);


    return 0;
}