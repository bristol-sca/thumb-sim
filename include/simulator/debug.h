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
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#define DEBUG_FETCH 0x00000001
#define DEBUG_DECODE 0x00000002
#define DEBUG_MEMORY 0x00000004
#define DEBUG_REGFILE 0x00000010
#define DEBUG_EXECUTE 0x00000020
#define DEBUG_ALL 0xFFFFFFFF

#define DEBUG_MASK (0xFFFFFFFF)

#if defined(DEBUG_ENABLED)
#define DEBUG_CMD(flags, x)            \
    if (((flags) & (DEBUG_MASK)) != 0) \
    {                                  \
        x;                             \
    }
#else
#define DEBUG_CMD(flags, x)
#endif /* DEBUG_ENABLED */

#endif /* _DEBUG_H_ */
namespace Simulator_Debug
{
class Debug
{
private:
    // This has been deleted to ensure the constructor and the copy
    // constructor cannot be called as this is just a utility class
    // containing nothing but static functions.
    Debug(const Debug &) = delete;

public:
    static size_t &Get_Cycle_Count()
    {
        // -1 as first cycle will then be cycle 0 after first increment
        static size_t cycle_count = -1;
        return cycle_count;
    }

    static void Increment_Cycle_Count()
    {
        Get_Cycle_Count()++;
    }

    static void Add_Fetch(const size_t p_fetch_first,
                          const size_t p_fetch_second)
    {
        Get_Fetch().push_back(std::make_pair(p_fetch_first, p_fetch_second));
    }

    static void Add_Decode(const std::string &p_decode)
    {
        Get_Decode().push_back(p_decode);
    }

    // All load and stores block the next cycle in ARM M0 - This should be
    // accounted for.
    static void Add_Execute(const std::string &p_execute)
    {
        Get_Execute().push_back(p_execute);
    }

    static void Add_Register(const std::string &p_name, const size_t p_value)
    {
        // Only store the changed registers
        // TODO: This is cheaper on memory but is it needed? What is the
        // difference? Registers can only be accessed by iterating through
        // them all
        // TODO: Also this if statement doesn't work
        /*
         *if (Get_Registers()[Get_Cycle_Count() - 1].end() !=
         *        Get_Registers()[Get_Cycle_Count() - 1].find(p_name) &&
         *    p_value != Get_Registers()[Get_Cycle_Count() - 1][p_name])
         */
        Get_Registers()[Get_Cycle_Count()][p_name] = p_value;
    }

    static std::vector<std::pair<uint16_t, uint16_t>> &Get_Fetch()
    {
        static std::vector<std::pair<uint16_t, uint16_t>> m_fetch;
        return m_fetch;
    }

    static std::vector<std::string> &Get_Decode()
    {
        static std::vector<std::string> m_decode;
        return m_decode;
    }

    static std::vector<std::string> &Get_Execute()
    {
        static std::vector<std::string> m_execute;
        return m_execute;
    }

    static std::map<size_t, std::map<std::string, size_t>> &Get_Registers()
    {
        static std::map<size_t, std::map<std::string, size_t>> m_registers;
        return m_registers;
    }
};
} // namespace Simulator_Debug
