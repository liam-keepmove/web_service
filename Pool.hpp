#pragma once

#ifndef SBUF_H
#define SBUF_H

#include<semaphore.h>
#include"wrapper.h"

/*
 * 循环队列,尾指针指向待插入的元素的位置(也就是最后一个元素的后面).
 * 判空判满由信号量决定,所以不用浪费一个元素位置或者添加一个tag来判断空满.
 * */
template<typename K=int>
class Pool {
private:
    typedef struct {
        K* buf;  //循环队列缓冲区指针,动态分配内存
        ssize_t bufLen;  //缓冲区长度
        ssize_t front;  //队列第一个元素下标
        ssize_t rear;  //队列最后一个元素的后一位(待插入位)的下标
        sem_t mutex;  //互斥信号量
        sem_t slots;  //缓冲区空闲槽位
        sem_t items;  //缓冲区可用项目
        //加锁队列,不用预留空间判断队空队满.
    } sbuf_t;
    sbuf_t sp{};
public:
    explicit Pool(int bufLen = MAXBUF);

    ~Pool();

    void push(const K& item);

    K pop(K* ans = nullptr);
};

//初始化一个sbuf_t
template<typename K>
Pool<K>::Pool(int bufLen) {
    sp.buf = (K*) Calloc(bufLen, sizeof(K));
    sp.bufLen = bufLen;
    sp.front = sp.rear = 0;
    Sem_init(&sp.mutex, 0, 1);
    Sem_init(&sp.slots, 0, bufLen);
    Sem_init(&sp.items, 0, 0);
}

template<typename K>
Pool<K>::~Pool() {
    free(sp.buf);
}

//一个元素从尾部压入队列
template<typename K>
void Pool<K>::push(const K& item) {
    P(&sp.slots);  //等待到有或直接减少一个空槽位,为插入元素做准备.(同步锁)
    P(&sp.mutex);  //互斥锁加锁
    sp.buf[sp.rear] = item;
    sp.rear = (sp.rear + 1) % sp.bufLen;
    V(&sp.mutex);  //互斥锁解锁
    V(&sp.items);  //可用元素数量加一.
}

//一个元素从头部出队列
template<typename K>
K Pool<K>::pop(K* ans) {
    K item = 0;
    P(&sp.items);  //等待到有或减少一个可用元素,为弹出元素做准备.(同步锁)
    P(&sp.mutex);  //互斥锁加锁
    item = sp.buf[sp.front];
    sp.front = (sp.front + 1) % sp.bufLen;
    V(&sp.mutex);  //互斥锁解锁
    V(&sp.slots);  //空槽位数量加一
    *ans = item;
    return item;
}

#endif  //SBUF_H
