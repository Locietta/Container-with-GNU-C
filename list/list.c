/**
 * @file list.c
 * @author Locietta
 * @brief list.h的实现
 * @details 带头双向链表以及简易迭代器
 * ----------------------------------------------------
 * - 循环链表 -> 带头双向链表
 * - 把(制造各种bug的)柔性数组成员模式换成了void *模式
 * @bug
 *
 * @version 0.2
 * @date 2020-06-07
 *
 *
 */

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// 双向链表节点
typedef struct __Node {
    __List prev, next;
    void *data;
} __Node;

/// 简易迭代器
inline iterator next_iterator(iterator it) {
    return it->next;
}

inline iterator prev_iterator(iterator it) {
    return it->prev;
}

iterator advance_iterator(iterator it, int step) {
    if (step > 0) {
        for (int i = 0; i < step; ++i) {
            it = it->next;
        }
    } else if (step < 0) {
        for (int i = 0; i > step; --i) {
            it = it->prev;
        }
    }
    return it;
}

void *__dereference_iterator(iterator it) {
    return it->data;
}

/// 释放节点
#define freeNode(node)                                                                   \
    do {                                                                                 \
        free(node->data);                                                                \
        free(node);                                                                      \
    } while (0)

__List head_init(void) {
    __List head = malloc(sizeof(__Node));
    head->next = head->prev = head;
    return head;
}

/// 链表函数实现
const static void *listFront(__List head) {
    return head->next->data;
}

const static void *listBack(__List head) {
    return head->prev->data;
}

static iterator listInsert(__list_privateParTable privateTable, iterator pos,
                           void *newdata) {
    if (newdata == NULL) return NULL;

    __Node *newNode = (__Node *) malloc(sizeof(__Node));
    newNode->data = malloc(privateTable->elemSize);
    memmove(newNode->data, newdata, privateTable->elemSize);
    newNode->next = pos;
    newNode->prev = pos->prev;
    newNode->prev->next = newNode;
    newNode->next->prev = newNode;
    ++privateTable->size;

    return newNode;
}

static list listTraverse(__list_privateParTable privateTable, void (*todo)(void *data)) {
    if (privateTable->size) {
        iterator cur = privateTable->head;
        for (int i = 0; i < privateTable->size; ++i) {
            todo(cur->data);
        }
        cur = cur->next;
    }
    return privateTable->this;
}

static list listDestory(__list_privateParTable privateTable) {
    while (privateTable->head->prev != privateTable->head) {
        __Node *temp = privateTable->head->prev;
        privateTable->head->prev = privateTable->head->prev->prev;
        privateTable->head->prev->next = privateTable->head;
        freeNode(temp);
    }
    free(privateTable->head);
    privateTable->size = -1;
    return privateTable->this;
}

// 只读全局变量,用作方法接口
const list_private list_methodTable = {
    .front = listFront,
    .back = listBack,
    .insert = listInsert,
    .traverse = listTraverse,
    .destory = listDestory,
};

