# TWINE's test kit for a Windows PC (port/usbkit; TEST.bat runs this). No install: Windows PowerShell 5.1, which
# every Windows 10 and 11 has. From any drive letter, a FAT32 or exFAT stick included.
#   1. a system report (Windows, CPU, memory, GPUs and drivers, displays, controllers, OpenGL, the hardware renderer's
#      Vulkan device) into results\<time>\system\
#   2. the headless runs of kit\runs.txt, as tools/endless_check.py makes them (its own folders, headless, dummy
#      sound, no cards), each into results\<time>\runs\<name>\ (port.log, out.txt, screenshots, a crash dump)
#   3. the game, to play (not with "noplay"); its log into results\<time>\play\
# results\<time>\summary.txt: a PASS / FAIL line for each step. tools/usbkit_compare.py (Linux) compares the folder
# with the same runs on Linux.
param([string]$Mode = "")
$ErrorActionPreference = "Continue"
$kit = Split-Path -Parent $MyInvocation.MyCommand.Path
$root = Split-Path -Parent $kit
$exe = Join-Path $root "twine.exe"
$stamp = Get-Date -Format "yyyyMMdd-HHmmss"
$res = Join-Path (Join-Path $root "results") $stamp
New-Item -ItemType Directory -Force -Path $res | Out-Null
$summary = Join-Path $res "summary.txt"

function Say($line) {
    Write-Host $line
    Add-Content -Path $summary -Value $line -Encoding ASCII
}

Say "TWINE test kit, $stamp, in $root"

# ---- 1. the system ----
$sys = Join-Path $res "system"
New-Item -ItemType Directory -Force -Path $sys | Out-Null
try {
    $r = @()
    $os = Get-CimInstance Win32_OperatingSystem
    $r += "Windows: $($os.Caption) $($os.Version) build $($os.BuildNumber) $($os.OSArchitecture)"
    foreach ($c in Get-CimInstance Win32_Processor) { $r += "CPU: $($c.Name) ($($c.NumberOfLogicalProcessors) threads)" }
    $r += "RAM: {0:N1} GB" -f ($os.TotalVisibleMemorySize / 1MB)
    foreach ($g in Get-CimInstance Win32_VideoController) {
        $r += "GPU: $($g.Name), driver $($g.DriverVersion) ($($g.DriverDate)), $($g.CurrentHorizontalResolution)x$($g.CurrentVerticalResolution) at $($g.CurrentRefreshRate) Hz"
    }
    foreach ($m in Get-CimInstance -Namespace root\wmi -ClassName WmiMonitorID -ErrorAction SilentlyContinue) {
        $name = ($m.UserFriendlyName | Where-Object { $_ -ne 0 } | ForEach-Object { [char]$_ }) -join ""
        $r += "Monitor: $name"
    }
    foreach ($p in Get-PnpDevice -PresentOnly -ErrorAction SilentlyContinue |
             Where-Object { $_.Class -in @("XnaComposite", "HIDClass") -and $_.FriendlyName -match "game|controller|xbox|dualshock|dualsense|wireless" }) {
        $r += "Controller: $($p.FriendlyName)"
    }
    $r | Out-File -FilePath (Join-Path $sys "system.txt") -Encoding ASCII
    $p = Start-Process -FilePath (Join-Path $kit "winsysinfo.exe") -WorkingDirectory $root -NoNewWindow -PassThru `
         -RedirectStandardOutput (Join-Path $sys "sysinfo.txt") -RedirectStandardError (Join-Path $sys "sysinfo-err.txt")
    if (-not $p.WaitForExit(60000)) { $p.Kill() }
    $d = Start-Process -FilePath "dxdiag.exe" -ArgumentList "/t", "`"$(Join-Path $sys 'dxdiag.txt')`"" -PassThru
    if (-not $d.WaitForExit(120000)) { $d.Kill() }
    $r += Get-Content (Join-Path $sys "sysinfo.txt")
    $r | ForEach-Object { Write-Host "  $_" }
    Say "PASS system report (results\$stamp\system)"
} catch {
    Say "FAIL system report: $_"
}

# ---- 2. the runs ----
$disc = Get-ChildItem -Path (Join-Path $root "disc") -Filter *.bin -ErrorAction SilentlyContinue |
        Sort-Object Length -Descending | Select-Object -First 1
