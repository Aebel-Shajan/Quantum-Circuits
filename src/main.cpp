#define _USE_MATH_DEFINES
#include "Matrix.h"
#include "QuantumCircuit.h"
#include "DerivedGates.h"
#include <iostream>
#include <memory>
#include <cmath>



///////////////////////////////////////////////////////////////////////////////
// Main function
///////////////////////////////////////////////////////////////////////////////

int main() {
    QuantumCircuit qc(3);
    qc.add_component(toffoli(2, 0, 1));
    qc.add_component(controlled(x(1), 0));
    qc.add_component(x(0));
    qc.add_component(x(1));
    qc.add_component(toffoli(2, 0, 1));
    qc.add_component(x(1));
    qc.add_component(controlled(x(1), 0));

    qc.draw_circuit();
    qc.draw_probability_distribution();
    qc.test_circuit();
    return 0;
}


// Test Functions
void print_test_result(std::string test_name, bool test_result) 
{
    std::cout << test_name << ":";
    if (test_result) {
        std::cout << " test passed." << std::endl;
    } else {
        std::cout << " test failed.xxxxxxxxxxxxxx" << std::endl;
    }
}

void check_hadamard() {
    QuantumCircuit qc(1);
    qc.add_component(h(0));
    Matrix expected=Matrix(2, 1);
    expected(0, 0)=1/sqrt(2);
    expected(1, 0)=1/sqrt(2);
    Matrix result=qc.get_final_state();
    draw_state(qc.get_initial_state());
    std::cout<<"->";
    draw_state(expected);
    print_test_result("Hadamard", expected==result);
}

void check_swap(std::vector<int> input_register) {
    QuantumCircuit qc(2);
    qc.add_component(swap(0,1));
    qc.set_input_register(input_register); // |01>
    std::vector<int> expected_register = {input_register[1], input_register[0]};
    Matrix expected=calculate_matrix_for_register(expected_register);
    Matrix result=qc.get_final_state();
    draw_state(qc.get_initial_state());
    std::cout<<"->";
    draw_state(expected);
    print_test_result("Swap", expected==result);
}

void check_toffoli(std::vector<int> input_register) {
    QuantumCircuit qc(3);
    qc.add_component(toffoli(2, 0, 1));// not at 2 controlled by 0 and 1
    qc.set_input_register(input_register);
    std::vector<int> expected_register = {input_register[0], input_register[1], input_register[2]};
    if (input_register[0] &&input_register[1]) {
        expected_register[2]=!expected_register[2];
    }
    Matrix expected=calculate_matrix_for_register(expected_register);
    Matrix result=qc.get_final_state();
    draw_state(qc.get_initial_state());
    std::cout<<"->";
    draw_state(qc.get_final_state());
    print_test_result("Toffoli", expected==result);
}

// Example circuits
QuantumCircuit full_adder_circuit()
{
    QuantumCircuit full_adder(4);
    full_adder.add_component(toffoli(3, 0, 1));
    full_adder.add_component(controlled(x(1), 0));
    full_adder.add_component(toffoli(3, 1, 2));
    full_adder.add_component(controlled(x(2), 1));
    full_adder.add_component(controlled(x(1), 0));
    return full_adder;
}

QuantumCircuit qft_circuit(size_t n)
{
    QuantumCircuit qft(n);
    for (int j{}; j<n; j++ ) {
        for (int k{}; k<j; k++) {
            double theta = M_PI/pow(2, j-k);
            qft.add_component(controlled(p(k, theta), j));
        }
        qft.add_component(h(j));
    }
    for (int i{}; i<floor(n/2); i++) {
        qft.add_component(swap(i, n-i-1));
    }
    return qft;
}


