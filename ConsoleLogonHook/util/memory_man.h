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
    inline const int VersionNumber = 104;
    inline const std::string offsetCacheFileName = "ConsoleLogonHookOffsetCache.txt";

    inline std::vector<std::pair<std::string, uintptr_t>> offsetCache;
    inline bool bIsDirty = false;

    static void LoadOffsetCache()
    {
        //return;
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
                        offsetCache.push_back(std::pair<std::string,uintptr_t>(splitted[0], std::clamp<uintptr_t>(std::stoll(splitted[1]),0,INT_MAX)));
                }
            }
        }
        file.close();
    }

    static void SaveOffsetCache()
    {
        //return;
        SPDLOG_INFO("is dirty {}",(int)bIsDirty);
        if (!bIsDirty) return;

        std::ofstream file(offsetCacheFileName);

        file << "VersionNumber" << ":" << VersionNumber << "\n";

        for (int i = 0; i < offsetCache.size(); ++i)
        {
            file << offsetCache[i].first << ":" << offsetCache[i].second << "\n";
        }

        //for (auto it = offsetCache.begin(); it != offsetCache.end(); ++it)
        //{
        //    file << (*it).first << ":" << (*it).second << "\n";
        //}

        file.close();
    }

    static uintptr_t FindInOffsetCache(std::string functionName)
    {
        for (int i = 0; i < offsetCache.size(); ++i)
        {
            auto pair = offsetCache[i];
            if (pair.first == functionName)
                return pair.second;
        }
        return 0;
    }

    // small UNWIND_INFO implementation
    typedef struct _UNWIND_INFO {
        unsigned char Version : 3;
        unsigned char Flags : 5;
    } UNWIND_INFO, * PUNWIND_INFO;

    static uintptr_t GetFunctionStart(uintptr_t address, uintptr_t BaseAddress)
    {
        DWORD64 ImgBase = 0;
        PRUNTIME_FUNCTION Function = nullptr;
        for (auto Func = RtlLookupFunctionEntry(address, &ImgBase, NULL); Func; Func = RtlLookupFunctionEntry(BaseAddress + (Func->BeginAddress - 1), &ImgBase, NULL))
        {
            if (reinterpret_cast<PUNWIND_INFO>(BaseAddress + Func->UnwindInfoAddress)->Flags & UNW_FLAG_CHAININFO)
            {
                continue;
            }

            Function = Func;
            break;
        }

        return Function ? BaseAddress + Function->BeginAddress : 0;
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

    static uintptr_t FindPattern(uintptr_t baseAddress, const char* signature, bool bFindTop = false)
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
                if (bFindTop)
                    return GetFunctionStart(address, baseAddress);
                return address;
            }
        }

        return NULL;
    }

    static bool TestPatterns(std::vector<std::string> signatures, uint8_t* adr)
    {
        for (int i = 0; i < signatures.size(); ++i)
        {
            auto signature = signatures[i];
            auto bytes = patternToByte(signature.c_str());
            if (IsBadReadPtr(adr, 8)) continue;

            if (bytes.size() >= 1 && bytes[0] != -1 && bytes[0] != adr[0]) continue;
            if (bytes.size() >= 2 && bytes[1] != -1 && bytes[1] != adr[1]) continue;
            if (bytes.size() >= 3 && bytes[2] != -1 && bytes[2] != adr[2]) continue;

            return true;
        }
        return false;
    }

    template<class T>
    static T FindPatternCached(std::string functionName, std::vector<std::string> signatures, bool bFindTop = false)
    {
        uintptr_t base_address = (uintptr_t)GetModuleHandle(L"ConsoleLogon.dll");
        ////offsetCache.find(functionName);
        //
        //for (int i = 0; i < signatures.size(); ++i)
        //{
        //    auto signature = signatures[i];
        //
        //    auto adr = FindPattern(base_address, signature.c_str(), bFindTop);
        //    if (adr > 0)
        //        return (T)adr;
        //}
        //
        //return 0;
        uintptr_t offset = 0;
        uintptr_t returnv = FindInOffsetCache(functionName);
        if (returnv)
        {
            offset = returnv;
            SPDLOG_INFO(std::format("returnv"));
            //auto bytes = patternToByte(signature);
            //uint8_t* adr = (uint8_t*)(base_address + offset);
            //if (IsBadReadPtr(adr, 8) || (bytes.size() >= 1 && adr[0] != bytes[0] && bytes[0] != -1) || (bytes.size() >= 2 && adr[1] != bytes[1] && bytes[1] != -1) || (bytes.size() >= 3 && adr[2] != bytes[2] && bytes[2] != -1))
            //{
            //    offsetCache.clear();
            //    return FindPatternCached<T>(functionName,signature, bFindTop);
            //}
            //MessageBoxW(0, std::format(L"{}", (void*)offset).c_str(), 0, 0);
        }
        else
        {
            for (int i = 0; i < signatures.size(); ++i)
            {
                auto signature = signatures[i];
                uintptr_t address = FindPattern(base_address, signature.c_str(), bFindTop);
                if (address <= 0)
                {
                    SPDLOG_INFO(std::format("{} {} ADDRESS IS NULL", functionName,i));
                    //return (T)(0);
                    continue;
                }
                SPDLOG_INFO("pushing back {} {}",functionName, (uintptr_t)(address - base_address));
                offsetCache.push_back(std::pair<std::string,uintptr_t>(functionName, (uintptr_t)(address - base_address)));
                bIsDirty = true;
                return (T)(address);
            }
            
        }
        
        return (T)(offset + base_address);
    }


    static void CheckCache()
    {
        uintptr_t savedVersion = FindInOffsetCache("VersionNumber");
        if (savedVersion != VersionNumber)
        {
            offsetCache.clear();
            SPDLOG_INFO("Version Number does not match! clearing offset cache");
        }
        //return;
        //auto SecurityOptionsView__RuntimeClassIntialise = (uint8_t*)(baseaddress + 0x36EB4);
        auto SecurityOptionsView__RuntimeClassIntialise = FindPatternCached<uint8_t*>("SecurityOptionsViewRuntimeClassIntialise", { "55 56 57 41 56 41 57 48 8B EC 48 83 EC 30" });
        if (IsBadReadPtr(SecurityOptionsView__RuntimeClassIntialise,8) || SecurityOptionsView__RuntimeClassIntialise[0] != 0x55 || SecurityOptionsView__RuntimeClassIntialise[1] != 0x56 || SecurityOptionsView__RuntimeClassIntialise[2] != 0x57)
        {
            offsetCache.clear();
            SecurityOptionsView__RuntimeClassIntialise = FindPatternCached<uint8_t*>("SecurityOptionsViewRuntimeClassIntialise", { "55 56 57 41 56 41 57 48 8B EC 48 83 EC 30" });
            if (!SecurityOptionsView__RuntimeClassIntialise)
                MessageBoxW(0,L"SecurityOptionsView__RuntimeClassIntialise pattern Broke!",0,0);
        }
    }
}
