#include "QuantumCircuit.h"


///////////////////////////////////////////////////////////////////////////////
// Helper functions
///////////////////////////////////////////////////////////////////////////////

std::string get_binary_representation(int num, int  size)
{
    return std::bitset<32>(num).to_string().substr(32-size);
}

Matrix calculate_matrix_for_register(std::vector<int> input_register) {
    Matrix qubit_zero(2, 1);
    qubit_zero(0, 0)=std::complex<double>(1, 0);
    Matrix qubit_one(2, 1);
    qubit_one(1, 0)=std::complex<double>(1, 0);
    // Store qubits in an array of matrices.
    std::vector<Matrix> tensor_product_list(input_register.size(), qubit_zero);
    for (size_t i=0; i<input_register.size(); i++)
    {
        if (input_register[i]==1)
        {
            tensor_product_list[i]=qubit_one;
        }
    }
    // Tensor product them together to get the vector repersenting the state.
    return perform_tensor_product(tensor_product_list);
}

bool is_power_of_two(int n) {
    if (n<=0) return false;
    return (n&(n-1))==0;
}

void draw_state(Matrix state)
{
    if (!is_power_of_two(state.get_rows())&&state.get_cols()==1)
    {
        std::cout<<"Error: State vector has incorrect dimensions"<<std::endl;
        return;
    }
    int states_printed=0;
    for (size_t i=0; i<state.get_rows(); i++)
    {
        std::complex<double> value=state(i, 0);
        const double tolerance = 1e-10;
        if (std::abs((value*std::conj(value)).real())>tolerance)
        {
            if (states_printed!=0)
            {
                std::cout<<" + ";
            }
            if (std::abs(1.0-value.real())>tolerance||std::abs(value.imag())>tolerance)
            {
                std::cout<<value;
            }
            std::cout<<"|"<<get_binary_representation(i, std::log2(state.get_rows()))<<"> ";
            states_printed++;
        }
    }
}


///////////////////////////////////////////////////////////////////////////////
// QuantumCircuit
///////////////////////////////////////////////////////////////////////////////

QuantumCircuit::QuantumCircuit(size_t register_size_in)
{
    register_size=register_size_in;
    input_register=std::vector<int>(register_size, 0);
    components.resize(register_size);
    evolve();
}

QuantumCircuit::~QuantumCircuit()
{
    // std::cout<<"QuantumCircuit destructor:"<<this<<" called!"<<std::endl;
    // shared pointers are automatically deleted when they go out of scope.
}

// Accessors
///////////////////////////////////////////////////////////////////////////////

Matrix QuantumCircuit::get_initial_state() const
{
    return calculate_matrix_for_register(input_register);
}

Matrix QuantumCircuit::get_final_state() const
{
    return get_matrix()*get_initial_state();
}

Matrix QuantumCircuit::get_state_after_step(size_t step_index) const
{
    return get_matrix_at_step(step_index)*get_initial_state();
}

size_t QuantumCircuit::get_register_size() const
{
    return register_size;
}

size_t QuantumCircuit::get_total_steps() const
{
    return total_steps;
}

Matrix QuantumCircuit::get_matrix_at_step(size_t step_index) const
{
    // Create an identity matrix that has the same size as n gates tensor producted
    //  together.
    Matrix resultant_matrix=identity_matrix(1<<register_size);
    for (size_t i=0; i<register_size; i++)
    {
        // Multiply by the gates matrix (which is changed to account for the circuits size).
        resultant_matrix=components[i][step_index]->get_matrix(register_size)*resultant_matrix;
    }
    return resultant_matrix;
}

Matrix QuantumCircuit::get_matrix() const
{
    Matrix circuit_matrix=get_matrix_at_step(0);
    for (size_t i=1; i<total_steps+1; i++)
    {
        circuit_matrix=get_matrix_at_step(i)*circuit_matrix;
    }
    return circuit_matrix;
}

bool QuantumCircuit::step_contains_multigate(size_t step_index) const
{
    for (size_t i=0; i<register_size; i++)
    {
        if (components[i][step_index]->get_gate_type()=="MultiGate")
        {
            return true;
        }
    }
    return false;
}

