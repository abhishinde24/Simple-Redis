#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "hashtable.h"

typedef bool (*CompareNodes)(HNode *, HNode *);

void h_init(HTable *htab, size_t n) {
    assert(n > 0 && ((n - 1) & n) == 0);
    htab->tab = (HNode **)calloc(sizeof(HNode *), n);
    htab->mask = n - 1;
    htab->size = 0;
}

// hash table inserting -- need to implement key is already present or not
void ht_insert(HTable *htable, HNode *node){

    size_t pos = htable->mask & node->hcode;
    HNode* next = htable->tab[pos];
    node->next = next;
    htable->tab[pos] = node;
    htable++;
}

HNode **ht_lookup(HTable *htable, HNode *key,CompareNodes cmp){
    size_t pos = key->hcode & htable->mask;
    HNode **from = &htable->tab[pos];
    while (*from) {
        if (cmp(*from, key)) {
            return from;
        }
        from = &(*from)->next;
    }
    return NULL;
}

HNode *h_detach(HTable *htab, HNode **from) {
    HNode *node = *from;
    *from = (*from)->next;
    htab->size--;
    return node;
}

HNode *ht_pop(HTable *htable, HNode *key,CompareNodes cmp){
HNode **from = ht_lookup(htable,key,cmp);     
   if( from ){
      return h_detach(htable,from);
   }  
   return NULL;
};


void hm_destroy(HTable *htable) {
    assert(htable->size == 0);
    free(htable);
}