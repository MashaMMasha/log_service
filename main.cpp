#include <thread>
#include "src/api/http_server.h"

int main() {
    int thread_count = std::thread::hardware_concurrency();
    run_http_server(thread_count);
}