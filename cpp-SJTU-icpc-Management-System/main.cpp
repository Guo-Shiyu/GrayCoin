
// local test marker
#define LOCAL_TEST
//      ^^^ comment the it when submit homework to online judjement

#include "io.hpp"
#include "system.hpp"

#ifdef LOCAL_TEST
#include <iostream>
#include <filesystem>
#endif // LOCAL_TEST


int main()
{

	// get the only instance of 'system'
	auto system = SystemProxy::instance();

#ifdef LOCAL_TEST

	std::cout << "Hello World!" << std::endl;

	// locate 'data' dictionary
	auto data_dict = std::filesystem::current_path().concat("\\data");
	
	// std::cout << data_dict.string() << std::endl;
	// for (const auto &data : data_dict)
	// {
		auto commands = Input::ReadCommandFromFile(data_dict.string().append("\\1.in"));
		auto writter = Output::Outputter::SendTo(data_dict.string().append("\\2xyz.txt"));
		for (const auto& instrc : commands)
		{
			auto [ret, appendix] = system->react(instrc);
			writter.write(instrc->opcode().as_enum(), ret, appendix);
		}
	// }

#else	// in OJ

#endif;

	std::cout << "[Info]Competition ends." << std::endl; // flush buffer 
	return 0;
}