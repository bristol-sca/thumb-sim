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
#include "simulator/simulator.h"

#include <cstdio>

#include "simulator/config.h"
#include "simulator/debug.h"
#include "simulator/processor.h"

void Simulator::InjectFault(const std::uint32_t cycle_number,
                            const Reg register_name,
                            const std::uint16_t bit_to_flip)
{
    fault = true;
    cycle_to_fault_before = cycle_number;
    register_to_fault = register_name;
    bit_to_fault = bit_to_flip;
}

void Simulator::AddTimeout(const std::uint32_t number_of_cycles)
{
    timeout = number_of_cycles;
}

int Simulator::run(const std::string &programBinFile)
{
    return run(programBinFile, MEM_SIZE_WORDS, MEM_ACCESS_WIDTH_WORDS);
}

int Simulator::run(const std::string &programBinFile,
                   uint32_t memSizeWordsIn,
                   uint32_t memAccessWidthWordsIn)
{
    int ret;

    // TODO: Why is this a pointer?
    proc = new Processor(memSizeWordsIn, memAccessWidthWordsIn);

    if ((ret = proc->reset(programBinFile)) != 0)
    {
        fprintf(stderr, "Failed to reset processor (%d)\n", ret);
        return ret;
    }

    // This is different from cycle_recorder.Get_Cycle_count()
    // as that only contains recorded cycles.
    std::uint32_t cycles_passed{ 0 };
    do
    {
        DEBUG_CMD(
            DEBUG_ALL,
            printf("== cycle %lu ==\n", cycle_recorder.Get_Cycle_Count()));

        // + 1 to ensure the fault happens before the cycle.
        if (fault && cycle_to_fault_before == cycles_passed + 1)
        {
            // Perform the fault injection.
            proc->injectFault(register_to_fault, bit_to_fault);
        }

        // Continue until simulateCycle() returns something other than 0
        if (proc->simulateCycle(&cycle_recorder) != 0)
        {
            break;
        }

        cycle_recorder.Increment_Cycle_Count();
        cycles_passed++;

        // If a timeout has been set, stop execution after the given number of
        // cycles have passed.
        if (timeout && timeout.value() == cycles_passed)
        {
            fprintf(stderr,
                    "Timeout reached before program excecution finished. "
                    "Exiting.\n");
            break;
        }
    } while (true);

    delete proc;

    return 0;
}
