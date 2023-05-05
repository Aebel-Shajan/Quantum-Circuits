#include "Matrix.h"
#include "QuantumCircuit.h"
#include <complex>

// Functions to make life easier
QuantumGate* h(size_t n) {return new HGate(n);}
QuantumGate* x(size_t n) {return new XGate(n);}
QuantumGate* y(size_t n) {return new YGate(n);}
QuantumGate* z(size_t n) {return new ZGate(n);}
QuantumGate* s(size_t n) {return new SGate(n);}
QuantumGate* c(QuantumGate* gate, size_t n) {return new ControlledGate(gate, n);}

int main() {
    /* Do not do this! Can only use dynamic memory allocation with pointers.
    Controlled_Gate cnot(new HGate(3), 0);
    qc.add_component(&cnot);

    Could've used shared pointers instead of raw pointers to fix this.
    */
    QuantumCircuit qc(2);
    qc.evolve();
    qc.add_component(h(1));
    qc.evolve(3);
    // qc.add_component(c(h(1), 0));
    // qc.add_component(c(h(2), 1));
    // qc.add_component(h(1));
    // qc.add_component(c(h(2), 1));
    // qc.add_component(h(2));
    qc.draw();
    std::vector<int> input_register = {0, 0};
    qc.input_register(input_register);
    return 0;
}