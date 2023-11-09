#ifndef DerivedGates_h
#define DerivedGates_h
#include "QuantumCircuit.h"
// Contains functions to create gates easily.

std::shared_ptr<SingleGate> h(size_t qubit_index);
std::shared_ptr<SingleGate> x(size_t qubit_index);
std::shared_ptr<SingleGate> y(size_t qubit_index);
std::shared_ptr<SingleGate> z(size_t qubit_index);
std::shared_ptr<SingleGate> s(size_t qubit_index);
std::shared_ptr<SingleGate> t(size_t qubit_index);
std::shared_ptr<SingleGate> p(size_t qubit_index, double phase);

/**
 * @brief Returns the adjoint of a single gate. Adjoint of a matrix is the
 * transpose of the complex conjugate of the matrix.
 *
 * @param gate
 * @return std::shared_ptr<SingleGate>
 */
std::shared_ptr<SingleGate> adjoint(std::shared_ptr<SingleGate> gate);

/**
 * @brief Creates a multi gate from a circuit. Assigns the matrix representation
 * of a given circuit to a new multi gate.
 *
 * @param circuit
 * @param qubit_index
 * @param symbol
 * @return std::shared_ptr<MultiGate>
 */
std::shared_ptr<MultiGate> gate_from_circuit(QuantumCircuit circuit,
    size_t qubit_index,
    std::string symbol);

/**
 * @brief Creates a controlled gate from an input target which is dependent on
 * the control index.
 *
 * @param target_gate
 * @param control_index
 * @return std::shared_ptr<MultiGate>
 */
std::shared_ptr<MultiGate> controlled(std::shared_ptr<SingleGate> target_gate,
    size_t control_index);

/**
 * @brief Creates a multigate who swaps the input at index 1 with index 2
 *
 * @param index_1
 * @param index_2
 * @param size
 * @return std::shared_ptr<MultiGate>
 */
std::shared_ptr<MultiGate> swap(size_t index_1, size_t index_2);

/**
 * @brief Creates a Toffoli/CCNOT gate. It does this by creating a circuit
 * and adding a combination of CNot, T, T* and H gates. More information here:
 * https://en.wikipedia.org/wiki/Toffoli_gate#/media/File:Qcircuit_ToffolifromCNOT.svg
 *
 * @param target_index
 * @param control_1
 * @param control_2
 * @return std::shared_ptr<MultiGate>
 */
std::shared_ptr<MultiGate> toffoli(size_t target_index, size_t control_1,
    size_t control_2);

#endif