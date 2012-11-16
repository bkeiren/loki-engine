#include "util/systeminfo/systeminfo.h"

namespace loki
{

namespace util
{

namespace system
{

SystemInfo* g_SystemInfo = NULL;

SystemInfo::SystemInfo()	:
	m_VersionOSMajor(0),
	m_VersionOSMinor(0),
	m_VersionOSBuild(0),
	m_VersionOSServicePackMajor(0),
	m_VersionOSServicePackMinor(0),
	m_VersionOS64Bit(false),
	m_VersionOSString("Unknown"),
	m_VersionOSServicePackString("Unknown"),
	m_ProductType(0),
	m_NumProcessors(1),
	m_ProcessorType(0),
	m_ProcessorArchitecture(0),
	m_MemoryAmountTotalPhysical(0),
	m_MemoryAmountTotalVirtual(0),
	m_CPUFrequency(0),
	m_ComputerName("Unknown"),
	m_UserName("Unknown")
{
	Collect();
}

SystemInfo::~SystemInfo()
{

}

bool SystemInfo::Collect()
{
	OSVERSIONINFOEXA OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);
	if (!GetVersionExA((OSVERSIONINFOA*)(&OSVersionInfo)))
	{
		LOG(VL_ERROR, "SystemInfo::Collect: Unable to obtain OS version info (with error code %i)", (int32)GetLastError());
		return false;
	}
	else
	{
		m_VersionOSMajor = OSVersionInfo.dwMajorVersion;
		m_VersionOSMinor = OSVersionInfo.dwMinorVersion;
		m_VersionOSBuild = OSVersionInfo.dwBuildNumber;
		m_VersionOSServicePackMajor = OSVersionInfo.wServicePackMajor;
		m_VersionOSServicePackMinor = OSVersionInfo.wServicePackMinor;
		m_VersionOSServicePackString = OSVersionInfo.szCSDVersion;

		switch (m_VersionOSMajor)
		{
		case 6:
			{
				switch (m_VersionOSMinor)
				{
				case 0:
					if (OSVersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						m_VersionOSString = "Windows Vista";
					}
					else
					{
						m_VersionOSString = "Windows Server 2008";
					}
					break;
				case 1:
					if (OSVersionInfo.wProductType == VER_NT_WORKSTATION)
					{
						m_VersionOSString = "Windows 7";
					}
					else
					{
						m_VersionOSString = "Windows Server 2008 R2";
					}
					break;
				default:
					m_VersionOSString = "Windows (Major Version 6 - Unknown Minor Version)";
					break;
				}
				break;
			}
		case 5:
			{
				switch (m_VersionOSMinor)
				{
				case 0:
					m_VersionOSString = "Windows 20000";
					break;
				case 1:
					m_VersionOSString = "Windows XP";
					break;
				case 2:
					if (GetSystemMetrics(SM_SERVERR2) == 0)
					{
						m_VersionOSString = "Windows Server 2003";
					}
					else
					{
						m_VersionOSString = "Windows Server 2003 R2";
					}
					break;
				default:
					m_VersionOSString = "Windows (Major Version 5 - Unknown Minor Version)";
					break;
				}
				break;
			}
		default:
			m_VersionOSString = "Windows (Unknown Major Version)";
		}
	}

	// Store whether the program is running on 64 bits or not.
	IsWow64Process(GetCurrentProcess(), (int32*)&m_VersionOS64Bit);

	MEMORYSTATUSEX MemoryInfo;
	MemoryInfo.dwLength = sizeof(MemoryInfo);
	if (!GlobalMemoryStatusEx(&MemoryInfo))
	{
		LOG(VL_ERROR, "SystemInfo::Collect: Unable to obtain memory information (with error code %i)", (int32)GetLastError());
		return false;
	}
	else
	{
		m_MemoryAmountTotalPhysical = MemoryInfo.ullTotalPhys;
		m_MemoryAmountTotalVirtual = MemoryInfo.ullTotalVirtual;
	}

	LARGE_INTEGER CPUFrequency;
	if (!QueryPerformanceFrequency(&CPUFrequency))
	{
		LOG(VL_ERROR, "SystemInfo::Collect: Unable to obtain CPU frequency information (with error code %i)", (int32)GetLastError());
		return false;
	}
	else
	{
		m_CPUFrequency = (uint32)CPUFrequency.QuadPart;
	}


