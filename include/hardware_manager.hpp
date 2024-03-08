#include "hardware_list_base.hpp"
namespace Hardware
{
    /** hardware manager **/
    template<typename... List>
    class Hardware_manager
    {
       public:
        ListBase::List_base<0, List...> hardware;

        template<typename... Args>
        explicit Hardware_manager(Args &&...args) : hardware(args...) {
        }

        template<size_t idx, typename... Args>
        auto get(Args &&...args) {
            return ListBase::get<idx>(hardware, args...);
        }

        template<size_t idx, typename... Args>
        void send(Args &&...args) {
            ListBase::send<idx>(hardware, args...);
        }

        template<size_t idx, typename... Args>
        [[maybe_unused]] void register_callback(Args &&...args) {
            ListBase::register_callback<idx>(hardware, args...);
        }

        void join() {
            ListBase::join(hardware);
        }
    };
}  // namespace Hardware