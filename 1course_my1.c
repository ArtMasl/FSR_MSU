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

void preparation(char * od, int w, int h){
    int i, j;
    for (i=0; i<h; i++)
           for (j=0; i<w; j++) {
                if (od[w*i + j]<100) od[w*i + j]=0;
                if (od[w*i + j]>168) od[w*i + j]=255;
           }
    return;
}

void fSobel(char * od, char * dc, int w, int h){
        char x, y;
        int i, j;
        for (i=1; i < h-1; i++)
           for (j=1; j < w-1; j++) {
                x=(-1)*od[w*(i-1)+(j-1)]-2*od[w*i+(j-1)]-1*od[w*(i+1)+(j-1)]+1*od[w*(i-1)+(j+1)]+2*od[w*i+(j+1)]+1*od[w*(i+1)+(j+1)];
                y=(-1)*od[w*(i-1)+(j-1)]-2*od[w*(i-1)+j]-1*od[w*(i-1)+(j+1)]+1*od[w*(i+1)+(j-1)]+2*od[w*(i+1)+j]+1*od[w*(i+1)+(j+1)];
                dc[i*w+j]=sqrt(x*x + y*y);
           }
        return;
}

void fGauss(char * od, char * dc, int w, int h){
        char x, y;
        int i, j;
        for (i=1; i < h-1; i++)
           for (j=1; j < w-1; j++) {
                dc[w*i+j]=0.12*od[w*i+j]+0.12*od[w*(i+1)+j]+0.12*od[w*(i-1)+j];
                 dc[w*i+j]=dc[w*i+j]+0.12*od[w*i+(j+1)]+0.12*od[w*i+(j-1)];
                dc[w*i+j]=dc[w*i+j]+0.09*od[w*(i+1)+(j+1)]+0.09*od[w*(i+1)+(j-1)];
                dc[w*i+j]=dc[w*i+j]+0.09*od[w*(i-1)+(j+1)]+0.09*od[w*(i-1)+(j-1)];
           }
        return;
}

void colouring(char * dc, char * mcod, int w, int h, int n){
        int i;
        int c[w*h];
        for (i=0; i<w*h; i++) c[i]=dc[i];
        for (i=1; i<w*h; i++) {
                mcod[i*n]=80+c[i]+0.5*c[i-1];
                mcod[i*n+1]=45+c[i];
                mcod[i*n+2]=150+c[i];
                mcod[i*n+3]=225;
        }
        return;
}

int main() {

    //char * filename = "hamster.png";
    char * filename = "Scull.png";
    int w, h, i, k=0, n=4;
    char * idata = loadPng(filename, &w, &h);
    if (idata == NULL){
        printf("I can not read the picture from the file %s. Error.\n", filename);
        return -1;
    }

    char* odata = (char*)malloc(w*h*sizeof(char));
    char* mcodata = (char*)malloc(w*h*sizeof(char)); 
    for (i=0; i < w*h*n; i+=n){
        odata[k]=0.299*idata[i]+0.578*idata[i+1]+0.114*idata[i+2];
        k++;
    }

    char * datacopy = odata;

   // preparation(odata, w, h);
   // fSobel(odata, datacopy, w, h);
    //fGauss(odata, datacopy, w, h);
    //colouring(datacopy, mcodata, w, h, n);

    //char * new_image = "hamster-modified.png";
    char * new_image = "Scull-modified.png";
    writePng(new_image, mcodata, w, h);

    return 0;
}
