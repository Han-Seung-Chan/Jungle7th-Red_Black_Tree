#include "rbtree.h"

#include <stdlib.h>

/**
 * @brief 레드-블랙 트리를 생성하는 함수
 *
 * @return rbtree* 생성한 레드-블랙 트리 반환
 */
rbtree *new_rbtree(void)
{
  // rb tree 구조체 생성
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // nil_node 생성
  node_t *nilNode = (node_t *)calloc(1, sizeof(node_t)); // 1 = 할당하고자 하는 객체의 개수, sizeof(node_t) = 각 객체의 크기

  // nilNode 초기화 (복잡 리터럴)
  *nilNode = (node_t){.color = RBTREE_BLACK, .key = -1, .parent = NULL, .left = NULL, .right = NULL};

  p->nil = nilNode;  // nilNode 붙여주기
  p->root = nilNode; // tree가 비어있을 경우 root는 nil

  return p;
}

/**
 * @brief 주어진 기준 노드를 바탕으로 트리를 왼쪽으로 회전시키는 함수
 *
 * @param t 대상이 되는 레드-블랙 트리
 * @param standardNode 회전 기준 노드
 */
void left_rotate(rbtree *t, node_t *standardNode)
{
  node_t *y = standardNode->right; // y를 기준 노드의 오른쪽 자식으로 설정

  standardNode->right = y->left; // y를 왼쪽 서브트리를 기준 노드의 오른쪽 서브트리로 변경

  // y의 왼쪽 자식이 존재한다면, 그 부모를 기준 노드로 변경
  if (y->left != NULL)
    y->left->parent = standardNode;

  y->parent = standardNode->parent; // y의 부모를 기준 노드의 부모로 변경

  // 기준 노드가 루트였다면, y를 새로운 루트로 설정
  if (standardNode->parent == NULL)
    t->root = y;

  // 기준 노드가 부모의 왼쪽 자식이었다면, y를 기준 노드 부모의 왼쪽 자식으로 설정
  else if (standardNode == standardNode->parent->left)
    standardNode->parent->left = y;

  // 기준 노드가 부모의 오른쪽 자식이었다면, y를 기준 노드 부모의 오른쪽 자식으로 설정
  else
    standardNode->parent->right = y;

  y->left = standardNode;   // y의 왼쪽 자식을 기준 노드로 설정
  standardNode->parent = y; // 기준 노드의 부모를 y로 설정
}

/**
 * @brief 주어진 기준 노드를 바탕으로 트리를 오른쪽으로 회전시키는 함수
 *
 * @param t 대상이 되는 레드-블랙 트리
 * @param standardNode 회전 기준 노드
 */
void right_rotate(rbtree *t, node_t *standardNode)
{
  // x를 기준 노드의 왼쪽 자식으로 설정
  node_t *x = standardNode->left;

  // x의 오른쪽 서브트리를 기준 노드의 왼쪽 서브트리로 변경
  standardNode->left = x->right;

  // x의 오른쪽 자식이 존재한다면, 그 부모를 기준 노드로 변경
  if (x->right != NULL)
    x->right->parent = standardNode;

  // x의 부모를 기준 노드의 부모로 변경
  x->parent = standardNode->parent;

  // 기준 노드가 루트였다면, x를 새로운 루트로 설정
  if (standardNode->parent == NULL)
    t->root = x;
  // 기준 노드가 부모의 오른쪽 자식이었다면, x를 기준 노드 부모의 오른쪽 자식으로 설정
  else if (standardNode == standardNode->parent->right)
    standardNode->parent->right = x;
  // 기준 노드가 부모의 왼쪽 자식이었다면, x를 기준 노드 부모의 왼쪽 자식으로 설정
  else
    standardNode->parent->left = x;

  // x의 오른쪽 자식을 기준 노드로 설정
  x->right = standardNode;

  // 기준 노드의 부모를 x로 설정
  standardNode->parent = x;
}

/**
 * @brief 레드-블랙 트리에 노드 삽입 후 재조정하는 함수
 *
 * @param t 대상이 되는 레드-블랙 트리
 * @param targetNode 재조정이 필요한 노드
 */
