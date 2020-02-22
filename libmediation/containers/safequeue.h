
#ifndef SAFE_QUEUE_H
#define SAFE_QUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
//封装位一个一个容器，需要T这样的模板，需要定义一个
//    typedef typename std::queue<T>::size_type size_type; 这样的类型
template <typename T>
class SafeQueue
{
   public:
    typedef typename std::queue<T>::size_type size_type;

    SafeQueue(const size_type& maxSize) : m_maxSize(maxSize) {}

    virtual ~SafeQueue() {}

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(m_mtx);

        return m_queue.empty();
    }

    size_type size() const
    {
        std::lock_guard<std::mutex> lk(m_mtx);

        return m_queue.size();
    }

    bool push(const T& val)
    {
        std::lock_guard<std::mutex> lk(m_mtx);

        if (m_queue.size() >= m_maxSize)
            return false;

        m_queue.push(val);
        return true;
    }

    T pop()
    {
        std::lock_guard<std::mutex> lk(m_mtx);

        //先进先出就是队列，所以取队头
        T val = m_queue.front();
        m_queue.pop();

        return val;
    }

   private:
    mutable std::mutex m_mtx;
    std::queue<T> m_queue;
    const size_type m_maxSize;

    //这种构造 和 =  这种构造是私有的，todo
    SafeQueue(const SafeQueue<T>&);
    SafeQueue& operator=(const SafeQueue<T>&);
};
//增加一个条件变量就是通知了
template <typename T> //T是传递给SafeQueue的

class SafeQueueWithNotification : public SafeQueue<T>
{
   public:
    SafeQueueWithNotification(const uint32_t maxSize, std::mutex& mtx, std::condition_variable& cond)
        : SafeQueue<T>(maxSize), m_mtx(mtx), m_cond(cond)
    {
    }

    //有数据来就通知
    bool push(const T& val)
    {
        std::lock_guard<std::mutex> lk(m_mtx);

        if (!SafeQueue<T>::push(val))
            return false;

        m_cond.notify_one();

        return true;
    }

   private:
    std::mutex& m_mtx;
    std::condition_variable& m_cond;

    SafeQueueWithNotification(const SafeQueueWithNotification<T>&);
    SafeQueueWithNotification& operator=(const SafeQueueWithNotification<T>&);
};

//带阻塞的
template <typename T>
class WaitableSafeQueue : public SafeQueueWithNotification<T>
{
   public:
    WaitableSafeQueue(const uint32_t maxSize) : SafeQueueWithNotification<T>(maxSize, m_mtx, m_cond) {}

    ~WaitableSafeQueue() override {}

    T pop()
    {
        std::unique_lock<std::mutex> lk(m_mtx);

        while (SafeQueue<T>::empty())
        {
            m_cond.wait(lk);
        }

        //这个是直接拷贝么？ SafeQueue 的引用和= 不可用
        T val = SafeQueue<T>::pop();

        return val;
    }

   private:
    std::mutex m_mtx;
    std::condition_variable m_cond;

    //todo 这样的构造函数怎使用这个类呢？
    WaitableSafeQueue(const WaitableSafeQueue<T>&);
    WaitableSafeQueue& operator=(const WaitableSafeQueue<T>&);
};

#endif  //_SAFE_QUEUE_H
