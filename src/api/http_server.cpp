#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
    tcp::socket socket_;
    boost::beast::flat_buffer buffer_;
    boost::beast::http::request<boost::beast::http::string_body> request_;

public:
    explicit HttpSession(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {}
};

class HttpServer {
    io_context& ioc_;
    ip::tcp::acceptor acceptor_;

public:
    HttpServer(io_context& ioc, short port) : ioc_(ioc), acceptor_(ioc, tcp::endpoint(tcp::v4(), port)) {
        accept();
    }

private:
    void accept() {
        acceptor_.async_accept([this](boost::system::error_code error, tcp::socket socket) {
            if (!error) {
                std::make_shared<HttpSession>(std::move(socket))->start();
            }
            accept();
        });
    }
};
void run_http_server(int thread_count) {
    io_context ioc;
    HttpServer server(ioc, 8080);

    std::vector<std::thread> threads;
    for (int i = 0; i < thread_count; ++i) {
        threads.emplace_back([&ioc] { ioc.run(); });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}