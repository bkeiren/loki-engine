#pragma once

#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

#include <Windows.h>
#include <LMCons.h>	// For MAX_USERNAME_LENGTH and UNLEN.

namespace loki
{

namespace util
{

class LkSystemInfo
{
	friend class LkEngine;
public:
	//////////////////////////////////////////////////////////////////////////
	// Collects system information.
	//////////////////////////////////////////////////////////////////////////
	bool Collect();

	//////////////////////////////////////////////////////////////////////////
	// Logs the collected system information. This will make it appear
	// in a log file.
	// NOTE: If used when debugging or tracking issues, consider dumping
	// the log buffer right after calling this function call to ensure
	// the data is written to disk.
	//////////////////////////////////////////////////////////////////////////
	void LogSystemInformation();

	//////////////////////////////////////////////////////////////////////////
	// A number of Get* functions that return various system info values.
	//////////////////////////////////////////////////////////////////////////
	const unsigned long GetVersionOSMajor();
	const unsigned long GetVersionOSMinor();
	const unsigned long GetVersionOSBuild();
	const unsigned long GetVersionOSServicePackMajor();
	const unsigned long GetVersionOSServicePackMinor();
	const bool GetVersionOS64Bit();
	const std::string& GetVersionOSString();
	const std::string& GetVersionOSServicePackString();
	const unsigned long GetNumProcessors();
	const unsigned long GetProcessorType();
	const unsigned short GetProcessorArchitecture();
	const uint64 GetMemoryAmountTotalPhysical();
	const uint64 GetMemoryAmountTotalVirtual();
	const uint32 GetCPUFrequencyHz();		// Hertz.
	const float GetCPUFrequencyGHz();	// Gigahertz.
	const std::string& GetComputerName();
	const std::string& GetUserName();
private:
	LkSystemInfo();
	~LkSystemInfo();

	unsigned long m_VersionOSMajor;
	unsigned long m_VersionOSMinor;
	unsigned long m_VersionOSBuild;
	unsigned long m_VersionOSServicePackMajor;	// Windows service pack major version.
	unsigned long m_VersionOSServicePackMinor;	// Windows service pack minor version.
	bool m_VersionOS64Bit;
	std::string m_VersionOSString;
	std::string m_VersionOSServicePackString;
	unsigned long m_ProductType;	// Windows product type.
	unsigned long m_NumProcessors;
	unsigned long m_ProcessorType;
	unsigned short m_ProcessorArchitecture;
	uint64 m_MemoryAmountTotalPhysical;
	uint64 m_MemoryAmountTotalVirtual;
	uint32 m_CPUFrequency;
	std::string m_ComputerName;
	std::string m_UserName;
};

extern LkSystemInfo* g_SystemInfo;

}

}

#endif