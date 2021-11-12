import numpy as np

targets = 'T1 T2 T3 T4 T5 T6-1 T6-2 T6-3 T7-1 T7-2 T8 T8-x T9-1 T9-2'.split()
speed = list(map(int, '246	310	598	586	418	474	396	55	282	604	27	530	261'.split()))
#speed = list(map(int, '374	525	1100	1375	700	821	587	64	417	1323	889	758	498'.split()))
speed = speed[:11] + [speed[10] / 2] + speed[11:]

m = []

for target in targets:
    s = open('results/' + target + '.txt').read().replace(',', '')
    s = list(map(int, s.split()[1::2]))
    s = [s[0] - s[1] - s[2], s[1], s[2], s[3] - s[4], s[4]]
    m.append(s)
    print(target.ljust(4, ' '), s)

a = np.mat(m)
b = np.array(speed)
x, *_ = np.linalg.lstsq(a, b)
print(x)
print(b)
print(np.array(a * x.reshape((5, 1))).reshape(len(speed)))
