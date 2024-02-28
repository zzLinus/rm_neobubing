#include <iostream>

#include "io.hpp"
#include "thread"

int main() {
    Io::Io_handler io;
    std::thread io_t(&Io::Io_handler::task, io);
    io_t.join();
    return 0;
}
