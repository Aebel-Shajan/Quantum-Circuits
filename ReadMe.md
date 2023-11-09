# Quantum Computing Simulation Project

This project simulates basic quantum computing operations using a C++ framework.
It includes representations of quantum gates, quantum circuits, and matrices 
that are used to represent quantum states.

## Description

This simulation provides a platform to experiment with quantum gates, circuits, 
and state manipulation. It uses fundamental concepts of quantum computing to 
demonstrate the effects of various quantum operations on simulated qubits.

## Getting Started

### Dependencies

* C++ compiler (GCC recommended)
* C++ standard library
* No external dependencies or libraries are required.

### Installing

* Download the source code from the repository or clone it using git:
  ```bash
    git clone https://github.com/Aebel-Shajan/Quantum-Circuits.git
  ```
* Navigate to project directory.

### Executing program
*Compile the source code using the following command:
    ```bash
        g++ -o QuantumCircuitSimulator src/*.cpp -Iinclude
    ```

* Run the executable:
    ```bash
        ./QuantumCircuitSimulator
    ```

### Usage
Go to main.cpp and edit the main function to run the desired simulation.

* To create a new quantum circuit with a register size of 3 do:
    ```cpp
        QuantumCircuit qc(3);
    ```
* To add a quantum gate (e.g. Hadamard gate to the 0th register) do:
    ```cpp
        qc.addComponent(h(0));
    ```
* To add a controlled quantum gate (e.g. CNOT gate with control qubit 0 and target qubit 1) do:
    ```cpp
        qc.addComponent(controlled(x(1), 0));
    ```
* (more gates can be found in DerivedGates.h)

* After compiling and running QuantumCircuitSimulator.exe the resulting quantum 
circuit and the outputs of different states should be printed to the console.

* For more information on the project look in Quantum_Circuit_Project.pdf. 
(This project was completed as part of the C++ module at The University of Manchester)

### Acknowledgments
Inspiration, code snippets, etc.

* [Qiskit](https://qiskit.org/)
