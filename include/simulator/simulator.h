/*
 * MIT License
 *
 * Copyright (c) 2018 Andres Amaya Garcia
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

// Forward Declaration
class Processor;

#include <cstdint>
#include <string>

#include "simulator/regfile.h"

#include "simulator/debug.h"

class Simulator
{
public:
    int run(const std::string &programBinFile);
    int run(const std::string &programBinFile,
            uint32_t memSizeWordsIn,
            uint32_t memAccessWidthWordsIn);
    const Thumb_Simulator::Debug &Get_Cycle_Recorder()
    {
        return cycle_recorder;
    }

    // Faults are injected before the cycle given by cycle_number is executed.
    void InjectFault(const std::uint32_t cycle_number,
                     const Reg register_name,
                     const std::uint16_t bit_to_flip);

    void AddTimeout(const std::uint32_t number_of_cycles);

private:
    Thumb_Simulator::Debug cycle_recorder;
    Processor *proc;

    // These are only for purposes of fault injection.
    bool fault{ false };
    Reg register_to_fault;
    std::uint32_t cycle_to_fault_before;
    std::uint16_t bit_to_fault;

    std::optional<std::uint32_t> timeout;
};

#endif /* _SIMULATOR_H_ */
