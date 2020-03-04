#pragma once

#include <iostream>

#include "json.h"

#include <unistd.h>
#include <limits.h>

using nlohmann::json;
namespace logger {

std::string curApp()
{
	char buf[PATH_MAX];
	ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf));
	return std::string(buf,len);
}


struct Message
{
	std::string appName;
	unsigned int line;
	std::string function_name;
	std::string text;
};

void to_json(json& j,const Message& msg) {
	j = json{
			{"application",msg.appName},
			{"line",msg.line},
			{"function",msg.function_name},
			{"text",msg.text}
		};
}

void from_json(const json& j,Message& msg) {
	j.at("line").get_to(msg.line);
	j.at("function").get_to(msg.function_name);
	j.at("text").get_to(msg.text);
}

} // namespace logger


/*	logging macro	*/
#define LOG(text) std::cout<<json( \
		logger::Message{logger::curApp(), \
						__LINE__, \
						__PRETTY_FUNCTION__, \
						text}\
		).dump(4)<<std::endl;
