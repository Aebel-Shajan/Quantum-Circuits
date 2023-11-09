#include "DerivedGates.h"


std::shared_ptr<SingleGate> h(size_t n)
{
    return std::make_shared<HGate>(n);
}

std::shared_ptr<SingleGate> x(size_t n)
{
    return std::make_shared<XGate>(n);
}

std::shared_ptr<SingleGate> y(size_t n)
{
    return std::make_shared<YGate>(n);
}

std::shared_ptr<SingleGate> z(size_t n)
{
    return std::make_shared<ZGate>(n);
}

std::shared_ptr<SingleGate> s(size_t n)
{
    return std::make_shared<SGate>(n);
}

std::shared_ptr<SingleGate> t(size_t n)
{
    return std::make_shared<TGate>(n);
}

std::shared_ptr<SingleGate> p(size_t n, double phase)
{
    return std::make_shared<PhaseGate>(n, phase);
}

std::shared_ptr<MultiGate> controlled(std::shared_ptr<SingleGate> gate, size_t n)
{
    return std::make_shared<ControlledGate>(gate, n);
}

std::shared_ptr<MultiGate> gate_from_circuit(QuantumCircuit qc, size_t n, std::string symbol)
{
    std::shared_ptr<MultiGate> gate=std::make_shared<MultiGate>(
        n, symbol, qc.get_matrix(), qc.get_register_size());
    return gate;
}

std::shared_ptr<MultiGate> swap(size_t index_1, size_t index_2)
{
    size_t first_index=std::min(index_1, index_2);
    size_t last_index=std::max(index_1, index_2);
    size_t gate_size=last_index-first_index+1;
    QuantumCircuit swap_circuit(gate_size);
    swap_circuit.add_component(controlled(x(0), gate_size-1));
    swap_circuit.add_component(controlled(x(gate_size-1), 0));
    swap_circuit.add_component(controlled(x(0), gate_size-1));
    std::shared_ptr<MultiGate> gate=gate_from_circuit(swap_circuit, first_index,
        std::to_string(first_index)+" <-> "+std::to_string(last_index));
    return gate;
}

std::shared_ptr<SingleGate> adjoint(std::shared_ptr<SingleGate> gate)
{
    std::shared_ptr<SingleGate> adjoint_gate=std::make_shared<SingleGate>(
        gate->get_index(),
        gate->get_symbol()+"*",
        gate->get_matrix().adjoint());
    return adjoint_gate;
}

std::shared_ptr<MultiGate> toffoli(size_t target, size_t control_1, size_t control_2)
{
    int first_qubit=std::min(std::min(control_1, control_2), target);
    int last_qubit=std::max(std::max(control_1, control_2), target);
    QuantumCircuit qc(last_qubit-first_qubit+1);
    int r_t=target-first_qubit;
    int r_c1=control_1-first_qubit;
    int r_c2=control_2-first_qubit;
    qc.add_component(h(r_t));
    qc.add_component(controlled(x(r_t), r_c2));
    qc.add_component(adjoint(t(r_t)));
    qc.add_component(controlled(x(r_t), r_c1));
    qc.add_component(t(r_t));
    qc.add_component(controlled(x(r_t), r_c2));
    qc.add_component(adjoint(t(r_t)));
    qc.add_component(controlled(x(r_t), r_c1));
    qc.add_component(t(r_c2));
    qc.add_component(t(r_t));
    qc.add_component(h(r_t));
    qc.add_component(controlled(x(r_c2), r_c1));
    qc.add_component(t(r_c1));
    qc.add_component(adjoint(t(r_c2)));
    qc.add_component(controlled(x(r_c2), r_c1));
    std::string gate_symbol="q"+std::to_string(control_1)+
        "q"+std::to_string(control_2)+"(+)q"+
        std::to_string(target);
    std::shared_ptr<MultiGate> gate=gate_from_circuit(qc,
        first_qubit,
        gate_symbol);
    return gate;
}