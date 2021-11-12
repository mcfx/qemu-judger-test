# QEMU Judger Test

This project was done 2 months ago, and I can't remember everything very well, so the steps here may be wrong.

## Setup

First, you need to clone [QEMU](https://github.com/qemu/qemu), put `judger.c` into `tests/plugin`, and compile QEMU. You need to update the paths in Makefile.

Then you can run `make all`, then there will be run results in `results/` directory.

Finally, you may run `python test.py` to find some coefficients.

## License of T[1-9] files

Test codes comes from [https://loj.ac/d/2685](https://loj.ac/d/2685), there's no particular license.