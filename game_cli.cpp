#include <iostream>

#include "life.hh"

int main(int argc, char* argv[])
{
    Game g(10, 10);
    while (true) {
        g.Print();

        std::cout << "> ";

        std::string command;

        std::cin >> command;

        std::cout << "Command is ["  << command << "]" << std::endl;

        return 0;
    }
    return 0;
}
