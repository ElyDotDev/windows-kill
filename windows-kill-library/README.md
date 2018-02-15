# windows-kill-library
### Library to send signal to process by PID in Windows, like POSIX kill

Windows has no process signaling mechanism like what POSIX provide using the kill command. But by using **windows-kill-library**, process signaling is possible. :)

### Why windows-kill-library?
Well, I'm a node.js developer. Node has a functionality that could send signals to other process, by their PID. This functionality works great on POSIX OSes but in windows, no signal is supported (even SIGINT and SIGBREAK, that node stated is supported in windows), and sending any of those signals to any process in windows, will result in an immediate kill of those process (Even if those process have listener on sent signals). In one of my project signal sending and listening was a serious need, so I've made some research about signal sending in windows, and found an OLD (2003) project named [SendSignal](http://www.latenighthacking.com/projects/2003/sendSignal/). Well that project doesn’t support 64bit systems and also could only send SIGNBREAK (Ctrl + Break) signal. More searches lead me to an [enhanced version of SendSignal](https://github.com/walware/statet/tree/master/de.walware.statet.r.console.core/cppSendSignal), that add support of 64bit systems, but only support sending SIGINT (Ctrl + C) signal. So I've decided to write a library that support both 32bit & 64bit systems, and also both SIGNBREAK and SIGINT signals. The result is the **windows-kill-library** that has all the functionality I've needed. If you want the standalone software to send signal, please see **[windows-kill](https://github.com/alirdn/windows-kill)**.


### Features
- Support both 32bit (Win32) & 64bit (x64) Windows
- Support both SIGNBREAK (Ctrl + Break) and SIGINT (Ctrl + C) Signals
- A library that could be used directly (#include), As a static library (.lib) and a dynamic library (.dll)

## Usage Instruction
### Prebuilt libraries
Fastest and easiest way to use the **windows-kill-library** is download the prebuilt libraries. You could find them in repo's [Releases page](https://github.com/alirdn/windows-kill/releases). The library has been built both as a dynamic library (.dll) and static library (.lib) for both 32bit (Win32) and 64bit (x64) Windows.

### Build & direct use of source code
If you want to use the source code, you must install Visual Studio plus C++ development tools first. I'm developing this project in Visual Studio 2015 update 3. So it's recommended to use the same Visual Studio version. But maybe other versions are ok to use. Also there is no configuration file for other compilers. So any contribution to add other compilers support is welcomed!

After the Visual Studio installation, clone the GitHub project or download the [latest master branch source code](https://github.com/alirdn/windows-kill/archive/master.zip) and extract the downloaded zip.
If you want to build the library, go to the project folder and open the windows-kill.sln in Visual Studio. From the build menu, click on batch build. There you could see different build configurations. For more information about build configuration see [Build Configurations](https://github.com/alirdn/windows-kill#build-configurations). Also you could include the library *windows-kill-library.h* in your C++ project source and use the library directly.

## Usage Examples
Using the **windows-kill-library** is easy & straightforward. The library only has one exported method. Also two constants are exported.

### WindowsKillLibrary::sendSignal
This method is the only thing you should call to send the signal to process. You should give this method the signal type that is exported as constant, and the target process id (PID).
```c++
/// <summary>
/// Sends the signal.
/// </summary>
/// <param name="signal_pid">The signal target process id.</param>
/// <param name="signal_type">The signal type.</param>
void sendSignal(DWORD signal_pid, DWORD signal_type);
```

This method will throw an exception in case of any error. If no exception thrown, The signal has been sent successfully.

All the exception could be catched by catching ```std::exception```. But two ```std::invalid_argument``` will be sent in case of invalid ```signal_type``` and ```signal_pid```. Invalid ```signal_type``` exception message is ```EINVAL``` and invalid ```signal_pid``` exception message is ```ESRCH```.

#### Exception handeling example code
```c++
#include <stdexcept>
#include "windows-kill-library.h"

using WindowsKillLibrary::sendSignal;
using WindowsKillLibrary::SIGNAL_TYPE_CTRL_C;
using WindowsKillLibrary::SIGNAL_TYPE_CTRL_BREAK;

try {
    sendSignal(signal_pid, signal_type);
    std::cout << "Signal sent successfuly. type: " << signal_type << " | pid: " << signal_pid << "\n";
}
catch (const std::invalid_argument& exception) {
    if (strcmp(exception.what(), "ESRCH") == 0) {
        std::cout << "Error: Pid dosen't exist." << std::endl;
    }
    else if(strcmp(exception.what(), "EINVAL") == 0){
        std::cout << "Error: Invalid signal type." << std::endl;
    }
    else {
        std::cout << "InvalidArgument: windows-kill-library:" << exception.what() << std::endl;
    }
}
catch (const std::system_error& exception) {
    std::cout << "SystemError " << exception.code() << ": " << exception.what() << std::endl;
}
catch (const std::runtime_error& exception) {
    if (strcmp(exception.what(), "EPERM") == 0) {
        std::cout << "Not enough permission to send the signal." << std::endl;
    }
    else {
        std::cout << "RuntimeError: windows-kill-library:" << exception.what() << std::endl;
    }
}
catch (const std::exception& exception) {
    std::cout << "Error: windows-kill-library:" << exception.what() << std::endl;
}
```

### WindowsKillLibrary::SIGNAL_TYPE_CTRL_BREAK
The **SIGBREAK** signal type.

### WindowsKillLibrary::SIGNAL_TYPE_CTRL_C
The **SIGINT** signal type.

## Resources
The **windows-kill-library** is based on:
- [SendSignal](http://www.latenighthacking.com/projects/2003/sendSignal/)
- [Enhanced version of SendSignal](https://github.com/walware/statet/tree/master/de.walware.statet.r.console.core/cppSendSignal)

## Contributing
We love contributions from everyone. Please read [Contributing guide](https://github.com/alirdn/windows-kill/CONTRIBUTING).

## License
[MIT](https://github.com/alirdn/windows-kill/LICENSE)
