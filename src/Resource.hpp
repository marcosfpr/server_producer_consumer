#ifndef CSERVER_RESOURCE_HPP
#define CSERVER_RESOURCE_HPP

#include <memory>
#include <thread>
#include <iostream>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>

using std::deque;

//#define DEBUG

namespace pcr{

#ifdef DEBUG
    static std::mutex coutMutex;
#endif

    template <class T>
    class Buffer{
    public:
        explicit Buffer(unsigned int size) : size_(size) {can_produce = true; can_consume = false;}
        virtual ~Buffer() = default;

        void add(T value){
            auto func_add = [this](){return buffer_.size() < size_;};
            std::unique_lock<std::mutex> locker(mutex_);
            condition_.wait(locker, func_add);
            buffer_.push_back(value);
//            locker.unlock();
            condition_.notify_one();

        }

        T remove() {
            auto func_remove = [this](){return buffer_.size() > 0;};
            while (true) {
                std::unique_lock<std::mutex> locker(mutex_);
                condition_.wait(locker, func_remove);
                T value = buffer_.back();
                buffer_.pop_back();
//                locker.unlock();
                return value;
            }
        }

    private:
        Buffer(const Buffer<T>& buf); // block copy constructor
        Buffer<T>& operator=(const Buffer<T>& buf); // block assign

        std::mutex mutex_;
        std::condition_variable condition_;
        bool can_produce, can_consume;

        deque<T> buffer_;
        unsigned int size_;
    };

    template <class T>
    class Producer{
    public:
        Producer(std::shared_ptr<Buffer<T>> buffer) : buffer_(buffer){}
        virtual ~Producer() = default;

        void produce(T element){
            buffer_->add(element);
            #ifdef DEBUG
                pcr::coutMutex.lock();
                std::cout << "Produced " << element << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                pcr::coutMutex.unlock();
            #endif
        }

        Producer(const Producer& p){
            this->buffer_ = p.buffer_;
        }

        Producer& operator=(const Producer& p){
            this->buffer_ = p.buffer_;
            return *this;
        }

    private:
        std::shared_ptr<Buffer<T>> buffer_;
    };

    template <class T>
    class ConsumeHandler{
    public:
        virtual void handle(T element) = 0;
    };

    template <class T>
    class Consumer{
    public:
        Consumer(std::shared_ptr<Buffer<T>> buffer, ConsumeHandler<T>* handler){
            buffer_ = buffer;
            handler_ = handler;
        }
        virtual ~Consumer() = default;

        void consume() {
            while (true){
                T element = buffer_->remove();
                #ifdef DEBUG
                pcr::coutMutex.lock();
                std::cout << "Consumed " << element << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                pcr::coutMutex.unlock();
                #endif
                handler_->handle(element);
            }
        }

        Consumer(const Consumer& c){
            this->buffer_ = c.buffer_;
            this->handler_ = c.handler_;
        }

        Consumer& operator=(const Consumer& c){
            this->buffer_ = c.buffer_;
            this->handler_ = c.handler_;
            return *this;
        }

    private:
        ConsumeHandler<T>* handler_;
        std::shared_ptr<Buffer<T>> buffer_;
    };

};
#endif //CSERVER_RESOURCE_HPP
