#ifndef HttpWeatherClient_APPLICATION_HPP
#define HttpWeatherClient_APPLICATION_HPP

#include "../headers/Client.hpp"
#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>

namespace pr_option = boost::program_options;

class Application {
public:
    Application(int argc, char *argv[]);

    int exec();

private:

    std::string readTokenFromFile(std::string_view filename);

    pr_option::variables_map vm;
};

#endif
