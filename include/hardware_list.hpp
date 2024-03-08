#include "ctype.h"
#include "functional"
#include "map"
#include "memory"
#include "thread"

namespace Hardware
{
    /** callback type define **/
    template<typename Rt, typename... Args>
    struct Callback_type;

    template<typename Rt, typename Key, typename... Args>
    struct Callback_type<std::function<Rt(Key, Args...)>>
    {
        using RtType = Rt;
        using KeyType = Key;
        using CallbackType = std::function<Rt(Args...)>;
    };

    template<typename Rt, typename Key, typename... Args>
    struct Callback_type<const std::function<Rt(Key, Args...)> &>
    {
        using RtType = Rt;
        using KeyType = Key;
        using CallbackType = std::function<Rt(Args...)>;
    };

    /** function type define **/
    template<typename Rt, typename... Args>
    struct Fun_type;

    template<class C, typename Rt, typename Arg>
    struct Fun_type<Rt (C::*)(Arg)>
    {
        using RtType = Rt;
        using ArgType = Arg;
    };

    template<typename Rt, typename Key, typename... Args>
    std::function<Rt(Key, Args...)> get_callback(std::map<Key, std::function<Rt(Args...)>> &callback_map) {
        return [&](const Key &key, Args &&...args) -> Rt {
            if (!callback_map.count(key)) {
                return Rt();
            } else {
                return (callback_map[key])(args...);
            }
        };
    }

    /** list base define **/
    template<size_t idx, typename... List>
    class List_base;

    template<size_t idx, typename Head, typename... Tail>
    class List_base<idx, Head, Tail...> : public List_base<idx + 1, Tail...>
    {
       public:
        using Inherited = List_base<idx + 1, Tail...>;
        using Type = Head;
        using callbackType = typename Fun_type<decltype(&Head::set_callback)>::ArgType;
        using CallbackRt = typename Callback_type<callbackType>::RtType;
        using CallbackKey = typename Callback_type<callbackType>::KeyType;
        using CallbackFun = typename Callback_type<callbackType>::CallbackType;

        std::map<CallbackKey, CallbackFun> callback_map;
        Head *val;
        std::shared_ptr<std::thread> thread;

        explicit constexpr List_base(Head &head, Tail &...tail) : Inherited(tail...) {
            val = &head;
            head.set_callback(get_callback(callback_map));
            thread = std::make_shared<std::thread>(&Head::task, &head);
        }

        template<typename... Args>
        void send(Args &&...args) {
            val->send(args...);
        }

        void register_callback(const CallbackKey &key, const CallbackFun &fun) {
            callback_map[key] = fun;
        }
    };

    template<size_t idx, typename Head>
    class List_base<idx, Head>
    {
       public:
        using Inherited = List_base<idx + 1>;
        using Type = Head;
        using callbackType = typename Fun_type<decltype(&Head::set_callback)>::ArgType;
        using CallbackRt = typename Callback_type<callbackType>::RtType;
        using CallbackKey = typename Callback_type<callbackType>::KeyType;
        using CallbackFun = typename Callback_type<callbackType>::CallbackType;

        std::map<CallbackKey, CallbackFun> callback_map;
        Head *val;
        std::shared_ptr<std::thread> thread;

        explicit constexpr List_base(Head &head) {
            val = &head;
            head.set_callback(get_callback(callback_map));
            thread = std::make_shared<std::thread>(&Head::task, &head);
        }

        template<typename... Args>
        void send(Args &&...args) {
            val->send(args...);
        }

        void register_callback(const CallbackKey &key, const CallbackFun &fun) {
            callback_map[key] = fun;
        }
    };

    /** hardware list define **/
    template<typename... List>
    class Hardware_list : public List_base<0, List...>
    {
       public:
        using Inherited = List_base<0, List...>;

        explicit constexpr Hardware_list(List &...list) : Inherited(list...) {
        }
    };

    /** send define **/
    template<size_t idx, typename Head, typename... Tail, typename... Args>
    void send_t(List_base<idx, Head, Tail...> &list, Args &&...args) {
        list.send(args...);
    }

    template<size_t idx, typename... List, typename... Args>
    void send(Hardware_list<List...> &list, Args &&...args) {
        send_t<idx>(list, args...);
    }

    /** join define**/

    template<size_t idx, typename Head, typename Sec, typename... Tail>
    void join(List_base<idx, Head, Sec, Tail...> &list) {
        join<idx + 1, Sec, Tail...>(list);
        list.thread->join();
    }

    template<size_t idx, typename Head>
    void join(List_base<idx, Head> &list) {
        list.thread->join();
    }

    /** get define **/
    template<size_t idx, typename Head, typename... Tail>
    Head *get(List_base<idx, Head, Tail...> &list) {
        return list.val;
    }

    /** register callback define **/
    template<size_t idx, typename Head, typename... Tail>
    void register_callback(
        List_base<idx, Head, Tail...> &list,
        const typename List_base<idx, Head, Tail...>::CallbackKey &key,
        const typename List_base<idx, Head, Tail...>::CallbackFun &fun) {
        list.register_callback(key, fun);
    }

}  // namespace Hardware