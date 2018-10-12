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

#include "simulator/config.h"
#include "simulator/debug.h"
#include "simulator/processor.h"

#include <cstdio>

int Simulator::run(const std::string &programBinFile)
{
    return run(programBinFile, MEM_SIZE_WORDS, MEM_ACCESS_WIDTH_WORDS);
}

int Simulator::run(const std::string &programBinFile,
                   uint32_t memSizeWordsIn,
                   uint32_t memAccessWidthWordsIn)
{
    int ret;
    uint32_t cycle = 0;

    proc = new Processor(memSizeWordsIn, memAccessWidthWordsIn);

    /* Avoid compiler warnings when not debugging */
    (void)cycle;

    if ((ret = proc->reset(programBinFile)) != 0)
    {
        fprintf(stderr, "Failed to reset processor (%d)\n", ret);
        return ret;
    }

    do
    {
        DEBUG_CMD(DEBUG_ALL, printf("== cycle %u ==\n", cycle++));
        Simulator_Debug::Debug::Increment_Cycle_Count();
    } while (proc->simulateCycle() == 0);

    delete proc;

    return 0;
}
