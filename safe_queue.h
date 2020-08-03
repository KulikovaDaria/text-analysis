#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <mutex>
#include <QQueue>


template<typename T>
class SafeQueue {
public:
  SafeQueue() = default;
  SafeQueue(const SafeQueue& q);
  ~SafeQueue() = default;
  SafeQueue operator=(const SafeQueue& obj);
  bool empty() const;
  void push(const T& val);
  T pop();
  void clear();
  int size();

private:
  QQueue<T> data;
  mutable std::mutex mutex;
};

template<typename T>
SafeQueue<T>::SafeQueue(const SafeQueue<T>& q) {
    data = q.data;
}

template<typename T>
SafeQueue<T> SafeQueue<T>::operator=(const SafeQueue<T>& obj) {
  std::lock_guard<std::mutex> lock(mutex);
  data = obj.data;
  return *this;
}

template<typename T>
bool SafeQueue<T>::empty() const {
  std::lock_guard<std::mutex> lock(mutex);
  return data.empty();
}

template<typename T>
void SafeQueue<T>::push(const T& val) {
  std::lock_guard<std::mutex> lock(mutex);
  data.push_back(val);
}

template<typename T>
T SafeQueue<T>::pop() {
  std::lock_guard<std::mutex> lock(mutex);
  T val = data.front();
  data.pop_front();
  return val;
}

template<typename T>
void SafeQueue<T>::clear() {
  std::lock_guard<std::mutex> lock(mutex);
  data.clear();
}

template<typename T>
int SafeQueue<T>::size() {
  std::lock_guard<std::mutex> lock(mutex);
  return data.size();
}


#endif // SAFE_QUEUE_H
