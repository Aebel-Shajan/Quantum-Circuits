#ifndef QuantumGate_H
#define QuantumGate_H
#include "Matrix.h"

class QuantumGate {
protected:
    std::string symbol = "?";
    size_t register_index =0;
    Matrix matrix;
public:
    // constructors and destructors
    QuantumGate() {}
    QuantumGate(size_t n, std::string symbol_in, Matrix matrix_in) {
        register_index = n;
        symbol = symbol_in;
        matrix = matrix_in;
    };
    virtual ~QuantumGate() {}
    // accessors
    std::string get_symbol() const;
    size_t get_index() const;
    Matrix get_matrix() const; 
    virtual Matrix get_matrix(size_t) const;
    virtual std::string get_icon(size_t) const;
    virtual bool is_gate_controlled() const; // default is not controlled
    virtual bool is_gate_valid(size_t register_size) const;
    virtual size_t get_control_index() const;// expected to give error if gate not controlled
};

class IGate : public QuantumGate {
public:
    IGate(size_t n): QuantumGate(n, "I", identity_matrix(2)) {};
    IGate() : IGate(0) {}
    ~IGate() {}
    std::string get_icon(size_t line_number) const;
};

class ControlledGate : public QuantumGate {
private:
    QuantumGate* gate;
    size_t control_index;
public:
    // constructors and destructors
    ControlledGate(QuantumGate*, size_t);
    ControlledGate() : ControlledGate(new IGate(0), 0) {}
    ~ControlledGate() {delete gate;}
    // accessors
    size_t get_control_index() const;
    Matrix get_matrix(size_t) const;
    std::string get_icon(size_t) const;
    bool is_gate_controlled() const;
    bool is_gate_valid(size_t) const;
};

class HGate : public QuantumGate {
public:
    HGate(size_t);
    HGate() : HGate(0) {}
    ~HGate() {}
};

class XGate : public QuantumGate {
public:
    XGate(size_t);
    XGate() : XGate(0) {}
    ~XGate() {}
};

class YGate : public QuantumGate {
public:
    YGate(size_t);
    YGate() : YGate(0) {}
    ~YGate() {}
};

class ZGate : public QuantumGate {
public:
    ZGate(size_t);
    ZGate() : ZGate(0) {}
    ~ZGate() {}
};

class SGate : public QuantumGate {
public:
    SGate(size_t);
    SGate() : SGate(0) {}
    ~SGate() {}
};

#endif