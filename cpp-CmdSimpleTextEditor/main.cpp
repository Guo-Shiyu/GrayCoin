/*
 * 使用string类，在控制台窗口实现一个行文本编辑器
 * 
 * 该文本编辑器具有以下功能
 * + 输入新行
 * + 显示目前已输入文本
 * + 删除指定行：1-N（行号）
 * + 在文本中查找单词：返回单词所在行号，单词不存给出相应提示
 * + 结束文本编辑器运行
 * 
 * 功能在界面上提示，进行以上操作，并显示相应结果
 * 
 */ 


#include <string>
#include <iostream>

// 文本内容
std::string TEXT;

// 显示目前已输入文本
void show();

// 输入新行
void newline();

// 删除指定行：1-N（行号）
void delline();

// 在文本中查找单词：返回单词所在行号，单词不存给出相应提示
void find();


// main loop
void loop();

int main()
{
    loop();

    return 0;
}

void loop()
{
    std::string input;
    bool flag = true;
    while(flag)
    {
        std::cout << "\n=========================\nplease input your command: \n s: show text \n d: delete a line \n f: find a word \n n: newline \n e: exit \n>>";
        std::cin >> input;
        switch (input.front())
        {
        case 's':
            show();
            break;
        
        case 'd':   
            delline();
            break;
        
        case 'f':   
            find();
            break;
        
        case 'n':   
            newline();
            break;
        
        case 'e':   
            flag = false;
            break;

        default:
            std::cout << "unknown command, please reinput command." << std::endl;
            break;
        }
    }
}

void show()
{
    std::cout << "--------" << std::endl;
    std::cout << TEXT << std::endl;
    std::cout << "--------" << std::endl;
}

// 输入新行
void newline()
{
    std::cout << "input new line:\n>>";
    char buf[256] = {'\0'};
    scanf("%[^\n]", &buf);
    std::string line(buf);
    if (TEXT.size() != 0 )
        TEXT.append("\n");
    TEXT.append(line);
}

// 删除指定行：1-N（行号）
void delline()
{
    std::cout << "input line number you want to delete: \n>>";    int number;
    std::cin>>number;

    int counter = 1;
    for (size_t i = 0; i < TEXT.size(); i++)
    {
        if (TEXT.at(i) == '\n')
            counter++;
        if (counter == number )
            TEXT.erase(i);    
    }
}

// 在文本中查找单词：返回单词所在行号，单词不存给出相应提示
void find()
{
    std::cout << "input word you want to find: \n>>";
    std::string target;
    std::cin>>target;

    size_t pos = TEXT.find_first_of(target);
    if (pos == std::string::npos)
    {
        std::cout << "can't find wotd: " << target << std::endl;
    }
    else 
    {
        int counter = 1;
        for (size_t i = 0; i < pos; i++)
        {
            if (TEXT.at(i) == '\n')
                counter++;
        }
        std::cout << '\'' << target << '\'' << " first found in line :" << counter;
    }
    
}