std::shared_ptr<QuantumComponent> QuantumCircuit::get_multigate_at_step(size_t step_index) const
{
    for (size_t i=0; i<register_size; i++)
    {
        if (components[i][step_index]->get_gate_type()=="MultiGate")
        {
            return components[i][step_index];
        }
    }
    throw std::invalid_argument("No multigate at step "+std::to_string(step_index));
}

bool QuantumCircuit::is_step_empty(size_t step_index) const
{
    for (size_t i=0; i<register_size; i++)
    {
        if (components[i][step_index]->get_symbol()!="I")
        {
            return false;
        }
    }
    return true;
}

bool QuantumCircuit::is_gate_in_circuit(std::shared_ptr<QuantumComponent> gate) const
{
    for (size_t i=0; i<register_size; i++)
    {
        for (size_t j=0; j<components[i].size(); j++)
        {
            if (components[i][j]==gate)
            {
                return true;
            }
        }
    }
    return false;
}

// Drawing Functions
///////////////////////////////////////////////////////////////////////////////

void QuantumCircuit::draw_circuit() const
{
    std::cout<<"QuantumCircuit : "<<this<<std::endl;
    for (size_t register_index=0; register_index<register_size; register_index++)
    {
        // Each gate is represented by three lines of string in the terminal.
        for (size_t line_index=0; line_index<3; line_index++)
        {
            // Label the qubit registers.
            std::string register_label=
                "q_"+std::to_string(register_index)+" : ==";
            if (line_index==1)
            {
                std::cout<<register_label;
            }
            else
            {
                std::cout<<std::string(register_label.length(), ' ');
            }
            for (size_t step_index=0; step_index<components[register_index].size(); step_index++)
            {
                // Loop through each step and draw each gate.
                std::shared_ptr<QuantumComponent> gate=components[register_index][step_index];
                // Steps with multigates are drawn differently
                if (step_contains_multigate(step_index))
                {
                    gate=get_multigate_at_step(step_index);
                }
                std::cout<<gate->get_terminal_output(line_index, register_index);
                // Add padding to make sure all components at a given step are aligned
                int max_line_length=5;
                for (size_t i=0; i<register_size; i++)
                {
                    if (components[i][step_index]->get_line_length()>5)
                    {
                        max_line_length=std::max(max_line_length, components[i][step_index]->get_line_length());
                    }
                }
                int padding=max_line_length-gate->get_line_length()+2;
                if (line_index==1)
                {
                    std::cout<<std::string(padding, '=');
                }
                else
                {
                    std::cout<<std::string(padding, ' ');
                }
            }
            std::cout<<std::endl;
        }
    }
}

void QuantumCircuit::draw_probability_distribution() const
{
    // Calculate initial state in vector form and draw as ket.
    std::cout<<"Intial state:"<<std::endl;
    draw_state(get_initial_state());
    // Draw basis states in ket form and calculate probabilities of each basis
    //  state.
    std::cout<<std::endl
        <<"Probabilities of final states:"<<std::endl;
    Matrix final_state=get_final_state();
    for (int i=0; i<1<<register_size; i++)
    {
        Matrix basis_state=Matrix(1<<register_size, 1);
        basis_state(i, 0)=1;
        draw_state(basis_state);
        // Draw probability distribution as a histogram.
        double probability=(final_state(i, 0)*std::conj(final_state(i, 0))).real();
        std::cout<<"||"<<std::to_string(probability).substr(0, 5)<<" ||";
        int filled_blocks=probability*50;
        for (int j=0; j<filled_blocks; j++)
        {
            std::cout<<"#";
        }
        std::cout<<std::endl;
    }
}

// Mutators
///////////////////////////////////////////////////////////////////////////////

void QuantumCircuit::set_input_register(std::vector<int> register_in)
{
    // Check input is valid.
    if (register_in.size()!=register_size)
    {
        throw std::invalid_argument("Input register size does not match circuit's register size!");
    }
    for (size_t i=0; i<register_in.size(); i++)
    {
        if (input_register[i]!=0&&input_register[i]!=1)
        {
            throw std::invalid_argument("Input register must be a vector of 0s and 1s!");
        }
    }
    input_register=register_in;
}

