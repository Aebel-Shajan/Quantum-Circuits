#ifndef QuantumComponent_H
#define QuantumComponent_H
#include "Matrix.h"
#include <memory>
#include <complex>

/**
 * @brief Base abstract class for all Quantum Gates
 *
 */
class QuantumComponent
{
protected:
    std::string symbol="?";
    size_t qubit_index=0;
    Matrix matrix;

    // Constructors
    QuantumComponent();
    QuantumComponent(size_t qubit_index, std::string symbol, Matrix matrix);
public:
    // Destructor
    virtual ~QuantumComponent() {}

    // Accessors
    std::string get_symbol() const;
    size_t get_index() const;
    virtual Matrix get_matrix() const=0;
    virtual Matrix get_matrix(size_t register_size) const=0;
    virtual std::string get_gate_type() const=0;
    virtual bool can_gate_fit(size_t register_size) const=0;
    virtual std::string get_terminal_output(size_t terminal_line,
        size_t register_index) const=0;
    virtual std::string get_line(std::string type) const;
    int get_line_length() const;
};

/**
 * @brief Abstract class for gates that only take up one register in a quantum
 * circuit.
 *
 */
class SingleGate : public QuantumComponent
{
public:
    // Constructors and destructors
    SingleGate();
    SingleGate(size_t qubit_index, std::string symbol, Matrix matrix);
    virtual ~SingleGate() {}

    // Accessors
    Matrix get_matrix() const;
    Matrix get_matrix(size_t register_size) const;
    std::string get_gate_type() const;
    bool can_gate_fit(size_t register_size) const;
    virtual std::string get_terminal_output(size_t terminal_line,
        size_t register_index) const;
};

/**
 * @brief Abstract class for gates that take as input multiple registers.
 *
 */
class MultiGate : public QuantumComponent
{
protected:
    size_t gate_size;

public:
    // Constructors and destructors
    MultiGate();
    MultiGate(size_t qubit_index, std::string symbol, Matrix matrix,
        size_t gate_size);
    virtual ~MultiGate() {}

    // Accessors
    size_t get_gate_size() const;
    Matrix get_matrix() const;
    Matrix get_matrix(size_t register_size) const;
    std::string get_gate_type() const;
    bool can_gate_fit(size_t register_size) const;
    virtual std::string get_terminal_output(size_t terminal_line,
        size_t register_index) const;
};

/**
 * @brief Derived class for gate that are controlled based on the input at
 * another register.
 *
 */
class ControlledGate : public MultiGate
{
private:
    size_t control_index; // Register index that controls gate
    size_t target_index;  // Register index of the gate being controlled
    Matrix get_controlled_matrix(Matrix gate_matrix) const;
    
public:
    // Constructors and destructors
    ControlledGate();
    ControlledGate(std::shared_ptr<SingleGate> gate, size_t control_index_in);
    ~ControlledGate() {};

    // Accessors
    size_t get_control_index() const;
    std::string get_terminal_output(size_t terminal_line,
        size_t register_index) const;
};

// Collection of single gates
class IGate : public SingleGate
{
public:
    IGate();
    IGate(size_t qubit_index);
    ~IGate() {}
    std::string get_terminal_output(size_t terminal_line,
        size_t register_index) const;
};

class HGate : public SingleGate
{
public:
    HGate();
    HGate(size_t qubit_index);
    ~HGate() {}
};

class XGate : public SingleGate
{
public:
    XGate();
    XGate(size_t qubit_index);
    ~XGate() {}
};

class YGate : public SingleGate
{
public:
    YGate();
    YGate(size_t qubit_index);
    ~YGate() {}
};

class ZGate : public SingleGate
{
public:
    ZGate();
    ZGate(size_t qubit_index);
    ~ZGate() {}
};

class SGate : public SingleGate
{
public:
    SGate();
    SGate(size_t qubit_index);
    ~SGate() {}
};

class TGate : public SingleGate
{
public:
    TGate();
    TGate(size_t qubit_index);
    ~TGate() {}
};

class PhaseGate : public SingleGate
{
public:
    PhaseGate();
    PhaseGate(size_t qubit_index, double phase);
    ~PhaseGate() {}
};
#endif