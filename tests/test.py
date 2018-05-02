#!/usr/bin/python3

import shlex
import subprocess

def add_test_state(tests_list, exe, inp, output, time, memory, state):
    sec = 1000
    mb = 1024
    prefix = '../build/tests/'
    cmd = '../build/handler_check --exe {} --input {} --output {} --time_limit {} --memory_limit {} --state {}'.format(prefix + exe, inp, output, time * sec, memory * mb, state)
    tests_list.append(cmd)

def add_test(tests_list, exe, inp, output, time, memory):
    sec = 1000
    mb = 1024
    prefix = '../build/tests/'
    cmd = '../build/handler --exe {} --input {} --output {} --time_limit {} --memory_limit {}'.format(prefix + exe, inp, output, time * sec, memory * mb)
    tests_list.append(cmd)


def main():
    tests = []
    add_test_state(tests, 'FPE', './return_state/in', '/tmp/output', 3, 32, 5)
    add_test_state(tests, 'FSZ', './return_state/in', '/tmp/output', 3, 32, 9)
    add_test_state(tests, 'ILL', './return_state/in', '/tmp/output', 3, 32, 3)
    add_test_state(tests, 'OK', './return_state/in', '/tmp/output', 3, 32, 0)
    add_test_state(tests, 'SEG', './return_state/in', '/tmp/output', 3, 32, 4)
    add_test_state(tests, 'TLE', './return_state/in', '/tmp/output', 3, 32, 1)
    add_test(tests, 'MLE', './return_state/in', '/tmp/output', 35, 32)
    add_test(tests, 'tle/1', './return_state/in', '/tmp/output', 6, 32)
    add_test(tests, 'tle/2', './return_state/in', '/tmp/output', 6, 32)
    add_test_state(tests, 'ill/1', './return_state/in', '/tmp/output', 3, 32, 3)
    add_test_state(tests, 'ill/2', './return_state/in', '/tmp/output', 3, 32, 3)
    add_test_state(tests, 'ill/3', './return_state/in', '/tmp/output', 3, 32, 3)
    

    counter = 0
    ok = 0

    for test in tests:
        counter += 1
        print('Test #{}: '.format(counter), end='')
        process = subprocess.Popen(shlex.split(test))
        process.communicate()
        exit_code = process.wait()
        if exit_code == 0:
            print('PASS')
            ok += 1
        else:
            print('FAILED')

    print('PASSED {} out of {}'.format(ok, counter))

if __name__ == '__main__':
    main()
