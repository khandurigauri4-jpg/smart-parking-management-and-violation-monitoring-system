# Smart Parking Management and Violation Monitoring System

A full-stack smart parking system for automated vehicle identification, parking allocation, reservations, digital payments, and violation management.

## License plate recognition (C++)

The first image-processing module is `image_processing/license_plate.cpp`. It uses:

- **C++** for the application
- **OpenCV** to open the webcam, process captured frames, and search for plate-shaped contours
- **Tesseract OCR** to attempt to read text from a detected plate

This module is independent of the university DSA component (C) and OOP component (C++).

### Requirements

- 64-bit Windows 10 or Windows 11
- Visual Studio Code (VS Code)
- Git, if cloning the repository
- A working webcam
- MSYS2 UCRT64, with its matching GCC, OpenCV, and Tesseract packages

Use the compiler and libraries from the same MSYS2 UCRT64 installation. Do not combine MSYS2 UCRT64 with the older MinGW.org compiler, another MinGW installation, or unrelated OpenCV/Tesseract libraries.

### 1. Check the computer before installing anything

Open **PowerShell** (VS Code's terminal is not required yet). These checks do not install, delete, or change anything:

```powershell
Get-ComputerInfo | Select-Object WindowsProductName, OsArchitecture
Get-Command git, code, winget -ErrorAction SilentlyContinue | Select-Object Name, Source
if (Get-Command git -ErrorAction SilentlyContinue) {
    git --version
} else {
    Write-Output 'Git is not on PATH.'
}
```

`OsArchitecture` should be `64-bit`. If Git is missing and the repository is not already downloaded, install Git for Windows from [git-scm.com/download/win](https://git-scm.com/download/win), then reopen PowerShell. If `code` is not found, VS Code may still be installed; open VS Code from the Start menu and use **File → Open Folder**. If `winget` is unavailable, use the MSYS2 website installer in step 3 instead.

Look for common MSYS2 installation locations:

```powershell
$candidates = @('C:\msys64', "$env:USERPROFILE\msys64", "$env:USERPROFILE\msys64-ucrt")
foreach ($candidate in $candidates) {
    [pscustomobject]@{
        Path = $candidate
        HasMsys2Shell = Test-Path (Join-Path $candidate 'usr\bin\bash.exe')
        HasPacman = Test-Path (Join-Path $candidate 'usr\bin\pacman.exe')
        HasUcrt64Compiler = Test-Path (Join-Path $candidate 'ucrt64\bin\g++.exe')
    }
}
```

An MSYS2 base installation is usable if both `HasMsys2Shell` and `HasPacman` are `True`. If you find one, use that root as `$msys`; the UCRT64 compiler may still need to be installed as a package in step 4. If none of these common locations matches but MSYS2 appears in the Start menu or Windows **Installed apps**, locate its folder and check it manually. If the MSYS2 shell or Pacman is missing, treat that installation as incomplete: do not delete or overwrite it; choose a different, unused installation directory instead.

Check for C++ compilers and OCR tools already on PATH:

```powershell
Get-Command g++, gcc, clang++, cl, tesseract, pkg-config -ErrorAction SilentlyContinue |
    Select-Object Name, Source
where.exe g++ 2>$null
where.exe tesseract 2>$null
```

An existing `g++` is not necessarily suitable. The supported compiler for this setup is specifically:

```text
<MSYS2 folder>\ucrt64\bin\g++.exe
```

For example, `C:\MinGW\bin\g++.exe` from MinGW.org is a different toolchain; do not use it for this build. OpenCV and Tesseract do not need separate installers if you install their MSYS2 UCRT64 packages in step 4.

### 2. Get or locate this repository

If the repository is already on the computer, locate its folder and use that path in later commands. If it is not downloaded and Git is installed, open PowerShell and clone the branch containing this module (`devansh-oop`):

```powershell
git clone --branch devansh-oop --single-branch https://github.com/khandurigauri4-jpg/smart-parking-management-and-violation-monitoring-system.git
```

The clone creates a `smart-parking-management-and-violation-monitoring-system` folder under the current PowerShell directory. You can choose a different parent folder first with `cd`.

Open the repository in VS Code. Either use **File → Open Folder**, or, if the `code` command was found:

```powershell
code 'C:\path\to\smart-parking-management-and-violation-monitoring-system'
```

Replace the example with the actual folder. In VS Code, use **Terminal → New Terminal** and select **PowerShell** from the terminal profile menu if needed.

### 3. Install MSYS2 only if it is missing or incomplete

If step 1 found an MSYS2 base installation with both its shell and Pacman, skip the installer and set `$msys` to that installation's root folder. For example:

```powershell
$msys = 'C:\msys64'
```

If MSYS2 is not installed, choose an unused, short installation path. The examples use `C:\msys64`. First check that the destination does not already exist:

```powershell
Test-Path 'C:\msys64'
```

Only if this returns `False`, install from PowerShell using WinGet:

```powershell
winget install --id MSYS2.MSYS2 --exact --accept-package-agreements --accept-source-agreements --override "in --confirm-command --accept-messages --root C:/msys64"
```

Read and confirm the installer prompts. If `C:\msys64` already exists, **do not run that command against the existing directory**. Instead, verify that existing installation or choose a different new directory, such as `C:\msys64-ucrt`, and use that chosen path consistently below.

If WinGet is unavailable, download the installer from [msys2.org](https://www.msys2.org/), run it, and select the unused installation folder you chose. Do not install over a partial or unknown existing folder.

After installation, set the path in the PowerShell terminal. Use the actual installation root, not its `ucrt64` subfolder:

```powershell
$msys = 'C:\msys64'
Test-Path "$msys\usr\bin\bash.exe"
Test-Path "$msys\usr\bin\pacman.exe"
```

Both checks should return `True`. If not, stop and check the installer location before proceeding.

### 4. Install or verify the UCRT64 compiler and dependencies

From the Windows Start menu, open the **MSYS2 UCRT64** terminal for the same `$msys` installation. It must be UCRT64, not MSYS, MinGW64, CLANG64, or another terminal profile.

Update the MSYS2 system packages:

```bash
pacman -Syu
```

If it tells you to close/reopen the terminal, do so, open **MSYS2 UCRT64** again, and run `pacman -Syu` until it reports there is nothing left to update.

Check whether the required UCRT64 packages are already installed:

```bash
pacman -Q mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-opencv mingw-w64-ucrt-x86_64-tesseract-ocr mingw-w64-ucrt-x86_64-tesseract-data-eng mingw-w64-ucrt-x86_64-pkgconf mingw-w64-ucrt-x86_64-qt6-5compat
```

If every package prints a version, they are installed. If any package is reported missing, install the complete list below. `--needed` tells Pacman to skip packages that are already up to date; it does not reinstall them unnecessarily:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-opencv mingw-w64-ucrt-x86_64-tesseract-ocr mingw-w64-ucrt-x86_64-tesseract-data-eng mingw-w64-ucrt-x86_64-pkgconf mingw-w64-ucrt-x86_64-qt6-5compat
```

Pacman may ask for confirmation and may install many dependencies. Review the package list and type `Y` to continue. These packages are installed inside MSYS2; they do not require manually adding random directories to the permanent Windows PATH.

Verify the tools and language data from that same UCRT64 terminal:

```bash
which g++
g++ --version
pkg-config --modversion opencv5 tesseract
tesseract --list-langs
```

`which g++` should resolve under `/ucrt64/bin`, the package command should print versions, and the Tesseract language list should contain `eng`. If any check fails, return to the package check/install above rather than downloading unrelated Windows builds.

### 5. Configure the VS Code PowerShell terminal

In VS Code choose **Terminal → New Terminal** and use **PowerShell**. Run these commands in that terminal, changing `$msys` to the MSYS2 root identified in steps 1–3:

```powershell
$msys = 'C:\msys64'
if (-not (Test-Path "$msys\ucrt64\bin\g++.exe")) { throw "UCRT64 GCC not found under $msys" }
if (-not (Test-Path "$msys\ucrt64\bin\pkg-config.exe")) { throw "UCRT64 pkg-config not found under $msys" }
if (-not (Test-Path "$msys\ucrt64\share\tessdata\eng.traineddata")) { throw "English Tesseract data not found under $msys" }

$env:Path = "$msys\ucrt64\bin;$env:Path"
$env:TESSDATA_PREFIX = "$msys\ucrt64\share\tessdata"
```

For example, if the installation is at `C:\Users\Alex\msys64`, set `$msys` to that exact path. Do not copy another person's user-specific path. These settings only affect this PowerShell terminal session; repeat them in a new terminal.

Check the selected tools and data paths:

```powershell
& "$msys\ucrt64\bin\g++.exe" --version
& "$msys\ucrt64\bin\pkg-config.exe" --modversion opencv5 tesseract
& "$msys\ucrt64\bin\tesseract.exe" --list-langs
```

### 6. Build the program in VS Code

In the same PowerShell terminal, change directory to the repository's actual root folder. The root is the folder containing `README.md` and `image_processing`:

```powershell
Set-Location 'C:\path\to\smart-parking-management-and-violation-monitoring-system'
Test-Path '.\image_processing\license_plate.cpp'
```

The test should return `True`. Then get the flags from the installed OpenCV/Tesseract packages and compile the C++ source:

```powershell
$flags = (& "$msys\ucrt64\bin\pkg-config.exe" --cflags --libs opencv5 tesseract) -split '\s+'
if ($LASTEXITCODE -ne 0) { throw 'pkg-config could not find OpenCV or Tesseract' }

& "$msys\ucrt64\bin\g++.exe" -Wall -Wextra -pedantic .\image_processing\license_plate.cpp -o "$env:TEMP\license_plate.exe" @flags
if ($LASTEXITCODE -ne 0) { throw 'Compilation failed; read the compiler error above' }
```

The executable is written to the current Windows account's temporary directory, not the repository. This avoids overwriting or adding build output to project files. Warnings may appear from OpenCV's headers; a successful build means GCC exits with code `0`.

### 7. Start and use it

Run it from the same PowerShell terminal, which has the MSYS2 runtime DLL directory in `PATH` and Tesseract data configured:

```powershell
& "$env:TEMP\license_plate.exe"
```

1. Wait for the camera preview window.
2. Point the camera at a clear, well-lit, nearly straight-on license plate.
3. Click the preview so it has keyboard focus.
4. Press **Space** to capture one frame.
5. Read the terminal:
   - `Image captured.` confirms a frame was captured.
   - `License plate not detected.` means the current contour/shape check did not find a candidate; OCR was not attempted for that frame.
   - `License plate detected.` means the program found a candidate and proceeded to the OCR stage.
   - `Vehicle Number: ...` is the recognized text, which may still need manual checking.
6. Press **Esc** while the preview has focus to close the program.

### Troubleshooting

- **`g++` is not recognized, or the wrong compiler is selected:** do not use a different MinGW compiler. Set `$msys` correctly and call `"$msys\ucrt64\bin\g++.exe"` as shown above.
- **`opencv2/opencv.hpp` not found:** check that the UCRT64 OpenCV package is installed with `pacman -Q`, then use the UCRT64 compiler and pkg-config commands. Do not copy headers manually.
- **`pkg-config` cannot find `opencv5` or `tesseract`:** open the correct MSYS2 UCRT64 terminal, install the UCRT64 packages, and verify the `$msys` path points to that same installation.
- **`Tesseract could not start` or `eng.traineddata` not found:** confirm the English data package is installed and that `$env:TESSDATA_PREFIX` is the directory containing `eng.traineddata`. The PowerShell preflight above checks this file.
- **The camera cannot be opened:** check Windows **Settings → Privacy & security → Camera**, allow desktop apps to access the camera, close other apps using it, and confirm Windows detects the webcam.
- **The camera opens but says `License plate not detected.`:** improve lighting, reduce glare, move the plate closer, and hold the camera steady and straight. This project uses a basic contour/aspect-ratio detector; it is not guaranteed to detect every plate and may select other rectangular objects.
- **OCR is empty or incorrect:** detection must succeed first. Then OCR still depends on a sharp, legible plate crop, supported characters, and favorable lighting. Do not treat recognized text as verified without checking it.
- **OpenCV prints compiler warnings:** warnings from OpenCV headers do not necessarily mean compilation failed; check the compiler exit code and whether the executable was produced.
- **A setup command says a folder already exists:** do not delete it just to follow the example. Check whether it contains `usr\bin\bash.exe`, `usr\bin\pacman.exe`, and `ucrt64\bin\g++.exe`; reuse only a complete installation, otherwise choose a different unused install location.

### What has been verified

On the development computer, the program compiled with MSYS2 UCRT64 GCC, OpenCV, and Tesseract. The webcam opened, and OpenCV captured frames. A separate C++ test initialized the Tesseract API with the installed English model. Webcam test frames did not consistently pass plate detection, so successful OCR of a real license plate has not yet been confirmed.

### Commit and push these changes

The tested working branch is `devansh-oop`. From the repository root in PowerShell, review the branch and worktree first:

```powershell
git status --short --branch
git branch --show-current
```

Stage only the license-plate source and this README, review what will be committed, then commit and push to the existing branch:

```powershell
git add README.md image_processing/license_plate.cpp
git diff --cached --stat
git diff --cached
git commit -m "Document and configure license plate module" -m "Co-authored-by: Copilot <223556219+Copilot@users.noreply.github.com>"
git push origin devansh-oop
```

The explicit `git add` paths avoid staging unrelated changes or generated executables. Confirm the staged diff contains only the intended files before committing. If working on a different branch, replace `devansh-oop` in the push command with that branch's name.
