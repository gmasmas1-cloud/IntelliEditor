Get-ChildItem bin/*.exe, bin/*.dll | ForEach-Object {
    $file = $_.FullName
    $bytes = [System.IO.File]::ReadAllBytes($file)
    $peOffset = [System.BitConverter]::ToUInt32($bytes, 0x3C)
    $machine = [System.BitConverter]::ToUInt16($bytes, $peOffset + 4)
    $arch = if ($machine -eq 0x8664) { "64-bit" } elseif ($machine -eq 0x014c) { "32-bit" } else { "Unknown ($machine)" }
    "$($_.Name): $arch"
}
