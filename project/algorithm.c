#include "./algorithm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
void FreeImage(PixelImage** PI)
{
    for(int i=0;i<(*PI)->property_.x_;i++)
    {
        free((*PI)->image_[i]);  
    }
    free((*PI)->image_);  
    free(*PI);  
    *PI=NULL;
}

void CreateImage(PixelImage** PI,uint16_t width,uint16_t height)
{
    (*PI)=(PixelImage*)malloc(sizeof(PixelImage));
    (*PI)->property_.x_=width;
    (*PI)->property_.y_=height;
    
    (*PI)->image_=(uint8_t**)calloc(width,sizeof(uint8_t*));
    
    for(int i=0;i<width;i++)
    {
        (*PI)->image_[i]=(uint8_t*)calloc(height,sizeof(uint8_t));
    }
}
void CreatePoint(PixelPoint** PP,uint32_t capacity)
{
    *PP=malloc(sizeof(PixelPoint));
    (*PP)->capacity_=capacity;
    (*PP)->co_=(Coordinate*)calloc(capacity,sizeof(Coordinate));
    (*PP)->size_=0;
}
void EnsureCapacity(PixelPoint* PP)
{
    if (PP->size_ >= (uint32_t)(0.9 * PP->capacity_))
    {
        uint32_t new_capacity = (uint32_t)(PP->capacity_ * 1.5);
        Coordinate* new_co = (Coordinate*)calloc(new_capacity, sizeof(Coordinate));
        if (!new_co) return;

        memcpy(new_co, PP->co_, PP->size_ * sizeof(Coordinate));


        free(PP->co_);
        PP->co_ = new_co;
        PP->capacity_ = new_capacity;
        printf("Capacity expansion occurs capacity is %d\n",new_capacity);
    }
}
void FreePoint(PixelPoint** PP)
{
    free((*PP)->co_);
    free(*PP);
}
void CreateNode(Coordinate start,Coordinate end,char type,uint16_t length,Line* L)
{
    if(L==NULL)return;
    Line* node=(Line*)malloc(sizeof(Line));
    node->end_=end;
    node->start_=start;
    node->length_=length;
    node->type_=type;
    if (L->next_ == NULL)
    {
        node->next_=L->next_;
        node->pre_=L;
        L->next_=node;
    }
    else
    {
        node->next_=L->next_;
        node->pre_=L;
        L->next_->pre_=node;
        L->next_=node;
    }
    L->length_++;
}
void FreeLine(Line* L)
{
    if (L == NULL) return;  // 防止空指针

    Line* current = L->next_;  // 从第一个节点开始
    while (current != NULL) 
    {
        Line* temp = current;  // 保存当前节点
        current = current->next_;  // 移动到下一个节点
        free(temp);  // 释放当前节点的内存
    }

    L->next_ = NULL;  // 清空头节点的 next 指针
    L->length_ = 0;  // 重置链表长度
    free(L);
}
void DeleteLineNode(Line* node,Line* head)
{
    if (node == NULL) return;
    if (node->pre_) {
        node->pre_->next_ = node->next_;
    }
    if (node->next_) {
        node->next_->pre_ = node->pre_;
    }
    head->length_--;
    free(node);
}

