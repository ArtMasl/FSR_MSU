#include <stdlib.h> 
#include <stdio.h> 
#include <math.h> 
#include "lodepng.h" 
 
int i, j; 
 
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
 
void pre(unsigned char *omat, int w, int h){ 
    for(i=2;i<h-1;i++) 
        for(j=2;j<w-1;j++){ 
            if(omat[w*i+j]<65) 
                omat[w*i+j]=0; 
            if(omat[w*i+j]>187) 
                omat[w*i+j]=255; 
        } 
    return; 
} 
 
void Gauss(unsigned char *omat, unsigned char *d, int w, int h){ 
     for(i=2;i<h-1;i++) 
        for(j=2;j<w-1;j++){ 
            d[w*i+j]=0.12*omat[w*i+j]+0.12*omat[w*(i+1)+j]+0.12*omat[w*(i-1)+j]; 
            d[w*i+j]=d[w*i+j]+0.12*omat[w*i+(j+1)]+0.12*omat[w*i+(j-1)]; 
            d[w*i+j]=d[w*i+j]+0.09*omat[w*(i+1)+(j+1)]+0.09*omat[w*(i+1)+(j-1)]; 
            d[w*i+j]=d[w*i+j]+0.09*omat[w*(i-1)+(j+1)]+0.09*omat[w*(i-1)+(j-1)]; 
        } 
   return; 
} 

void Sobel(char * od, char * dc, int w, int h){
        char x, y;
        int i, j;
        for (i=1; i < h-1; i++)
           for (j=1; j < w-1; j++) {
                x=(-1)*od[w*(i-1)+(j-1)]-2*od[w*i+(j-1)]-1*od[w*(i+1)+(j-1)]+1*od[w*(i-1)+(j+1)]+2*od[w*i+(j+1)]+1*od[w*(i+1)+(j+1)];                y=(-1)*od[w*(i-1)+(j-1)]-2*od[w*(i-1)+j]-1*od[w*(i-1)+(j+1)]+1*od[w*(i+1)+(j-1)]+2*od[w*(i+1)+j]+1*od[w*(i+1)+(j+1)];                dc[i*w+j]=sqrt(x*x + y*y);
           }
        return;
}
 
void color(unsigned char *dmat, unsigned char *mpicture, int w, int h){ 
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
    int w, h; 
    int k=0; 
    char * picture = loadPng(filename, &w, &h); 
    if (picture == NULL){ 
        printf("I can not read the picture from the file %s. Error.\n", filename); 
        return -1; 
    } 
 
 
    unsigned char *image=(unsigned char*)malloc(h*w*sizeof(unsigned char)); 
    unsigned char *image_1=(unsigned char*)malloc(h*w*sizeof(unsigned char));
    unsigned char *image_2=(unsigned char*)malloc(h*w*sizeof(unsigned char));
    unsigned char *image_3=(unsigned char*)malloc(h*w*sizeof(unsigned char)); 
    unsigned char *data=(unsigned char*)malloc(h*w*4*sizeof(unsigned char)); 
 
    for(i=0;i<h*w*4;i=i+4){ 
 
         image[k]=0.299*picture[i]+0.587*picture[i+1]+0.114*picture[i+2]; 
         k++; 
    } 
 
    pre(image, w, h); 
    Sobel(image, image_1, w, h);
    Gauss(image_1, image_2, w, h);
    Sobel(image_2, image_3, w, h); 
    color(image_3, data, w, h); 
   
     
 
    char * new_image = "esm.png"; 
    writePng(new_image, data, w, h); 
    free(image); 
    free(image_1); 
    free(image_2); 
    free(data); 
    return 0; 
}
