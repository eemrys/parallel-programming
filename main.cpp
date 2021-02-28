#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <ctime>
using namespace std;

clock_t time_sum, time_delta, time_sum2, time_sum3;
mutex m_st_del, m_st_pop, m_st_top, m_st_size, m_st_st, m_st_swap, m_st_eq, m_st_push, m_st_disp;

template <typename T>
class Stack {
public:
    Stack() = default;
    Stack(const Stack& st);
    Stack& operator=(const Stack& st);
    ~Stack();
    bool empty() const { return head_ == nullptr; }
    void push(const T val);
    T& top();
    void pop();
    int size();
    void swap(Stack& sec);
    void output();
private:
    struct Node {
        Node* next_{ nullptr };
        T data_{ 0 };
        Node(Node* next_, const T& data_) : next_(next_), data_(data_) {}
    };
    Node* head_{ nullptr };
};

template <typename T>
Stack<T>::Stack(const Stack<T>& st) {
    lock_guard<mutex> lock(m_st_st);
    head_ = nullptr;
    if (st.empty()) { return; }
    head_ = new Node(nullptr, st.head_->data_);
    Node* data = st.head_;
    Node* cur = head_;
    while (data->next_ != nullptr) {
        data = data->next_;
        cur->next_ = new Node(data->next_, data->data_);
        if (cur->next_ != nullptr)
            cur = cur->next_;
    }
}

template <typename T>
Stack<T>& Stack<T>::operator=(const Stack<T>& st) {
    lock_guard<mutex> lock(m_st_eq);
    if (&st == this) return *this;
    Stack temp(st);
    Node* tmp = temp.head_;
    temp.head_ = head_;
    head_ = tmp;
    return *this;
}

template <typename T>
Stack<T>::~Stack() {
    lock_guard<mutex> lock(m_st_del);
    while (!empty()) pop();
}

template <typename T>
void Stack<T>::swap(Stack<T>& st) {
    lock_guard<mutex> lock(m_st_swap);
    Node* n = st.head_;
    st.head_ = head_;
    head_ = n;
}

template <typename T>
int Stack<T>::size() {
    lock_guard<mutex> lock(m_st_size);
    int ind = 0;
    Node* st = head_;
    while (head_ != nullptr) {
        ++ind;
        head_ = head_->next_;
    }
    head_ = st;
    return ind;
}

template <typename T>
void Stack<T>::push(const T val) {
    lock_guard<mutex> lock(m_st_push);
    head_ = new Node(head_, val);
}

template <typename T>
T& Stack<T>::top() {
    lock_guard<mutex> lock(m_st_top);
    if (empty())
        throw std::runtime_error("Stack is empty!");
    return head_->data_;
}

template <typename T>
void Stack<T>::pop() {
    lock_guard<mutex> lock(m_st_pop);
    if (empty())
        throw std::runtime_error("Stack is empty!");
    Node* old = head_;
    head_ = head_->next_;
    delete old;
}


template <typename T>
void Stack<T>::output() {
    lock_guard<mutex> lock(m_st_disp);
    cout << endl << 2 << "  " << this_thread::get_id() << endl;
    clock_t time_clock;
    time_clock = clock();
    Node* n = head_;
    cout << endl;
    for (int i = 0; i < this->size(); ++i) {
        cout << n->data_ << "  ";
        n = n->next_;
    }
    cout << endl;
    time_clock = clock() - time_clock;
    time_sum2 = time_sum2 + time_clock;
}

mutex m_vec_vec, m_vec_val, m_vec_del, m_vec_disp, m_vec_swap, m_vec_pb, m_vec_pop, m_vec_eq, m_vec_elem;

template <typename T>
class Vector_n {
private:
    unsigned long long size_{ 0 };
    unsigned long long capacity_{ 0 };
    T* data_start_;
public:
    Vector_n() {}
    Vector_n(unsigned long long start_size_, T val_);
    ~Vector_n() { delete(data_start_); }
    unsigned long long size() const { return size_; }
    T& operator()(unsigned long long i);
    Vector_n operator=(Vector_n v);
    Vector_n(const Vector_n& v);
    void display();
    void push_back(T val);
    void pop();
    void swap(Vector_n& v);
};

