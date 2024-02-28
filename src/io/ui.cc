#include "ui.hpp"

namespace Ui
{
    Ncurses_ui::Ncurses_ui(const InputHandler &event_handler) {
        input_handler = event_handler;
        debug = new Types::debug_info_t;
        cur_speed_x = 0;
        cur_speed_y = 0;
    }

    Ncurses_ui::~Ncurses_ui() {
        delete debug;
    }

    void Ncurses_ui::render() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        auto screen = ImTui_ImplNcurses_Init(true);
        ImTui_ImplText_Init();
        uint64_t last = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::high_resolution_clock::now().time_since_epoch())
                            .count();
        uint64_t last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                              std::chrono::high_resolution_clock::now().time_since_epoch())
                              .count();
        // NOTE: UI and keyboard event
        while (true) {
            float spdslider;
            uint64_t now = std::chrono::duration_cast<std::chrono::milliseconds>(
                               std::chrono::high_resolution_clock::now().time_since_epoch())
                               .count();
            ImTui_ImplNcurses_NewFrame();
            ImTui_ImplText_NewFrame();

            ImGui::NewFrame();

            ImGui::SetNextWindowPos(ImVec2(4, 27), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(50.0, 15.0), ImGuiCond_Once);
            ImGui::Begin("Debug");
            ImGui::Text(
                "Time per frame %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

            ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(1, 1, 0, 1));
            ImGui::Text("velocity : vx vy wz");
            ImGui::Text(" %f %f %f", debug->vx, debug->vy, debug->wz);
            // ImGui::SliderFloat(
            //     "speed percent",
            //     &spdslider,
            // Config::PIDCTRL_MAX_SPEED / 2,
            // Config::PIDCTRL_MAX_SPEED,
            //"ratio = %.3f");
            ImGui::TextColored(ImVec4(1, 1, 1, 1), "can pkg : %0lx", debug->pkg);

            ImGui::Text("time %lu %lu %lu", last, now, now - last);

            ImGui::Text("Hi mon! %d: ", debug->err);
            ImGui::Text("PID %s: ", debug->debuginfo1.c_str());
            ImGui::Text("PID %s: ", debug->debuginfo2.c_str());
            ImGui::Text("PID %s: ", debug->debuginfo3.c_str());
            ImGui::Text(
                "candump id :%x\n dlc : %d \n%02x %02x %02x %02x %02x %02x %02x %02x",
                debug->can_f.can_id,
                debug->can_f.can_dlc,
                debug->can_f.data[0],
                debug->can_f.data[1],
                debug->can_f.data[2],
                debug->can_f.data[3],
                debug->can_f.data[4],
                debug->can_f.data[5],
                debug->can_f.data[6],
                debug->can_f.data[7]);

            if (ImGui::IsKeyPressed('w', true) && now - last > 70) {
                cur_speed_y = input_handler(Types::Kb_event::UP, spdslider);
                last = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::high_resolution_clock::now().time_since_epoch())
                           .count();
            }
            if (ImGui::IsKeyPressed('r', true) && now - last > 70) {
                cur_speed_y = input_handler(Types::Kb_event::DOWN, spdslider);
                last = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::high_resolution_clock::now().time_since_epoch())
                           .count();
            }

            if (ImGui::IsKeyPressed('a', true) && now - last_x > 70) {
                cur_speed_x = input_handler(Types::Kb_event::LEFT, spdslider);
                last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch())
                             .count();
            }
            if (ImGui::IsKeyPressed('s', true) && now - last_x > 70) {
                cur_speed_x = input_handler(Types::Kb_event::RIGHT, spdslider);
                last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch())
                             .count();
            }

            // NOTE: dec velocity over time
            if (now - last > 300) {
                cur_speed_y = input_handler(Types::Kb_event::STOP_Y, 0);
                last = std::chrono::duration_cast<std::chrono::milliseconds>(
                           std::chrono::high_resolution_clock::now().time_since_epoch())
                           .count();
            }
            if (now - last_x > 300) {
                cur_speed_x = input_handler(Types::Kb_event::STOP_X, 0);
                last_x = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::high_resolution_clock::now().time_since_epoch())
                             .count();
            }

            // FIXME:
            if (ImGui::IsKeyPressed('q', true)) {
                input_handler(Types::Kb_event::SPIN_L, spdslider);
            }
            if (ImGui::IsKeyPressed('f', true)) {
                input_handler(Types::Kb_event::SPIN_R, spdslider);
            }

            ImGui::PopStyleColor(1);

            ImGui::SameLine();

            ImGui::End();

            ImGui::SetNextWindowPos(ImVec2(30, 5), ImGuiCond_Once);
            ImGui::SetNextWindowSize(ImVec2(50.0, 10.0), ImGuiCond_Once);
            ImGui::Begin("Chassis info");
            ImGui::Text("Current direction:");
            if (cur_speed_y > 0) {
                ImGui::Text("  ^");
            } else {
                ImGui::Text("");
            }

            if (cur_speed_x < 0) {
                ImGui::Text("<");
            } else if (cur_speed_x > 0) {
                ImGui::Text("    >");
            } else {
                ImGui::Text("");
            }

            if (cur_speed_y < 0) {
                ImGui::Text("  v");
            } else {
                ImGui::Text("");
            }

            // [3]  [0]
            //
            // [2]  [1]
            int index[] = { 3, 0, 2, 1 };

            for (int i = 0; i < 4; i++) {
                if (debug->wheel_speed[index[i]] != 0.0) {
                    debug->wheel_speed[index[i]] /= 2 * spdslider;
                }

                if (debug->wheel_speed[index[i]] > 0) {
                    ImGui::ProgressBar(debug->wheel_speed[index[i]], ImVec2(10.0f, 1.0f));
                } else {
                    ImGui::ProgressBar(-debug->wheel_speed[index[i]], ImVec2(10.0f, 1.0f));
                }
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                ImGui::Text("Motor %d", index[i]);
                if (i % 2 == 0) {
                    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
                } else {
                    ImGui::Text("");
                }
            }

            ImGui::End();

            ImGui::Render();

            ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen);
            ImTui_ImplNcurses_DrawScreen();
        }

        ImTui_ImplText_Shutdown();
        ImTui_ImplNcurses_Shutdown();
    }

}  // namespace Ui
