#ifndef SAFE_MAP_H
#define SAFE_MAP_H

#include <mutex>
#include <QMap>
#include <QMultiMap>
#include <QFile>
#include <QVector>


template<typename T>
class SafeMap {
public:
  SafeMap() = default;
  ~SafeMap() = default;
  int get(T key) const;
  void inc(T key);
  void clear();
  int size();
  QMapIterator<T, int> getIter();

private:
  QMap<T, int> data;
  mutable std::mutex mutex;
};

template<typename T>
int SafeMap<T>::get(T key) const {
    std::lock_guard<std::mutex> lock(mutex);
    return data[key];
}

template<typename T>
void SafeMap<T>::inc(T key) {
  std::lock_guard<std::mutex> lock(mutex);
  data[key]++;
}

template<typename T>
QMapIterator<T, int> SafeMap<T>::getIter() {
    std::lock_guard<std::mutex> lock(mutex);
    QMapIterator<T, int> it(data);
    return it;
}

template<typename T>
void SafeMap<T>::clear() {
  std::lock_guard<std::mutex> lock(mutex);
  data.clear();
}

template<typename T>
int SafeMap<T>::size() {
  std::lock_guard<std::mutex> lock(mutex);
  return data.size();
}

#endif // SAFE_MAP_H
