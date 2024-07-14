#ifndef _LIBRA_H_
#define _LIBRA_H_

#include "linux/mm_types.h"
#include "linux/types.h"
#include <linux/list.h>
#include <linux/mm.h>

extern spinlock_t popl_list_lock;
extern struct list_head popl_list;
#define PO_HASH_SIZE 64
#define O_HASH_SIZE 32

enum popl_flags {
	P_NO_SPLIT,
	P_ALREADY_SPLIT,
	P_NEED_SPLIT,
	P_NEED_COLLAPSE,
};

typedef struct population_node {
	struct hlist_node phash_node;
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	/*dedup*/
	unsigned long addr;
	int this_share;
	int all_share;
	int cows;
	/*sample*/
	bool already_meet;
	int last_istart; //sample move
	int hash_active;
	int zeros;
	int init_zeros;
	int huge_age;
	int small_age;
	unsigned int flags;
	unsigned int list_flag;
    u32 hash_array[O_HASH_SIZE]; // 添加哈希数组
	struct list_head popl_l;
} popl_node_t;

popl_node_t *libra_popl_node_lookup(struct mm_struct *mm,
		unsigned long address);
void libra_popl_node_insert(struct mm_struct *mm, 
		unsigned long address,
		popl_node_t *node);
void libra_popl_node_delete(struct mm_struct *mm, 
		unsigned long address);
void init_population_node(popl_node_t *node);

popl_node_t *libra_popltable_node_alloc(void);
void libra_popltable_node_free(popl_node_t *node);
void libra_clear_popltable_range(struct mm_struct *mm, 
		unsigned long start, unsigned long end); 
#endif 