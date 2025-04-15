#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "./algorithm.h"
typedef struct Arrow
{ 
    Coordinate Co;
    uint8_t width;
}Arrow;
uint8_t FindLineWidth(Arrow* arrow, uint16_t length)
{
    uint8_t count[256] = {0};
    uint8_t maxWidth = 0;
    for (uint16_t i = 0; i < length; i++)
    {
        uint8_t width = arrow[i].width;
        count[width]++;
        if (count[width] > count[maxWidth])
        {
            maxWidth = width;
        }
    }
    return maxWidth;
}
PixelPoint* SearchArrows(Arrow* arr,uint16_t n,uint8_t base)
{
    int start = -1;  // 记录子序列起始索引
    uint16_t length = 0;  // 记录当前子序列的长度
    PixelPoint* p=NULL;
    CreatePoint(&p,8);
    for (uint16_t i = 0; i < n; i++)
    {
        
        if(arr[i].width > base)
        { 
            if (start == -1)
            { 
                start = i;
                length = 1;
            }
            else
            { 
                length++;
            }
        }
        else
        {
            if (length > 6 && length <20 )
            {
                p->co_[p->size_++] = arr[start].width<arr[start+length-1].width ? arr[start].Co : arr[start+length-1].Co;
                
            }
            if(length>21)
            {
                base=arr[start+length-1].width;
            }
            start = -1;
            length = 0;
        }
    }
    return p;
}
void AddMarkLine(Line* line,PixelPoint* p,Line* lineHead)
{
    if(p->size_==0)
    {
        line->type_='L';
    }
    else if(p->size_==2)
    {
        line->type_='M';
    }
    else if(p->size_>2)
    {
        uint16_t length;
        while(p->size_-- && p->size_>0)
        {
            if(p->co_[0].x_==p->co_[1].x_)
            {
                length=p->co_[p->size_].y_-p->co_[p->size_-1].y_;
            }
            else if(p->co_[0].y_==p->co_[1].y_)
            {
                length=p->co_[p->size_].x_-p->co_[p->size_-1].x_;
            }
            CreateNode(p->co_[p->size_-1],p->co_[p->size_],'M',length,lineHead);
        }
        DeleteLineNode(line,lineHead);
    }
}
uint8_t SearchHorPixel(PixelImage* img,Line* line,Line* lineHead)
{
    uint8_t up=0,down=0,exitWhileFlag=0;
    uint16_t index=0;
    Arrow* arrow=(Arrow*)malloc(sizeof(Arrow)*line->length_);
    while(index<line->length_)
    {
        up=0;
        down=0;
        while(img->image_[line->start_.x_+index][line->start_.y_-up]==0)
        {
            up++;
            if(up>10)
            {
                exitWhileFlag=1;
                up=0;
                down=0;
                break;
            }
        }
        while(img->image_[line->start_.x_+index][line->start_.y_+down+1]==0 && exitWhileFlag!=1)
        {
            down++;
            if(down>10)
            {
                down=0;
                up=0;
                break;
            }
        }
        exitWhileFlag=0;
        arrow[index].Co.x_=line->start_.x_+index;
        arrow[index].Co.y_=line->start_.y_;
        arrow[index].width= up>0 && down>0 ? up+down : 1;
        index++;
    }
    FILE* file=fopen("./asdas.txt","a");
    fprintf(file,"\n");
    for(int i=0;i<index;i++)
    {
        fprintf(file,"%d,",arrow[i].width);
    }
    
    PixelPoint* p=SearchArrows(arrow,line->length_,FindLineWidth(arrow,line->length_));
    printf("%d\n",p->size_);


    AddMarkLine(line,p,lineHead);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    FreePoint(&p);
    free(arrow);
    return 0;
    
}
void FindHorArrow(Line* horLineHead,PixelImage* img)
{
    uint8_t arrawNum=0;
    Line* line=horLineHead->next_;
    uint16_t a=horLineHead->length_;
    for(int i=0;i<a;i++)
    {
        SearchHorPixel(img,line,horLineHead);
        line=line->next_;
    }
}



uint8_t SearchVerPixel(PixelImage* img,Line* line,Line* lineHead)
{
    uint8_t left=0,right=0,exitWhileFlag=0;
    uint16_t index=0;
    Arrow* arrow=(Arrow*)malloc(sizeof(Arrow)*line->length_);
    while(index<line->length_)
    {
        left=0;
        right=0;
        while(img->image_[line->start_.x_+right+1][line->start_.y_+index]==0)
        {
            right++;
            if(right>10)
            {
                exitWhileFlag=1;
                right=0;
                left=0;
                break;
            }
        }
        while(img->image_[line->start_.x_-left-1][line->start_.y_+index]==0 && exitWhileFlag!=1)
        {
            left++;
            if(left>10)
            {
                right=0;
                left=0;
                break;
            }
        }
        exitWhileFlag=0;
        arrow[index].Co.x_=line->start_.x_;
        arrow[index].Co.y_=line->start_.y_+index;
        arrow[index].width= left>0 && right>0 ? left+right+1 : 1;
        index++;
    }
    PixelPoint* p=SearchArrows(arrow,line->length_,FindLineWidth(arrow,line->length_));
    AddMarkLine(line,p,lineHead);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    FreePoint(&p);
    free(arrow);
    return 0;
}
void FindVerArrow(Line* verLineHead,PixelImage* img)
{
    uint8_t arrawNum=0;
    Line* line=verLineHead->next_;
    uint16_t a=verLineHead->length_;
    for(int i=0;i<a;i++)
    {
        SearchVerPixel(img,line,verLineHead);
        line=line->next_;
    }
}
void RemoveMarkLine(LineType* line,PixelImage* img)
{
    Line* verLineHead=line->verLine_;
    Line* horLineHead=line->horLine_;
    FindHorArrow(horLineHead,img);
    FindVerArrow(verLineHead,img);
    Line* l=verLineHead->next_;
    
    FILE* file=fopen("sdad.txt","w");
    for(int i=0;i<verLineHead->length_;i++)
    {
        if(l->type_=='M')fprintf(file,"%d,%d,%d,%d\n",l->start_.x_,l->start_.y_,l->end_.x_,l->end_.y_);
        l=l->next_;
    }
    l=horLineHead->next_;
    for(int j=0;j<horLineHead->length_;j++)
    {
        if(l->type_=='M')fprintf(file,"%d,%d,%d,%d\n",l->start_.x_,l->start_.y_,l->end_.x_,l->end_.y_);
        l=l->next_;
    }
    fclose(file);
}