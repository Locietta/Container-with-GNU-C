/**
 * @file list.h
 * @author Locietta (https://github.com/Locietta)
 * @brief 抽象链表接口(仅支持GCC)
 * @details 模仿STL的list容器写的接口(注意:使用了GUN扩展)
 * - 2020/06/24: 原来的代码丢了,又重新写了一遍(死)
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

#define __deref(type, void_ptr) (*(type *) void_ptr) 

/** 简易迭代器 @{ */

typedef struct __Node *iterator;

iterator next_iterator(iterator it); // it++;

iterator prev_iterator(iterator it); // it--;

iterator advance_iterator(iterator it, int step); // advance(it, step);

#define deref_iterator(type, it) (*(type *) __dereference_iterator(it)) //*it

/** @} */

/// 容器定义
typedef struct list list;
struct list {
    // Private:
    // List head;
    // int size;
    // size_t elemSize;
    // int (*cmp)(void *data1, void *data2);
    // void (*destructor)(void *_Block);

    // Public:
    int (*size)(void);
    bool (*empty)(void);
    list (*setcmp)(int (*cmp)(void *data1, void *data2));
    list (*setdestructor)(void (*destructor)(void *_Block));

    iterator (*begin)(void);
    iterator (*end)(void);
    const void *(*front)(void);
    const void *(*back)(void);
    list (*push_front)(void *newdata);
    list (*push_back)(void *newdata);
    iterator (*insert)(iterator pos, void *newdata);
    list (*pop_front)(void);
    list (*pop_back)(void);
    iterator (*erase)(iterator pos);
    list (*remove)(void *data);
    list (*remove_if)(int (*key)(void *data));
    list (*sort)(void); // 快排,需要先调用setcmp()
    list (*reverse)(void);
    list (*for_each)(iterator first, iterator last, void (*todo)(void *data));
    list (*traverse)(void (*todo)(void *data));

    list (*destory)(void);
};

/// 构造
#define newlist(type) (newList_private(type))

/// 析构
#define deleteList(L) (L.destory())

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
    void (*destructor)(void *_Block);

} * __list_privateParTable, list_privateParTable;

extern __List head_init(void);

/// 方法实现接口
typedef struct list_private_info list_private;
struct list_private_info {

    const void *(*front)(__List head);
    const void *(*back)(__List head);
    list (*push_front)(__list_privateParTable privateTable, void *newdata);
    list (*push_back)(__list_privateParTable privateTable, void *newdata);
    iterator (*insert)(__list_privateParTable privateTable, iterator pos, void *newdata);
    list (*pop_front)(__list_privateParTable privateTable);
    list (*pop_back)(__list_privateParTable privateTable);
    iterator (*erase)(__list_privateParTable privateTable, iterator pos);
    list (*remove)(__list_privateParTable privateTable, void *data);
    list (*remove_if)(__list_privateParTable privateTable, int (*key)(void *data));
    list (*sort)(__list_privateParTable privateTable);
    list (*reverse)(__list_privateParTable privateTable);
    list (*for_each)(__list_privateParTable privateTable, iterator first, iterator last,
                     void (*todo)(void *data));

    list (*destory)(__list_privateParTable privateTable);
};

extern const list_private list_methodTable;

#define newList_private(type)                                                                      \
    ({                                                                                             \
        list_privateParTable privateTable = {                                                      \
            .size = 0,                                                                             \
            .elemSize = sizeof(type),                                                              \
            .cmp = NULL,                                                                           \
            .destructor = free,                                                                    \
        };                                                                                         \
        privateTable.head = head_init();                                                           \
        privateTable.this = (list){                                                                \
            .size = lambda(                                                                        \
                int, (void) { return privateTable.size; }),                                        \
            .empty = lambda(                                                                       \
                bool, (void) { return !privateTable.size; }),                                      \
            .setcmp = lambda(                                                                      \
                list,                                                                              \
                (int (*cmp)(void *data1, void *data2)) {                                           \
                    privateTable.cmp = cmp;                                                        \
                    return privateTable.this;                                                      \
                }),                                                                                \
            .setdestructor = lambda(                                                               \
                list,                                                                              \
                (void (*destructor)(void *_Block)) {                                               \
                    privateTable.destructor = destructor;                                          \
                    return privateTable.this;                                                      \
                }),                                                                                \
            .begin = lambda(                                                                       \
                iterator, (void) { return next_iterator(privateTable.head); }),                    \
            .end = lambda(                                                                         \
                iterator, (void) { return privateTable.head; }),                                   \
            .front = lambda(                                                                       \
                const void *, (void) { return list_methodTable.front(privateTable.head); }),       \
            .back = lambda(                                                                        \
                const void *, (void) { return list_methodTable.back(privateTable.head); }),        \
            .push_front = lambda(                                                                  \
                list,                                                                              \
                (void *newdata) { return list_methodTable.push_front(&privateTable, newdata); }),  \
            .push_back = lambda(                                                                   \
                list,                                                                              \
                (void *newdata) { return list_methodTable.push_back(&privateTable, newdata); }),   \
            .insert = lambda(                                                                      \
                iterator,                                                                          \
                (iterator pos, void *newdata) {                                                    \
                    return list_methodTable.insert(&privateTable, pos, newdata);                   \
                }),                                                                                \
            .pop_front = lambda(                                                                   \
                list, (void) { return list_methodTable.pop_front(&privateTable); }),               \
            .pop_back = lambda(                                                                    \
                list, (void) { return list_methodTable.pop_back(&privateTable); }),                \
            .erase = lambda(                                                                       \
                iterator, (iterator pos) { return list_methodTable.erase(&privateTable, pos); }),  \
            .remove = lambda(                                                                      \
                list, (void *data) { return list_methodTable.remove(&privateTable, data); }),      \
            .remove_if = lambda(                                                                   \
                list,                                                                              \
                (int (*key)(void *data)) {                                                         \
                    return list_methodTable.remove_if(&privateTable, key);                         \
                }),                                                                                \
            .sort = lambda(                                                                        \
                list, (void) { return list_methodTable.sort(&privateTable); }),                    \
            .reverse = lambda(                                                                     \
                list, (void) { return list_methodTable.reverse(&privateTable); }),                 \
            .for_each = lambda(                                                                    \
                list,                                                                              \
                (iterator first, iterator last, void (*todo)(void *data)) {                        \
                    return list_methodTable.for_each(&privateTable, first, last, todo);            \
                }),                                                                                \
            .traverse = lambda(                                                                    \
                list,                                                                              \
                (void (*todo)(void *data)) {                                                       \
                    return list_methodTable.for_each(                                              \
                        &privateTable, next_iterator(privateTable.head), privateTable.head, todo); \
                }),                                                                                \
            .destory = lambda(                                                                     \
                list, (void) { return list_methodTable.destory(&privateTable); }),                 \
        };                                                                                         \
        privateTable.this;                                                                         \
    })

#endif // (!defined LIST_H)