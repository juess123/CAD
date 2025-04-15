#ifndef FRAME_H
#define FRAME_H
#include "./algorithm.h"
#define QUEUESIZE 400000
#define EDGECOORDINATESIZE 12000
#define RBIMG 450000
void RemoveBorderFunc(const char* imagePath,PixelPoint** targetImg,PixelImage** image);
#endif