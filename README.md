# 4kdownloader+ Hook

This project is a hook used to modify the behavior of the program 4k Video Downloader Plus.   

## Purpose

I found it very annoying that 4k Video Downloader Plus opened the browser to display the thank you page after each update. Therefore, I used this hook template to block all system calls to `ShellExecuteA` and `ShellExecuteW`. These functions are used to open a link in the default browser.

## How It Works

The hook intercepts calls to `ShellExecuteA` and `ShellExecuteW` and prevents them from executing if the URL starts with "http". This effectively blocks the program from opening the thank you page in the browser, but still allows opeing video files from inside 4k Video Downloader Plus.

## How to use

Inside the 4kdownloaderplus folder, create a new folder named like the .exe file with the ending `.plugin` . For example `4kvideodownloaderplus.exe.plugins`.  
Place the compiled DLL file (``example_plugin.dll``) inside this folder. Then copy the ``version.dll`` file into the 4kdownloaderplus folder and use the program as usual.

## Recent Updates

According to the [patch notes](https://www.4kdownload.com/blog/2024/11/19/new-1-10-version-of-4k-video-downloader-plus-is-here/), the developers have recently removed this annoying behavior. However, this hook can still be useful for other purposes or for older versions of the software.

## Disclaimer

**This project is for educational purposes only. I am not responsible for any harm or damage caused by the use of this hook.**

# pseudo-version-dll

Original repo: https://github.com/t-mat/pseudo-version-dll

## What's this?

This is a Visual Studio 2022 project which generates (pseudo) `version.dll`.  Since it passes all arguments to genuine `version.dll`, virtually both of them have same functionality as `version.dll`.

Since many Windows executables load `version.dll`, you can use this project to add functionalities/hooks/hacks to target executable.
This project also provides same functionality for `lz32.dll`, `hid.dll` and `msimg32.dll`.

For hooking purpose, you may also interest in API hook library [minhook](https://github.com/TsudaKageyu/minhook).


## How to build

- Run build-vs2022.bat
- Artifacts will be put under artifacts/
  - If you need, you can rename `version.dll` to `lz32.dll`, `hid.dll` or `msimg32.dll`.


## Test

- Check your target executable by [`Dependencies`](https://github.com/lucasg/Dependencies).
  - If your target doesn't use `version.dll` or supported DLLs, you can't use this project.
- Put `artifacts/version.dll` (or renamed one) into your tareget's directory.  Executable and DLL must be put on the same directory.
  - If your target is `<PATH>/<TO>/<YOUR>/<TARGET>.exe` you must put `version.dll` in `<PATH>/<TO>/<YOUR>/version.dll`
- Put `artifacts/plugins/example_plugin.dll` to `<PATH>/<TO>/<YOUR>/<TARGET>.exe.plugins/example_plugin.dll`
- Open [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)
- Run your`<TARGET>.exe`
  - DebugView shows `CreateFileW()` activities which is hooked by `example_plugin.dll`.


## Plugin loading rules

- `version.dll` loads all DLLs which are placed in `YOUR-TARGET.exe-PATH/YOUR-TARGET.exe.plugins/`.
    - DLLs are loaded from mutiple directories recursively.
        - If directory's or file's name is started with `.`, they're ignored. e.g. `.my-private-dir/`, `.my-private-file.dll`, etc.
    - DLL loading order is decided by `std::sort()` for `std::wstring` paths of DLLs.
        - e.g. `YOUR-PATH\01\*.dll` is loaded before `YOUR-PATH\99\*.dll` 