void *rbtree_insert_fixup(rbtree *t, node_t *targetNode)
{
  // 타겟 노드의 부모가 빨간색인 경우에만 재조정 수행 (레드-블랙 트리의 속성 4 위반)
  while (targetNode->parent->color == RBTREE_RED)
  {
    node_t *grandParent = targetNode->parent->parent;
    if (targetNode->parent == grandParent->left) // 부모가 할아버지의 왼쪽 자식일 때
    {
      node_t *uncle = grandParent->right;

      // Case 1 : 삼촌노드가 빨간색 색상인 경우
      if (uncle->color == RBTREE_RED)
      {
        // 부모와 삼촌을 검은색으로, 할아버지를 빨간색으로 변경
        targetNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        grandParent->color = RBTREE_RED;
        // 할아버지 노드를 새로운 타겟으로 설정하여 위로 올라가며 계속 검사
        targetNode = grandParent;
      }
      else // 삼촌 노드가 검은색인 경우
      {
        // Case 2: 타겟 노드가 부모의 오른쪽 자식인 경우
        if (targetNode == targetNode->parent->right)
        {
          // 부모를 새로운 타겟으로 설정하고 왼쪽으로 회전
          targetNode = targetNode->parent;
          left_rotate(t, targetNode);
        }

        // Case 3: 타겟 노드가 부모의 왼쪽 자식인 경우
        if (targetNode == targetNode->parent->left)
        {
          // 부모를 검은색으로, 할아버지를 빨간색으로 변경하고 오른쪽으로 회전
          targetNode->parent->color = RBTREE_BLACK;
          grandParent->color = RBTREE_RED;
          right_rotate(t, grandParent);
        }
      }
    }
    else // 부모가 할아버지의 오른쪽 자식일 때
    {
      node_t *uncle = grandParent->left;

      // Case 1: 삼촌 노드가 빨간색인 경우
      if (uncle->color == RBTREE_RED)
      {
        // 할아버지를 빨간색으로, 부모와 삼촌을 검은색으로 변경
        grandParent->color = RBTREE_RED;
        targetNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        // 할아버지 노드를 새로운 타겟으로 설정
        targetNode = grandParent;
      }
      else // 삼촌 노드가 검은색인 경우
      {
        // Case 2: 타겟 노드가 부모의 왼쪽 자식인 경우
        if (targetNode == targetNode->parent->left)
        {
          // 부모를 새로운 타겟으로 설정하고 오른쪽으로 회전
          targetNode = targetNode->parent;
          right_rotate(t, targetNode);
        }

        // Case 3: 타겟 노드가 부모의 오른쪽 자식인 경우
        if (targetNode == targetNode->parent->right)
        {
          // 부모를 검은색으로, 할아버지를 빨간색으로 변경하고 왼쪽으로 회전
          targetNode->parent->color = RBTREE_BLACK;
          grandParent->color = RBTREE_RED;
          left_rotate(t, grandParent);
        }
      }
    }
  }
  // 루트 노드를 검은색으로 설정 (레드-블랙 트리의 속성 2)
  t->root->color = RBTREE_BLACK;
}

/**
 * @brief RB 트리에 주어진 key 값을 가진 노드를 삽입하는 함수
 *
 * @param t key를 삽입할 RB 트리
 * @param key 삽입하고자 하는 key 값
 * @return node_t* 삽입한 노드 반환
 */
node_t *rbtree_insert(rbtree *t, const key_t key)
{
  node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
  *newNode = (node_t){.color = RBTREE_RED, .key = 1, .left = NULL, .right = NULL};

  node_t *parent = t->nil;
  node_t *cur = t->root;

  while (cur != t->nil)
  {
    parent = cur;
    if (cur->key >= key)
      cur = cur->left;
    else
      cur = cur->right;
  }

  newNode->parent = parent;

  if (parent == t->nil)
    t->root = newNode;
  else if (parent->key >= key)
    parent->left = newNode;
  else
    parent->right = newNode;

  rbtree_insert_fixup(t, newNode);
  return newNode;
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
