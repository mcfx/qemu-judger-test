CXX = riscv64-linux-gnu-g++
CXXFLAGS = -O2
QEMU = /mnt/e/qemu/build/qemu-riscv64
QEMUFLAGS = -E LD_LIBRARY_PATH=/usr/riscv64-linux-gnu/lib/ -d plugin -plugin /mnt/e/qemu/build/tests/plugin/libjudger.so

targets = T1 T2 T3 T4 T5 T6-1 T6-2 T6-3 T7-1 T7-2 T8 T8-x T9-1 T9-2

$(targets):
	$(CXX) $(CXXFLAGS) $@.cpp -o $@
	$(QEMU) $(QEMUFLAGS) $@ 2> results/$@.txt
	rm $@

all: $(targets)

clean:
	rm $(targets)
