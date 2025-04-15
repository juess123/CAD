#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <stdint.h>
typedef struct Coordinate
{
    uint16_t x_;
    uint16_t y_;
}Coordinate;
typedef struct PixelPoint
{
    Coordinate* co_;
    uint32_t size_;
    uint32_t capacity_;
}PixelPoint;
typedef struct PixelImage
{
    Coordinate property_;
    uint8_t** image_;
}PixelImage;
typedef struct Line
{
    char type_;
    uint16_t length_;
    Coordinate start_;
    Coordinate end_; 
    struct Line* next_;
    struct Line* pre_;
}Line;
typedef struct LineType
{
    Line* verLine_;
    Line* horLine_;
}LineType;

void FreeImage(PixelImage** P);
void CreateImage(PixelImage** PI,uint16_t width,uint16_t height);
void CreatePoint(PixelPoint** PP,uint32_t capacity);
void EnsureCapacity(PixelPoint* PP);
void FreePoint(PixelPoint** PP);
void CreateNode(Coordinate start,Coordinate end,char type,uint16_t length,Line* L);
void FreeLine(Line* L);
void DeleteLineNode(Line* node,Line* head);
#endif