#ifndef UHUB_SAFE_QUEUE_H
#define UHUB_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class SafeQueue {
    /**
     * A thread-safe queue.
     * @tparam T The type of the elements in the queue.
     * @param maxSize The maximum size of the queue.
     */
public:
    explicit SafeQueue(int maxSize = 100);
    bool push(const T& data);
    bool push(T&& data);

    template <typename ...Args>
    bool emplace(Args&&... args);

    bool try_pop(T& data);
    void wait_pop(T &data);
    bool empty();
    void clear();
private:
    std::queue<T> queue;
    int maxSize;
    std::mutex mtx;
    std::condition_variable cv;
};


template <typename T>
SafeQueue<T>::SafeQueue(int maxSize) : maxSize(maxSize) {}

template <typename T>
bool SafeQueue<T>::push(const T& data) {
    std::lock_guard<std::mutex> lock(mtx);
    if (queue.size() >= maxSize)
        return false;

    queue.push(data);
    cv.notify_one();
    return true;
}

template <typename T>
bool SafeQueue<T>::push(T&& data) {
    std::lock_guard<std::mutex> lock(mtx);
    if (queue.size() >= maxSize)
        return false;

    queue.push(move(data));
    cv.notify_one();
    return true;
}

template <typename T>
template <typename ...Args>
bool SafeQueue<T>::emplace(Args &&...args) {
    std::lock_guard<std::mutex> lock(mtx);
    if (queue.size() >= maxSize)
        return false;

    queue.emplace(std::forward<Args>(args)...);
    cv.notify_one();
    return true;
}

template <typename T>
bool SafeQueue<T>::try_pop(T& data) {
    std::lock_guard<std::mutex> lock(mtx);
    if (queue.empty())
        return false;
    data = std::move(queue.front());
    queue.pop();
    return true;
}

template <typename T>
void SafeQueue<T>::wait_pop(T &data) {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [this](){return !queue.empty();});
    data = std::move(queue.front());
    queue.pop();
}

template <typename T>
bool SafeQueue<T>::empty() {
    std::lock_guard<std::mutex> lock(mtx);
    return queue.empty();
}

template <typename T>
void SafeQueue<T>::clear() {
    std::lock_guard<std::mutex> lock(mtx);
    while (!queue.empty())
        queue.pop();
}

#endif //UHUB_SAFE_QUEUE_H
