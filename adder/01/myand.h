#include "systemc.h"

SC_MODULE(myand){
    sc_in<bool> A, B;
    sc_out<bool> CO;

    void do_and(){
        CO = A & B;
    }

    SC_CTOR(myand){
        SC_METHOD(do_and)
        sensitive << A << B;
    }

};
