//
// Created by Dio Joyce on 2018/11/8.
//

#include "simple_free.h"
extern void* first_block;
/*
 * 释放块
 * 将free设置为1（表示空闲）
 * 如果当前块是最后一个块(，则回退break指针释放内存
 */
void _free(void *p) {
    /* p是数据区首地址，需要减去meta区的大小才是t_block的首地址 */
    t_block block = (t_block)(p - META_BLOCK_SIZE);
    /* 设置为空闲 */
    block->free = 1;
    /* 前面那个块也是空闲的，则合并两个块 */
    if(block->prev && block->prev->free)
        block = fusion(block->prev);
    /* 合并后面那个块 */
    if(block->next)
        fusion(block);
    else {  /* 后面没有块了，说明当前块是链表末尾的块 */
        /*
         * 前面还有块，则从链表中删除当前块
         */
        if(block->prev)
            block->prev->next = NULL;
        else    /* 前面也没有块了，说明当前块是最后一个块，所有块都被释放，设置fist_block为NULL */
            first_block = NULL;
        /*
         * 释放内存
         */
        brk(block);
    }
}

/*
 * 如果当前块和它后面那个块都是空闲的，则合并这两个空闲块
 */
t_block fusion(t_block block) {
    if (block->next && block->next->free) {
        /*
         * 块的大小会增加元数据区大小和后面那个块的大小
         * 两个块合并后只需要一个元数据区，因此第二个块的元数据区会作为合并后的块的数据区
         */
        block->size += META_BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        if(block->next)
            block->next->prev = block;
    }
    return block;
}
