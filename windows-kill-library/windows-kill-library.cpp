#include "stdafx.h"
#include "windows-kill-library.h"
#include "signal.h"
#include "sender.h"

namespace WindowsKillLibrary
{
	void sendSignal(DWORD signal_type, DWORD signal_pid) {
		Signal the_signal(signal_type, signal_pid);
		Sender::send(the_signal);
	}
};

