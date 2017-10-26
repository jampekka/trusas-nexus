#include "TmsiAmplifier.h"
#include <vector>
#include <iostream>
#include <string>
#include <chrono>
#include "json.hpp"

using std::vector;
using std::string;
using json = nlohmann::json;

static double getTimeSinceEpoch() {
    auto t = static_cast<unsigned long>
        (std::chrono::duration_cast<std::chrono::microseconds>
            (std::chrono::system_clock::now().time_since_epoch()).count());
    
    return t/1e6;
}

void output_samples(const auto channels, const auto samples) {
	auto ts = getTimeSinceEpoch();
	json data;
	for(int i=0; i < channels.size(); ++i) {
		data[channels[i]] = samples[i];
	}
	
	json out = json::array({
		{"ts", ts},
		data
		});
	std::cout << out << std::endl; 
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		std::cerr << "I need a bluetooth device address!" << std::endl;
		return 1;
	}

	TmsiAmplifier amp(argv[1], BLUETOOTH_AMPLIFIER);
	vector<string> enabled_channels;
	for(int i = 0; i < amp.channels_desc.size(); ++i) {
		channel_desc &desc = amp.channels_desc[i];
		enabled_channels.push_back(desc.name);

	}
	amp.set_active_channels(enabled_channels);
	amp.start_sampling();
	vector<float> samples(enabled_channels.size());
	std::cout << enabled_channels.size() << std::endl;
	for(;;) {
		amp.fill_samples(samples);
		output_samples(enabled_channels, samples);
	}
}
