
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <device_functions.h>
#include <cuda.h>

#include <stdio.h>
#include <stdlib.h>


_global__ void imageCutter (int* fotoOriginal, int* fotoNueva, int posFotoNueva, int fotoSize, int N){
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    if(tid < fotoSize*fotoSize){
    	fotoNueva[tid] = fotoOriginal[posFotoNueva+tid%fotoSize+(tid/fotoSize)*N];
    }
}


// si queremos calcular el algoritmo en todos los puntos posibles (una combinacion por kernel), tendremos que correr 4*fotoSize² kernels distintos

// Para que el algoritmo sea mas optimo, se pueden normalizar las imagenes primero, esto permite que donde se encuentren puntos negros sea igual a encontrar puntos blancos




/* Ideas para optimizar:
- no probar todos los puntos, saltarse cada 4 por ejemplo y luego calcular los puntos cercanos al ganador 
- que cada kernel se encargue de varias combinaciones
	- se podria usar memoria comparitda ya que los datos deberian estar bien cercanos !!
	- Guardamos en memoria compartida la informacion que necesitemos para las N iteraciones
	- Generamos un espacio en memoria compartida para hacer la reduccion
	- Cada vez que realizemos una iteracion, hacemos la reduccion y guardamos el valor en memoria global
- downsample the two images first, that way processing time is lowered


*/


// acercamiento mas simple (y menos optimizado)
_global__ void crossCorrelation (int* foto1, int* foto2, int pixelCount, int fotoSize, int Iteracion, int minX, int difX, int minY, int difY, int posXFoto2, int posYFoto2){
    int tid = threadIdx.x + blockIdx.x * blockDim.x;
    extern  __shared__ int sharedMemory[];
    int* f1 = sharedMemory;
    int* f2 = sharedMemory[pixelCount];

    f1[tid] = foto1[(minX + tid / difY) * fotoSize + (minY + tid % difY)];
    f2[tid] = foto2[(minX + tid / difY - posXFoto2) * fotoSize + (minY + tid % difY - posYFoto2)];




}








