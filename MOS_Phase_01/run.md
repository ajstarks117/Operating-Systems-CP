# How to Run MOS Phase-1

## Compile
```bash
g++ mos.cpp -o mos.exe
```

## Run
```bash
./mos.exe
```

## Files
| File | Purpose |
|------|---------|
| `input.txt` | Job file (must exist before running) |
| `output.txt` | Generated output (created automatically) |

## Input Format
```
$AMJ<jobid><ttl><tll>
<packed 4-char instructions on one line>
$DTA
<40-char data card>
$END<jobid>
```

## Example
```
$AMJ000100030001
GD10PD10H
$DTA
HELLOWORLD123456789012345678901234567890
$END0001
```
