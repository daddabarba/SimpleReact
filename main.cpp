#include <iostream>
#include <armadillo>

#include "ReactiveCPP/Defs.h"

#include "ReactiveCPP/Variable.h"
#include "ReactiveCPP/Operator.h"
#include "ReactiveCPP/ParameterNode.h"


int main() {

    auto A = REACT_CONC::make_variable<arma::Mat<double>>(arma::randu(3, 3)*2 -1);
    auto B = REACT_CONC::make_variable<arma::Mat<double>>(arma::ones(3, 3)*2);
    auto C = REACT_CONC::make_variable<arma::Mat<double>>(arma::ones(3, 3)*3);

    auto X = REACT_CONC::make_variable<arma::Mat<double>>();

    REACT_CONC::make_operator(
            [](arma::Mat<double> a, arma::Mat<double> b, arma::Mat<double> c) -> arma::Mat<double> {
                return (a * b) + c;
            },
            REACT_CONC::ANY,
            X, A, B, REACT_CONC::make_parameter(*C)
    );

    std::cout << "Armadillo test"<<std::endl;

    std::cout<<"(a: \n" << A->get()<<"\n + b: \n" <<B->get() << "\n) * c: \n" << C->get()<< "\n = x: \n" << X->get()<< "\n\n" << std::endl;
    A->set(A->get().for_each([](arma::mat::elem_type& val)->void{  if(val<0){val = 0;}}));
    A->set(A->get()*2);
    std::cout<<"(a: \n" << A->get()<<"\n + b: \n" <<B->get() << "\n) * c: \n" << C->get()<< "\n = x: \n" << X->get()<< "\n\n" << std::endl;
    C->set(C->get()/3);
    std::cout<<"(a: \n" << A->get()<<"\n + b: \n" <<B->get() << "\n) * c: \n" << C->get()<< "\n = x: \n" << X->get()<< "\n\n" << std::endl;
    A->set(A->get()/2);
    std::cout<<"(a: \n" << A->get()<<"\n + b: \n" <<B->get() << "\n) * c: \n" << C->get()<< "\n = x: \n" << X->get()<< "\n\n" << std::endl;



    return 0;
}