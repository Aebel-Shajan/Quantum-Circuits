#include "QuantumComponent.h"


///////////////////////////////////////////////////////////////////////////////
// QuantumComponent
///////////////////////////////////////////////////////////////////////////////
QuantumComponent::QuantumComponent() : QuantumComponent(0, "I", identity_matrix(2)) {};

QuantumComponent::QuantumComponent(size_t n, std::string symbol_in, Matrix matrix_in)
{
    qubit_index=n;
    symbol=symbol_in;
    matrix=matrix_in;
};

std::string QuantumComponent::get_symbol() const
{
    return symbol;
}

size_t QuantumComponent::get_index() const
{
    return qubit_index;
}

std::string QuantumComponent::get_line(std::string type) const
{
    // Return a line of strings depending on what type is asked.
    // I tried using extended ascii characters but when I executed the main.exe
    // file it would print out giberish. Probably has something to do with the 
    // encoding of the windows console.
    if (type=="edge")
    { // "+---+""
        return "+"+std::string(symbol.size()+2, '-')+"+";
    }
    else if (type=="symbol")
    {
        // "| X |"
        return "| "+symbol+" |";
    }
    else if (type=="middle")
    {
        // "|   |"
        return "|"+std::string(symbol.size()+2, ' ')+"|";
    }
    else if (type=="blank")
    {
        // "    "
        return std::string(symbol.size()+4, ' ');
    }
    else if (type=="horizontal")
    {
        // "===="
        return std::string(symbol.size()+4, '=');
    }
    else if (type=="vertical")
    {
        // " | "
        int total_size=symbol.size()+4;
        int left_size=(total_size-1)/2;
        int right_size=total_size-left_size-1;
        return std::string(left_size, ' ')+"|"+std::string(right_size, ' ');
    }
    else if (type=="intersected")
    {
        // "==|== "
        int total_size=symbol.size()+4;
        int left_size=(total_size-1)/2;
        int right_size=total_size-left_size-1;
        return std::string(left_size, '=')+"|"+std::string(right_size, '=');
    }
    else if (type=="intersected_edge")
    {
        // "+==|==+"
        int total_size=symbol.size()+4;
        int left_size=(total_size-1)/2;
        int right_size=total_size-left_size-1;
        return std::string(left_size, '+')+"|"+std::string(right_size, '+');
    }
    else if (type=="circled")
    {
        // "===O=="
        int total_size=symbol.size()+4;
        int left_size=(total_size-1)/2;
        int right_size=total_size-left_size-1;
        return std::string(left_size, '=')+"O"+std::string(right_size, '=');
    }
    throw std::invalid_argument("Invalid line type for QuantumComponent::get_line()");
}

int QuantumComponent::get_line_length() const
{
    return symbol.size()+4;
}


///////////////////////////////////////////////////////////////////////////////
// SingleGate
///////////////////////////////////////////////////////////////////////////////
SingleGate::SingleGate() : SingleGate(0, "I", identity_matrix(2)) {};
SingleGate::SingleGate(size_t n, std::string symbol_in, Matrix matrix_in) : QuantumComponent(n, symbol_in, matrix_in) {};
Matrix SingleGate::get_matrix() const
{
    return matrix;
}

Matrix SingleGate::get_matrix(size_t register_size) const
{
    if (!can_gate_fit(register_size))
    {
        throw std::invalid_argument("Gate cannot fit in register for SingleGate::get_matrix()");
    }
    std::vector<Matrix> matrices(register_size, identity_matrix(2));
    matrices[get_index()]=matrix;
    return perform_tensor_product(matrices);
}

std::string SingleGate::get_gate_type() const
{
    return "SingleGate";
}

bool SingleGate::can_gate_fit(size_t register_size) const
{
    return (get_index()<register_size&&get_index()>=0);
}

std::string SingleGate::get_terminal_output(size_t terminal_line, size_t register_index) const
{
    // Return lines of strings so that something like this can be printed to 
    // console:
    // "+-----+"
    // "|  X  |"
    // "+-----+"
    if (terminal_line>=3)
    {
        throw std::invalid_argument("Invalid line number for SingleGate::get_terminal_output()");
    }
    if (terminal_line==1)
    {
        return get_line("symbol");
    }
    return get_line("edge");
}


