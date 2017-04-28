#pragma once
#include <Windows.h>

#ifdef _USRDLL
#ifdef WINDOWSKILLLIBRARY_EXPORTS
#define WINDOWSKILLLIBRARY_API __declspec(dllexport)
#else
#define WINDOWSKILLLIBRARY_API __declspec(dllimport)
#endif
#else
#define WINDOWSKILLLIBRARY_API
#endif

#define WINDOWSKILLLIBRARY_VERSION "1.0.0"

namespace WindowsKillLibrary
{
	/// <summary>
	/// Signal type of CTRL + C
	/// </summary>
	const DWORD SIGNAL_TYPE_CTRL_C = (int)CTRL_C_EVENT;

	/// <summary>
	/// Signal type of CTRL + Break
	/// </summary>
	const DWORD SIGNAL_TYPE_CTRL_BREAK = (int)CTRL_BREAK_EVENT;

	/// <summary>
	/// Sends the signal.
	/// </summary>
	/// <param name="signal_type">The signal type.</param>
	/// <param name="signal_pid">The signal target process id.</param>
	void WINDOWSKILLLIBRARY_API sendSignal(DWORD signal_type, DWORD signal_pid);
};

