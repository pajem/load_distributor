#include "distributor.h"

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
	Distributor test_distributor(task, 4);

	std::vector<std::string> data = {{"data1"}, {"data2"}, {"data3"}, {"data4"}};

	for (auto& datum : data) {
		zmq::message_t message(datum.begin(), datum.end());
		test_distributor.distribute(std::move(message));
	}

	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}
