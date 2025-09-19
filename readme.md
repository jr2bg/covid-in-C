Used `C` as de programming language

As we want to pass it to CUDA, the number of ifs will be minimized

# usage

## main
### Powershell
```
gcc -Wall config_reader.c lib.c single_evolution.c simulator.c -o sim.exe; .\sim.exe <seiqsf|seisf>
```

## tests
```
gcc -Wall lib.c config_reader.c tests.c -o test
```