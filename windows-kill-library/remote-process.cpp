#include "stdafx.h"
#include "remote-process.h"
#include "signal.h"
#include "ctrl-routine.h"

namespace WindowsKillLibrary {
	RemoteProcess::RemoteProcess() {
		this->process_token = NULL;
		this->handle = NULL;
		this->remote_thread = NULL;
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

			try {
				if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &this->process_token)) {
					throw std::runtime_error(std::string("remote-process:open:OpenProcessToken:code:") + std::to_string(GetLastError()));
				}
				
				if (!this->setPrivilege(true)) {
					throw std::runtime_error(std::string("remote-process:open:setPrivilege(true)"));
				}

				this->handle = OpenProcess(RemoteProcess::NEEDEDACCESS, false, this->the_signal->getPid());

				if (this->handle == NULL) {
					this->setPrivilege(false);
					throw std::runtime_error(std::string("remote-process:open:setPrivilege(true):OpenProcess:code:") + std::to_string(GetLastError()));
				}
			}
			catch (std::runtime_error& exception) {
				if (this->process_token != NULL) {
					if (!CloseHandle(this->process_token)) {
						throw std::runtime_error(std::string("remote-process:open:close_process_token:code:") + std::to_string(GetLastError()));
					}
				}
				throw std::runtime_error(std::string(exception.what()));
			}
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
			throw std::runtime_error(std::string("remote-process:startRemoteThread:CreateRemoteThread:code:") + std::to_string(GetLastError()));
		}

		try {
			if (ResumeThread(this->remote_thread) == (DWORD)-1) {
				throw std::runtime_error(std::string("remote-process:startRemoteThread:ResumeThread:code:") + std::to_string(GetLastError()));
			}

			if (WaitForSingleObject(this->remote_thread, INFINITE) != WAIT_OBJECT_0) {
				throw std::runtime_error(std::string("remote-process:startRemoteThread:WaitForSingleObject:code:") + std::to_string(GetLastError()));
			}

			DWORD exit_code = 0;
			if (!GetExitCodeThread(this->remote_thread, (LPDWORD)&exit_code)) {
				throw std::runtime_error(std::string("remote-process:startRemoteThread:GetExitCodeThread:code:") + std::to_string(GetLastError()));
			}

			if (exit_code = STATUS_CONTROL_C_EXIT) {
				this->closeRemoteThread();
			} else {
				this->closeRemoteThread();
				throw std::runtime_error(std::string("remote-process:startRemoteThread:remote_function:code"));
			}
		}
		catch (std::runtime_error& exception) {
			this->closeRemoteThread();
			throw std::runtime_error(std::string(exception.what()));
		}
	}

	void RemoteProcess::closeRemoteThread(void) {
		if (this->remote_thread != NULL) {
			if (!CloseHandle(this->remote_thread)) {
				throw std::runtime_error(std::string("remote-process:startRemoteThread:remote_thread:code:") + std::to_string(GetLastError()));
			}
		}
	}
}