if (-not $disc) {
    Say "FAIL runs: no disc image (put your own .bin and .cue into disc\ next to twine.exe)"
} else {
    $runs = Get-Content (Join-Path $kit "runs.txt") | Where-Object { $_ -and -not $_.StartsWith("#") }
    foreach ($line in $runs) {
        $f = $line.Split("|", 4)
        $name, $secs, $must = $f[0], [double]$f[1], $f[2]
        $vars = @([regex]::Matches($f[3], '"([^"=]+)=([^"]*)"') | ForEach-Object { , @($_.Groups[1].Value, $_.Groups[2].Value) })
        $dir = Join-Path (Join-Path $res "runs") $name
        foreach ($s in "conf\twine", "data\twine", "state") { New-Item -ItemType Directory -Force -Path (Join-Path $dir $s) | Out-Null }
        $saved = @{}
        foreach ($k in [Environment]::GetEnvironmentVariables().Keys) {
            if ($k -like "TWINE_*" -or $k -like "XDG_*" -or $k -like "SDL_AUDIO*") { $saved[$k] = [Environment]::GetEnvironmentVariable($k); [Environment]::SetEnvironmentVariable($k, $null) }
        }
        $vals = @{ XDG_CONFIG_HOME = (Join-Path $dir "conf"); XDG_DATA_HOME = (Join-Path $dir "data"); XDG_STATE_HOME = (Join-Path $dir "state")
                  TWINE_HEADLESS = "1"; TWINE_MOUSE = "0"; TWINE_NO_INTRO = "1"; TWINE_CARD1 = "none"; TWINE_CARD2 = "none"
                  SDL_AUDIO_DRIVER = "dummy"; SDL_AUDIODRIVER = "dummy"; TWINE_DISC = $disc.FullName; TWINE_TURBO = "1"
                  TWINE_TEST_VBLANKS = [string][math]::Round($secs * 59.94); TWINE_CRASH_DUMPS = $dir }
        foreach ($v in $vars) { $vals[$v[0]] = $v[1] }
        foreach ($k in $vals.Keys) { [Environment]::SetEnvironmentVariable($k, $vals[$k]) }
        $t0 = Get-Date
        $p = Start-Process -FilePath $exe -WorkingDirectory $dir -NoNewWindow -PassThru `
             -RedirectStandardOutput (Join-Path $dir "out.txt") -RedirectStandardError (Join-Path $dir "err.txt")
        $stuck = -not $p.WaitForExit([int](($secs * 2 + 60) * 1000))
        if ($stuck) { $p.Kill(); $p.WaitForExit() }
        $wall = ((Get-Date) - $t0).TotalSeconds
        foreach ($k in $vals.Keys) { [Environment]::SetEnvironmentVariable($k, $null) }
        foreach ($k in $saved.Keys) { [Environment]::SetEnvironmentVariable($k, $saved[$k]) }
        $log = ""
        if (Test-Path (Join-Path $dir "port.log")) { $log = Get-Content -Raw (Join-Path $dir "port.log") }
        $over = $log -match "test: the run's \d+ vblanks are over"
        $crash = $log -match "crash:|fatal:|watchdog: no frame"
        $has = $log -match $must
        $note = ""
        if ($log -match "gpu_hw: (no Vulkan GPU device[^\r\n]*)") { $note = " (hardware renderer: $($Matches[1]))" }
        $ok = $over -and $has -and -not $crash -and -not $stuck
        $state = if ($ok) { "PASS" } else { "FAIL" }
        Say ("{0} run {1}: {2:N1} s, ended {3}, crash {4}, '{5}' {6}{7}" -f $state, $name, $wall, $over, $crash, $must, $(if ($has) { "seen" } else { "not seen" }), $note)
    }
}

# ---- 3. play ----
if ($Mode -ne "noplay") {
    Write-Host ""
    Write-Host "Now the game starts as it would for you. Play as long as you like; quit it to finish the test."
    $play = Join-Path $res "play"
    New-Item -ItemType Directory -Force -Path $play | Out-Null
    [Environment]::SetEnvironmentVariable("TWINE_CRASH_DUMPS", $play)
    $t0 = Get-Date
    $p = Start-Process -FilePath $exe -WorkingDirectory $root -PassThru
    $p.WaitForExit()
    $mins = ((Get-Date) - $t0).TotalMinutes
    # (portable: the game worked in the twine folder next to twine.exe)
    Copy-Item -Path (Join-Path $root "twine\port.log") -Destination $play -ErrorAction SilentlyContinue
    Copy-Item -Path (Join-Path $root "twine\twine.ini") -Destination $play -ErrorAction SilentlyContinue
    $log = ""
    if (Test-Path (Join-Path $play "port.log")) { $log = Get-Content -Raw (Join-Path $play "port.log") }
    $crash = $log -match "crash:|fatal:"
    $state = if ($crash -or $p.ExitCode -ne 0) { "FAIL" } else { "PASS" }
    Say ("{0} play: {1:N1} min, exit code {2}, crash {3}" -f $state, $mins, $p.ExitCode, $crash)
}
Say "results: $res"
