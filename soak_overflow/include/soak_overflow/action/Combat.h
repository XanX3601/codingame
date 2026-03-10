#ifndef SOAK_OVERFLOW_ACTION_COMBAT_H
#define SOAK_OVERFLOW_ACTION_COMBAT_H

#include <variant>

#include <soak_overflow/action/Shoot.h>
#include <soak_overflow/action/Throw.h>

namespace soak_overflow::action
{
    using Combat = std::variant<Shoot, Throw>;
}

#endif
