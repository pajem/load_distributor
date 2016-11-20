#include "distributor.h"

#include <cstdlib>

Distributor::Distributor(const Processor::Task& task, uint16_t num_processors)
	: m_num_processors(num_processors) {
	for (uint16_t i = 0; i != m_num_processors; ++i) {
		m_processors.emplace_back((std::make_unique<Processor>(task)));
	}
}

void Distributor::distribute(zmq::message_t&& message) {
	auto index = std::rand() % m_num_processors;
	m_processors[index]->process(std::move(message));
}
