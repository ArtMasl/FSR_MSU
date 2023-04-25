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
      if ( gray < 96 ) return 1;
      else return 0;
}

void preprocess_image_Gauss(char * image, int width, int height, int n) {
        int x, y, i, j;
        double g[3][3];
        g[0][0] = g[0][2] = g[2][0] = g[2][2] =  0.0924;
        g[0][1] = g[1][0] = g[1][2] = g[2][1] =  0.1192;
        g[1][1] = 0.1538;
        for (x=1; x<=(width-2); x++)
                for (y=1; y<=(height-2); y++)
                        for (i=-1; i<=1; i++)
                                for (j=-1; j<=1; j++)
                                        image[n*(y+j)*width + n*i + n*x] += g[1-i][1-j];
}
  void preprocess_image_Sobel_x(char * image, int width, int height, int n) {
        int x, y, i, j;
        double s[3][3];
        s[0][0] = s[2][0] =  -1;
        s[0][2] = s[2][2] =  1;
        s[1][0] = -2;
        s[1][2] = 2;
        s[0][1] = s[1][1] = s[2][1] =  0;
        for (x=1; x<=(width-2); x++)
                for (y=1; y<=(height-2); y++)
                        for (i=-1; i<=1; i++)
                                for (j=-1; j<=1; j++)
                                        image[n*(y+j)*width + i + n*x] += s[1-i][1-j];
}

void preprocess_image_Sobel_y(char * image, int width, int height, int n) {
        int x, y, i, j;
        double s[3][3];
        s[0][0] = s[0][2] =  -1;
        s[2][0] = s[2][2] =  1;
        s[0][1] = -2;
        s[2][1] = 2;
        s[1][0] = s[1][1] = s[1][2] =  0;
        for (x=1; x<=(width-2); x++)
                for (y=1; y<=(height-2); y++)
                        for (i=-1; i<=1; i++)
                                for (j=-1; j<=1; j++)
                                        image[n*(y+j)*width + i + n*x] += s[1-i][1-j];
}
void preprocess_image_Sobel_all(char * image_x, char * image_y, int width, int height, int n) {
        int i;
        for (i=0; i<n*width*height; i++) image_x[i] = sqrt(image_x[i]*image_x[i] + image_y[i]*image_y[i]);
}

int exists(int i,int j,int width,int height){
    if ((i>0) && (i<height) && (j>0) && (j<width)) return 1;
    return 0;
}

void DFS(int i,int j,int w,int h,unsigned char* image, int* components,int adj_num)
{
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

    //char * filename = "C_ver1.png";
    char * filename = "Scull.png";
    int w, h, i, j, c, k=0, adj_num=0;
    int r, g, b, a, n=4;
    int r1, g1, b1, a1;
    char * picture = loadPng(filename, &w, &h);
    char * picture_1 = loadPng(filename, &w, &h);
    if (picture == NULL){
        printf("I can not read the picture from the file %s. Error.\n", filename);
        return -1;
    }

    if (picture_1 == NULL){
        printf("I can not read the picture from the file %s. Error.\n", filename);
        return -1;
    }

    //preprocess_image_Gauss(picture, w, h, n);

    preprocess_image_Sobel_x(picture, w, h, n);
    preprocess_image_Sobel_y(picture_1, w, h, n);
    preprocess_image_Sobel_all(picture, picture_1, w, h, n);
 
    for (int i = 0; i < w; i++){
        for (int j = 0; j < h; j++){
            get_pixel(i, j, &r, &g, &b, &a, picture, w);
            if (is_black(r, g, b)) set_pixel(i, j, 0, 0, 0, a, picture, w);
            else set_pixel(i, j, 255, 255, 255, a, picture, w);
        }
    }

    char* image = (char*)malloc(w*h*sizeof(char));
    for (i = 0; i < n*w*h; i+=n) {
        image[k] = 0.34375*picture[i] + 0.5*picture[i+1] + 0.15625*picture[i+2];
        k++;
    }

    int* comps = (int*)malloc((w*h)*sizeof(int));
    for (i=0; i < w*h; i++) comps[i] = 0;

    for (i = 2; i < h-1; i++)
        for (j = 2; j < w-1; j++)
            if (!comps[w*i + j]) {
                   adj_num++;
                DFS(i, j, w, h, image, comps, adj_num);
            }

    char* data = (char*)malloc(n*w*h*sizeof(char));

    k=0;
    for (i=0; i < n*w*h; i+=n)
    {
        c = comps[k]%50 + comps[k]%37;
        data[i] = 4*c - 70;
        data[i+1] = 3*c + 40;
        data[i+2] = 5*c + 60;
        if (n==4) data[i+3] = 255;
        k++;
    }

    //char * new_image = "C_ver1-modified.png";
    char * new_image = "Scull-modified.png";
    writePng(new_image, data, w, h);

    return 0;
}
                                                              
