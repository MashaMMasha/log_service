#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace boost::beast;

class HttpSession : public std::enable_shared_from_this<HttpSession> {
    tcp::socket socket_;
    flat_buffer buffer_;
    http::request<http::string_body> request_;

public:
    explicit HttpSession(tcp::socket socket) : socket_(std::move(socket)) {}

    void start() {}
private:
    void read_request() {
        std::shared_ptr<HttpSession> self = shared_from_this();
        http::async_read(socket_, buffer_, request_,
                                       [self](boost::system::error_code error, std::size_t bytes_transferred) {
                                           if (!error) {
                                               self->process_request();
                                           }
                                       });
    }
    void process_request() {
        http::response<http::string_body> response;
        try {
            // parse request body and validate it
        } catch (...) {
            response.result(http::status::bad_request);
            response.body() = "Invalid request body";
        }
        send_response(response);
    }

    void send_response(http::response<http::string_body> response) {
        std::shared_ptr<HttpSession> self = shared_from_this();
        http::async_write(socket_, response,
                                        [self](boost::system::error_code error, std::size_t bytes_transferred) {
                                            self->socket_.shutdown(tcp::socket::shutdown_send, error);
                                        });
    }
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