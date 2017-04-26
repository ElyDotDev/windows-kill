#pragma once
#include "ctrl-routine.h"
#include "signal.h"

namespace WindowsKillLibrary {
	/// <summary>
	/// The main part that will send the given signal to target process.
	/// </summary>
	class Sender
	{
	private:
		Sender();
		static CtrlRoutine ctrl_c_routine;
		static CtrlRoutine ctrl_break_routine;
	public:
		/// <summary>
		/// Sends the signal to the target process.
		/// </summary>
		/// <param name="the_signal">Signal instance</param>
		static void send(Signal the_signal);
	};
}