///////////////////////////////////////////////////////////////////////////////
// MultiGate
///////////////////////////////////////////////////////////////////////////////
MultiGate::MultiGate() : MultiGate(0, "I", identity_matrix(2), 1) {};
MultiGate::MultiGate(size_t n, std::string symbol_in, Matrix matrix_in, size_t gate_size_in) : QuantumComponent(n, symbol_in, matrix_in)
{
    if (1<<gate_size_in!=matrix_in.get_rows()||1<<gate_size_in!=matrix_in.get_cols())
    {
        throw std::invalid_argument("Matrix size does not match gate size for MultiGate constructor");
    }
    gate_size=gate_size_in;
};
size_t MultiGate::get_gate_size() const
{
    return gate_size;
}

Matrix MultiGate::get_matrix() const
{
    return matrix;
}

Matrix MultiGate::get_matrix(size_t register_size) const
{
    // Returns matrix in the context of the circuit.
    if (!can_gate_fit(register_size))
    {
        throw std::invalid_argument("Gate cannot fit in register for MultiGate::get_matrix()");
    }
    std::vector<Matrix> matrices(register_size-gate_size+1, identity_matrix(2)); // right
    matrices[get_index()]=matrix;
    return perform_tensor_product(matrices);
}

std::string MultiGate::get_gate_type() const
{
    return "MultiGate";
}

bool MultiGate::can_gate_fit(size_t register_size) const
{
    return (get_index()+get_gate_size()<=register_size&&get_index()>=0);
}

std::string MultiGate::get_terminal_output(size_t terminal_line, size_t register_index) const
{
    // Return lines of strings so that something like this can be printed to 
    // console:
    // "+-----+"
    // "|  X  |"
    // "|     |"
    // "|     |"
    // "|     |"
    // "+-----+"
    if (terminal_line>=3)
    {
        throw std::invalid_argument("Invalid line number for MultiGate::get_terminal_output()");
    }
    if (register_index<get_index() or register_index>=get_index()+get_gate_size())
    { // not in gate
        if (terminal_line==1)
        {
            return get_line("horizontal");
        }
        return get_line("blank");
    }
    int relative_index=register_index-get_index();
    if (relative_index==0)
    {
        if (terminal_line==0)
        {
            return get_line("edge");
        }
    }
    if (relative_index==get_gate_size()-1)
    {
        if (terminal_line==2)
        {
            return get_line("edge");
        }
    }
    if (relative_index==floor(get_gate_size()/2))
    {
        if (terminal_line==1)
        {
            return get_line("symbol");
        }
    }
    return get_line("middle");
}


///////////////////////////////////////////////////////////////////////////////
// ControlledGate
///////////////////////////////////////////////////////////////////////////////
ControlledGate::ControlledGate() : ControlledGate(std::make_shared<XGate>(1), 0) {};

ControlledGate::ControlledGate(std::shared_ptr<SingleGate> gate, size_t control_index_in)
{
    if (control_index_in==gate->get_index())
    {
        throw std::invalid_argument("Controlled gate cannot be at the same index as the index it is controlled by.");
    }
    control_index=control_index_in;
    target_index=gate->get_index();
    // Set the qubit index to index that "appears first".
    qubit_index=std::min(control_index, target_index);
    symbol=gate->get_symbol();
    gate_size=abs(control_index-target_index)+1;
    matrix=get_controlled_matrix(gate->get_matrix());
}

Matrix ControlledGate::get_controlled_matrix(Matrix gate_matrix) const
{
    // Controlled gates have matrices of the form:
    // I x ... x|0><0| x ... x I x ... x I +
    // I x ... x|1><1| x ... x U x ... x I
    // where U is the gate that is being controlled
    Matrix zero(2, 2); // |0><0|
    zero(0, 0)=1;
    Matrix one(2, 2); // |1><1|
    one(1, 1)=1;
    Matrix I=identity_matrix(2);
    std::vector<Matrix> matrices_0(gate_size, identity_matrix(2));
    std::vector<Matrix> matrices_1(gate_size, identity_matrix(2));
    int relative_control=control_index-qubit_index;
    int relative_target=target_index-qubit_index;
    matrices_0[relative_control]=zero;
    matrices_1[relative_control]=one;
    matrices_1[relative_target]=gate_matrix;
    Matrix controlled_matrix=perform_tensor_product(matrices_0)+perform_tensor_product(matrices_1);
    return controlled_matrix;
}

size_t ControlledGate::get_control_index() const
{
    return control_index;
}

