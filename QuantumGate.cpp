#include "QuantumGate.h"
#include "Matrix.h"


// Quantum_Gate
std::string QuantumGate::get_symbol() const {
    return symbol;
}

size_t QuantumGate::get_index() const {
    return register_index;
}

Matrix QuantumGate::get_matrix() const {
    return matrix;
}

/**
 * @brief Returns the matrix of the gate in the context of a larger register.
 * 
 * @param register_size
 * @return Matrix 
 */
Matrix QuantumGate::get_matrix(size_t register_size) const {
    std::vector<Matrix> matrices(register_size, identity_matrix(2));
    if (!is_gate_valid(register_size)){
        throw std::invalid_argument("Invalid gate for QuantumGate::get_matrix()");
    }
    matrices[get_index()] = matrix;
    for (size_t i = 1; i < register_size; i++) {
        matrices[i] = matrices[i-1].tensor_product(matrices[i]);
    }
    return matrices[register_size-1];
}

/**
 * @brief Returns a single line of the gate's icon. Icon is printed when drawing circuit.
 * 
 * @param line_number 
 * @return std::string 
 */
std::string QuantumGate::get_icon(size_t line_number) const {
    if (line_number >= 3) {
        throw std::invalid_argument("Invalid line number for Quantum_Gate::get_icon()");
    }
    std::string icon[3] = {
        "┌───┐",
        "┤ " + symbol + " ├",
        "└───┘"
    };
    return icon[line_number];
}

/**
 * @brief Returns whether the gate is controlled or not. Default QuantumGate is not controlled.
 * 
 * @return false 
 */
bool QuantumGate::is_gate_controlled() const {
    return false;
}

/**
 * @brief Returns whether the gate is valid for a register of the given size.
 * 
 * @param register_size 
 * @return true 
 * @return false 
 */
bool QuantumGate::is_gate_valid(size_t register_size) const {
    return (get_index() < register_size && get_index() >= 0);
}

/**
 * @brief Returns the index of the control qubit. Throws error if gate is not controlled.
 * 
 * @return size_t 
 * @throws std::logic_error if gate not controlled.
 */
size_t QuantumGate::get_control_index() const {
    throw std::logic_error("Quantum_Gate::get_control_index() called on non-controlled gate");
}


// Controlled_Gate
/**
 * @brief Construct a new ControlledGate object from a QuantumGate and a control index.
 * 
 * @param gate_in 
 * @param control_index_in 
 * @throws std::invalid_argument if control_index_in == register_index
 * @throws std::invalid_argument if gate_in itself is a controlled gate
 */
ControlledGate::ControlledGate(QuantumGate* gate_in, size_t control_index_in) {
    if (control_index == register_index) {
        throw std::invalid_argument("Controlled gate cannot be at the same index as control index");
    }
    if (gate_in->is_gate_controlled()) {
        throw std::invalid_argument("Input gate cannot be another controlled gate");
    }
    control_index = control_index_in;
    gate = gate_in;
    register_index = gate->get_index();
    symbol = gate->get_symbol();
    matrix = gate->get_matrix();
}

/**
 * @brief Returns register index which controls if the gate is applied or not.
 * 
 * @return size_t 
 */
size_t ControlledGate::get_control_index() const {
    return control_index;
}

/**
 * @brief Returns the matrix of a controlled gate in the context of a larger register.
 * 
 * @param register_size 
 * @return Matrix 
 */
