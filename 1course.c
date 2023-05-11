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

void preparation(unsigned char * od, int w, int h){
    int i, j;
    for (i=2; i<h; i++)
           for (j=2; j<w; j++) {
                if (od[w*i + j]<100) od[w*i + j]=0;
		else if (od[w*i + j]>168) od[w*i + j]=255;
	//	else od[w*i + j]=128;
           }
    return;
}

void Sobel(unsigned char * od, unsigned char * dc, int w, int h){
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

void Gauss(unsigned char * od, unsigned char * dc, int w, int h){
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

void colouring(unsigned char * dc,unsigned char * mcod, int w, int h, int n){
        int i;
        for (i=1; i<w*h; i++) {
                mcod[i*n]=80+dc[i]+0.5*dc[i-1];
                mcod[i*n+1]=45+dc[i];
                mcod[i*n+2]=150+dc[i];
                mcod[i*n+3]=255;
        }
        return;
}


int exists(int i,int j,int width,int height){
    if ((i>0) && (i<height) && (j>0) && (j<width)) return 1;
    return 0;
}

/*void DFS(int i,int j,int w,int h,unsigned char* image, int* components,int adj_num)
{
    components[w*i + j] = adj_num;
    if (exists(i, j-1, w, h))
        if ((fabs(image[w*i + j] - image[w*i + (j-1)]) <= 10) && !(components[w*i + (j-1)]))
            DFS(i, j-1, w, h, image, components, adj_num);
    if (exists(i, j+1, w, h))
        if ((fabs(image[w*i + j] - image[w*i + (j+1)]) <= 10) && !(components[w*i + (j+1)]))
            DFS(i, j+1, w, h, image, components, adj_num);
    if (exists(i-1, j, w, h))
        if ((fabs(image[w*i + j] - image[w*(i-1) + j]) <= 10) && !(components[w*(i-1) + j]))
            DFS(i-1, j, w, h, image, components, adj_num);
    if (exists(i-1, j-1, w, h))
        if ((fabs(image[w*i + j] - image[w*(i-1) + (j-1)]) <= 10) && !(components[w*(i-1) + (j-1)]))
            DFS(i-1, j-1, w, h, image, components, adj_num);
    if (exists(i-1, j+1, w, h))
        if ((fabs(image[w*i + j] - image[w*(i-1) + (j+1)]) <= 10) && !(components[w*(i-1) + (j+1)]))
            DFS(i-1, j+1, w, h, image, components, adj_num);
    if (exists(i+1, j, w, h))
        if ((fabs(image[w*i + j] - image[w*(i+1) + j]) <= 10) && !(components[w*(i+1) + j]))
            DFS(i+1, j, w, h, image, components, adj_num);
    if (exists(i+1, j-1, w, h))
        if ((fabs(image[w*i + j] - image[w*(i+1) + (j-1)]) <= 10) && !(components[w*(i+1) + (j-1)]))
            DFS(i+1, j-1, w, h, image, components, adj_num);
    if (exists(i+1, j+1, w, h))
        if ((fabs(image[w*i + j] - image[w*(i+1) + (j+1)]) <= 10) && !(components[w*(i+1) + (j+1)]))
            DFS(i+1, j+1, w, h, image, components, adj_num);
    return;
}*/

void DFS(int i,int j,int w,int h,unsigned char* image, int* components,int adj_num) {
    components[w*i + j] = adj_num;
    if (exists(i, j-2, w, h))
        if ((fabs(image[w*i + j] - image[w*i + (j-2)]) <= 10) && !(components[w*i + (j-2)]))
            DFS(i, j-2, w, h, image, components, adj_num);

 

    if (exists(i-2, j+1, w, h))
        if ((fabs(image[w*i + j] - image[w*(i-2) + (j+1)]) <= 10) && !(components[w*(i-2) + (j+1)]))
            DFS(i-2, j+1, w, h, image, components, adj_num);


    if (exists(i+2, j+1, w, h))
        if ((fabs(image[w*i + j] - image[w*(i+2) + (j+1)]) <= 10) && !(components[w*(i+2) + (j+1)]))
            DFS(i+2, j+1, w, h, image, components, adj_num);

    return;
}


int main() {

    //char * filename = "hamster.png";
    char * filename = "scull.png";
    int w, h, i, j, k=0, adj_num=0, n=4;
    char * picture = loadPng(filename, &w, &h);
    if (picture == NULL){
        printf("I can not read the picture from the file %s. Error.\n", filename);
        return -1;
    }


    unsigned char* image = (unsigned char*)malloc(w*h*sizeof(unsigned char));
    for (i = 0; i < n*w*h; i+=n) {
        image[k] = 0.299*picture[i] + 0.578*picture[i+1] + 0.114*picture[i+2];
        k++;
    }
    unsigned char* image_0 = (unsigned char*)malloc(w*h*sizeof(unsigned char));
    unsigned char* image_1 = (unsigned char*)malloc(w*h*sizeof(unsigned char));
   // unsigned char* image_2 = (unsigned char*)malloc(w*h*sizeof(unsigned char));
    //unsigned char* image_3 = (unsigned char*)malloc(w*h*sizeof(unsigned char));
    //unsigned char* image_4 = (unsigned char*)malloc(w*h*sizeof(unsigned char));
   // unsigned char* image_5 = (unsigned char*)malloc(w*h*sizeof(unsigned char));
   // unsigned char* image_6 = (unsigned char*)malloc(w*h*sizeof(unsigned char));
    unsigned char* data = (unsigned char*)malloc(n*w*h*sizeof(unsigned char));

    preparation(image, w, h);
   // Sobel(image, image_0, w, h);
    Gauss(image, image_0, w, h);
    Sobel(image_0, image_1, w, h);
    colouring(image_1, data, w, h, n);
    /*int* comps = (int*)malloc((n*w*h)*sizeof(int));
    for (i=0; i < n*w*h; i++) comps[i] = 0;

    for (i = 2; i < h-1; i++)
        for (j = 2; j < w-1; j++)
            if (!comps[w*i + j]) {
                   adj_num++;
                DFS(i, j, w, h, image_1, comps, adj_num);
            }

    k=1;
    for (i=0; i < n*w*h; i+=n)
    {
        data[i] = 80+comps[k]+0.5*comps[k-1];
        data[i+1] = 45+comps[k];
        data[i+2] = 150+comps[k];
        if (n==4) data[i+3] = 255;
        k++;
	}*/ 

    //char * new_image = "hamster-modified.png";
    char * new_image = "esm.png";
    writePng(new_image, data, w, h);

    return 0;
}                                         
