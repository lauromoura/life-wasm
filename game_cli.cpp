#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <functional>

#include "life.hh"

typedef std::function<void(Game &, std::vector<std::string> const& command)> Command;

int main(int argc, char* argv[])
{
    Game g(10, 10);
    int generation = 0;
    while (true) {
        std::cout << "Generation " << generation << std::endl;
        g.Print();

        std::cout << "> ";

        std::string command_line;

        std::cout << "Command after is ["  << command_line << "]" << std::endl;
        std::getline(std::cin, command_line);

        std::cout << "Command is ["  << command_line << "]" << std::endl;

        std::vector<std::string> tokens;

        std::istringstream iss(command_line);
        std::copy(std::istream_iterator<std::string>(iss),
                  std::istream_iterator<std::string>(),
                  std::back_inserter(tokens));

        if (tokens.size() == 0) {
            g.Evolve();
            generation++;
            continue;
        }

        std::string command = tokens[0];

        if (command == "exit")
            return 0;
        else if (command == "tick") {
            g.Evolve();
            generation++;
        } else if (command == "toggle") {

            if (tokens.size() < 3) {
                std::cerr << "Toggle requires row and col as arguments" << std::endl;
                continue;
            }
            int row = std::atoi(tokens[1].c_str());
            int col = std::atoi(tokens[2].c_str());

            std::cout << "Toggling " << row << ", " << col << std::endl;

            g.Toggle(row, col);


        }
    }
    return 0;
}
