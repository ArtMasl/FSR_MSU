#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


char* loadPng(const char* filename, int* width, int* height) {

  unsigned char* image = NULL;

  int error = lodepng_decode32_file(&image, width, height, filename);
  if(error)
  {
    printf("error %u: %s\n", error, lodepng_error_text(error));
  }

  return (image);
}



void writePng(const char* filename, const unsigned char* image, unsigned width, unsigned height) {
  unsigned char* png;
  int pngsize;

  int error = lodepng_encode32(&png, &pngsize, image, width, height);
  if(!error) {
    lodepng_save_file(png, pngsize, filename);
  }

  /*if there's an error, display it*/
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  free(png);
}

void get_pixel(int x, int y, int *r, int *g, int *b, int *a, char* image, int width )
{

   *r =  image[4 * width * y + 4 * x + 0];
   *g =  image[4 * width * y + 4 * x + 1];
   *b =  image[4 * width * y + 4 * x + 2];
   *a =  image[4 * width * y + 4 * x + 3];

   return;
}

void set_pixel(int x, int y, int r, int g, int b, int a, char* image, int width )
{

   image[4 * width * y + 4 * x + 0] = r;
   image[4 * width * y + 4 * x + 1] = g;
   image[4 * width * y + 4 * x + 2] = b;
   image[4 * width * y + 4 * x + 3] = a;

   return;
}

int is_close(int r1, int g1, int b1, int a1, int r2, int g2, int b2, int a2) {
    int e_r = 10;
    int e_g = 10;
    int e_b = 10;
    int e_a = 10;
    if ((fabs(r1 - r2)<e_r)&&(fabs(g1 - g2) < e_g)&&(fabs(b1 - b2) < e_b)&&(fabs(a1 - a2) < e_a)) return 1;
    else return 0;
   }


int is_black(int r, int g, int b) {
  // Here is the place for experiments and improvments
      int gray=(r+g+b)/3;
      if ( gray < 128 ) return 1;
      else return 0;
}

int main() {

    char * filename = "scull.png";
    int w, h, i, j, k=0;
    int r, g, b, a;
    int r1, g1, b1, a1;
    char * picture = loadPng(filename, &w, &h);
    if (picture == NULL){
        printf("I can not read the picture from the file %s. Error.\n", filename);
        return -1;
    }
    char* image = (char*)malloc(w*h*sizeof(char));
    for (i = 0; i < 4*w*h; i+=4) {
        image[k] = 0.34375*picture[i] + 0.5*picture[i + 1] + 0.15625*picture[i + 2];
        k++;
    } 
    

    for (i=2; i < h-1; i++) {
        for (j=2; j < w-1; j++) {
            if (image[w*i + j] < 92) image[w*i + j] = 0;
            if (image[w*i + j] <= 149  && image[w*i + j] >= 92) image[w*i + j] = 127;
            if (image[w*i + j] > 149) image[w*i + j] = 255;
        }
    }

    char * new_image = "scull-modified.png";
    writePng(new_image, image, w, h);

    return 0;
}
