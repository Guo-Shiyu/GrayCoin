//#include <unistd.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>

enum Mode
{
    READ = 0,
    WRITE = 1,
};

void run(char *argv[]);

void fileOp(const std::string &, Mode &, const uint32_t &, const uint32_t &);

void run(char *argv[])  //分析各参数内容
{

    std::string filename;

    Mode mode = Mode::READ;

    uint32_t block_size = 0;
    uint32_t block_count = 0;

    filename = argv[1];

    if (argv[2][0] != '-')
    {
        throw std::runtime_error("");
    }

    switch (argv[2][1])
    {
    case 'r':
        mode = Mode::READ;
        break;
    case 'w':
        mode = Mode::WRITE;
        break;
    default:
        std::cerr << "mode only can be read(-r) or write(-w)" << std::endl;
        break;
    }

    try
    {
        block_size = std::atoi(argv[3]);

        block_count = std::atoi(argv[4]);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        throw std::runtime_error("");
    }

    // std::cout << filename << mode << block_size << block_count << std::endl;

    fileOp(filename, mode, block_size, block_count);
}

void fileOp(const std::string &filename, Mode &mode, const uint32_t &block_size, const uint32_t &block_count)  //执行命令
{
    int fd;
    char* buff = new char[block_size + 1]; //保证正确输出字符串
    memset(buff, 0, block_size + 1);
    size_t res;

    if (mode == Mode::READ)
    {
        fd = open(filename.c_str(), O_RDONLY);
    }
    else
    {
        fd = open(filename.c_str(), O_WRONLY);
    }

    if (fd < 0)
    {
        std::cerr << "file open failed" << std::endl;
        throw std::ios_base::failure("");
    }
    try
    {
        if (mode == Mode::READ)
        {
            for (auto i = 0; i < block_count; i++)
            {
                res = read(fd, (void *)buff, block_size); //读取内容
                memset(buff, 0, block_size);
            }
        }
        else
        {
            int pos = 0;
            pos = lseek(fd, 0, SEEK_END);  //文件指针移动至末尾
            
            for (auto i = 0; i < block_count; i++)
            {
                memset(buff, '0', block_size);
                res = write(fd, (void *)buff, block_size);   //写入文件内容
            }
        }

        close(fd);
    }
    catch (std::exception &e)
    {
        close(fd);
        throw std::ios::failure("");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)  //命令行操作
    {
        std::cerr << "run <filename> [-r|-w] <block_size> <block_count>" << std::endl;
        exit(-1);
    }

    try
    {
        run(argv);
    }
    catch (const std::runtime_error &e)
    {
        std::cerr << "run <filename> [-r|-w] <block_size> <block_count>" << std::endl;
        exit(-1);
    }
    return 0;
}