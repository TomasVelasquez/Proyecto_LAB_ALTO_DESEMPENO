#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief 
 * Funcion que genera al azar dos posiciones en las que tomar las fotos, asegurando que el area en las que coincidan sea un cierto porcentaje del area total
 * en la que podrian coincidir
 * @param setArea porcentaje del area en el que coinciden ambas fotos (valor flotante de 0 a 1)
 * @param foto1 direccion de memoria donde se guarda la posicion del primer valor del primer cuadrado
 * @param foto2 direccion de memoria donde se guarda la posicion del primer valor del segundo cuadrado
 * @param fotoSize tamaño de un lado de la foto (las fotos son cuadradas)
 * @param M tamaño vertical de la mesa
 * @param N tamaño horizontal de la mesa
 */
void escoger_fotos(float setArea, int* foto1, int*foto2, int fotoSize, int M, int N){

    // Escogemos una posicion de foto 1 tal que la segunda foto pueda estar en contacto con la foto1 y desde cualquier direccion sin superar los maximos de la mesa
    int x1;
    int y1;
    x1 = fotoSize+1+rand()%(M-3*fotoSize-2);
    y1 = fotoSize+1+rand()%(N-3*fotoSize-2);

    // Escogemos un x2 de tal forma que sea posible seleccionar un y2 que cumpla con setArea
    int x2;
    x2 = rand()%((int)(x1 + fotoSize*(1 - setArea) - 1 - x1 - fotoSize*(setArea-1) - 1)) + x1 + fotoSize*(setArea-1) + 1;

    // Existen dos posibles valores de y2 que cumplen que el area se ajuste a setArea, por lo que escogeremos uno al azar
    int y2;
    if(rand()%2 == 1)
        y2 = y1 + fotoSize - (setArea*fotoSize*fotoSize)/(fotoSize-abs(x2-x1));
    else
        y2 = y1 - fotoSize + (setArea*fotoSize*fotoSize)/(fotoSize-abs(x2-x1));

    // Entregamos a foto 1 y foto 2 la direccion en la cual comenzaran sus cuadrados
    *foto1 = N * x1 + y1;
    *foto2 = N * x2 + y2;
}



void calculoArea(int* minX, int* maxX, int* minY, int* maxY, int iteracion, int fotoSize){
    if(Iteracion%(2*fotoSize-1)<fotoSize){
        *min_y = 0;
        *max_y = Iteracion%(2*fotoSize-1);
    }
    else{
        *min_y = Iteracion%(2*fotoSize-1) - fotoSize + 1;
        *max_y = fotoSize - 1;
    }

    if(Iteracion/(2*fotoSize-1)<fotoSize){
        *min_x = 0;
        *max_x = Iteracion/(2*fotoSize-1);
    }
    else{
        *min_x = Iteracion/(2*fotoSize-1) - fotoSize + 1;
        *max_x = fotoSize - 1;
    }
}



int main(){
    srand(time(NULL));
    int foto1;
    int foto2;
    escoger_fotos(0.1,&foto1,&foto2,100,1000,1000);
    int x1 = foto1/1000;
    int y1 = foto1%1000;
    int x2 = foto2/1000;
    int y2 = foto2%1000;
    printf("(x1 = %d, y1 = %d)\n(x2 = %d, y2 = %d)", x1, y1, x2, y2);

    return 0;
}
