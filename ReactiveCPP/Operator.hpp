//
// Created by daddabarba on 2/1/19.
//

#ifndef REACT_OPERATOR_H
#define REACT_OPERATOR_H

#include "Defs.hpp"

#include <utility>
#include <functional>
#include <tuple>

#include "Notification.hpp"
#include "utils.hpp"

REACT_CONC_START


// ABSTRACT OPERATOR

template<
    typename TFun,
    typename TOut,
    typename ... TIn
>
class Operator : public Observer{

public:

    using RetType = typename std::result_of<TFun(TIn...)>::type;

    Operator(TFun&& fun, Variable<TOut>&& output, Variable<TIn>&& ... args) :
        output(output),
        expression(fun),
        arguments(std::forward<TIn*>((TIn*)args)...)
    {
        this->evaluate();
        link_predecessors(std::forward<Variable<TIn>>(args)...);
    };

    template<typename T>
    auto link_predecessors(Variable<T>&& pred) -> void{
        pred.link_successor(this);
    }

    template<typename T, typename ...Ts>
    auto link_predecessors(Variable<T>&& pred, Variable<Ts>&& ...others ) -> void {
        link_predecessors(std::forward<Variable<T>>(pred));
        link_predecessors(std::forward<Variable<Ts>>(others)...);
    }

protected:

    Variable<TOut>& output;
    std::function<RetType(TIn...)> expression;
    std::tuple<TIn*...> arguments;

    auto has_changed() -> void override {
        this->evaluate();
    }

    auto evaluate() -> void {
        this->evaluate(SEQ::gen_seq<std::tuple_size<decltype(this->arguments)>::value>());
    };

    template<int ...Is>
    auto evaluate(SEQ::seq<Is...>) -> void {
        this->evaluate((TOut*)output, (*std::get<Is>(this->arguments))...);
    }

    virtual void evaluate(TOut*, TIn...) = 0;

};

// OPERATOR UPDATE

template<
        typename TFun,
        typename TOut,
        typename ...TIn
>
class Operator_Update : public Operator<TFun, TOut, TIn...>{

public:

    Operator_Update(TFun&& fun, Variable<TOut>&& output, Variable<TIn>&& ... args) :
        Operator<TFun, TOut, TIn...>(
                std::forward<TFun>(fun),
                std::forward<Variable<TOut>>(output),
                std::forward<Variable<TIn>>(args)...)
    {};

protected:

    auto evaluate(TOut* out, TIn... arguments) -> void override{
        this->expression(out, arguments...);
        this->output.reload();
    }

};


// OPERATOR SET

template<
        typename TFun,
        typename TOut,
        typename ...TIn
>
class Operator_Set : public Operator<TFun, TOut, TIn...>{

public:

    Operator_Set(TFun&& fun, Variable<TOut>&& output, Variable<TIn>&& ... args) :
            Operator<TFun, TOut, TIn...>(
                    std::forward<TFun>(fun),
                    std::forward<Variable<TOut>>(output),
                    std::forward<Variable<TIn>>(args)...)
    {};

protected:

    auto evaluate(TOut*, TIn... arguments) -> void override{
        this->output.set(this->expression(arguments...));
    }

};


// CONSTRUCTOR HELPERS

template<
        typename TFun,
        typename TOut,
        typename ... TIn
>
auto make_operator_update(TFun &&fun, Variable <TOut> *output, Variable <TIn> *... args) -> Operator<TFun, TOut, TIn...>*{
    return new Operator_Update<TFun, TOut, TIn...>(
            std::forward<TFun>(fun),
            std::forward<Variable<TOut>>(std::move(*output)),
            std::forward<Variable<TIn>>(std::move(*args))...
    );
}

template<
        typename TFun,
        typename TOut,
        typename ... TIn
>
auto make_operator_set(TFun &&fun, Variable <TOut> *output, Variable <TIn> *... args) -> Operator<TFun, TOut, TIn...>*{
    return new Operator_Set<TFun, TOut, TIn...>(
            std::forward<TFun>(fun),
            std::forward<Variable<TOut>>(std::move(*output)),
            std::forward<Variable<TIn>>(std::move(*args))...
    );
}

REACT_CONC_END

#endif //REACT_OPERATOR_H
