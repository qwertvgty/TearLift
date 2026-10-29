$ErrorActionPreference = 'Stop'
$launcher = Join-Path $PSScriptRoot '..\tools\launch_debug.ps1'
$root = Join-Path ([IO.Path]::GetTempPath()) ('ultrs-launch-test-' + [guid]::NewGuid().ToString('N'))
$names = @('TOTK_AUDIT', 'TOTK_WATCH', 'TOTK_DUMP_KEY_AT')
$saved = @{}
foreach ($n in $names) { $saved[$n] = @{ Present = (Test-Path -LiteralPath ('Env:' + $n)); Value = [Environment]::GetEnvironmentVariable($n) } }
try {
    New-Item -ItemType Directory -Path $root | Out-Null
    # DryRun checks existence only. This empty placeholder must never be executed.
    $fixture = Join-Path $root 'UltraTears.exe'
    New-Item -ItemType File -Path $fixture | Out-Null
    $env:TOTK_AUDIT = 'test-audit'
    [Environment]::SetEnvironmentVariable('TOTK_WATCH', $null, 'Process') # empty on some Windows PowerShell hosts
    $watchPresent = Test-Path Env:TOTK_WATCH
    $watchValue = [Environment]::GetEnvironmentVariable('TOTK_WATCH')
    Remove-Item Env:TOTK_DUMP_KEY_AT -ErrorAction SilentlyContinue
    $cfg = & $launcher -GameDir $root -Threads 4 -DryRun | ConvertFrom-Json
    if ($cfg.DiagnosticsStillSet.Count -ne 0) { throw 'Native presence-based diagnostics would still be enabled' }
    if ($cfg.Threads -ne 4 -or $cfg.Overrides.TOTK_VK_FEEDBACK_COPY -ne '1') { throw 'Wrong launch configuration' }
    if ($env:TOTK_AUDIT -ne 'test-audit') { throw 'Non-empty caller environment not restored' }
    if ((Test-Path Env:TOTK_WATCH) -ne $watchPresent -or [Environment]::GetEnvironmentVariable('TOTK_WATCH') -cne $watchValue) {
        throw 'Empty/absent caller environment not restored'
    }
    if (Test-Path Env:TOTK_DUMP_KEY_AT) { throw 'Absent caller variable was turned into an empty variable' }
    Write-Output 'Clean launcher environment: all checks passed (DryRun only; no process launched)'
} finally {
    foreach ($n in $names) {
        if ($saved[$n].Present) { [Environment]::SetEnvironmentVariable($n, $saved[$n].Value, 'Process') }
        else { Remove-Item -LiteralPath ('Env:' + $n) -ErrorAction SilentlyContinue }
    }
    if (Test-Path -LiteralPath (Join-Path $root 'UltraTears.exe')) { Remove-Item -LiteralPath (Join-Path $root 'UltraTears.exe') }
    if (Test-Path -LiteralPath $root) { Remove-Item -LiteralPath $root } # deliberately not recursive
}
