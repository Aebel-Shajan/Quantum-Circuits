#include "QuantumCircuit.h"


// Helper functions
/**
 * @brief Returns a string of the binary representation of input number num 
 * with size number of bits.
 * 
 * @param num 
 * @param size 
 * @return std::string 
 */
std::string binary_string(int num, int size){
    return std::bitset<32>(num).to_string().substr(32-size);
}


// QuantumCircuit class
size_t QuantumCircuit::get_register_size() const {
    return register_size;
}
size_t QuantumCircuit::get_total_steps() const {
    return total_steps;
}

/**
 * @brief Returns true if there is a controlled gate at given step.
 * 
 * @param step_index 
 * @return QuantumGate* 
 * @throws std::invalid_argument if step_index >= total_steps.
 * @throws std::logic_error if no control gate is found at step.
 */
QuantumGate* QuantumCircuit::get_controlled_gate_at_step(size_t step_index) const {
    if (step_index >= total_steps) {
        throw std::invalid_argument("Error: step index out of range! ");
    }
    for (size_t i = 0; i < register_size; i++) {
        if (components[i][step_index]->is_gate_controlled()) {
            return components[i][step_index];
        }
    }
    throw std::logic_error("Error: no control gate found at step! ");
}

/**
 * @brief Returns the tensor product representing all gates at a given step.
 * 
 * @param step_index 
 * @return Matrix 
 */
Matrix QuantumCircuit::get_matrix_at_step(size_t step_index) const {
    if (is_step_controlled(step_index)) {
        QuantumGate* control_gate = get_controlled_gate_at_step(step_index);
        return control_gate->get_matrix(register_size);

    }
    Matrix step_matrix = identity_matrix(1<<register_size);
    for (size_t i = 0; i < register_size; i++) {
        step_matrix = components[i][step_index]->get_matrix(register_size) * step_matrix;
    }
    return step_matrix;
}

/**
 * @brief Returns the matrix representation of the whole circuit.
 * 
 * @return Matrix 
 */
Matrix QuantumCircuit::get_matrix() const {
    Matrix circuit_matrix = get_matrix_at_step(0);
    for (size_t i = 1; i < total_steps+1; i++) {
        circuit_matrix = get_matrix_at_step(i)*circuit_matrix;
    }
    return circuit_matrix;
}

/**
 * @brief Returns true if there is a controlled gate at given step.
 * 
 * @param step_index 
 * @return true 
 * @return false 
 */