template <typename T>
Vector_n<T>::Vector_n(unsigned long long start_size_, T val_) {
    lock_guard<mutex> lock(m_vec_val);
    if (start_size_ >= capacity_)
        capacity_ = start_size_ * 2;
    data_start_ = new T[capacity_];
    size_ = start_size_;
    unsigned long long i = 0;
    for (i = 0; i < size_; ++i)
        *(data_start_ + i) = val_;
}

template <typename T>
T& Vector_n<T>::operator()(unsigned long long i) {
    lock_guard<mutex> lock(m_vec_elem);
    if (i < 0 || i >= size_)
        throw "Out of range";
    return *(data_start_ + i);
}

template <typename T>
Vector_n<T> Vector_n<T>::operator=(Vector_n<T> v) {
    lock_guard<mutex> lock(m_vec_eq);
    delete(data_start_);
    size_ = v.size_;
    capacity_ = v.capacity_;
    data_start_ = new T[capacity_];
    unsigned long long i = 0;
    for (i = 0; i < size_; ++i)
        *(data_start_ + i) = *(v.data_start_ + i);
    return *this;
}

template <typename T>
Vector_n<T>::Vector_n(const Vector_n<T>& v) {
    lock_guard<mutex> lock(m_vec_vec);
    size_ = v.size_;
    capacity_ = v.capacity_;
    data_start_ = new T[capacity_];
    unsigned long long i = 0;
    for (i = 0; i < size_; ++i)
        *(data_start_ + i) = *(v.data_start_ + i);
}

template <typename T>
void Vector_n<T>::display() {
    lock_guard<mutex> lock(m_vec_disp);
    clock_t time_clock;
    time_clock = clock();
    cout << endl << "Vector size = " << size_ << endl;
    for (unsigned long long i = 0; i < size_; ++i)
        cout << *(data_start_ + i) << ' ';
    cout << endl;
    time_clock = clock() - time_clock;
    time_sum3 = time_sum3 + time_clock;
}

template <typename T>
void Vector_n<T>::pop() {
    lock_guard<mutex> lock(m_vec_pop);
    if (size_ > 0)
        --size_;
}
template <typename T>
void Vector_n<T>::push_back(T elem) {
    lock_guard<mutex> lock(m_vec_elem);
    if (size_ + 1 > capacity_)
        resize(size() + 1);
    else
        ++size_;
    *(data_start_ + size_ - 1) = elem;
}

template <typename T>
void Vector_n<T>::swap(Vector_n <T>& vec) {
    lock_guard<mutex> lock(m_vec_swap);
    Vector_n <T> v(vec);
    vec = *this;
    *this = v;
}

mutex m_qu_push, m_qu_size, m_qu_disp, m_qu_swap, m_qu_empty, m_qu_del, m_qu_pop, m_qu_eq, m_qu_val;
template <typename T>
class Queue {
public:
    Queue() = default;
    Queue(const Queue& q);
    Queue(const int size, T val);
    ~Queue();
    void push(const T val);
    bool empty() const { lock_guard<mutex> lock(m_qu_empty); return begin_ == nullptr; }
    T& top();
    void pop();
    void output();
    void swap(Queue& vec);
    int size();
    Queue& operator=(const Queue& q);

private:
    struct Node {
        Node* next_{ nullptr };
        Node* prev_{ nullptr };
        T data_{ 0 };
        Node(Node* prev, const T& data) : prev_(prev), data_(data) { }
    };
    Node* begin_{ nullptr };
    Node* end_{ nullptr };
};

template <typename T>
Queue<T>::Queue(const Queue<T>& q) {
    begin_ = nullptr;
    if (q.empty()) return;
    begin_ = new Node(nullptr, q.begin_->data_);
    end_ = begin_;
    Node* data = q.begin_;
    while (data != q.end_) {
        data = data->next_;
        end_->next_ = new Node(end_, data->data_);
        end_->next_->prev_ = end_;
        end_ = end_->next_;
    }
}

