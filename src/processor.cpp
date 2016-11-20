#include "processor.h"

#include <iostream>

uint64_t Processor::index = 0;
zmq::context_t Processor::m_context(1);

Processor::Processor(const Task& task)
	: m_pull_socket(m_context, ZMQ_PULL)
	, m_push_socket(m_context, ZMQ_PUSH)
	, 	m_index(index++)
	, m_task(task) {
	// connect sockets
	m_pull_socket.bind("inproc://processor_pipe" + std::to_string(m_index));
	m_push_socket.connect("inproc://processor_pipe" + std::to_string(m_index));

	// start processor
	m_processor_thread = std::thread([this] {
		zmq::pollitem_t poll_item = {m_pull_socket, 0, ZMQ_POLLIN, 0};
		m_keep_going = true;
		while (m_keep_going) {
			// 100ms timeout
			zmq::poll(&poll_item, 1, 100);

			if (poll_item.revents & ZMQ_POLLIN) {
				zmq::message_t message;
				m_pull_socket.recv(&message);

				std::cout << "Processing message[" << m_index << ']' << std::endl;
				m_task(std::move(message));
			}
		}
	});

	std::cout << "Constructed Processor[" << m_index << ']' << std::endl;
}

Processor::~Processor() {
	m_keep_going = false;
	m_processor_thread.join();

	std::cout << "Destroyed Processor[" << m_index << ']' << std::endl;
}

void Processor::process(zmq::message_t&& message) {
	m_push_socket.send(message);
}
