#include <iostream>

#include "io.hpp"
#include "thread"

#include <memory>

int main() {
    std::shared_ptr<int> p = std::make_unique<int>(0);

    Io::Io_handler io;
    std::thread io_t(&Io::Io_handler::task, io);
    io_t.join();
    return 0;
}
