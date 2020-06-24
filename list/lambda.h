/**
 * @file lambda.h
 * @author Locietta
 * @brief 用宏实现一个简单的lambda表达式(匿名函数)
 * @note 用到了GNU扩展，需要gcc编译 "hack, but so far stable"
 * @bug 
 * @version 0.1
 * @date 2020-06-13
 *
 */

#ifndef LAMBDA_H
#define LAMBDA_H

#define lambda(return_type, function_body)                                               \
    ({ return_type anonymous_func function_body anonymous_func; })

#endif // (!defined LAMBDA_H)