	unsigned long UserNameLength = UNLEN + 1;
	char UserName[UNLEN + 1];
	if (!GetUserNameA(UserName, &UserNameLength))
	{
		LOG(VL_ERROR, "SystemInfo::Collect: Unable to obtain user name (with error code %i)", (int32)GetLastError());
		return false;
	}
	else
	{
		m_UserName = UserName;
	}

	unsigned long ComputerNameLength = MAX_COMPUTERNAME_LENGTH + 1;
	char ComputerName[MAX_COMPUTERNAME_LENGTH + 1];
	if (!GetComputerNameA(ComputerName, &ComputerNameLength))
	{
		LOG(VL_ERROR, "SystemInfo::Collect: Unable to obtain computer name (with error code %i)", (int32)GetLastError());
		return false;
	}
	else
	{
		m_ComputerName = ComputerName;
	}

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	m_NumProcessors = sysInfo.dwNumberOfProcessors;
	m_ProcessorType = sysInfo.dwProcessorType;
	m_ProcessorArchitecture = sysInfo.wProcessorArchitecture;


	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	m_DesktopResolution = int2(desktop.right, desktop.bottom);

	return true;
}

void SystemInfo::LogSystemInformation() const
{
	LOG(VL_ALWAYS, "System Information:\n\tOS: %u.%u.%u %s SP%u.%u (%s %s)\n\tCPU: %u cores @ %.2f GHz\n\tRAM: %I64d MB Physical\t%I64d MB Virtual", 
				   m_VersionOSMajor, 
				   m_VersionOSMinor, 
				   m_VersionOSBuild,
				   ((m_VersionOS64Bit) ? ("x64") : ("x86")),
				   m_VersionOSServicePackMajor, 
				   m_VersionOSServicePackMinor, 
				   m_VersionOSString.c_str(), 
				   m_VersionOSServicePackString.c_str(), 
				   m_NumProcessors,
				   GetCPUFrequencyGHz(),
				   BYTE_TO_MB(m_MemoryAmountTotalPhysical),
				   BYTE_TO_MB(m_MemoryAmountTotalVirtual));
}

const unsigned long SystemInfo::GetVersionOSMajor() const
{
	return m_VersionOSMajor;
}

const unsigned long SystemInfo::GetVersionOSMinor() const
{
	return m_VersionOSMinor;
}

const unsigned long SystemInfo::GetVersionOSBuild() const
{
	return m_VersionOSBuild;
}

const unsigned long SystemInfo::GetVersionOSServicePackMajor() const
{
	return m_VersionOSServicePackMajor;
}

const unsigned long SystemInfo::GetVersionOSServicePackMinor() const
{
	return m_VersionOSServicePackMinor;
}

const bool SystemInfo::GetVersionOS64Bit() const
{
	return m_VersionOS64Bit;
}

const std::string& SystemInfo::GetVersionOSString() const
{
	return m_VersionOSString;
}

const std::string& SystemInfo::GetVersionOSServicePackString() const
{
	return m_VersionOSServicePackString;
}

const unsigned long SystemInfo::GetNumProcessors() const
{
	return m_NumProcessors;
}

const unsigned long SystemInfo::GetProcessorType() const
{
	return m_ProcessorType;
}

const unsigned short SystemInfo::GetProcessorArchitecture() const
{
	return m_ProcessorArchitecture;
}

const uint64 SystemInfo::GetMemoryAmountTotalPhysical() const
{
	return m_MemoryAmountTotalPhysical;
}

const uint64 SystemInfo::GetMemoryAmountTotalVirtual() const
{
	return m_MemoryAmountTotalVirtual;
}

const uint32 SystemInfo::GetCPUFrequencyHz() const
{
	return m_CPUFrequency;
}

const f32 SystemInfo::GetCPUFrequencyGHz() const
{
	return ((f32)m_CPUFrequency) / 1000000;
}

const std::string& SystemInfo::GetComputerName() const
{
	return m_ComputerName;
}

const std::string& SystemInfo::GetUserName() const
{
	return m_UserName;
}

const int2& SystemInfo::GetDesktopResolution() const
{
	return m_DesktopResolution;
}

}

}	// Namespace util.

}	// Namespace loki.