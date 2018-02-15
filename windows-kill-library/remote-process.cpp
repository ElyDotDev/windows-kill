#include "stdafx.h"
#include "remote-process.h"
#include "signal.h"
#include "ctrl-routine.h"

namespace WindowsKillLibrary {
	RemoteProcess::RemoteProcess() {
		this->process_token = (HANDLE)NULL;
		this->handle = (HANDLE)NULL;
		this->remote_thread = (HANDLE)NULL;
	}

	void RemoteProcess::setSignal(Signal* the_signal) {
		this->the_signal = the_signal;
	}

	void RemoteProcess::setCtrlRoutine(CtrlRoutine* the_ctrl_routine) {
		this->the_ctrl_routine = the_ctrl_routine;
	}

	void RemoteProcess::open(void) {
		this->handle = OpenProcess(RemoteProcess::NEEDEDACCESS, false, this->the_signal->getPid());

		if (this->handle == NULL) {
			if (GetLastError() != ERROR_ACCESS_DENIED) {
				throw std::runtime_error(std::string("remote-process:open:OpenProcess:code:") + std::to_string(GetLastError()));
			}

			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &this->process_token)) {
				this->closeProcessToken();
				throw std::runtime_error(std::string("remote-process:open:OpenProcessToken:code:") + std::to_string(GetLastError()));
			}

			if (!this->setPrivilege(true)) {
				this->closeProcessToken();
				throw std::runtime_error(std::string("remote-process:open:setPrivilege(true)"));
			}

			this->handle = OpenProcess(RemoteProcess::NEEDEDACCESS, false, this->the_signal->getPid());

			if (this->handle == NULL) {
				this->setPrivilege(false);
				this->closeProcessToken();
				throw std::runtime_error(std::string("remote-process:open:setPrivilege(true):OpenProcess:code:") + std::to_string(GetLastError()));
			}
		}
	}

	void RemoteProcess::closeProcessToken(void) {
		if (this->process_token != NULL) {
			if (!CloseHandle(this->process_token)) {
				throw std::runtime_error(std::string("remote-process:open:close_process_token:code:") + std::to_string(GetLastError()));
			}
			this->process_token = (HANDLE)NULL;
		}
	}

	bool RemoteProcess::setPrivilege(bool enable_privilege) {
		TOKEN_PRIVILEGES tp;
		LUID luid;

		if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid)) {
			return false;
		}

		tp.PrivilegeCount = 1;
		tp.Privileges[0].Luid = luid;
		if (enable_privilege) {
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		}
		else {
			tp.Privileges[0].Attributes = 0;
		}

		AdjustTokenPrivileges(this->process_token, false, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
		
		if (GetLastError() != 0L) {
			return false;
		}

		return true;
	}

	void RemoteProcess::startRemoteThread(void) {
		this->remote_thread = CreateRemoteThread(
			this->handle,
			NULL,
			0,
			this->the_ctrl_routine->getAddress(),
			(LPVOID)this->the_signal->getType(),
			CREATE_SUSPENDED,
			NULL
		);

		if (this->remote_thread == NULL) {
			this->closeHandle();
			throw std::runtime_error(std::string("remote-process:startRemoteThread:CreateRemoteThread:code:") + std::to_string(GetLastError()));
		}

		if (ResumeThread(this->remote_thread) == (DWORD)-1) {
			this->closeHandle();
			this->closeRemoteThread();
			throw std::runtime_error(std::string("remote-process:startRemoteThread:ResumeThread:code:") + std::to_string(GetLastError()));
		}

		if (WaitForSingleObject(this->remote_thread, INFINITE) != WAIT_OBJECT_0) {
			this->closeHandle();
			this->closeRemoteThread();
			throw std::runtime_error(std::string("remote-process:startRemoteThread:WaitForSingleObject:code:") + std::to_string(GetLastError()));
		}

		DWORD exit_code = 0;
		if (!GetExitCodeThread(this->remote_thread, (LPDWORD)&exit_code)) {
			this->closeHandle();
			this->closeRemoteThread();
			throw std::runtime_error(std::string("remote-process:startRemoteThread:GetExitCodeThread:code:") + std::to_string(GetLastError()));
		}

		if (exit_code = STATUS_CONTROL_C_EXIT) {
			this->closeHandle();
			this->closeRemoteThread();
		}
		else {
			this->closeHandle();
			this->closeRemoteThread();
			throw std::runtime_error(std::string("remote-process:startRemoteThread:remote_function:code"));
		}
	}

	void RemoteProcess::closeRemoteThread(void) {
		if (this->remote_thread != NULL) {
			if (!CloseHandle(this->remote_thread)) {
				throw std::runtime_error(std::string("remote-process:startRemoteThread:remote_thread:code:") + std::to_string(GetLastError()));
			}
			this->remote_thread = (HANDLE)NULL;
		}
	}

	void RemoteProcess::closeHandle(void) {
		if (this->handle != NULL) {
			if (!CloseHandle(this->handle)) {
				throw std::runtime_error(std::string("remote-process:open:close_process_token:code:") + std::to_string(GetLastError()));
			}
			this->handle = (HANDLE)NULL;
		}
	}
}
