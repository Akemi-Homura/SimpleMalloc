//
// Created by Dio Joyce on 2018/11/8.
//

#ifndef SIMPLEMALLOC_SIMPLE_MALLOC_H
#define SIMPLEMALLOC_SIMPLE_MALLOC_H

#include <unistd.h>

/*
 * 内存块
 * 将堆内存空间以块（Block）的形式组织起来，
 * 每个块由meta区和数据区组成，meta区记录数据块的元信息（数据区大小、空闲标志位、指针等等），
 * 数据区是真实分配的内存区域，并且数据区的第一个字节地址即为malloc返回的地址
 */
typedef struct s_block *t_block;
struct s_block {
    size_t size;  /* 数据区大小 */
    t_block next; /* 指向下个块的指针 */
    t_block prev; /* 指向前一个块的指针 */
    int free;     /* 是否是空闲块 */
    int padding;  /* 填充4字节，保证meta块长度为8的倍数 */
    char data[1];  /* 这是一个虚拟字段，表示数据块的第一个字节，长度不应计入meta */
};

#define META_BLOCK_SIZE 32 /* 由于存在虚拟的data字段，sizeof不能正确计算meta长度，这里手工设置 */

/*
 * 使size对齐8字节
 * 当size不是8的倍数，增大size使它满足是8的倍数
 */
size_t align8bit(size_t size);

/*
 * 查找block。
 * 从头开始查找，找到第一个块空闲且大小大于要求size的块
 */
t_block find_block(t_block *last, size_t size);

/*
 * 分裂块
 * 为了防止出现很小的size占据很大的一块block，在剩余数据区足够大的情况下，将其分裂为一个新的block
 */
void split_block(t_block origin_block, size_t size);

/*
 * 增加块
 * 如果现有的块都不能满足要求，则在链表最后开辟一个新的block
 */
t_block extend_block(t_block last, size_t size);


/*
 * _malloc 实现思路
 * 维护一个s_block的链表，表示已分配的块。
 * 需要分配内存地址时，先在这个链表中查找大小足够并且空闲的块，如果有，则返回这个块的数据区的首地址
 * 如果找不到，则在链表尾部新增一个块，并把这个块的数据区首地址返回。
 */
void *_malloc(size_t size);


#endif //SIMPLEMALLOC_SIMPLE_MALLOC_H
