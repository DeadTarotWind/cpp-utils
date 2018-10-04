#include <type_traits>
#include <chrono>
#include <utility>

namespace cpputils
{

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static auto execute(F&& func, Args&&... args) ->
        std::enable_if_t<!std::is_void_v<decltype(func(args...))>, std::pair<typename TimeT::rep, decltype(func(args...))>>
    {
        auto start = std::chrono::steady_clock::now();

        auto result = std::forward<decltype(func)>(func)(std::forward<Args>(args)...);

        auto duration = std::chrono::duration_cast<TimeT>(std::chrono::steady_clock::now() - start);
        return std::make_pair(duration.count(), result);
    }

    template<typename F, typename ...Args>
    static auto execute(F&& func, Args&&... args) ->
        std::enable_if_t<std::is_void_v<decltype(func(args...))>, typename TimeT::rep>
    {
        auto start = std::chrono::steady_clock::now();

        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);

        auto duration = std::chrono::duration_cast<TimeT>(std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};

}
