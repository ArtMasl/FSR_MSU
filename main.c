#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>


//fits into a picture or not
int exists(int i,int j,int width,int height){
    if ((i >= 1) && (i <= height - 1) && (j >= 1) && ( j <= width - 1)) return 1;
    return 0;
}

//col_num associated with convexity component number
void dfs(int i,int j,int w,int h,unsigned char* ourIm, int* components,int col_num)
{
    components[w*i+j] = col_num;
    //pixels in triangle which surround us
    if (exists(i, j - 2, w, h))
        //if differs in color in 80 and not visited yet
        if ((abs(ourIm[w * i + j] - ourIm[w*i + (j - 2)]) <= 80) && !(components[w*i + (j - 2)]))
            dfs(i, j - 2, w, h, ourIm, components, col_num);


    if (exists(i - 2, j + 1, w, h))
        if ((abs(ourIm[w * i + j] - ourIm[w*(i - 2) + (j + 1)]) <= 80) && !(components[w*(i - 2) + (j + 1)]))
            dfs(i - 2, j + 1, w, h, ourIm, components, col_num);


    if (exists(i + 2, j + 1, w, h))
        if ((abs(ourIm[w*i + j] - ourIm[w*(i + 2) + (j + 1)]) <= 80) && !(components[w*(i + 2) + (j + 1)]))
            dfs(i + 2, j + 1, w, h, ourIm, components, col_num);


    return;
}

int main()
{

    char* inPath = "and.png";
    int width, height, n; //n = number of color channels


    //image loading, getting information about size
    unsigned char* imageData= stbi_load(inPath, &width, &height, &n, 0);
    if (imageData == NULL) {
        printf("ERROR: can't read file %s\n", inPath);
        return 1;
    }

    int i, j, k = 0;
    //switching to a single-channel image
    unsigned char* Image = (unsigned char*)malloc(width*height*sizeof(unsigned char));
    for (i = 0; i < width * height * n; i = i + n) {
        Image[k] = 0.34375*imageData[i] + 0.5*imageData[i + 1] + 0.15625*imageData[i + 2];
        k++;
    }


    //preparation, contrasting
    for (i = 2; i < height - 1; i++) {
        for (j = 2; j < width - 1; j++) {
            if (Image[width*i + j] < 92) Image[width*i + j] = 0;
            if (Image[width*i + j] <= 149  && Image[width*i + j] >= 92) Image[width*i + j] = 127;
             //if (Image[width*i + j] < 110  && Image[width*i + j] >= 100) Image[width*i + j] = 190;
           // if (Image[width*i + j] < 120  && Image[width*i + j] >= 110) Image[width*i + j] = 240;
            //if (Image[width*i + j] == 120) Image[width*i + j] = 255;
            if (Image[width*i + j] > 149) Image[width*i + j] = 255;
        }
    }


    int* components = (int*)malloc((width*height)*sizeof(int));
    for (i = 0; i < width*height; i++) components[i] = 0;

    printf("components is created\n");

    int col_num = 0; //col_num associated with convexity component number

    for (i = 2; i < height - 1; i++)
        for (j = 2; j < width - 1; j++)
            if (!components[width*i + j])
            {
                col_num++;
                //printf("start DFS %d %d\n",i,j);
                dfs(i,j,width,height,Image,components,col_num);
            }

    printf("OK DFS\n");

    unsigned char* odata = (unsigned char*)malloc(width*height*n*sizeof(unsigned char));
    k = 0; int coef;
    for (i = 0; i < height*width*n; i += n)
    {
        //now pixels in one component have the same number
        coef = components[k]%50 + components[k]%37;
        odata[i] = 4*coef - 70;
        odata[i + 1] = 3*coef + 40;
        odata[i + 2] = 5*coef + 60;
        if (n == 4) odata[i + 3] = 255; //four channels
        k++;
    }

    printf("OK odata\n");

/*
    //Gauss
    for (i = 1; i < height-1; i++) {
        for (j = 2; j < width-1; j++) {
            newIm[width*i+j] = newIm[width*i+j] + 0.09*Image[width*(i-1)+(j+1)] + 0.09*Image[width*(i-1)+(j-1)];
        }
    }
    stbi_write_png(outPath, width, height, 1, newIm, 0);
*/

    char* outPath = "output2.png";

    stbi_write_png(outPath, width, height, n, odata, 0);
    printf("Hello world!\n");
    return 0;

}
