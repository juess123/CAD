#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "./stb_image.h"
#include "./stb_image_write.h"
#include "./frame.h"
#include <stdio.h>
#include <stdint.h>
uint32_t BFS(PixelImage* pixelImg,Coordinate* startPoint,PixelImage* bound)
{
    PixelPoint* p=NULL;
    CreatePoint(&p,40000);
    uint32_t front = 0, rear = 0;  
    uint32_t result_count=0;
    p->co_[rear].x_ = startPoint->x_;
    p->co_[rear].y_ = startPoint->y_;
    rear++;
    bound->image_[startPoint->x_][startPoint->y_] = 1;  
    int8_t directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    while (front < rear) {
        uint16_t x = p->co_[front].x_;
        uint16_t y = p->co_[front].y_;
        front++;
        p->size_++;
        for (uint8_t i = 0; i < 4; i++) {
            int nx = x + directions[i][0];
            int ny = y + directions[i][1];
            if (nx >= 0 && ny >= 0 && nx < pixelImg->property_.x_ && ny < pixelImg->property_.y_ && !bound->image_[nx][ny] && pixelImg->image_[nx][ny] == 0)
            {
                if (rear >= p->capacity_)
                {
                    EnsureCapacity(p);
                }
                bound->image_[nx][ny] = 1;  
                p->co_[rear].x_ = nx;
                p->co_[rear].y_ = ny;
                rear++;
            }
        }
    }
    printf("./frame.c/BFS:\n-----BFS of funtion use queue size is %u,set QUEUESIZE size is %u -----\n",rear,p->capacity_);
    uint32_t findPiexlNum=p->size_;
    FreePoint(&p);
    return findPiexlNum;
}
void EdgeBoundBoxCo(PixelImage* bound,PixelPoint* edgeBound)
{
    for(uint16_t x=bound->property_.x_/2; x<bound->property_.x_; x++)
    {
        for(uint16_t y=bound->property_.y_/2;y>0;y--)
        {
            if(bound->image_[x][y]==1 && y>0)
            {
                edgeBound->co_[edgeBound->size_].x_=x;
                edgeBound->co_[edgeBound->size_].y_=y;
                edgeBound->size_++;
                EnsureCapacity(edgeBound);
                break;
            }
            
        }
        if(edgeBound->size_>0 && edgeBound->co_[edgeBound->size_-1].y_==bound->property_.y_/2)
        {
            edgeBound->size_--;
            break;
        }
        for(uint16_t y=bound->property_.y_/2+1; y<bound->property_.y_ ; y++)
        {
            
            if(bound->image_[x][y]==1 && y<bound->property_.y_)
            {
                edgeBound->co_[edgeBound->size_].x_=x;
                edgeBound->co_[edgeBound->size_].y_=y;
                edgeBound->size_++;
                EnsureCapacity(edgeBound);
                break;
            }
            
        }
        //if(edgeBound->size_>EDGECOORDINATESIZE-1000)printf("EDGECOORDINATESIZE overflow\n");
    }

    for(uint16_t x=bound->property_.x_/2-1;x>0;x--)
    {
        for(uint16_t y=bound->property_.y_/2;y>0;y--)
        {
            if(bound->image_[x][y]==1)
            {
                edgeBound->co_[edgeBound->size_].x_=x;
                edgeBound->co_[edgeBound->size_].y_=y;
                edgeBound->size_++;
                EnsureCapacity(edgeBound);
                break;
            }
            
        }
        if(edgeBound->size_>0 && edgeBound->co_[edgeBound->size_-1].y_==bound->property_.y_/2)
        {
            edgeBound->size_--;
            break;
        }
        for(uint16_t y=bound->property_.y_/2+1;y<bound->property_.y_;y++)
        {
            if(bound->image_[x][y]==1)
            {
                edgeBound->co_[edgeBound->size_].x_=x;
                edgeBound->co_[edgeBound->size_].y_=y;
                edgeBound->size_++;
                EnsureCapacity(edgeBound);
                break;
            }
        }
    }
    printf("./frame.c/BoundingBoxCo:\n-----edgeBoundBoxCoSize is %u,set EDGECOORDINATESIZE size is %u -----\n",edgeBound->size_,edgeBound->capacity_);
}
uint32_t TargetImage(PixelImage* pixelImg,PixelPoint* RBImg,PixelPoint* edgebound)
{
    uint16_t starty1=0,starty2=0,fixedPointx=0;
    for(int i=0;i<edgebound->size_;)
    {
        fixedPointx=edgebound->co_[i].x_;
        
        starty1=edgebound->co_[i++].y_;
        starty2=edgebound->co_[i++].y_;
        for(uint16_t y=starty1+1;y<starty2;y++)
        {
            if(pixelImg->image_[fixedPointx][y]==0)
            {
                RBImg->co_[RBImg->size_].x_=fixedPointx;
                RBImg->co_[RBImg->size_].y_=y;
                RBImg->size_++;
                EnsureCapacity(RBImg);
            }
        }
    }
    printf("./frame.c/TargetImage:\n-----Final number of pixels %u,Set FINALNUMBEROFPIXELS number is %u -----\n",RBImg->size_,RBImg->capacity_);
    return RBImg->size_;
    
}
void FindStartingPointOfBFS(Coordinate* startPoint,PixelImage* pixelImg)
{

    for(uint16_t y=startPoint->y_+3;y<pixelImg->property_.y_-3;y++)
    {
        for(uint16_t x=startPoint->x_+3;x<pixelImg->property_.x_-3;x++)
        {
            if(pixelImg->image_[x][y]==0)
            {
                printf("./frame.c/FindStartingPointOfBFS:\n-----(%d,%d)is the starting point coordinates of BFS traversal-----\n",x,y);
                //printf("%d\n",pixelImg->image_[8][8]);
                startPoint->x_=x;
                startPoint->y_=y;
                return;
            }
        }
    }
    printf("No coordinates found\n");

}
void BinarizeImage(uint8_t *img_data, uint16_t threshold, PixelImage* pixelimg, uint8_t channels)
{
    for (int y = 0; y < pixelimg->property_.y_; y++) {
        for (int x = 0; x < pixelimg->property_.x_; x++) {
            int index = (y * pixelimg->property_.x_ + x) * channels; // 根据通道数调整索引

            uint16_t gray = 0;
            
            if (channels == 1) { 
                // 灰度图，直接使用像素值
                gray = img_data[index];
            } 
            else if (channels == 3) { 
                // RGB图，使用平均法计算灰度值
                uint8_t r = img_data[index];
                uint8_t g = img_data[index + 1];
                uint8_t b = img_data[index + 2];
                gray = (r + g + b) / 3;
            } 
            else if (channels == 4) { 
                // RGBA图，忽略 Alpha 通道
                uint8_t r = img_data[index];
                uint8_t g = img_data[index + 1];
                uint8_t b = img_data[index + 2];
                gray = (r + g + b) / 3;
                // 如果需要考虑 Alpha 通道，可以加权计算
                // uint8_t a = img_data[index + 3];
                // gray = (r + g + b) * (a / 255.0);
            } 
            else { 
                printf("Unsupported image format with %d channels!\n", channels);
                return;
            }
            pixelimg->image_[x][y] = (gray >= threshold) ? 255 : 0;
        }
    }
}
void WriteImageFunc(PixelImage* image,PixelPoint* point)
{
    for (int x = 0;x<image->property_.x_; x++)
    {
        for(int y=0;y<image->property_.y_;y++)
        {
            if(image->image_[x][y]==0)
            {
                point->co_[point->size_].x_=x;
                point->co_[point->size_++].y_=y;
                EnsureCapacity(point);
            }
        }
    }
    printf("./frame.c/WriteImageFunc:\n-----Final number of pixels %u,Set FINALNUMBEROFPIXELS number is %u -----\n",point->size_,point->capacity_);
}
void RemoveBorderFunc(const char* imagePath,PixelPoint** targetImg,PixelImage** image)
{
    printf("image path: %s\n",imagePath);
    PixelImage *img=NULL,*bound=NULL;
    PixelPoint *edgeBound,*RBImg;
    int width=0,height=0,channels=0,threshold=180;
    
    uint8_t* imageData=stbi_load(imagePath, &width, &height, &channels, 0);
    if (imageData == NULL)
    {
        printf("Failed to load image,The image does not exist or the path is incorrect\n");
        return;
    }
    CreateImage(&img,(uint16_t)width,(uint16_t)height);
    
    
    BinarizeImage(imageData,threshold,img,channels);

    stbi_image_free(imageData);//free imageData space
    
    Coordinate* startPoint=calloc(1,sizeof(Coordinate));
    FindStartingPointOfBFS(startPoint,img);

    CreateImage(&bound,(uint16_t)width,(uint16_t)height);

    int temp=0;
    while(BFS(img,startPoint,bound)<100)
    {
        if(temp++>60000)return;
        startPoint->x_+=1;
        if(startPoint->x_>=bound->property_.x_-10)
        {
            startPoint->x_=3;
            startPoint->y_+=1;
        }
        FindStartingPointOfBFS(startPoint,img);
    }
    
    CreatePoint(&edgeBound,6000);
   
    EdgeBoundBoxCo(bound,edgeBound);

    CreatePoint(&RBImg,100000);
    
    if(TargetImage(img,RBImg,edgeBound)<1000)
    {
        WriteImageFunc(img,RBImg);
    }
    FreeImage(&bound);
    FreePoint(&edgeBound);
    free(startPoint);
    *targetImg=RBImg; 
    *image=img;
    return;
}