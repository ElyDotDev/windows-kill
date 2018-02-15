# windows-kill
### Send signal to process by PID in Windows, like POSIX kill

Windows has no process signaling mechanism like what POSIX provide using the kill command. But **windows-kill** could send signal to process by PID. :)

### Why windows-kill?
Well, I'm a node.js developer. Node has a functionality that could send signals to other process, by their PID. This functionality works great on POSIX OSes but in windows, no signal is supported (even SIGINT and SIGBREAK, that node stated is supported in windows), and sending any of those signals to any process in windows, will result in an immediate kill of those process (Even if those process have listener on sent signals). In one of my project signal sending and listening was a serious need, so I've made some research about signal sending in windows, and found an OLD (2003) project named [SendSignal](http://www.latenighthacking.com/projects/2003/sendSignal/). Well that project doesn’t support 64bit systems and also could only send SIGNBREAK (Ctrl + Break) signal. More searches lead me to an [enhanced version of SendSignal](https://github.com/walware/statet/tree/master/de.walware.statet.r.console.core/cppSendSignal), that add support of 64bit systems, but only support sending SIGINT (Ctrl + C) signal. So I've decided to write a library that support both 32bit & 64bit systems, and also both SIGNBREAK and SIGINT signals. The result is the **windows-kill-library** that has all the functionality I've needed and is the heart of **windows-kill**. For information about **windows-kill-library** navigate to [windows-kill-library folder](https://github.com/alirdn/windows-kill/tree/master/windows-kill-library).


### Features
- Support both 32bit (Win32) & 64bit (x64) Windows
- Support both SIGNBREAK (Ctrl + Break) and SIGINT (Ctrl + C) Signals
- A library that could be used directly (#include), As a static library (.lib) and a dynamic library (.dll)
- Prebuilt binaries and libraries

## Installation
### Prebuilt Binaries
Fastest and easiest way to install and use the **windows-kill** is download the prebuilt binaries. You could find them in repo's [Releases page](https://github.com/alirdn/windows-kill/releases).
Both the 32bit (Win32) and 64bit (x64) versions are available.

### Compile & Build from Source code
If you want to compile from the source code, you must install Visual Studio plus C++ development tools first. I'm developing this project in Visual Studio 2015 update 3. So it's recommended to use the same Visual Studio version. But maybe other versions are ok to use. Also I didn't compile the project using other C++ compilers. So any contribution to add other compilers support is welcomed!

After the Visual Studio installation, clone the GitHub project or download the [latest master branch source code](https://github.com/alirdn/windows-kill/archive/master.zip) and extract the downloaded zip.
Go to the project folder and open the windows-kill.sln in Visual Studio. From the build menu, click on batch build. There you could see different build configurations. For more information about build configuration see [Build Configurations](#build-configurations).

## Usage Examples
Using the **windows-kill** is easy & straightforward. It's just like POSIX kill. Just navigate to the folder that contains windows-kill.exe and open a cmd from that folder. Or add that folder in Environment Variables so you could use the command from any cmd.
If signal sending was successful or any error occurred during the sending, appropriate message will be print in cmd.

### Sending signal to PID
```bash
>windows-kill -SIGNALTYPE PID
```

#### Sending SIGBREAK (Ctrl + Break) to sample 1234 PID
```bash
>windows-kill -SIGBREAK 1234
```
```bash
>windows-kill -1 1234
```

#### Sending SIGINT (Ctrl + C) to sample 1234 PID
```bash
>windows-kill -SIGINT 1234
```
```bash
>windows-kill -2 1234
```

#### List supported signal types
```bash
>windows-kill -l
```

#### Usage help
```bash
>windows-kill -h
```

## Build Configurations
**windows-kill** Visual Studio solution contains different build configuration. There are two main **Debug** and **Release** configuration category. Each of these categories have two separate configurations for building the **windows-kill-library** as a dynamic link library (.dll) or static link library (.lib). Also all the build configurations in Visual Studio has 32bit (Win32) & 64bit (x64) versions. In Summary there are 4 build configurations:
- **Debug Dll**: Build **windows-kill-library** as a .dll with debug enabled.
- **Debug Lib**: Build **windows-kill-library** as a .lib with debug enabled.
- **Release Dll**: Build **windows-kill-library** as a .dll without debug.
- **Release Lib**: Build **windows-kill-library** as a .lib without debug.

## Resources
The **windows-kill** & **windows-kill-library** are based on:
- [SendSignal](http://www.latenighthacking.com/projects/2003/sendSignal/)
- [Enhanced version of SendSignal](https://github.com/walware/statet/tree/master/de.walware.statet.r.console.core/cppSendSignal)

## Contributing
We love contributions from everyone. Please read [Contributing guide](https://github.com/alirdn/windows-kill/CONTRIBUTING).

## License
[MIT](https://github.com/alirdn/windows-kill/LICENSE)
