//
// Created by Dio Joyce on 2018/11/8.
//

# include "simple_malloc.h"

void *first_block = NULL; /* 第一个块 */
/*
 * 增加块
 * 如果现有的块都不能满足要求，则在链表最后开辟一个新的block
 */
t_block extend_block(t_block last, size_t size) {
    t_block block;
    /* 获得当前break指针 */
    /*
     * break指针简介
     * Linux维护一个break指针，这个指针指向堆空间的某个地址。
     * 从堆起始地址到break之间的地址空间为映射好的，可以供进程访问；而从break往上，是未映射的地址空间，如果访问这段空间则程序会报错。
     * 因此可理解为break为可访问的堆内存地址的上限
     */

    /*
     * void* sbrk(int increment)
     * sbrk将break从当前位置移动increment所指定的增量
     * sbrk成功时返回break移动之前所指向的地址，否则返回(void *)-1。
     */
    block = sbrk(0);
    /* 增加break指针 */
    if(sbrk((int)(META_BLOCK_SIZE + size)) == (void *)-1)
        return NULL;
    block->size = size;
    block->next = NULL;
    /*
     * 在链表尾部增加一个块
     */
    if(last)
        last->next = block;
    block->free = 0;
    return block;
}

/*
 * 分裂块
 * 为了防止出现很小的size占据很大的一块block，在剩余数据区足够大的情况下，将其分裂为一个新的block
 */
void split_block(t_block origin_block, size_t size) {
    /*
     * 原block分裂后的大小为size
     * new_block（新block） 的大小为原block的size - size（原block剩下的大小） - META_BLOCK_SIZE (s_block元数据区的大小）
     */
    t_block new_block;
    new_block = (t_block )origin_block->data + size;
    new_block->size = origin_block->size - size - META_BLOCK_SIZE ;
    new_block->next = origin_block->next;
    /*
     * 新block 设置为空闲
     */
    new_block->free = 1;
    origin_block->size = size;
    origin_block->next = new_block;
}

/*
 * 查找block。
 * 从头开始查找，找到第一个块空闲且大小大于要求size的块
 */
t_block find_block(t_block *last, size_t size) {
    t_block block = first_block;
    /*
     * 从frist_block开始，查找第一个符合要求的block并返回block起始地址，如果找不到这返回NULL。
     * 这里在遍历时会更新一个叫last的指针，这个指针始终指向当前遍历的block。这是为了如果找不到合适的block而开辟新block使用的
     */
    while(block && !(block->free && block->size >= size)) {
        *last = block;
        block = block->next;
    }
    return block;
}

/*
 * 使size对齐8字节
 * 当size不是8的倍数，增大size使它满足是8的倍数
 */
size_t align8bit(size_t size) {
    /*
     * 位运算，如果 and 7 （二进制为111）结果为0，说明是8的倍数
     */
    if((size & 0x7) == 0)
        return size;
    /*
     * 相当于 (size / 8 + 1) * 8
     */
    return ((size >> 3) + 1) << 3;
}

/*
 * _malloc 实现思路
 * 维护一个s_block的链表，表示已分配的块。
 * 需要分配内存地址时，先在这个链表中查找大小足够并且空闲的块，如果有，则返回这个块的数据区的首地址
 * 如果找不到，则在链表尾部新增一个块，并把这个块的数据区首地址返回。
 */
void *_malloc(size_t size) {
    t_block block, last;
    size_t s;
    /* 对齐地址 */
    s = align8bit(size);
    if(first_block) {
        /* 查找合适的block */
        last = first_block;
        block = find_block(&last, s);
        if(block) {
            /* 如果可以，则分裂 */
            if ((block->size - s) >= ( META_BLOCK_SIZE + 8))
                split_block(block, s);
            block->free = 0;
        } else {
            /* 没有合适的block，开辟一个新的 */
            block = extend_block(last, s);
            if(!block)
                return NULL;
        }
    } else {
        /* 如果第一个block为NULL，则新开辟一个块 ，并且设置为first_block */
        block = extend_block(NULL, s);
        if(!block)
            return NULL;
        first_block = block;
    }
    return block->data;
}