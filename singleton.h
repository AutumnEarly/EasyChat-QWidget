#ifndef SINGLETON_H
#define SINGLETON_H

/*********************************************************************************
  * @Version:
  * @FileName:      singleton.h
  * @Brief:         公共单例模板基类

  * @Author:        秋初
  * @Date:          2024-09-07
  * @History:
**********************************************************************************/
#include <iostream>
#include <memory>
#include <mutex>

template <typename T>
class Singleton {
protected:
    Singleton () = default;
    Singleton(const Singleton<T>&) = delete;
    Singleton& operator = (const Singleton<T>& st) = delete;

    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag,[&]() {

            _instance = std::shared_ptr<T>(new T);
        });
        return _instance;
    }
};

template <typename T>
std::shared_ptr<T> Singleton<T>::_instance = nullptr;


#endif // SINGLETON_H
