#include "../headers/Application.hpp"

Application::Application(int argc, char *argv[]) {
    try {
        pr_option::options_description desc{"Options"};
        desc.add_options()
                ("help,h", "Show help")
                ("city,c", pr_option::value<std::string>(),
                 "Enter city (If the city name is divided into several words - use _ instead of space)")
                ("port,p", pr_option::value<std::string>(), "Enter port");

        pr_option::store(parse_command_line(argc, argv, desc), vm);
        pr_option::notify(vm);

        if (vm.count("help") || vm.empty()) {
            std::cout << desc << '\n';
            exit(0);
        }
    }
    catch (const pr_option::error &ex) {
        std::cerr << ex.what() << '\n';
        exit(0);
    }

}

std::string Application::readTokenFromFile(std::string_view filename) {
    std::string token;
    std::ifstream file;
    file.exceptions(std::ifstream::badbit);
    try {
        file.open(filename.data());
        std::string line;
        while (std::getline(file, line)) {
            token.append(line);
        }
    } catch (const std::ifstream::failure &e) {
        std::cerr << e.what() << '\n';
    }
    file.close();
    return token;
}

int Application::exec() {
    std::string city;
    std::string token;
    NetworkLayer::IPAddress ipAddress{"api.openweathermap.org", "80"};

    if (vm.count("city")) {
        city = vm["city"].as<std::string>();
    }

    if (vm.count("port")) {
        ipAddress.port = vm["port"].as<std::string>();
    }

    token = readTokenFromFile("token.txt");

    std::string weatherResp;

    NetworkLayer::HttpWeatherClient weatherClient{ipAddress, city, token};

    try {
        weatherClient.start();
        weatherClient.send();
        weatherResp = weatherClient.receive();
        std::cout << weatherResp << '\n';
    } catch (const std::exception &exception) {
        std::cerr << exception.what();
    } catch (...) {
        std::cerr << "unknown exception\n";
    }
    return 0;
}