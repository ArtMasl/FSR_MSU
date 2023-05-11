#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lodepng.h"  
 
char* loadPng(const char* filename, int* width, int* height) { 
   
  unsigned char* image = NULL; 
 
  int error = lodepng_decode32_file(&image, width, height, filename); 
  if(error){ 
    printf("error %u: %s\n", error, lodepng_error_text(error)); 
  } 
   
  return (image); 
} 
 
 
void writePng(const char* filename, const unsigned char* image, unsigned width, unsigned height) { 
  unsigned char* png; 
  long unsigned int pngsize; 
 
  int error = lodepng_encode32(&png, &pngsize, image, width, height); 
  lodepng_save_file(png, pngsize, filename); 
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error)); 
 
  free(png); 
} 
 
void preparation(unsigned char *omat, int w, int h){ 
    int i, j;
    for(i=2;i<h-1;i++) 
        for(j=2;j<w-1;j++){ 
            if(omat[w*i+j]<65) 
                omat[w*i+j]=0; 
            if(omat[w*i+j]>187) 
                omat[w*i+j]=255; 
        } 
    return; 
} 
 
void Gauss(unsigned char *omat, unsigned char *d, int w, int h){     int i, j; 
     for(i=2;i<h-1;i++) 
        for(j=2;j<w-1;j++){ 
            d[w*i+j]=0.12*omat[w*i+j]+0.12*omat[w*(i+1)+j]+0.12*omat[w*(i-1)+j]; 
            d[w*i+j]=d[w*i+j]+0.12*omat[w*i+(j+1)]+0.12*omat[w*i+(j-1)]; 
            d[w*i+j]=d[w*i+j]+0.09*omat[w*(i+1)+(j+1)]+0.09*omat[w*(i+1)+(j-1)]; 
            d[w*i+j]=d[w*i+j]+0.09*omat[w*(i-1)+(j+1)]+0.09*omat[w*(i-1)+(j-1)]; 
        } 
   return; 
} 
 
void colouring(unsigned char *dmat, unsigned char *mpicture, int w, int h){
    int i;	
    for(i=1;i<w*h;i++) { 
        mpicture[i*4]=87+dmat[i]+0.5*dmat[i-1]; 
        mpicture[i*4+1]=100+dmat[i]; 
        mpicture[i*4+2]=10+dmat[i]; 
        mpicture[i*4+3]=120; 
    } 
    return; 
} 
 
 
int main() { 
 
    char * filename = "scull.png"; 
    int w, h, i, k=0, n=4;
    char * picture = loadPng(filename, &w, &h); 
    if (picture == NULL){ 
        printf("I can not read the picture from the file %s. Error.\n", filename); 
        return -1; 
    } 
 
    unsigned char* image = (unsigned char*)malloc(h*w*sizeof(unsigned char)); 
    
    for(i=0;i<n*h*w;i+=n){ 
 	image[k]=0.299*picture[i]+0.587*picture[i+1]+0.114*picture[i+2]; 
         k++; 
    } 

    unsigned char* image_1 = (unsigned char*)malloc(h*w*sizeof(unsigned char));
    unsigned char* data = (unsigned char*)malloc(n*h*w*sizeof(unsigned char));

    preparation(image, w, h); 
    Gauss(image, image_1, w, h); 
    colouring(image_1, data, w, h); 
   
    char* new_image = "esm.png"; 
    writePng(new_image, data, w, h);  
    return 0; 
}
