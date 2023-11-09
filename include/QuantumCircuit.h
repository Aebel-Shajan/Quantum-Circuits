#ifndef QuantumCircuit_H
#define QuantumCircuit_H
#include "Matrix.h"
#include "QuantumComponent.h"
#include <iostream>
#include <vector>
#include <bitset>
#include <memory>
#include <iterator>

/**
 * @brief QuantumCircuit class. Creates a circuit from individual
 * QuantumComponents using the add_component() function. Has n amount of
 * registers to which quantum gates can be add, where n = register_size. The
 * number of steps the circuit has is equal to the variable total_steps. Use the
 * draw_circuit() function to print the circuit to the console.
 */
class QuantumCircuit
{
private:
    std::vector<std::vector<std::shared_ptr<QuantumComponent>>> components;
    size_t register_size;
    size_t total_steps=-1;
    std::vector<int> input_register;

public:
    // Constructor and destructor
    QuantumCircuit(size_t register_size);
    ~QuantumCircuit();

    // Accessors
    Matrix get_initial_state() const;
    Matrix get_final_state() const;
    Matrix get_state_after_step(size_t step_index) const;
    size_t get_register_size() const;
    size_t get_total_steps() const;
    Matrix get_matrix_at_step(size_t step_index) const;
    Matrix get_matrix() const;
    bool step_contains_multigate(size_t step_index) const;
    std::shared_ptr<QuantumComponent> get_multigate_at_step(size_t step_index)
        const;
    bool is_step_empty(size_t step_index) const;
    bool is_gate_in_circuit(std::shared_ptr<QuantumComponent>) const;

    // Functions to draw output to console
    void draw_circuit() const;
    void draw_probability_distribution() const;

    // Mutators
    void set_input_register(std::vector<int> input_register);
    void add_component(std::shared_ptr<QuantumComponent> gate);
    void replace_component(std::shared_ptr<QuantumComponent> gate,
        size_t register_index,
        size_t step_index);
    void evolve();
    void evolve(size_t step_number);
    void ask_for_input();
    void test_circuit();

};

// Non Member functions
std::string get_binary_representation(int number, int register_size);
Matrix calculate_matrix_for_register(std::vector<int> register_values);
bool is_power_of_two(int number);
void draw_state(Matrix state);
#endif
