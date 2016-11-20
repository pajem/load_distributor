#include "processor.h"

#include <chrono>
#include <iostream>
#include <functional>
#include <string>
#include <vector>

#include <zmq.hpp>

void print(zmq::message_t&& message) {
	std::string data(message.data<char>(), message.size());
	std::cout << data << std::endl;
}

int main() {
	auto task = std::bind(&print, std::placeholders::_1);
	Processor test_processor(task);

	std::vector<std::string> data = {{"data1"}, {"data2"}, {"data3"}};

	for (auto& datum : data) {
		zmq::message_t message(datum.begin(), datum.end());
		test_processor.process(std::move(message));
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}
