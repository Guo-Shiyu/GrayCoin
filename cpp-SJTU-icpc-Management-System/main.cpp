#include "system.hpp"
#include "io.hpp"
#include <filesystem>
#include <iostream>

// local test marker
#define LOCAL_TEST
//      ^^^ comment the it when submit homework to online judjement

int main()
{
    std::cout << "Hello World!" << std::endl;
    
    // get the only instance of 'system'
    auto system = SystemProxy::instance();

#ifdef LOCAL_TEST

    // locate 'data' dictionary
    auto datafiles = std::filesystem::path("xxx\\xxxx\\data");
    for (const auto &data : datafiles)
    {
        auto cmds = Input::ReadCommandFromFile(data.string());
        auto writter = Output::Outputter::SendTo(data.string());
        for (const auto &instrc : cmds)
        {
            auto [ret, appendix] = system->react(instrc);
            writter.write(instrc->opcode().as_enum(), ret, appendix);
        }
    }

#else

#endif;

    return 0;
}