#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  // rb tree 구조체 생성
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // nil_node 생성
  node_t *nil_node = (node_t *)calloc(1, sizeof(node_t)); // 1 = 할당하고자 하는 객체의 개수, sizeof(node_t) = 각 객체의 크기

  // nil_node 초기화 (복잡 리터럴)
  *nil_node = (node_t){.color = RBTREE_BLACK, .key = -1, .parent = NULL, .left = NULL, .right = NULL};

  p->nil = nil_node;  // nil_node 붙여주기
  p->root = nil_node; // tree가 비어있을 경우 root는 nil

  return p;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}
