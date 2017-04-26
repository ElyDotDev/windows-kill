#include "stdafx.h"
#include "windows-kill-library.h"
#include "signal.h"
#include "sender.h"

namespace WindowsKillLibrary
{
	void sendSignal(int signal_type, int signal_pid) {
		Signal the_signal((DWORD)signal_type, (DWORD)signal_pid);
		Sender::send(the_signal);
	}
};

