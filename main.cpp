#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <pthread.h>
#include <cassert>

using namespace std;


struct Node {
    int data_;
    Node *next_;
    mutex mutex_;
    Node():next_(nullptr){};
    Node(int data):data_(data), next_(nullptr){};
};

void Add(int data, Node *head) {
    // TODO 加锁的插入操作
    // 空指针不能加锁 记得异常处理
    // 分三种情况判断：已经存在、已经位于队尾、正常插入（位于队中）
    assert(head);
    try {
        head->mutex_.lock();
        if (head->next_) head->next_->mutex_.lock();
    } catch (const std::system_error& e) {}
    Node *pre = head;   // must not equal nullptr
    Node *next = head->next_;
    while (next && next->data_ < data) {
        try {
            pre->mutex_.unlock();
            if (next->next_) next->next_->mutex_.lock();
        } catch (const std::system_error& e) {}
        pre = next;
        next = next->next_;
    }
    Node *node = new Node(data);
    pre->next_ = node;
    node->next_ = next;
    try {
        pre->mutex_.unlock();
        if (next) next->mutex_.unlock();
    } catch (const std::system_error& e) {}
}

void Add_nolock(int data, Node *head) {
    // TODO 不加锁的插入操作
    assert(head);
    Node *pre = head;   // must not equal nullptr
    Node *next = head->next_;
    // find position
    while (next && next->data_ > data) {
        pre = next;
        next = next->next_;
    }
    Node *node = new Node(data);
    pre->next_ = node;
    node->next_ = next;
}

void Contain(int data, Node *head) {
    // TODO 查找列表中是否含有该数据
    assert(head);
    head->mutex_.lock(); // tutor said that head should be locked, so be it...
    Node *cur = head->next_;
//    if (cur) cur->mutex_.lock();
    while (cur && cur->data_ != data) {
//        cur->mutex_.unlock();
        cur = cur->next_;
//        cur->mutex_.lock();
    }
    head->mutex_.unlock();
}

void Remove(int data, Node *head) {
    // TODO 移除列表中特定数据
    assert(head);
    try {
        head->mutex_.lock();
        if (head->next_) head->next_->mutex_.lock();
        if (head->next_->next_) head->next_->next_->mutex_.lock();
    } catch (const std::system_error& e) {}
    Node *pre = head;   // must not equal nullptr
    Node *cur = head->next_;
    Node *next = (cur) ? cur->next_ : nullptr;
    while (cur && cur->data_ != data) {
        try {
            pre->mutex_.unlock();
            if (next && next->next_) next->next_->mutex_.lock();
        } catch (const std::system_error& e) {}
        pre = cur;
        cur = cur->next_;
        if (next) next = next->next_;
    }
    if (!cur) {     // data not found
        try {
            pre->mutex_.unlock();
        } catch (const std::system_error& e) {}
        return ;
    }
    pre->next_ = next;
    try {
        pre->mutex_.unlock();
        if (next) next->mutex_.unlock();
    } catch (const std::system_error& e) {}
    delete cur;
}

void Print(Node *head) {
    // TODO  遍历打印
    // 只需要锁头节点
    assert(head);
    head->mutex_.lock();
    Node *cur = head->next_;
    while (cur) {
        printf("%d ", cur->data_);
        cur = cur->next_;
    }
    printf("\n");
    head->mutex_.unlock();
}

int main() {
    Node *head = new Node();
    // Test Case 1
//    vector<thread> addthreads;
//    for (int i = 0; i < 100; i++)
//        addthreads.emplace_back(Add_nolock, i, head);
//    for (auto &thread1 : addthreads)
//        thread1.join();
//    Print(head);
    // Test Case 2
//    vector<thread> addthreads;
//    for (int i = 0; i < 100; i++)
//        addthreads.emplace_back(Add, i, head);
//    for (auto &thread1 : addthreads)
//        thread1.join();
//    Print(head);
//
//    vector<thread> remthreads;
//    for (int i = 0; i < 100; i++)
//        remthreads.emplace_back(Remove, i, head);
//    for (auto &thread2 : remthreads)
//        thread2.join();
//    Print(head);

    // Test case 3
    vector<thread> addthreads;
    for (int i = 0; i < 100; i = i + 2)
        addthreads.emplace_back(Add, i, head);
    for (auto &thread1 : addthreads)
        thread1.join();

    Print(head);

    vector<thread> addremthreads;
    for (int i = 0; i < 100; i = i + 2) {
        addremthreads.emplace_back(Add, i+1, head);
        addremthreads.emplace_back(Remove, i, head);
    }
    for (auto &thread3 : addremthreads)
        thread3.join();

    Print(head);

    // Test case 4
//    vector<thread> addthreads;
//    for (int i = 0; i < 100; i++)
//        addthreads.emplace_back(Add, i, head);
//    for (auto &thread1 : addthreads)
//        thread1.join();
//    Print(head);
//    vector<thread> conremthreads;
//    for (int i = 0; i < 97; i = i + 2) {
//        conremthreads.emplace_back(Contain, i+3, head);
//        conremthreads.emplace_back(Remove, i, head);
//    }
//    for (auto &thread4 : conremthreads)
//        thread4.join();
//    Print(head);
    delete head;

    return 0;
}
