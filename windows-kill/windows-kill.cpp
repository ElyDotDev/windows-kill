#include "stdafx.h"
#include "windows-kill-library.h"
#include <cstdlib>
#include <iostream>
#include <Windows.h>

using std::cout;
using std::endl;
using std::invalid_argument;
using std::system_error;
using std::runtime_error;
using std::exception;

using WindowsKillLibrary::sendSignal;
using WindowsKillLibrary::SIGNAL_TYPE_CTRL_C;
using WindowsKillLibrary::SIGNAL_TYPE_CTRL_BREAK;

#define WINDOWSKILL_VERSION "1.1.4"

int main(int argc,char *argv[])
{
	DWORD signal_type;
	DWORD signal_pid;
	char* endptr;

	cout << "Windows Kill " << WINDOWSKILL_VERSION << " | Windows Kill Library "<< WINDOWSKILLLIBRARY_VERSION << endl;

	if (argc == 1) {
		cout << "Not enough argument. Use -h for help." << endl;
		return EXIT_FAILURE;
	}
	else if (argc == 2) {
		if (strcmp(argv[1], "-h") == 0) {
			cout << "You should give the signal type and the pid to send the signal.\n"
				<< "Example: windows-kill -SIGINT 1234\n"
				<< "-l\t List of available signals type." << endl;
		}
		else if (strcmp(argv[1], "-l") == 0) {
			cout << "Availabe Signal Types\n"
				 << "\t(1) (SIGBREAK) : CTR + Break\n"
				 << "\t(2) (SIGINT) : CTR + C\n" << endl;
		}
		else {
			cout << "Not enough argument. Use -h for help." << endl;
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	else if (argc == 3) {
		if (strcmp(argv[1], "-1") == 0 || strcmp(argv[1], "-SIGBREAK") == 0) {
			signal_type = SIGNAL_TYPE_CTRL_BREAK;
		}
		else if (strcmp(argv[1], "-2") == 0 || strcmp(argv[1], "-SIGINT") == 0) {
			signal_type = SIGNAL_TYPE_CTRL_C;
		}
		else {
			cout << "Signal type " << argv[1] << " not supported. Use -h for help." << endl;
			return EXIT_FAILURE;
		}

		signal_pid = strtoul(argv[2], &endptr, 10);

		if ((endptr == argv[1]) || (*endptr != '\0')) {
			cout << "Invalid pid: " << argv[2] << endl;
			return EXIT_FAILURE;
		}
	}

	try {
		sendSignal(signal_pid, signal_type);
		cout << "Signal sent successfully. type: " << signal_type << " | pid: " << signal_pid << "\n";
	}
	catch (const invalid_argument& exception) {
		if (strcmp(exception.what(), "ESRCH") == 0) {
			cout << "Error: Pid dosen't exist." << endl;
			return EXIT_FAILURE;
		}
		else if(strcmp(exception.what(), "EINVAL") == 0){
			cout << "Error: Invalid signal type." << endl;
			return EXIT_FAILURE;
		}
		else {
			cout << "InvalidArgument: windows-kill-library: " << exception.what() << endl;
			return EXIT_FAILURE;
		}
	}
	catch (const system_error& exception) {
		cout << "SystemError " << exception.code() << ": " << exception.what() << endl;
		return EXIT_FAILURE;
	}
	catch (const runtime_error& exception) {
		if (strcmp(exception.what(), "EPERM") == 0) {
			cout << "Not enough permission to send the signal." << endl;
			return EXIT_FAILURE;
		}
		else {
			cout << "RuntimeError: windows-kill-library: " << exception.what() << endl;
			return EXIT_FAILURE;
		}
	}
	catch (const exception& exception) {
		cout << "Error: windows-kill-library: " << exception.what() << endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