std::string ControlledGate::get_terminal_output(size_t terminal_line, size_t register_index) const
{
    // Return lines of strings so that something like this can be printed to 
    // console:
    // "+-----+"
    // "|  X  |"
    // "+++|+++"
    //     |
    //     |
    //  ===O===
    if (terminal_line>=3*gate_size)
    {
        throw std::invalid_argument("Invalid line number for MultiGate::get_terminal_output()");
    }
    if (register_index<qubit_index or register_index>=qubit_index+gate_size)
    {
        if (terminal_line==1)
        {
            return get_line("horizontal");
        }
        return get_line("blank");
    }
    if (register_index==target_index)
    {
        std::string terminal_output[3]={
            get_line("edge"),
            get_line("symbol"),
            get_line("intersected_edge") };
        if (control_index<target_index)
        {
            terminal_output[0]=get_line("intersected_edge");
            terminal_output[2]=get_line("edge");
        }
        return terminal_output[terminal_line%3];
    }
    if (register_index==control_index)
    {
        std::string terminal_output[3]={
            get_line("vertical"),
            get_line("circled"),
            get_line("blank") };
        if (control_index<target_index)
        {
            terminal_output[0]=get_line("blank");
            terminal_output[2]=get_line("vertical");
        }
        return terminal_output[terminal_line%3];
    }
    std::string terminal_output[3]={
        get_line("vertical"),
        get_line("intersected"),
        get_line("vertical") };
    return terminal_output[terminal_line];
}


///////////////////////////////////////////////////////////////////////////////
// Derived Single Gates
///////////////////////////////////////////////////////////////////////////////
// Was gonna use only functions to create these but the minimum spec said I
// should create "Derived classes for a suitable selection of gates"

// Identity Gate
IGate::IGate() : IGate(0) {};
IGate::IGate(size_t n) : SingleGate(n, "I", identity_matrix(2)) {};
std::string IGate::get_terminal_output(size_t terminal_line, size_t register_index) const
{
    if (terminal_line>=3)
    {
        throw std::invalid_argument("Invalid line number for I_Gate::get_icon()");
    }
    if (terminal_line==1)
    {
        return get_line("horizontal");
    }
    return get_line("blank");
}

// Hadamard Gate
HGate::HGate() : HGate(0) {};
HGate::HGate(size_t n)
{
    qubit_index=n;
    symbol="H";
    matrix=Matrix(2, 2);
    matrix(0, 0)=std::complex<double>(1/sqrt(2), 0);
    matrix(0, 1)=std::complex<double>(1/sqrt(2), 0);
    matrix(1, 0)=std::complex<double>(1/sqrt(2), 0);
    matrix(1, 1)=std::complex<double>(-1/sqrt(2), 0);
}

// Pauli X Gate
XGate::XGate() : XGate(0) {};
XGate::XGate(size_t n)
{
    qubit_index=n;
    symbol="X";
    matrix=Matrix(2, 2);
    matrix(0, 1)=std::complex<double>(1, 0);
    matrix(1, 0)=std::complex<double>(1, 0);
}

// Pauli Y Gate
YGate::YGate() : YGate(0) {};
YGate::YGate(size_t n)
{
    qubit_index=n;
    symbol="Y";
    matrix=Matrix(2, 2);
    matrix(0, 1)=std::complex<double>(0, -1);
    matrix(1, 0)=std::complex<double>(0, 1);
}

// Pauli Z Gate
ZGate::ZGate() : ZGate(0) {};
ZGate::ZGate(size_t n)
{
    qubit_index=n;
    symbol="Z";
    matrix=Matrix(2, 2);
    matrix(0, 0)=std::complex<double>(1, 0);
    matrix(1, 1)=std::complex<double>(-1, 0);
}

// S Gate
SGate::SGate() : SGate(0) {};
SGate::SGate(size_t n)
{
    qubit_index=n;
    symbol="S";
    matrix=Matrix(2, 2);
    matrix(0, 0)=std::complex<double>(1, 0);
    matrix(1, 1)=std::complex<double>(0, 1);
}

// T Gate
TGate::TGate() : TGate(0) {};
TGate::TGate(size_t n)
{
    qubit_index=n;
    symbol="T";
    matrix=Matrix(2, 2);
    matrix(0, 0)=std::complex<double>(1, 0);
    matrix(1, 1)=std::complex<double>(1/sqrt(2), 1/sqrt(2));
}

PhaseGate::PhaseGate() : PhaseGate(0, 0) {};
PhaseGate::PhaseGate(size_t n, double phase) {
    qubit_index=n;
    symbol="P(" + std::to_string(phase) + ")";
    matrix=Matrix(2,2);
    matrix(0,0) = std::complex<double>(1,0);
    matrix(1,1) = std::exp(phase*std::complex<double>(0,1));
}
