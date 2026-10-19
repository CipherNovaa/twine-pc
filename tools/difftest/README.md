# Differential tests for NON_MATCHING C (experimental)

Banked near misses (`#ifdef NON_MATCHING`) don't match the original bytes, so these scripts check that they
*behave* the same: `mips.py` interprets a function's original asm (branches, delay slots, `jal` stubs),
and the drivers run it next to the candidate C compiled for the host (`gcc -m32`), with globals mapped at
fixed addresses, then compare memory and the log of calls.

- `mips.py`: the asm interpreter (`Sim`)
- `harness.py`: shared setup (symbol allocation, memory image, comparison)
- `d63.py`, `d59.py`: drivers for func_800463C8 (random inputs, callees stubbed) and func_80059E4C
- `sim.py`, `sim2.py`: straight-line table functions (func_800274F4, func_80066B14)

Written by a helper during the 2026-10-19 overnight session; adapt a driver per function.
