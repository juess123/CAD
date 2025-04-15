#ifndef DETECTLINE_H
#define DETECTLINE_H
#include "./algorithm.h"
#define VERTICAL 1
#define HORIZONTAL 0
PixelPoint* LineHandle(PixelPoint* targetImg,LineType* line);
void RemoveLineHandle(PixelPoint* targetImg);


#endif