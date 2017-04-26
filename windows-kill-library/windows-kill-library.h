#pragma once
#include <Windows.h>

#ifdef _USRDLL
#ifdef WINDOWSKILLLIBRARY_EXPORTS
#define MATHLIBRARY_API __declspec(dllexport)
#else
#define MATHLIBRARY_API __declspec(dllimport)
#endif
#else
#define MATHLIBRARY_API
#endif // 



namespace WindowsKillLibrary
{
	/// <summary>
	/// Signal type of CTRL + C
	/// </summary>
	const int SIGNAL_TYPE_CTRL_C = (int)CTRL_C_EVENT;

	/// <summary>
	/// Signal type of CTRL + Break
	/// </summary>
	const int SIGNAL_TYPE_CTRL_BREAK = (int)CTRL_BREAK_EVENT;

	/// <summary>
	/// Sends the signal.
	/// </summary>
	/// <param name="signal_type">The signal type.</param>
	/// <param name="signal_pid">The signal target process id.</param>
	void MATHLIBRARY_API sendSignal(int signal_type, int signal_pid);
};

