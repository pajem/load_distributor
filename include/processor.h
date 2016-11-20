#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include <atomic>
#include <functional>
#include <thread>

#include <zmq.hpp>

class Processor {
public:
	using Task = std::function<void(zmq::message_t&&)>;

	Processor(const Task& task);
	~Processor();

	void process(zmq::message_t&& message);

private:
	static uint64_t index;
	static zmq::context_t m_context;
	zmq::socket_t m_pull_socket;
	zmq::socket_t m_push_socket;
	uint64_t m_index;
	Task m_task;
	std::atomic_bool m_keep_going{false};
	std::thread m_processor_thread;
};

#endif // PROCESSOR_H_
