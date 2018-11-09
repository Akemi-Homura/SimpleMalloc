//
// Created by Dio Joyce on 2018/11/8.
//

#ifndef SIMPLEMALLOC_SIMPLE_FREE_H
#define SIMPLEMALLOC_SIMPLE_FREE_H

#include "simple_malloc.h"

/*
 * 如果当前块和它后面那个块都是空闲的，则合并这两个空闲块
 */
t_block fusion(t_block block);
void _free(void *p);

#endif //SIMPLEMALLOC_SIMPLE_FREE_H
