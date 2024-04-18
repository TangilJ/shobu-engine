/*
 * For the bindings CMake project, might need to set the following CMake options
 * -DPython_ROOT_DIR=D:\\anaconda3\\envs\\torch
 * -DPYTHON_LIBRARY=D:\\anaconda3\\envs\\torch\\libs\\python312.lib
 * -DPYTHON_INCLUDE_DIR=D:\\anaconda3\\envs\\torch\\include
 * -DPYTHON_EXECUTABLE=D:\\anaconda3\\envs\\torch\\python.exe
*/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "engine/movegen.h"
#include "engine/print.h"

namespace py = pybind11;

std::vector<State> getNextStates(const Board &board)
{
    std::vector<State> states;
    states.reserve(250);

    generateAllMovesInPly(board, states);

    return states;
}

PYBIND11_MODULE(engine, m)
{
    m.doc() = "ShobuEngine bindings for Python";

    py::class_<Quarter>(m, "Quarter")
        .def(py::init<Bitboard, Bitboard>())
        .def_readonly("own", &Quarter::own)
        .def_readonly("enemy", &Quarter::enemy)
        .def("__eq__", &Quarter::operator ==);

    py::class_<Board>(m, "Board")
        .def(py::init<Quarter, Quarter, Quarter, Quarter>())
        .def_readonly("topLeft", &Board::topLeft)
        .def_readonly("topRight", &Board::topRight)
        .def_readonly("bottomLeft", &Board::bottomLeft)
        .def_readonly("bottomRight", &Board::bottomRight)
        .def("__eq__", &Board::operator ==);

    py::enum_<PassiveSide>(m, "PassiveSide")
        .value("Left", PassiveSide::Left)
        .value("Right", PassiveSide::Right);

    py::enum_<AggressiveSide>(m, "AggressiveSide")
        .value("Top", AggressiveSide::Top)
        .value("Bottom", AggressiveSide::Bottom);

    py::class_<Move>(m, "Move")
        .def_readonly("passiveSourceIndex", &Move::passiveSourceIndex)
        .def_readonly("aggressiveSourceIndex", &Move::aggressiveSourceIndex)
        .def_readonly("passiveSide", &Move::passiveSide)
        .def_readonly("aggressiveSide", &Move::aggressiveSide)
        .def_readonly("direction", &Move::direction)
        .def_readonly("timesMoved", &Move::timesMoved);

    py::class_<State>(m, "State")
        .def_readwrite("board", &State::board)
        .def_readwrite("move", &State::move);

    m.def(
        "get_next_states",
        &getNextStates,
        "Get successor states given a board state."
    );
    
    m.def(
        "to_bit_string",
        &toBitString,
        "Convert a bitboard to a string representation."
    );
}