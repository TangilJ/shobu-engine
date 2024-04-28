/*
 * For the bindings CMake project, might need to set the following CMake options
 * -DPython_ROOT_DIR=D:\\anaconda3\\envs\\torch
 * -DPYTHON_LIBRARY=D:\\anaconda3\\envs\\torch\\libs\\python312.lib
 * -DPYTHON_INCLUDE_DIR=D:\\anaconda3\\envs\\torch\\include
 * -DPYTHON_EXECUTABLE=D:\\anaconda3\\envs\\torch\\python.exe
*/

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <immintrin.h>
#include "engine/movegen.h"
#include "engine/print.h"
#include "engine/consts.h"

namespace py = pybind11;

std::vector<State> getNextStates(const Board &board)
{
    // There are no successor states if it's a terminal node
    if (checkWin(board) != Win::GameOngoing)
        return {};

    std::vector<State> states;
    states.reserve(250);
    generateAllMovesInPly(board, states);
    return states;
}

int popcnt(const unsigned int b)
{
    return _mm_popcnt_u32(b);
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
        .def_readonly("top_left", &Board::topLeft)
        .def_readonly("top_right", &Board::topRight)
        .def_readonly("bottom_left", &Board::bottomLeft)
        .def_readonly("bottom_right", &Board::bottomRight)
        .def("__eq__", &Board::operator ==);

    py::enum_<PassiveSide>(m, "PassiveSide")
        .value("Left", PassiveSide::Left)
        .value("Right", PassiveSide::Right);

    py::enum_<AggressiveSide>(m, "AggressiveSide")
        .value("Top", AggressiveSide::Top)
        .value("Bottom", AggressiveSide::Bottom);

    py::enum_<Direction>(m, "Direction")
        .value("Up", Direction::Up)
        .value("UpRight", Direction::UpRight)
        .value("Right", Direction::Right)
        .value("DownRight", Direction::DownRight)
        .value("Down", Direction::Down)
        .value("DownLeft", Direction::DownLeft)
        .value("Left", Direction::Left)
        .value("UpLeft", Direction::UpLeft);

    py::enum_<Win>(m, "Win")
        .value("GameOngoing", Win::GameOngoing)
        .value("OwnWin", Win::OwnWin)
        .value("OpponentWin", Win::OpponentWin);

    py::class_<Move>(m, "Move")
        .def_readonly("passive_source_index", &Move::passiveSourceIndex)
        .def_readonly("aggressive_source_index", &Move::aggressiveSourceIndex)
        .def_readonly("passive_side", &Move::passiveSide)
        .def_readonly("aggressive_side", &Move::aggressiveSide)
        .def_readonly("direction", &Move::direction)
        .def_readonly("times_moved", &Move::timesMoved);

    py::class_<State>(m, "State")
        .def_readwrite("board", &State::board)
        .def_readwrite("move", &State::move)
        .def_readwrite("win", &State::win);

    m.attr("start_state") = startState;

    m.def(
        "get_next_states",
        &getNextStates,
        "Get successor states given a board state."
    );

    m.def(
        "reverse_board",
        &reverseBoard,
        "Swap the sides of the board."
    );

    m.def(
        "to_bit_string",
        &toBitString,
        "Convert a bitboard to a string representation."
    );

    m.def(
        "print_board",
        py::overload_cast<const Board>(&print),
        "Print the board."
    );

    m.def(
        "print_bitboard",
        py::overload_cast<const Bitboard>(&print),
        "Print the board."
    );

    m.def(
        "popcnt",
        &popcnt,
        "Population count"
    );
}