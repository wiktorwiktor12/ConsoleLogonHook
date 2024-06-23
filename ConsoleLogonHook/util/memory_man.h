#pragma once
#include <windows.h>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>
#include "util.h"

#define REL(addr, offset) ((addr + offset + 4) + *(int32_t*)(addr + offset))

namespace memory
{
    inline const std::string offsetCacheFileName = "ConsoleLogonHookOffsetCache.txt";

    inline std::map<std::string, uintptr_t> offsetCache;
    inline bool bIsDirty = false;

    static void LoadOffsetCache()
    {
        offsetCache.clear();

        if (!PathFileExistsA(offsetCacheFileName.c_str()))
        {
            //MessageBoxW(0,L"File no exist",L"",0);
            return;
        }

        std::ifstream file(offsetCacheFileName);

        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                if (line.find(":") != std::string::npos)
                {
                    auto splitted = split(line, ":");
                    if (splitted.size() >= 2)
                        offsetCache.insert_or_assign(splitted[0], std::clamp<uintptr_t>(std::stoll(splitted[1]),0,INT_MAX));
                }
            }
        }
        file.close();
    }

    static void SaveOffsetCache()
    {
        if (!bIsDirty) return;

        std::ofstream file(offsetCacheFileName);

        for (auto it = offsetCache.begin(); it != offsetCache.end(); ++it)
        {
            file << (*it).first << ":" << (*it).second << "\n";
        }

        file.close();
    }

    static std::vector<int> patternToByte(const char* pattern)
    {
        auto bytes = std::vector<int>{};
        const auto start = const_cast<char*>(pattern);
        const auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else { bytes.push_back(strtoul(current, &current, 16)); }
        }
        return bytes;
    }

    static uintptr_t FindPattern(uintptr_t baseAddress, const char* signature)
    {
        const auto dosHeader = (PIMAGE_DOS_HEADER)baseAddress;
        const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)baseAddress + dosHeader->e_lfanew);

        const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
        auto patternBytes = patternToByte(signature);
        const auto scanBytes = reinterpret_cast<std::uint8_t*>(baseAddress);

        const auto s = patternBytes.size();
        const auto d = patternBytes.data();

        for (auto i = 0ul; i < sizeOfImage - s; ++i)
        {
            bool found = true;
            for (auto j = 0ul; j < s; ++j)
            {
                if (scanBytes[i + j] != d[j] && d[j] != -1)
                {
                    found = false;
                    break;
                }
            }
            if (found)
            {
                uintptr_t address = reinterpret_cast<uintptr_t>(&scanBytes[i]);
                return address;
            }
        }

        return NULL;
    }

    template<class T>
    static T FindPatternCached(std::string functionName, const char* signature)
    {
        uintptr_t base_address = (uintptr_t)GetModuleHandle(L"ConsoleLogon.dll");

        uintptr_t offset = 0;
        auto it = offsetCache.find(functionName);
        if ((*it).second)
        {
            offset = (*it).second;
            auto bytes = patternToByte(signature);
            uint8_t* adr = (uint8_t*)(base_address + offset);
            if (IsBadReadPtr(adr, 8) || (bytes.size() >= 1 && adr[0] != bytes[0] && bytes[0] != -1) || (bytes.size() >= 2 && adr[1] != bytes[1] && bytes[1] != -1) || (bytes.size() >= 3 && adr[2] != bytes[2] && bytes[2] != -1))
            {
                offsetCache.clear();
                return FindPatternCached<T>(functionName,signature);
            }
            //MessageBoxW(0, std::format(L"{}", (void*)offset).c_str(), 0, 0);
        }
        else
        {
            uintptr_t address = FindPattern(base_address, signature);
            if (address <= 0)
            {
                MessageBoxW(0, std::format(L"{} ADDRESS IS NULL", s2ws(functionName)).c_str(), L"", 0);
                return (T)(0);
            }
            offsetCache.insert_or_assign(functionName, (uintptr_t)(address - base_address));
            bIsDirty = true;
            return (T)(address);
        }
        
        return (T)(offset + base_address);
    }


    static void CheckCache()
    {
        //auto SecurityOptionsView__RuntimeClassIntialise = (uint8_t*)(baseaddress + 0x36EB4);
        auto SecurityOptionsView__RuntimeClassIntialise = FindPatternCached<uint8_t*>("SecurityOptionsViewRuntimeClassIntialise","48 89 5C 24 10 4C 89 44 24 18 55 56 57 41 56");
        if (IsBadReadPtr(SecurityOptionsView__RuntimeClassIntialise,8) || SecurityOptionsView__RuntimeClassIntialise[0] != 0x48 || SecurityOptionsView__RuntimeClassIntialise[1] != 0x89 || SecurityOptionsView__RuntimeClassIntialise[2] != 0x5C)
        {
            offsetCache.clear();
            SecurityOptionsView__RuntimeClassIntialise = FindPatternCached<uint8_t*>("SecurityOptionsViewRuntimeClassIntialise", "48 89 5C 24 10 4C 89 44 24 18 55 56 57 41 56");
            if (!SecurityOptionsView__RuntimeClassIntialise)
                MessageBoxW(0,L"SecurityOptionsView__RuntimeClassIntialise pattern Broke!",0,0);
        }
    }
}
