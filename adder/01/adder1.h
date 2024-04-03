#include "systemc.h"
#include "myand.h"
#include "myxor.h"

SC_MODULE(adder1) {
    sc_in<bool> A, B;
    sc_out<bool> S, CO;
    myand n1;
    myxor x1;
    sc_signal<bool> S1, S2;

    SC_CTOR(adder1): n1("AND"), x1("XOR") {
        n1.A(A);
        n1.B(B);
        n1.CO(CO);

        x1.A(A);
        x1.B(B);
        x1.S(S);
    }
};
