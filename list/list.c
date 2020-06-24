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

/// 泛型交换函数, 但是不但能处理数组成员(@bug can't swap array data, or array member in a struct)
#define SWAP(a, b)                                                                                 \
    do {                                                                                           \
        typeof(a) _t = b;                                                                          \
        b = a;                                                                                     \
        a = _t;                                                                                    \
    } while (0)

/// 双向链表节点
typedef struct __Node {
    __List prev, next;
    void *data;
} __Node;

/// 释放节点
#define freeNode(node)                                                                             \
    do {                                                                                           \
        privateTable->destructor(node->data);                                                      \
        free(node);                                                                                \
    } while (0)

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

static list listPush_front(__list_privateParTable privateTable, void *newdata) {
    if (newdata != NULL) {
        __Node *newNode = (__Node *) malloc(sizeof(__Node));
        newNode->data = malloc(privateTable->elemSize);
        memmove(newNode->data, newdata, privateTable->elemSize);

        newNode->next = privateTable->head->next;
        newNode->prev = privateTable->head;
        newNode->next->prev = newNode;
        newNode->prev->next = newNode;
        ++privateTable->size;
    }
    return privateTable->this;
}

static list listPush_back(__list_privateParTable privateTable, void *newdata) {
    if (newdata != NULL) {
        __Node *newNode = (__Node *) malloc(sizeof(__Node));
        newNode->data = malloc(privateTable->elemSize);
        memmove(newNode->data, newdata, privateTable->elemSize);

        newNode->next = privateTable->head;
        newNode->prev = privateTable->head->prev;
        newNode->next->prev = newNode;
        newNode->prev->next = newNode;
        ++privateTable->size;
    }
    return privateTable->this;
}

static iterator listInsert(__list_privateParTable privateTable, iterator pos, void *newdata) {
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

static list listPop_front(__list_privateParTable privateTable) {
    if (privateTable->size) {
        __Node *temp = privateTable->head->next;
        privateTable->head->next = temp->next;
        temp->next->prev = privateTable->head;
        freeNode(temp);
        --privateTable->size;
    }

    return privateTable->this;
}

static list listPop_back(__list_privateParTable privateTable) {
    if (privateTable->size) {
        __Node *temp = privateTable->head->prev;
        privateTable->head->prev = temp->prev;
        temp->prev->next = privateTable->head;
        freeNode(temp);
        --privateTable->size;
    }

    return privateTable->this;
}

static iterator listErase(__list_privateParTable privateTable, iterator pos) {
    if (pos == privateTable->head) {
        return NULL; // try to erase head node (like list.end() node)
    } else {
        __Node *ret = pos->next;
        pos->next->prev = pos->prev;
        pos->prev->next = pos->next;
        freeNode(pos);
        --privateTable->size;
        return ret;
    }
}

static list listRemove(__list_privateParTable privateTable, void *data) {
    if (privateTable->cmp != NULL && data != NULL) {
        iterator cur = privateTable->head->next;
        while (cur != privateTable->head) {
            if (!privateTable->cmp(cur->data, data)) {
                iterator temp = cur;
                cur = cur->next;
                temp->next->prev = temp->prev;
                temp->prev->next = temp->next;
                freeNode(temp);
                --privateTable->size;
            } else {
                cur = cur->next;
            }
        }
    }
    return privateTable->this;
}

static list listRemove_if(__list_privateParTable privateTable, int (*key)(void *data)) {
    iterator cur = privateTable->head->next;
    while (cur != privateTable->head) {
        if (key(cur->data)) {
            iterator temp = cur;
            cur = cur->next;
            temp->next->prev = temp->prev;
            temp->prev->next = temp->next;
            freeNode(temp);
            --privateTable->size;
        } else {
            cur = cur->next;
        }
    }
    return privateTable->this;
}

static list listFor_each(__list_privateParTable privateTable, iterator first, iterator last,
                         void (*todo)(void *data)) {
    for (iterator it = first; it != last; it = it->next) {
        todo(it->data);
    }
    return privateTable->this;
}

static list listReverse(__list_privateParTable privateTable) {
    iterator current = privateTable->head;
    for (int i = -1; i < privateTable->size; ++i) {
        SWAP(current->next, current->prev);
        current = current->prev;
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

// 链表快排实现

static iterator partition(__list_privateParTable privateTable, iterator low, iterator high) {
    iterator i = low;
    void *pivot = low->data;
    while (low != high) {
        while (low != high && privateTable->cmp(high->data, pivot) >= 0) {
            high = high->prev;
        }
        while (low != high && privateTable->cmp(pivot, low->data) >= 0) {
            low = low->next;
        }
        if (low != high) {
            SWAP(low->data, high->data);
        }
    }
    SWAP(low->data, i->data);
    return low;
}

static void qusort(__list_privateParTable privateTable, iterator low, iterator high) {
    if (low == high) {
        return;
    }
    iterator p = partition(privateTable, low, high);
    if (low != p) {
        qusort(privateTable, low, p->prev);
    }
    if (high != p) {
        qusort(privateTable, p->next, high);
    }
}

static list listSort(__list_privateParTable privateTable) {
    if (privateTable->cmp != NULL) {
        qusort(privateTable, privateTable->head->next, privateTable->head->prev);
    }
    return privateTable->this;
}

// 只读全局变量,用作方法接口
const list_private list_methodTable = {
    .front = listFront,
    .back = listBack,
    .push_front = listPush_front,
    .push_back = listPush_back,
    .insert = listInsert,
    .pop_front = listPop_front,
    .pop_back = listPop_back,
    .erase = listErase,
    .remove = listRemove,
    .remove_if = listRemove_if,
    .sort = listSort,
    .reverse = listReverse,
    .for_each = listFor_each,
    .destory = listDestory,
};
