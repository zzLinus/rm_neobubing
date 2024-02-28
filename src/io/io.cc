#include "io.hpp"
#include <chrono>
#include <thread>

namespace Io
{
    Io_handler::Io_handler() {
        kb = new Input::Kb_ctrl();
        ui = new Ui::Ncurses_ui(
            [&](Types::Kb_event event, fp32 spdslider) { return kb->event_handler(event, spdslider); });
    }

    Io_handler::~Io_handler() {
        delete ui;
        delete kb;
    }

    void Io_handler::task() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            ui->render();
        }
    }
}  // namespace Io
