# Launch without inherited audit/script settings. Does not affect an already running game or persistent settings.
[CmdletBinding()]
param(
    [string]$GameDir = (Join-Path $PSScriptRoot '..\dist'),
    [string]$ExecutableName = 'UltraTears.exe',
    [ValidateSet(1, 2, 4, 8)][int]$Threads = 1,
    [string]$SaveDir,
    [switch]$DryRun
)
$ErrorActionPreference = 'Stop'
$root = (Resolve-Path -LiteralPath $GameDir).Path
$exe = Join-Path $root $ExecutableName
if (-not (Test-Path -LiteralPath $exe -PathType Leaf)) { throw "Game executable missing: $exe" }
$clear = @(
    'TOTK_AUDIT', 'TOTK_INPUT_SCRIPT', 'TOTK_HEADLESS', 'TOTK_SHADER_COMPILE_TRACE', 'TOTK_VK_VALIDATION',
    'TOTK_VK_DRAWTRACE', 'TOTK_VK_TRACE', 'TOTK_SNAP', 'TOTK_GPU_PROF', 'TOTK_GPU_PASSPROF',
    'TOTK_VK_CHECKPOINTS', 'TOTK_VK_SPLIT', 'TOTK_VK_STORES_LOG', 'TOTK_WATCH',
    'TOTK_SHOT', 'TOTK_DUMP', 'TOTK_DUMP_EXIT', 'TOTK_DUMP_KEY', 'TOTK_DUMP_KEY_AT'
)
$set = @{
    TOTK_VK_FEEDBACK_COPY = '1'
    TOTK_VK_RECORD_THREADS = [string]$Threads
    TOTK_SHADER_AHEAD = '0' # profile warmed on-demand binaries without background compilation competing for CPU
}
if ($SaveDir) { $set.TOTK_PROFILE_DIR = (Resolve-Path -LiteralPath $SaveDir).Path }
$saved = @{}
foreach ($name in ($clear + @($set.Keys))) { $saved[$name] = [Environment]::GetEnvironmentVariable($name, 'Process') }
try {
    foreach ($name in $clear) { [Environment]::SetEnvironmentVariable($name, $null, 'Process') }
    foreach ($name in $set.Keys) { [Environment]::SetEnvironmentVariable($name, $set[$name], 'Process') }
    if ($DryRun) {
        $remaining = @($clear | Where-Object { [Environment]::GetEnvironmentVariable($_, 'Process') })
        [pscustomobject]@{ Executable = $exe; Arguments = '--debug'; Threads = $Threads; Overrides = $set; Cleared = $clear; DiagnosticsStillSet = $remaining } |
            ConvertTo-Json -Depth 4
    } else {
        $running = @(Get-Process -Name UltraTears, UltraTears-kbm -ErrorAction SilentlyContinue |
            Where-Object { $_.Path -and (Split-Path -Parent $_.Path) -eq $root })
        if ($running.Count) { throw 'Exit the existing game first; this launcher never closes it or starts a competing instance.' }
        if ($Threads -gt 1) { Write-Warning 'Multi-threaded feedback copying is experimental; compare picture and stability before judging performance.' }
        # The user's interactive game window, not a hidden background helper.
        Start-Process -FilePath $exe -WorkingDirectory $root -ArgumentList '--debug' -WindowStyle Normal -PassThru |
            Select-Object Id, ProcessName
    }
} finally {
    foreach ($name in $saved.Keys) { [Environment]::SetEnvironmentVariable($name, $saved[$name], 'Process') }
}
