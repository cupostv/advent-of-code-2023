#pragma once

#include <type_traits>
#include <ranges>

namespace helper {

    bool find(const std::ranges::common_range auto & container,
              const std::ranges::range_value_t<decltype(container)> value) {
        return std::ranges::find(container, value) != container.end();
    }

}
