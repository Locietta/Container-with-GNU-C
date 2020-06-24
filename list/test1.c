#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "lambda.h"

int main(void) {
    srand(1324);
    list myList = newlist(int);
    for (int i = 0; i < 10; ++i) {
        int temp = rand();
        myList.insert(myList.end(), &temp);
    }
    int shit1 = 114, shit2 = 514;
    myList.push_front(&shit1).push_back(&shit2).push_back(&shit2).push_back(&shit2).push_back(&shit2);
    for (iterator it = myList.begin(); it != myList.end(); it = next_iterator(it)) {
        printf("%d ", deref_iterator(int, it));
    }
    void (*print)(void *) = lambda(void, (void *data) { printf("%d ", __deref(int, data)); });
    putchar('\n');
    myList.pop_back().traverse(print);
    putchar('\n');
    myList.pop_front().reverse().traverse(print);
    putchar('\n');
    myList.setcmp(lambda(int, (void *data1, void *data2) { 
        return (*(int *) data1 - *(int *) data2); 
    })).remove(&shit2).traverse(print);
    putchar('\n');
    myList.sort().traverse(print).destory();
    
    return 0;
}