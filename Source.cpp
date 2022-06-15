#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Source.h"

#include <opencv2\opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

/* Function to linearly interpolate between a0 and a1
 * Weight w should be in the range [0.0, 1.0]
 */
float interpolate(float a0, float a1, float w) {
    /* // You may want clamping by inserting:
     * if (0.0 > w) return a0;
     * if (1.0 < w) return a1;
     */
    return (a1 - a0) * w + a0;
    /* // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
     * return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
     *
     * // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
     * return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
     */
}

typedef struct {
    float x, y;
} vector2;

/* Create pseudorandom direction vector
 */
vector2 randomGradient(int ix, int iy) {
    // No precomputed gradients mean this works for any number of grid coordinates
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;
    a *= 3284157443; b ^= a << s | a >> w - s;
    b *= 1911520717; a ^= b << s | b >> w - s;
    a *= 2048419325;
    float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
    vector2 v;
    v.x = cos(random); v.y = sin(random);
    return v;
}

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y) {
    // Get gradient from integer coordinates
    vector2 gradient = randomGradient(ix, iy);

    // Compute the distance vector
    float dx = x - (float)ix;
    float dy = y - (float)iy;

    // Compute the dot-product
    return (dx * gradient.x + dy * gradient.y);
}

// Compute Perlin noise at coordinates x, y
float perlin(float x, float y) {
    // Determine grid cell coordinates
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGridGradient(x0, y0, x, y);
    n1 = dotGridGradient(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGridGradient(x0, y1, x, y);
    n1 = dotGridGradient(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return value;
}


int main(int argc, char* argv[]) {

    int square_size = 1000;
    float* ruido = (float*)malloc(square_size * square_size * sizeof(float));


    for (int x = 0; x < square_size; x++) {
        for (int y = 0; y < square_size; y++)
            ruido[square_size * x + y] = perlin((float)x / (0.03*square_size), (float)y / (0.03*square_size));
    }

    float min = 100;
    float max = -100;
    
    for (int x = 0; x < square_size; x++) {
        for (int y = 0; y < square_size; y++) {
            if (ruido[square_size * x + y] < min)
                min = ruido[square_size * x + y];
            if(ruido[square_size * x + y] > max)
                max = ruido[square_size * x + y];
        }
    }

    printf("min = %f \n", min);
    printf("max = %f \n", max);
    

    Mat image = Mat(square_size, square_size, CV_8UC3, Scalar(0,0,255));
    namedWindow("Display window"); // Create a window for display
    
    for (int x = 0; x < square_size; x++) {
        for (int y = 0; y < square_size; y++) {
            image.at<Vec3b>(Point(x, y))[0] = 184 * (0.69 + ruido[square_size * x + y]);
            image.at<Vec3b>(Point(x, y))[1] = 184 * (0.69 + ruido[square_size * x + y]);
            image.at<Vec3b>(Point(x, y))[2] = 184 * (0.69 + ruido[square_size * x + y]);
        }
    }
    

    imshow("Display window", image); // Show our image inside it
    waitKey(20000); // Wait for a keystroke i

    return 0;
}
