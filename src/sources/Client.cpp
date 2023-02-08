#include "../headers/Client.hpp"

namespace NetworkLayer {

    HttpWeatherClient::HttpWeatherClient(const IPAddress &connectionIP,
                                         const std::string &city,
                                         const std::string &token) :
            contextPtr(std::make_shared<boost::asio::io_context>()),
            stream{*contextPtr},
            ipAddress{connectionIP},
            city{city},
            token{token} {}


    void HttpWeatherClient::start() {
        boost::system::error_code error;
        tcp::resolver resolver(*contextPtr);
        tcp::resolver::results_type endpoints{resolver.resolve(ipAddress.host, ipAddress.port)};
        stream.connect(endpoints, error);

        if (error) {
            throw std::logic_error("Cannot resolve connection");
        }
    }

    void HttpWeatherClient::send() {
        std::string target{
                "/data/2.5/weather?q=" + city + "&appid=" + token + "&units=metric"};
        http::request<http::string_body> req{http::verb::get, target, 10};

        beast::error_code error;
        http::write(stream, req, error);

        if (error) {
            throw std::runtime_error("Bad request");
        }
    }

    [[nodiscard]] std::string HttpWeatherClient::receive() {
        auto resp{p_receive()};

        auto statusCode{static_cast<int32_t>(resp.result())};
        if (statusCode != 200) {
            if (statusCode == 404) {
                throw std::logic_error("Can't get weather in " + city);
            } else if (statusCode == 401) {
                throw std::runtime_error("Invalid api key");
            }
            throw std::runtime_error("Bad response");
        }

        std::string resp_str{boost::asio::buffers_begin(resp.body().data()),
                             boost::asio::buffers_end(resp.body().data())};


        try {
            auto temperature{std::to_string(boost::json::parse(resp_str).at("main").at("temp").to_number<double>())};
            auto wind_speed{std::to_string(boost::json::parse(resp_str).at("wind").at("speed").to_number<double>())};
            auto wind_direction{boost::json::parse(resp_str).at("wind").at("deg").to_number<int32_t>()};

            std::string receive_str{"City: " + city + "\n" +
                                    "Temperature: " + temperature.substr(0, temperature.find('.') + 3) + "°C\n" +
                                    "Wind's speed: " + wind_speed.substr(0, wind_speed.find('.') + 3) + " meter/sec\n" +
                                    "Wind's direction: " + std::to_string(wind_direction) + "°\n"};

            return receive_str;
        } catch (const boost::exception &exception) {
            throw std::runtime_error("Response parse error");
        }
    }

    http::response<http::dynamic_body> HttpWeatherClient::p_receive() {
        http::response<http::dynamic_body> resp;
        beast::flat_buffer buffer;
        beast::error_code error;

        http::read(stream, buffer, resp, error);
        stream.socket().shutdown(tcp::socket::shutdown_both);

        if (error) {
            throw std::runtime_error("Response error");
        }
        return resp;
    }
}
