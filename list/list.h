/**
 * @file list.h
 * @author Locietta (https://github.com/Locietta)
 * @brief 抽象链表接口(仅支持GNU GCC)
 * @details 模仿STL的list容器写的接口(注意:使用了GUN扩展)
 * @version 0.2
 * @date 2020-06-07
 *  last modified 2020-06-24
 */

#ifndef LIST_H
#define LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "lambda.h"

/* Public: interface */

#define __deref(type, void_ptr) (*(type *) void_ptr) //*it

/** 简易迭代器 @{ */

typedef struct __Node *iterator;

iterator next_iterator(iterator it); // it++;

iterator prev_iterator(iterator it); // it--;

iterator advance_iterator(iterator it, int step); // advance(it, step);

#define deref_iterator(type, it) (*(type *) __dereference_iterator(it))

/** @} */

/// 容器定义
typedef struct list list;
struct list {
    // Private:
    // List head;
    // int size;
    // size_t elemSize;
    // int (*cmp)(void *data1, void *data2);

    // Public:
    int (*size)(void);
    bool (*empty)(void);
    iterator (*begin)(void);
    iterator (*end)(void);
    const void *(*front)(void);
    const void *(*back)(void);
    iterator (*insert)(iterator pos, void *newdata);
    list (*traverse)(void (*todo)(void *data));

    list (*destory)(void);
};

/// 构造
#define newlist(type) (newList_private(type))


/* Private: implementations */
// 你就装作下面的东西都不存在（

extern void *__dereference_iterator(iterator it);

typedef struct __Node *__List;

typedef struct list_privateParTable {
    list this;

    __List head;
    int size;
    size_t elemSize;
    int (*cmp)(void *data1, void *data2);

} * __list_privateParTable, list_privateParTable;

extern __List head_init(void);

/// 方法实现接口
typedef struct list_private_info list_private;
struct list_private_info {

    const void *(*front)(__List head);
    const void *(*back)(__List head);
    iterator (*insert)(__list_privateParTable privateTable, iterator pos, void *newdata);
    list (*traverse)(__list_privateParTable privateTable, void (*todo)(void *data));

    list (*destory)(__list_privateParTable privateTable);
};

extern const list_private list_methodTable;

#define newList_private(type)                                                                     \
    ({                                                                                   \
        list_privateParTable privateTable = {                                            \
            .size = 0,                                                                   \
            .elemSize = sizeof(type),                                                       \
            .cmp = NULL,                                                                 \
        };                                                                               \
        privateTable.head = head_init();                                                 \
        privateTable.this = (list){                                                      \
            .size = lambda(                                                              \
                int, (void) { return privateTable.size; }),                              \
            .empty = lambda(                                                             \
                bool, (void) { return !privateTable.size; }),                            \
            .begin = lambda(                                                             \
                iterator, (void) { return next_iterator(privateTable.head); }),          \
            .end = lambda(                                                               \
                iterator, (void) { return prev_iterator(privateTable.head); }),          \
            .front = lambda(                                                             \
                const void *,                                                            \
                (void) { return list_methodTable.front(privateTable.head); }),           \
            .back = lambda(                                                              \
                const void *,                                                            \
                (void) { return list_methodTable.back(privateTable.head); }),            \
            .insert = lambda(                                                            \
                iterator,                                                                \
                (iterator pos, void *newdata) {                                          \
                    return list_methodTable.insert(&privateTable, pos, newdata);         \
                }),                                                                      \
            .traverse = lambda(                                                          \
                list,                                                                    \
                (void(*todo)(void *data)) {                                              \
                    return list_methodTable.traverse(&privateTable, todo);               \
                }),                                                                      \
            .destory = lambda(                                                           \
                list, (void) { return list_methodTable.destory(&privateTable); }),   \
        };                                                                               \
        privateTable.this;                                                               \
    })

#endif // (!defined LIST_H)