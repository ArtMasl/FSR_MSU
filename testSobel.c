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
      if ( gray < 64 ) return 1;
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
                                        image[n*y*width + n*i + n*x + n*j] += g[1-i][1-j];
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
    if ((i >= 1) && (i <= height - 1) && (j >= 1) && ( j <= width - 1)) return 1;
    return 0;
}

void dfs(int i,int j,int w,int h,unsigned char* ourIm, int* components,int col_num)
{
    components[w*i + j] = col_num;
    //pixels in triangle which surround us
    if (exists(i, j-2, w, h))
        //if differs in color in 10 and not visited yet
        if ((fabs(ourIm[w*i + j] - ourIm[w*i + (j-2)]) <= 10) && !(components[w*i + (j-2)]))
            dfs(i, j-2, w, h, ourIm, components, col_num);


    if (exists(i-2, j+1, w, h))
        if ((fabs(ourIm[w*i + j] - ourIm[w*(i-2) + (j+1)]) <= 10) && !(components[w*(i-2) + (j+1)]))
            dfs(i - 2, j + 1, w, h, ourIm, components, col_num);


    if (exists(i+2, j+1, w, h))
        if ((fabs(ourIm[w*i + j] - ourIm[w*(i+2) + (j+1)]) <= 10) && !(components[w*(i+2) + (j+1)]))
            dfs(i+2, j+1, w, h, ourIm, components, col_num);

 

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
    preprocess_image_Sobel_x(picture, w, h, n);
    preprocess_image_Sobel_y(picture_1, w, h, n);
    if (picture == NULL){
        printf("I can not read the picture from the file %s. Error.\n", filename);
        return -1;
    }

    if (picture_1 == NULL){
        printf("I can not read the picture from the file %s. Error.\n", filename);
        return -1;
    }
    preprocess_image_Sobel_all(picture, picture_1, w, h, n);
    char * new_image = "Scull-Sobel.png";
    writePng(new_image, picture, w, h);

    return 0;
}

