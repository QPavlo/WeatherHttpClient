#ifndef HttpWeatherClient_CLIENT_HPP
#define HttpWeatherClient_CLIENT_HPP

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/json.hpp>
#include <iostream>

namespace NetworkLayer {
    namespace beast = boost::beast;
    namespace http = beast::http;
    namespace ip = boost::asio::ip;
    using tcp = ip::tcp;
    using ContextPtr = std::shared_ptr<boost::asio::io_context>;

    struct IPAddress {
        std::string host;
        std::string port;
    };

    class HttpWeatherClient {
    public:
        /**
         * @param connectionIP - structure that contains host and port for connection
         * @param city - the name of the city in which you need to know the weather
         * @param token - token for connect to server
         * */
        HttpWeatherClient(const IPAddress &connectionIP,
                          const std::string &city,
                          const std::string &token);

        /**
         * @brief Connection to server
         * @returns successful connection
         * */
        void start();

        /**
         * @brief Prepare request and call send
         * */
        void send();

        /**
         * @brief Convert response to string
         * @returns Server response
         * */
        [[nodiscard]] std::string receive();

    private:

        /**
         * @brief Receive server response
         * @returns server response
         * */
        http::response<http::dynamic_body> p_receive();

        std::string city;
        std::string token;
        ContextPtr contextPtr;
        IPAddress ipAddress;
        beast::tcp_stream stream;
    };
}

#endif
