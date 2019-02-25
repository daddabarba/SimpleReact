//
// Created by daddabarba on 25/02/19.
//

#ifndef RLIB_RL_AGENT_H
#define RLIB_RL_AGENT_H

#include "Defs.hpp"

#include "../ReactiveCPP/Defs.hpp"
#include "../ReactiveCPP/Variable.hpp"

#include <armadillo>
#include <random>

RLIB_INTERFACES_START

class RL_Agent {

public:

    RL_Agent(Space_Size state_space_size, Space_Size action_space_size);

    // RL algorithms
    virtual void setPolicy() = 0;

    // Data retrieval
    Action policy(State);
    Action policy();

    // Getters and Setters

    Space_Size get_S_size();
    Space_Size get_A_size();

    arma::Mat<double> getP();

    State get_current_state();
    void set_current_state(State);


protected:

    Space_Size state_space_size, action_space_size;

    REACT_CONC::Variable<arma::Mat<double>> P;
    REACT_CONC::Variable<State> current_state;

    std::mt19937 generator;
};

RLIB_INTERFACES_END
#endif //RLIB_RL_AGENT_H
