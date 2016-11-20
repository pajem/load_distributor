#ifndef DISTRIBUTOR_H_
#define DISTRIBUTOR_H_

#include <cstdint>
#include <memory>
#include <vector>

#include "processor.h"

class Distributor {
public:
	Distributor(const Processor::Task& task, uint16_t num_processors);

	void distribute(zmq::message_t&&);

private:
	uint16_t m_num_processors;
	std::vector<std::unique_ptr<Processor>> m_processors;
};

#endif // DISTRIBUTOR_H_
