#include "stdafx.h"
#include "windows-kill-library.h"
#include "signal.h"
#include "sender.h"

namespace WindowsKillLibrary
{
	void sendSignal(DWORD signal_pid, DWORD signal_type) {
		Signal the_signal(signal_pid, signal_type);
		Sender::send(the_signal);
	}
};

