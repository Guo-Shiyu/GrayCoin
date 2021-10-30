#include "./base.hpp"
#include <vector>
#include <optional>

namespace Input
{
using InputSeq = std::vector<Command>;

// read a input file, return all command in a vector ( in order )
InputSeq ReadCommandFromFile(std::string filename)
{

}
    
} // namespace Input

namespace Output
{

class Outputter
{
private:
    /* data */
public:
    Outputter(/* args */);
    ~Outputter();

    void write(OpKind kind, int ret_code, std::optional<std::string> appendix)
    {

    }

    static Outputter& SendTo(std::string target) 
    {
        // todo 
        static Outputter put;
        return put;
    }
};

Outputter::Outputter(/* args */)
{
}

Outputter::~Outputter()
{
}


    
} // namespace Output
