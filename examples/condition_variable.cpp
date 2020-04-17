#include <condition_variable>
#include <iostream>
#include <mutex>
#include <ostream>
#include <queue>
#include <sstream>
#include <thread>
#include <utility>

template <typename DataType>
class Spsc_q {
public:
    void add(DataType&& data);
    void process();

private:
    mutable std::mutex m_;
    std::condition_variable cond_;
    std::queue<DataType> queue_;
};

template <typename DataType>
void Spsc_q<DataType>::add(DataType&& data)
{
    {
        std::lock_guard<std::mutex> lock{m_};
        queue_.push(std::forward<DataType>(data));
    }

    cond_.notify_one();
}

template <typename DataType>
void Spsc_q<DataType>::process()
{
    for(;;) {
        std::unique_lock<std::mutex> lock{m_};
        cond_.wait(lock, [this] { return !queue_.empty(); });
        auto data = queue_.front();
        queue_.pop();
        lock.unlock();

        std::cout << "log " << data << "\n";
    }
}

class Log_object {
public:
    Log_object(const std::string& level, const std::string& msg);

    friend std::ostream& operator<<(std::ostream&, const Log_object&);

private:
    std::string level_;
    std::string msg_;
};

Log_object::Log_object(const std::string& level, const std::string& msg)
    : level_{level}
    , msg_{msg}
{
}

std::ostream& operator<<(std::ostream& os, const Log_object& log)
{
    os << "[" << log.level_ << "] [" << std::this_thread::get_id() << "] "
       << log.msg_;
    return os;
}

template <typename Q>
void produce_logs(Q& q)
{
    for(int i = 0; i < 10; ++i) {
        std::stringstream ss;
        ss << std::this_thread::get_id();
        q.add(Log_object{"INFO", ss.str() + " here is a message"});
    }
}

int main()
{
    Spsc_q<Log_object> log_q;
    std::thread th_c{[&log_q] { log_q.process(); }};
    std::thread th_p{[&log_q] { produce_logs(log_q); }};

    th_p.join();
    th_c.join();
}
