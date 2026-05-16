$file = "bin/IntelliEditor.exe"
if (Test-Path $file) {
    $bytes = [System.IO.File]::ReadAllBytes($file)
    $peOffset = [System.BitConverter]::ToUInt32($bytes, 0x3C)
    $machine = [System.BitConverter]::ToUInt16($bytes, $peOffset + 4)
    if ($machine -eq 0x8664) { "IntelliEditor.exe is 64-bit" }
    elseif ($machine -eq 0x014c) { "IntelliEditor.exe is 32-bit" }
    else { "IntelliEditor.exe is Unknown ($machine)" }
} else {
    "File not found"
}
