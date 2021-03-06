//
// Created by daddabarba on 26/02/19.
//

#include "RL_Agent.hpp"
#include "../Defs.hpp"

#include "../../ReactiveCPP/Operator.hpp"
#include "../../ReactiveCPP/ParameterNode.hpp"

RLIB_BASES::RL_Agent::RL_Agent(Space_Size state_space_size, Space_Size action_space_size, double alpha, double gamma) :
        alpha(alpha),
        gamma(gamma),
        state_space_size(state_space_size),
        action_space_size(action_space_size),
        P(REACT_CONC::make_variable<arma::Mat<double>>(arma::Mat<double>(arma::randu(action_space_size, 1)))),
        current_state(REACT_CONC::make_variable<State>(0)),
        clock(REACT_CONC::make_variable<unsigned long>(0)),
        generator((unsigned long)time(nullptr))
{
    REACT_CONC::make_operator_set(
            [](State, unsigned long current_time) -> unsigned long {
                return current_time + 1;
            },
            get_clock(),
            get_current_state(),
            REACT_CONC::make_parameter(get_clock()));
};

auto RLIB_BASES::RL_Agent::policy(State state) -> Action {
    this->set_current_state(state);
    return this->policy();
}

auto RLIB_BASES::RL_Agent::policy() -> Action {
    auto M = getP()->get();

    return ((Action)std::discrete_distribution<int>(
            M.size(),
            0.0,
            1.0*M.size(),
            [M](double i) -> double{
                return M[(unsigned int)ceil(i-0.5)];
            }
    )(get_generator()));
}

auto RLIB_BASES::RL_Agent::get_alpha() -> const double {
    return this->alpha;
}

auto RLIB_BASES::RL_Agent::get_gamma() -> const double {
    return this->gamma;
}

auto RLIB_BASES::RL_Agent::get_S_size() -> const Space_Size{
    return this->state_space_size;
}

auto RLIB_BASES::RL_Agent::get_A_size() -> const Space_Size{
    return this->action_space_size;
}

auto RLIB_BASES::RL_Agent::getP() -> REACT_CONC::Variable<arma::Mat<double>>*{
    return this->P;
}

auto RLIB_BASES::RL_Agent::get_current_state() -> REACT_CONC::Variable<State>* {
    return this->current_state;
}

auto RLIB_BASES::RL_Agent::set_current_state(State state) -> void {

    if (state<0 || state>=this->state_space_size)
        return;

    this->current_state->set(state);
}

auto RLIB_BASES::RL_Agent::get_clock() -> REACT_CONC::Variable<unsigned long>* {
    return this->clock;
}

auto RLIB_BASES::RL_Agent::get_generator() -> std::mt19937&{
    return std::forward<std::mt19937&>(this->generator);
}