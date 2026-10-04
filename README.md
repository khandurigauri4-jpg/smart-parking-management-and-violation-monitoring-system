# Smart Parking Management and Violation Monitoring System

A full-stack smart parking system for automated vehicle identification, parking allocation, reservations, digital payments, and violation management.

## License plate recognition (C++)

The first image-processing module is `image_processing/license_plate.cpp`. It uses:

- **C++** for the application
- **OpenCV** to open the webcam, process captured frames, and search for plate-shaped contours
- **Tesseract OCR** to attempt to read text from a detected plate

This module is independent of the university DSA component (C) and OOP component (C++).

### Requirements

- 64-bit Windows 10 or 11
- Visual Studio Code
- Git, if cloning this repository
- A working webcam
- MSYS2 **UCRT64** with its matching GCC, OpenCV, and Tesseract packages

Use the MSYS2 UCRT64 compiler for this program. Do not mix it with another MinGW compiler or unrelated OpenCV/Tesseract libraries.

### 1. Get the repository

If the project is not already on the computer, open PowerShell and clone it:

```powershell
git clone https://github.com/khandurigauri4-jpg/smart-parking-management-and-violation-monitoring-system.git
cd smart-parking-management-and-violation-monitoring-system
```

If the repository is already present, open its folder in VS Code instead:

```powershell
code 'C:\path\to\smart-parking-management-and-violation-monitoring-system'
```

Replace the example path with the actual location of the repository.

### 2. Install MSYS2

First check whether MSYS2 is already installed. If it is, use its actual installation directory in the steps below.

If MSYS2 is not installed and `winget` is available, install it from PowerShell:

```powershell
winget install --id MSYS2.MSYS2 --exact --accept-package-agreements --accept-source-agreements --override "in --confirm-command --accept-messages --root C:/msys64"
```

Alternatively, download the installer from [msys2.org](https://www.msys2.org/) and install to `C:\msys64`.

> The developer's tested installation is at `C:\Users\devan\msys64-ucrt`. That path belongs to that Windows account. On another computer, the example commands use `C:\msys64`; adjust `$msys` if MSYS2 was installed elsewhere.

### 3. Update MSYS2 and install the dependencies

Open **MSYS2 UCRT64** from the Windows Start menu. Make sure the terminal title or prompt identifies the **UCRT64** environment.

Update its system packages:

```bash
pacman -Syu
```

If the terminal closes during the update, open **MSYS2 UCRT64** again and run `pacman -Syu` again.

Install the compiler, OpenCV, Tesseract OCR, English language data, package-config tool, and OpenCV's Qt display dependency:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-opencv mingw-w64-ucrt-x86_64-tesseract-ocr mingw-w64-ucrt-x86_64-tesseract-data-eng mingw-w64-ucrt-x86_64-pkgconf mingw-w64-ucrt-x86_64-qt6-5compat
```

Check that the tools and English data are available:

```bash
g++ --version
pkg-config --modversion opencv5 tesseract
tesseract --list-langs
```

The Tesseract language list should contain `eng`.

### 4. Build and run in VS Code's PowerShell terminal

In VS Code, open **Terminal → New Terminal**. The commands below are for the integrated **PowerShell** terminal, not Command Prompt.

Set `$msys` to the MSYS2 install folder. The example is for a standard install at `C:\msys64`:

```powershell
$msys = 'C:\msys64'
$env:Path = "$msys\ucrt64\bin;$env:Path"
$env:TESSDATA_PREFIX = "$msys\ucrt64\share\tessdata"
```

If using the developer's tested install, set it instead like this:

```powershell
$msys = 'C:\Users\devan\msys64-ucrt'
$env:Path = "$msys\ucrt64\bin;$env:Path"
$env:TESSDATA_PREFIX = "$msys\ucrt64\share\tessdata"
```

These environment variables apply to the current terminal session. Set them again whenever opening a new terminal.

Change directory to the actual repository location. For example:

```powershell
cd 'C:\path\to\smart-parking-management-and-violation-monitoring-system'
```

Build the program. This writes the executable to the current Windows user's temporary folder rather than adding a build artifact to the repository:

```powershell
$flags = (& "$msys\ucrt64\bin\pkg-config.exe" --cflags --libs opencv5 tesseract) -split '\s+'
& "$msys\ucrt64\bin\g++.exe" -Wall -Wextra -pedantic image_processing\license_plate.cpp -o "$env:TEMP\license_plate.exe" @flags
```

If the command completes successfully, run it from that same terminal:

```powershell
& "$env:TEMP\license_plate.exe"
```

Compile again after changing the C++ source.

### 5. Use the program

1. Wait for the webcam window to open.
2. Point the camera at a clear, well-lit license plate.
3. Click the camera window so it has keyboard focus.
4. Press **Space** to capture a frame.
5. Read the messages in the VS Code terminal:
   - `License plate not detected.` means the OpenCV contour/shape check did not find a candidate in that frame.
   - `License plate detected.` means the program found a candidate and will try OCR.
   - `Vehicle Number: ...` is the text returned by Tesseract.
6. Press **Esc** while the camera window is focused to exit.

### Troubleshooting

- **`opencv2/opencv.hpp: No such file or directory`**: install the UCRT64 OpenCV package and compile with the UCRT64 `g++.exe` command above.
- **`Tesseract could not start` / `eng.traineddata` not found**: ensure the English data package is installed and `$env:TESSDATA_PREFIX` points to the `tessdata` directory, as shown above.
- **The camera will not open**: check Windows camera privacy permissions, close other applications using the webcam, and confirm Windows can see the camera.
- **`License plate not detected.`**: try moving closer, improving lighting, reducing glare, and holding the plate straight. The current detector uses a basic contour and aspect-ratio check; it can miss plates and can mistake other rectangular shapes for plates.
- **OCR output is wrong or empty**: OCR quality depends on a clear crop, focus, lighting, and plate layout. A successful Tesseract initialization does not guarantee correct recognition.
- **OpenCV header warnings during compilation**: warnings in OpenCV's own headers may appear; the build is successful if GCC exits successfully and creates `license_plate.exe`.

### Verification status

The source has compiled with MSYS2 UCRT64 GCC, OpenCV, and Tesseract. The webcam has opened, and a separate C++ smoke test initialized Tesseract with the installed English language data. In webcam tests so far, frames have not consistently passed the plate-detection step, so successful OCR of a real plate has not yet been confirmed.

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