void QuantumCircuit::add_component(std::shared_ptr<QuantumComponent> gate)
{
    // Check input
    if (is_gate_in_circuit(gate))
    {
        throw std::invalid_argument("Gate already in circuit!");
        return;
    }
    if (!gate->can_gate_fit(register_size))
    {
        throw std::invalid_argument("Gate is not within circuit's register size!");
        return;
    }
    int target_index=gate->get_index();
    std::string gate_type=gate->get_gate_type();
    // Add component by replacing the Identity gate at the current step.
    if (gate_type=="SingleGate")
    {
        if (components[target_index][total_steps]->get_symbol()!="I")
        {
            evolve();
        }
        replace_component(gate, target_index, total_steps);
    }
    else if (gate_type=="MultiGate")
    {
        if (!is_step_empty(total_steps))
        {
            evolve();
        }
        replace_component(gate, target_index, total_steps);
        evolve();
    }
    else
    {
        throw std::invalid_argument("Gate is not a valid gate type!");
    }
}

void QuantumCircuit::replace_component(std::shared_ptr<QuantumComponent> gate, size_t register_index, size_t step_index)
{
    if (is_gate_in_circuit(gate))
    {
        throw std::invalid_argument("Gate already in circuit!");
        return;
    }
    if (!gate->can_gate_fit(register_size))
    {
        throw std::invalid_argument("Gate is not within circuit's register size!");
        return;
    }
    // release ownership of old gate from circuit and assign new gate.
    components[register_index][step_index].reset();
    components[register_index][step_index]=gate;
}

void QuantumCircuit::evolve()
{
    for (size_t i=0; i<register_size; i++)
    {
        components[i].push_back(std::make_shared<IGate>(i));
    }
    total_steps++;
}

void QuantumCircuit::evolve(size_t num_steps)
{
    for (size_t i=0; i<num_steps; i++)
    {
        evolve();
    }
}

void QuantumCircuit::ask_for_input()
{
    while (true)
    {
        try
        {
            // Ask user for input.
            std::string example_input(register_size, '0');
            example_input[rand()%register_size]='1';
            std::cout<<"[n to quit] Input the qubits (without spaces eg: "+
                example_input+"): ";
            std::string input;
            std::cin>>input;
            std::vector<int> input_vector;
            for (int i=0; i<input.size(); i++)
            {
                if (input[i]=='n')
                {
                    exit(0);
                }
            }
            // Check if they want to quit.
            if (input.size()!=get_register_size())
            {
                throw std::invalid_argument(
                    "Invalid input size: "+
                    std::to_string(input.size())+". Expected: "+
                    std::to_string(register_size)+".");
            }
            // Input users input into a qubit register.
            for (int i=0; i<register_size; i++)
            {
                while (input[i]!='0'&&input[i]!='1')
                {
                    std::cout<<"Invalid input at index"<<i
                        <<". Please input 0 or 1 or to quit type 'n': ";
                    std::cin>>input[i];
                    if (input[i]=='n')
                    {
                        exit(0);
                    }
                }
                input_vector.push_back(input[i]-'0');
            }
            // Draw the input in its ket form.
            set_input_register(input_vector);
            draw_state(get_initial_state());
            std::cout<<" --> ";
            draw_state(get_final_state());
            std::cout<<std::endl;
        }
        catch (const std::exception& e)
        {
            std::cout<<e.what()<<std::endl;
        }
    }
}

void QuantumCircuit::test_circuit() {
    std::vector<int> initial_input = input_register;
    std::cout<<"input states -> output states"<<std::endl;
    for (int i = 0; i < (1 << register_size); i++) {
        std::bitset<32> bits(i); // convert the number to a bitset
        std::vector<int> input_register_; // initialize the vector of binary digits
        for (int j = 0; j < register_size; j++) { // loop over the binary digits
            input_register_.push_back(bits[j]); // add the j-th bit to the vector
        }
        input_register = input_register_;
        draw_state(get_initial_state());
        std::cout<<"->";
        draw_state(get_final_state());
        std::cout<<std::endl;
    }
    input_register = initial_input;
}