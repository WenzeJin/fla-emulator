import subprocess, os, itertools
import time

COLORS = {"default": "\033[0m", "red": "\033[31m", "green": "\033[32m"}

def color(name, text):
    if os.isatty(1):
        return COLORS[name] + text + COLORS["default"]
    return text

total = 0
ac = 0
total_time = 0

with open('test/testcases.txt') as test, open('test/testcases_ans.txt') as ans:
    print('=================== Correctness Test ====================')
    print('%4s %34s %6s' % ('Case','Target','Result'))
    for t, a, i in zip(test, ans, itertools.count(1)):
        print('%4d %33s' % (i, t.split()[1]), end=': ')
        total += 1
        try:
            start = time.perf_counter()
            tm = subprocess.run(('bash', '-c', t), stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, encoding="utf-8", timeout=1)
            end = time.perf_counter()
            if tm.returncode != 0:
                print(color("red", "Runtime Error"), "stderr:", tm.stderr.strip())
            elif tm.stdout.strip() == a.strip():
                print(color("green", "Passed"))
                ac += 1
                total_time += end - start
            else:
                print(color("red", "Failed"), "ans:", a.strip(), "stdout:", tm.stdout.strip())
        except TimeoutError:
            print(color("red", "TLE"))
        except:
            print(color("red", "Runtime Error"))

with open('test/testcases_robustness.txt') as test, open('test/robustness_ans.txt') as ans:
    print('==================== Robustness Test ====================')
    print('%4s %34s %6s' % ('Case','Target','Result'))
    for t, a, i in zip(test, ans, itertools.count(1)):
        print('%4d %33s' % (i, t.split()[1]), end=': ')
        total += 1
        try:
            tm = subprocess.run(('bash', '-c', t), stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True, encoding="utf-8", timeout=1)
            if tm.returncode == 0:
                print(color("red", "Failed"), "Expected \"" + a.strip() + "\" in stderr but got nothing")
            elif tm.stderr.strip() == a.strip():
                print(color("green", "Passed"))
                ac += 1
            else:
                print(color("red", "Failed"), "Expected \"" + a.strip() + "\" in stderr but got " + tm.stderr.strip())
        except TimeoutError:
            print(color("red", "TLE"))
        except:
            print(color("red", "Runtime Error"))

print(f'Passed {ac}/{total}')
print(f'Total Time for Passed Cases: {total_time * 1000:.1f} ms')
