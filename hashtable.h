#include <stddef.h>
#include <stdint.h>

#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct HNode HNode;

// hashtable node, should be embedded into the payload
typedef struct HNode {
    HNode *next;
    uint64_t hcode;
    int32_t value;
} HNode;

typedef struct  {
    HNode** tab;
    size_t mask ;
    size_t size ;
} HTable;

HNode **ht_lookup(HTable *htable, HNode *key, bool (*cmp)(HNode *, HNode *));
void ht_insert(HTable *htable, HNode *node);
HNode *ht_pop(HTable *htable, HNode *key, bool (*cmp)(HNode *, HNode *));
void ht_destroy(HTable *htable);
void h_init(HTable *htable,size_t n);   
#endif