Matrix ControlledGate::get_matrix(size_t register_size) const {
    std::vector<Matrix> matrices(register_size, identity_matrix(2));
    if (!is_gate_valid(register_size)){
        throw std::invalid_argument("Invalid gate for QuantumGate::get_matrix()");
    }
    // want to do 
    // I ⊗ ... ⊗|0><0|⊗ ... ⊗ I ⊗ ... ⊗ I + 
    // I ⊗ ... ⊗|1><1|⊗ ... ⊗ U ⊗ ... ⊗ I
    // where U is the gate that is being controlled 
    Matrix U = gate->get_matrix(); 
    Matrix zero = std::vector<std::vector<std::complex<double>>> \
    {{1, 0},\
    {0, 0}};// |0><0|
    Matrix one = std::vector<std::vector<std::complex<double>>> \
    {{0, 0},\
    {0, 1}};// |1><1|
    Matrix I = identity_matrix(2);
    std::vector<Matrix> first_product(register_size, I);
    std::vector<Matrix> second_product(register_size, I);
    first_product[control_index] = zero;
    second_product[control_index] = one;
    second_product[register_index] = U;
    for (int i = 1; i < register_size; i++) {
        first_product[i] = first_product[i-1].tensor_product(first_product[i]);
    }
    for (int i = 1; i < register_size; i++) {
        second_product[i] = second_product[i-1].tensor_product(second_product[i]);
    }
    Matrix controlled_matrix = first_product[register_size-1] + second_product[register_size-1];
    return controlled_matrix;
}

/**
 * @brief Returns a single line of the controlled gate's icon. Icon is printed when drawing circuit.
 * 
 * @param line_number 
 * @return std::string 
 */
std::string ControlledGate::get_icon(size_t line_number) const{
    if (line_number >= 6) {
        throw std::invalid_argument("Invalid line number for Controlled_Gate::get_icon()");
    }
    std::string icon [6] = {
        "┌───┐",
        "┤ " + symbol + " ├",
        "└─╥─┘",
        "  ║  ",
        "─███─",
        "     "
    };
    if (register_index > control_index) {
        icon[0] = "┌─╨─┐";
        icon[2] = "└───┘";
        icon[3] = "     ";
        icon[5] = "  ║  ";
    };
    return icon[line_number]; 
}
bool ControlledGate::is_gate_controlled() const {
    return true;
}

/**
 * @brief Returns wether the controlled gate is valid for a register of the given size.
 * 
 * @param register_size 
 * @return true 
 * @return false 
 */
bool ControlledGate::is_gate_valid(size_t register_size) const {
    return (get_index() < register_size && get_index() >= 0 &&\
            get_control_index() < register_size && get_control_index() >= 0);
}


// Identity Gate
/**
 * @brief Returns a single line of the identity gate's icon. Icon is printed when drawing circuit.
 * 
 * @param line_number 
 * @return std::string 
 */
std::string IGate::get_icon(size_t line_number) const {
    if (line_number >= 3) {
            throw std::invalid_argument("Invalid line number for I_Gate::get_icon()");
        }
        std::string icon[3] = {
            "     ",
            "─────",
            "     "
        };
        return icon[line_number];
}


// Hadamard Gate
HGate::HGate(size_t n) {
    register_index = n;
    symbol = "H";
    matrix = std::vector<std::vector<std::complex<double>>> \
    {{1/sqrt(2), 1/sqrt(2)},\
    {1/sqrt(2), -1/sqrt(2)}};
}


// Pauli X Gate
XGate::XGate(size_t n) {
    register_index = n;
    symbol = "X";
    matrix = std::vector<std::vector<std::complex<double>>> \
    {{0, 1},\
    {1, 0}};
}


// Pauli Y Gate
YGate::YGate(size_t n) {
    register_index = n;
    symbol = "Y";
    matrix = std::vector<std::vector<std::complex<double>>> \
    {{0, (0,-1)},\
    {(0,1), 0}};
}


// Pauli Z Gate
ZGate::ZGate(size_t n) {
    register_index = n;
    symbol = "Z";
    matrix = std::vector<std::vector<std::complex<double>>> \
    {{1, 0},\
    {0, -1}};
}


// Phase Gate
SGate::SGate(size_t n) {
    register_index = n;
    symbol = "S";
    matrix = std::vector<std::vector<std::complex<double>>> \
    {{1, 0},\
    {0, (0,1)}};
}