bool QuantumCircuit::is_step_controlled(size_t step_index) const {
    for (size_t i = 0; i < register_size; i++) {
        if (components[i][step_index]->is_gate_controlled()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Returns true if there are gates which are not identity gates at given step.
 * 
 * @param step_index 
 * @return true 
 * @return false 
 */
bool QuantumCircuit::is_step_empty(size_t step_index) const {
    for (size_t i = 0; i < register_size; i++) {
        if (components[i][step_index]->get_symbol() != "I") {
            return false;
        }
    }
    return true;
}

/**
 * @brief Checks if the pointer to a gate is already in the circuit. This is to avoid
 * segmenation fault when deleting gates.
 * 
 * @param step_index 
 * @return true 
 * @return false 
 */
bool QuantumCircuit::is_gate_in_circuit(QuantumGate* gate) const {
    for (size_t i = 0; i < register_size; i++) {
        for (size_t j = 0; j < components[i].size(); j++) {
            if (components[i][j] == gate) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Draws a visual representation of the circuit in the terminal. Accounts for 
 * different types of gates and controlled gates.
 * 
 */
void QuantumCircuit::draw() const {
    for (size_t register_index = 0; register_index < register_size; register_index++) {
        for (size_t line_index = 0; line_index < 3; line_index++){
            if (line_index == 1) {
                std::cout<<"Register "<<register_index<<": ";
            } else {
                std::cout<<"            ";
            }
            for (size_t step_index = 0; step_index < components[register_index].size(); step_index++) {
                QuantumGate* gate = components[register_index][step_index];
                if(is_step_controlled(step_index)){
                    QuantumGate* control_gate = get_controlled_gate_at_step(step_index);
                    size_t gate_index = control_gate->get_index();
                    size_t control_index = control_gate->get_control_index();
                    bool in_between = \
                    register_index < control_index && register_index > gate_index || \
                    register_index > control_index && register_index < gate_index;
                    if (in_between) {
                        std::string output[3] = {"  ║  ", "──╫──", "  ║  "};
                        std::cout<<output[line_index];
                    } else if (register_index == control_index) {
                        std::cout<<control_gate->get_icon(line_index+3);
                    } else {
                        std::cout<<gate->get_icon(line_index);
                    }
                } else {
                    std::cout<<gate->get_icon(line_index);
                }
            }
        std::cout<<std::endl;
        }
    }
}

/**
 * @brief Draws the probabilities of measuring each final state with the given input
 * register. 
 * 
 * @param input_register vector of 0s and 1s representing the input qubit register.
 * @throws invalid_argument if input register size does not match circuit's register size.
 * @throws invalid_argument if input register is not a vector of 0s and 1s.
 */
void QuantumCircuit::input_register(std::vector<int> input_register) const{
    if (input_register.size() != register_size) {
        throw std::invalid_argument("Input register size does not match circuit's register size!");
    }
    for (size_t i = 0; i < register_size; i++) {
        if (input_register[i] != 0 && input_register[i] != 1) {
            throw std::invalid_argument("Input register must be a vector of 0s and 1s!");
        }
    }

    Matrix qubit_zero(2, 1);
    qubit_zero(0, 0) = 1;
    Matrix qubit_one(2, 1);
    qubit_one(1, 0) = 1;
    Matrix final_state = input_register[0] == 0 ? qubit_zero : qubit_one;
    for (size_t i = 1; i < register_size; i++) {
        Matrix next_state = input_register[0] == 0 ? qubit_zero : qubit_one;
        final_state = final_state.tensor_product(next_state);
    }
    final_state = get_matrix()*final_state;
    std::cout<<"Intial state:"<<std::endl<<"|";
    for (int i = 0; i < register_size; i++) {
        std::cout<<input_register[i];
        }
    std::cout<<">"<<std::endl;
    std::cout<<"Probabilities of final states:"<<std::endl;
    for (int i = 0; i < 1 << register_size; i++) {
        double probability = (final_state(i, 0)*final_state(i,0)).real();
        std::cout<<"|"<<binary_string(i, register_size)<<"> "<<std::to_string(probability).substr(0, 5)<<" ";
        std::cout<<" ║";
        int filled_blocks = probability*100;
        for (int j = 0; j < filled_blocks; j++) {
            std::cout<<"█";
        }
        std::cout<<std::endl;
    }
}

/**
 * @brief Adds a dynamiccally allocated pointer to a gate to the circuit. 
 * 
 * @param gate 
 * @throws invalid_argument if gate is already in circuit.
 * @throws invalid_argument if gate is not within circuit's register size.
 */
void QuantumCircuit::add_component(QuantumGate* gate) {
    if (is_gate_in_circuit(gate)) {
        throw std::invalid_argument("Gate already in circuit!");
        return;
    }
    if (!gate->is_gate_valid(register_size)) {
        throw std::invalid_argument("Gate is not within circuit's register size!");
        return;
    }
    int target_index = gate->get_index();
    if (!gate->is_gate_controlled()){
        if (components[target_index][total_steps]->get_symbol() == "I") {
            replace_component(gate, target_index, total_steps);
        } else {
            evolve();
            replace_component(gate, target_index, total_steps);
        }
    } else {
        if (!is_step_empty(total_steps)) {
            evolve();
        }
        replace_component(gate, target_index, total_steps);
        evolve();
    }
}

/**
 * @brief Replaces a gate in the circuit with a new gate by deleting the old gate.
 * 
 * @param gate 
 * @param register_index 
 * @param total_steps_index 
 */
void QuantumCircuit::replace_component(QuantumGate* gate, size_t register_index, size_t total_steps_index) {
    delete components[register_index][total_steps_index];
    components[register_index][total_steps_index] = gate;
}

/**
 * @brief Evolves the circuit by one step by adding identity gates to each register.
 * 
 */
void QuantumCircuit::evolve(){
    for (size_t i = 0; i < register_size; i++) {
        components[i].push_back(new IGate(i));
    }
    total_steps++;
}

void QuantumCircuit::evolve(size_t num_steps){
    for (size_t i = 0; i < num_steps; i++) {
        evolve();
    }
}

