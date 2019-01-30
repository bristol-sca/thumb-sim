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

#include <cstdint>
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

//#define DEBUG_ENABLED true

#if defined(DEBUG_ENABLED)
#define DEBUG_CMD(flags, x)            \
    if (((flags) & (DEBUG_MASK)) != 0) \
    {                                  \
        x;                             \
    }
#else
#define DEBUG_CMD(flags, x)
#endif /* DEBUG_ENABLED */

namespace Thumb_Simulator
{
class Debug
{
private:
    // This has been deleted to ensure the constructor and the copy
    // constructor cannot be called as this is just a utility class
    // containing nothing but functions.
    // Debug(const Debug &) = delete;

    // TODO: size_t is unsigned, -1 is
    // negative. TODO: Check this is still right
    // -1 as first cycle will then be cycle 0 after first increment
    std::size_t m_cycle_count{ 0 };
    std::vector<std::pair<std::uint16_t, std::uint16_t>> m_fetch;
    std::vector<std::string> m_decode;
    std::vector<std::string> m_execute;
    std::vector<std::map<std::string, std::size_t>> m_registers;
    std::string m_extra_data; // Extra cryptographic data that is not traces
                              // that needs to be stored.

    bool m_recording = false;

public:
    const std::size_t &Get_Cycle_Count() const
    {
        return m_cycle_count;
    }

    void Increment_Cycle_Count()
    {
        if (m_recording)
        {
            m_cycle_count++;
        }
    }

    void Add_Fetch(const std::size_t p_fetch_first,
                   const std::size_t p_fetch_second)
    {
        if (m_recording)
        {
            m_fetch.push_back(std::make_pair(p_fetch_first, p_fetch_second));
        }
    }

    void Add_Decode(const std::string &p_decode)
    {
        if (m_recording)
        {
            m_decode.push_back(p_decode);
        }
    }

    // All load and stores block the next cycle in ARM M0 - This should be
    // accounted for.
    void Add_Execute(const std::string &p_execute)
    {
        if (m_recording)
        {
            m_execute.push_back(p_execute);
        }
    }

    void Add_Register(const std::string &p_name, const std::size_t p_value)
    {
        if (!m_recording)
        {
            return;
        }
        // If this is the first register to be recorded for this cycle, add a
        // new map for the cycle.
        if (m_registers.size() == m_cycle_count)
        {
            m_registers.push_back(
                std::map<std::string, std::size_t>{ { p_name, p_value } });
        }
        else
        {
            m_registers[m_cycle_count][p_name] = p_value;
        }
    }

    void Add_Extra_Data(const std::string &p_extra_data)
    {
        m_extra_data += p_extra_data;
    }

    const std::vector<std::pair<std::uint16_t, std::uint16_t>> &Get_Fetch()
        const
    {
        return m_fetch;
    }

    const std::vector<std::string> &Get_Decode() const
    {
        return m_decode;
    }

    const std::vector<std::string> &Get_Execute() const
    {
        return m_execute;
    }

    const std::vector<std::map<std::string, std::size_t>> &Get_Registers()
        const
    {
        return m_registers;
    }

    const std::string &Get_Extra_Data() const
    {
        return m_extra_data;
    }

    void Start_Trigger()
    {
        m_recording = true;
    }

    void Stop_Trigger()
    {
        m_recording = false;
    }
};
} // namespace Thumb_Simulator

#endif /* _DEBUG_H_ */
