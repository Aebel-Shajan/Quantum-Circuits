#ifndef QuantumCircuit_H
#define QuantumCircuit_H

#include "Matrix.h"
#include "QuantumGate.h"
#include <iostream>
#include <vector>
#include <bitset>

/**
 * @brief QuantumCircuit class. Creates a circuit from individual QuantumGates using the add_component() function.
 *  Has n amount of registers to which quantum gates can be add, where n = register_size. The number of steps the 
 * circuit has is equal to the variable total_steps. Use the draw() function to print the circuit to the console.
 * Use the input_register() function to input a register of zero and one qubits into the circuit.
 * 
 */
class QuantumCircuit {
private:
    std::vector<std::vector<QuantumGate*>> components;
    size_t register_size;
    size_t total_steps = -1;
public:
    // constructor and destructor
    QuantumCircuit(size_t register_size_in) {
        register_size = register_size_in;
        components.resize(register_size);
        evolve();
    }
    ~QuantumCircuit() {
        std::cout<<"QuantumCircuit destructor called!"<<std::endl;
        for (size_t i = 0; i < register_size; i++) {
            for (size_t j = 0; j < components[i].size(); j++) {
                delete components[i][j];
            }
        }   
    }
    // accessors
    size_t get_register_size() const;
    size_t get_total_steps() const;
    QuantumGate* get_controlled_gate_at_step(size_t) const;
    Matrix get_matrix_at_step(size_t) const;
    Matrix get_matrix() const;
    bool is_step_empty(size_t step_index) const;
    bool is_step_controlled(size_t) const;
    bool is_gate_in_circuit(QuantumGate*) const;
    void draw() const;
    void input_register(std::vector<int>) const;

    // mutators
    void add_component(QuantumGate*) ;
    void replace_component(QuantumGate*, size_t, size_t) ;
    void evolve() ;
    void evolve(size_t) ;
};

#endif