template <typename T>
Queue<T>::Queue(const int size, T val) {
    lock_guard<mutex> lock(m_qu_val);
    for (int i = 0; i < size; ++i)
        this->push(val);
}

template <typename T>
Queue<T>::~Queue() {
    lock_guard<mutex> lock(m_qu_del);
    while (!empty()) pop();
}

template <typename T>
T& Queue<T>::top() {
    if (empty())
        throw std::runtime_error("Queue is empty!");
    return begin_->data_;
}

template <typename T>
void Queue<T>::pop() {
    lock_guard<mutex> lock(m_qu_pop);
    if (empty())
        throw std::runtime_error("Queue is empty!");
    Node* old = begin_;
    begin_ = begin_->next_;
    delete old;
}

template <typename T>
void Queue<T>::push(const T val) {
    lock_guard<mutex> lock(m_qu_push);
    if (empty()) {
        end_ = new Node(nullptr, val);
        begin_ = end_;
    }
    else {
        end_->next_ = new Node(end_, val);
        end_->next_->prev_ = end_;
        end_ = end_->next_;
    }
}

template <typename T>
int Queue<T>::size() {
    lock_guard<mutex> lock(m_qu_size);
    Node* n = begin_;
    int sum = 0;
    while (n != nullptr) {
        ++sum;
        n = n->next_;
    }
    return sum;
}

template <typename T>
void Queue<T>::output() {
    lock_guard<mutex> lock(m_qu_disp);
    cout << this_thread::get_id() << endl;
    clock_t time_clock;
    time_clock = clock();
    Node* n = begin_;
    cout << endl;
    for (int i = 0; i < this->size(); ++i) {
        cout << n->data_ << "  ";
        n = n->next_;
    }
    cout << endl;
    time_clock = clock() - time_clock;
    time_sum = time_sum + time_clock;
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& q) {
    lock_guard<mutex> lock(m_qu_eq);
    if (&q == this)
        return *this;
    Queue temp(q);
    Node* tmp = temp.begin_;
    temp.begin_ = begin_;
    begin_ = tmp;
    return (*this);
}

template <typename T>
void Queue<T>::swap(Queue<T>& q) {
    lock_guard<mutex> lock(m_qu_swap);
    Node* h = q.begin_;
    q.begin_ = begin_;
    begin_ = h;
}


Queue<int> q1;

void QueueConnect() {
    cout << this_thread::get_id() << endl;
    clock_t time_clock = clock() - time_delta;
    printf("Connect: %f seconds.\n", ((double)time_clock) / CLOCKS_PER_SEC);
    q1.output();
}

Stack<int> s1;


void StackConnect() {
    cout << this_thread::get_id() << endl;
    clock_t time_clock = clock() - time_delta;
    printf("Connect: %f seconds.\n", ((double)time_clock) / CLOCKS_PER_SEC);
    s1.output();
}

Vector_n< int> v1;

void VectorConnect() {
    cout << this_thread::get_id() << endl;
    clock_t time_clock = clock() - time_delta;
    printf("Connect: %f seconds.\n", ((double)time_clock) / CLOCKS_PER_SEC);
    v1.display();
}

int main() {
    for (int i = 0; i < 10; ++i) {
        q1.push(1);
    }
    time_delta = clock();
    clock_t time1;
    time1 = clock();


    thread t1(QueueConnect);
    thread t2(QueueConnect);
    thread t3(QueueConnect);
    thread t4(QueueConnect);
    thread t5(QueueConnect);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    
    time1 = clock() - time1;
    cout << "Coeff: " << ((double)time_sum) / ((double)time1) << endl;
/*
    for (int i = 0; i < 10; ++i) {
        s1.push(1);
    }
    time_delta = clock();
    clock_t time2;
    time2 = clock();


    thread t6(StackConnect);
    thread t7(StackConnect);
    thread t8(StackConnect);
    thread t9(StackConnect);
    thread t10(StackConnect);
    t6.join();
    t7.join();
    t8.join();
    t9.join();
    t10.join();
    s1.output();
    time2 = clock() - time2;
    cout << ((double)time_sum2) / ((double)time2) << endl;
*/
}
