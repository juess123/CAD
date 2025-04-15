#ifdef _WIN32
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif
#include "./algorithm.h"
#include "./frame.h"
#include "./detectLine.h"
#include "./algorithm.h"
#include "./markingLine.h"
#include <time.h>
#include <stdio.h>
EXPORT PixelPoint* RBF(char* path)
{

    clock_t start=clock();
    PixelPoint* targetImg=NULL;
    PixelImage* image=NULL;
    RemoveBorderFunc(path,&targetImg,&image);
    //FreePoint(&targetImg);
    FreeImage(&image);

    clock_t end=clock();
    printf("./main.c/RemoveBorderFunc:\n-----c program RemoveBorderFunc useing time is  %lfs -----\n",(double) (end-start)/CLOCKS_PER_SEC);
    return targetImg;
}
EXPORT PixelPoint* RLF(char* path)
{
    clock_t start=clock();

    PixelPoint* targetImg=NULL;
    PixelImage* image=NULL;
    RemoveBorderFunc(path,&targetImg,&image);
    RemoveLineHandle(targetImg);

    FreeImage(&image);
    //FreePoint(&targetImg);6 

    clock_t end=clock();
    
    printf("./main.c/RemoveLineFunc:\n-----c program RemoveLineFunc useing time is  %lfs -----\n",(double) (end-start)/CLOCKS_PER_SEC);
    return targetImg;
}
EXPORT PixelPoint* SLF(char* path,uint32_t* linepiexlNum)
{
    clock_t start=clock();
    PixelPoint* targetImg=NULL;
    PixelPoint* p=NULL;
    PixelImage* image=NULL;
    LineType line;
    uint16_t numLine=0;
    RemoveBorderFunc(path,&targetImg,&image);
    p=LineHandle(targetImg,&line);
    *linepiexlNum=targetImg->size_;
    FreeImage(&image);
    FreePoint(&targetImg);
    //FreePoint(&p);
    FreeLine(line.horLine_);
    FreeLine(line.verLine_);
    clock_t end=clock();
    
    printf("./main.c/ShowLineFunc:\n-----c program ShowLineFunc useing time is  %lfs -----\n",(double) (end-start)/CLOCKS_PER_SEC);
    return p;
}
EXPORT void FML(char* path)
{
    clock_t start=clock();
    PixelPoint* targetImg=NULL;
    PixelImage* image=NULL;
    LineType line;
    RemoveBorderFunc(path,&targetImg,&image);
    LineHandle(targetImg,&line);
    RemoveMarkLine(&line,image);


    FreeLine(line.horLine_);
    FreeLine(line.verLine_);
}
int main()
{
    PixelPoint* p1=NULL;
    p1=RBF("../photo/asdasdasdasdas.jpg");
    printf("%d\n",p1->size_);

    FreePoint(&p1);
    

    return 0;
}  