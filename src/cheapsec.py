#! /usr/bin/python3

import sys
import os
import subprocess
import pty
import json

# ----------- global vars -----------


help_str = """
Usage:  cheapsec        <executable path | glibc path>
        cheapsec -t[es] <executable path | glibc path>
        cheapsec -l     <glibc version>
        cheapsec -h

Options:
    [no option]:    Lookup exploitabilities and security checks for glibc library
    -l:             Lookup exploitabilities and security checks for the specified glibc version
    -t:             Run tests for exploitabilities and security checks
    -te:            Run tests for exploitabilities
    -ts:            Run tests for security checks
    -h:             Print help message

Arguments:
    <executable path>:      Path to executable, cheapsec will test the glibc library linked with executable
    <glibc path>:           Path to glibc library, e.g. /lib/x86_64-linux-gnu/
    <glibc version>:        Version of glibc library, e.g. 2.27
"""
file_wd = os.path.dirname(os.path.realpath(__file__))
stats = {
    "exp": {},
    "check": {}
}


# ----------- utility functions -----------


# compile using specified glibc
def compile_with_glibc(src_path: str, glibc_path: str, flags: str = "") -> bool:
    ret_code = subprocess.run("gcc \"{}\" -Wl,--rpath={} -Wl,--dynamic-linker={}/ld-linux-x86-64.so.2 -o cheapsec_test {}".format(
        src_path, glibc_path, glibc_path, flags), shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode
    return ret_code == 0


# get glibc path from executable path
def linked_glibc(exec_path: str) -> str:
    # be like: 	libc.so.6 => /opt/libcbench/glibc/2.15/lib/libc.so.6 (0x00007f46683cb000)
    ret_content = subprocess.check_output(["ldd", exec_path])
    ret_content = ret_content.decode("utf-8")
    ret_content = ret_content.split("=>")[1].split("(")[0].strip()

    # remove the last /libc.so.6
    ret_content = ret_content[:ret_content.rfind("/")]

    return ret_content


# get glibc path from arguments
def get_glibc_path(arg_path: str) -> str:
    if os.path.exists(arg_path):
        if os.path.isdir(arg_path):
            glibc_path = arg_path
        else:
            glibc_path = linked_glibc(arg_path)
    else:
        print("Error: invalid path")
        glibc_path = ""
    return glibc_path


# get glibc version from glibc path
def version_of_glibc(glibc_path: str) -> str:
    if not compile_with_glibc(os.path.join(file_wd, "../testing/get_glibc_version.c"), glibc_path):
        print("Error: failed to compile get_glibc_version.c")
        return ""

    ret_content = subprocess.check_output(["./cheapsec_test"])
    ret_content = ret_content.decode("utf-8")

    try:
        os.remove("cheapsec_test")
    except:
        pass

    return ret_content


# get exploitabilities and checks from database
def lookup_exp_check(glibc_version: str) -> bool:
    try:
        with open(os.path.join(file_wd, "../data/stats.json"), "r") as f:
            all_stats = json.load(f)
            stats["check"] = all_stats[glibc_version]["check"]
            stats["exp"] = all_stats[glibc_version]["exp"]
    except:
        return False

    return True


# run exploitability tests
def run_exp_tests(glibc_path: str) -> bool:
    # list file in testing/exps
    exp_dirs = os.listdir(os.path.join(file_wd, "../testing/exps"))
    exp_dirs = [exp for exp in exp_dirs if os.path.isdir(
        os.path.join(file_wd, "../testing/exps", exp))]
    exp_dirs.sort()

    # get glibc version
    glibc_version = version_of_glibc(glibc_path)

    print("Compiling and running tests for exploitabilities...")
    for exp in exp_dirs:
        for exp_file in os.listdir(os.path.join(file_wd, "../testing/exps", exp)):
            stats["exp"][exp] = "U"
            if not exp_file.endswith(".c"):
                continue

            exp_file = os.path.join(file_wd, "../testing/exps", exp, exp_file)

            # compile
            if not compile_with_glibc(exp_file, glibc_path):
                print("Error: failed to compile {}".format(exp_file))
                stats["exp"][exp] = "E"
                continue

            if glibc_version == "" or (int)(glibc_version.split(".")[1]) <= 26:
                # run in a pseudo-terminal
                # because glibc <= 2.26 use '__libc_message' which directly output to /dev/tty
                try:
                    pid, fd = pty.fork()
                    if pid == 0:
                        os.execvp(
                            "timeout", ["timeout", "1s", "./cheapsec_test"])
                    else:
                        ret_code = os.waitpid(pid, 0)[1]

                    if ret_code == 0:
                        stats["exp"][exp] = "Y"
                        break
                    else:
                        stats["exp"][exp] = "N"
                except:
                    stats["exp"][exp] = "E"

            else:
                # run in a subprocess
                ret_code = subprocess.run(["timeout", "1s", "./cheapsec_test"],
                                          stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL).returncode
                if ret_code == 0:
                    stats["exp"][exp] = "Y"
                    break
                else:
                    stats["exp"][exp] = "N"

        try:
            os.remove("cheapsec_test")
        except:
            pass

    return True


# run security check tests
def run_check_tests(glibc_path: str) -> bool:
    # list file in testing/checks
    check_files = os.listdir(os.path.join(file_wd, "../testing/checks"))
    check_files = [check for check in check_files if check.endswith(".c")]
    check_files.sort()

    # get glibc version
    glibc_version = version_of_glibc(glibc_path)

    print("Compiling and running tests for security checks...")
    for check in check_files:
        check_file = os.path.join(file_wd, "../testing/checks", check)
        check = check[:-2]

        # compile
        # check whether contains '#include <pthread.h>'
        if "#include <pthread.h>" in open(check_file).read():
            compile_flags = "-lpthread"
        else:
            compile_flags = ""

        if not compile_with_glibc(check_file, glibc_path, compile_flags):
            print("Error: failed to compile {}".format(check_file))
            stats["check"][check] = "E"
            continue

        if glibc_version == "" or (int)(glibc_version.split(".")[1]) <= 26:
            # run in a pseudo terminal
            # because glibc <= 2.26 use '__libc_message' which directly output to /dev/tty
            try:
                pid, fd = pty.fork()
            except:
                stats["check"][check] = "E"
                continue

            if pid == 0:
                # child
                os.execvp("timeout", ["timeout", "1s", "./cheapsec_test"])
            else:
                # parent
                ret_content = b""
                while True:
                    try:
                        ret_content += os.read(fd, 1024)
                    except:
                        break

            ret_content = ret_content.decode("utf-8")
        else:
            # directly run
            try:
                ret_content = subprocess.check_output(
                    ["timeout", "1s", "./cheapsec_test"], stderr=subprocess.STDOUT)
            except subprocess.CalledProcessError as e:
                ret_content = e.output
            ret_content = ret_content.decode("utf-8")

        if check in ret_content:
            stats["check"][check] = "Y"
        else:
            stats["check"][check] = "N"

        # remove executable
        try:
            os.remove("cheapsec_test")
        except:
            pass

    return True


# statistic of exploitabilities and security checks, print them
def statistics():
    if len(stats["exp"]) == 0 and len(stats["check"]) == 0:
        print("No results to show")
        return

    if len(stats["exp"]) != 0:
        print("Exploitabilities: (Y - yes, N - no, E - error, U - unknown)")

        for exp in stats["exp"]:
            if stats["exp"][exp] == "Y":
                print("\033[1;32mY\t{}\033[0m".format(exp))
            elif stats["exp"][exp] == "N":
                print("\033[1;31mN\t{}\033[0m".format(exp))
            elif stats["exp"][exp] == "E":
                print("\033[1;33mE\t{}\033[0m".format(exp))
            else:
                print("\033[1;33mU\t{}\033[0m".format(exp))

        print()

    if len(stats["check"]) != 0:
        print("Security checks: (Y - yes, N - no, E - error, U - unknown)")

        for check in stats["check"]:
            if stats["check"][check] == "Y":
                print("\033[1;32mY\t{}\033[0m".format(check))
            elif stats["check"][check] == "N":
                print("\033[1;31mN\t{}\033[0m".format(check))
            elif stats["check"][check] == "E":
                print("\033[1;33mE\t{}\033[0m".format(check))
            else:
                print("\033[1;33mU\t{}\033[0m".format(check))

        print()


# ----------- main functions -----------


def main():
    arg_path = ""  # path to executable or glibc
    glibc_path = ""  # path to glibc

    exp_flag = False  # run exploitability tests
    check_flag = False  # run security check tests
    lookup_flag = True  # lookup exploitabilities and security checks
    lookup_version = ""  # glibc version to lookup

    # deal with arguments
    if len(sys.argv) == 1:
        # no arguments
        print(help_str)
        return
    if sys.argv[1] == "-h":
        # help
        print(help_str)
        return
    elif sys.argv[1].startswith("-t"):
        # run tests
        if len(sys.argv) < 3:
            print(
                "Error: missing executable path or glibc path, run 'cheapsec -h' for more information")
            exit(1)

        lookup_flag = False
        if sys.argv[1] == "-t" or sys.argv[1] == "-tes" or sys.argv[1] == "-tse":
            exp_flag, check_flag = True, True
        elif sys.argv[1] == "-te":
            exp_flag, check_flag = True, False
        elif sys.argv[1] == "-ts":
            exp_flag, check_flag = False, True
        else:
            print("Error: invalid option, run 'cheapsec -h' for more information")
            exit(1)

        arg_path = sys.argv[2]
    elif sys.argv[1] == "-l":
        # lookup specific version
        if len(sys.argv) < 3:
            print(
                "Error: missing glibc version, run 'cheapsec -h' for more information")
            exit(1)

        lookup_version = sys.argv[2]
    elif sys.argv[1].startswith("-"):
        # invalid option
        print("Error: invalid option, run 'cheapsec -h' for more information")
        exit(1)
    else:
        arg_path = sys.argv[1]

    # get glibc path
    if arg_path != "":
        glibc_path = get_glibc_path(arg_path)
        if glibc_path == "":
            exit(1)

    if lookup_flag:
        # lookup exploitabilities and security checks

        # get glibc version
        if lookup_version == "":
            lookup_version = version_of_glibc(glibc_path)
            if lookup_version == "":
                exit(1)
            else:
                print("Target glibc version seems to be {}".format(lookup_version))

        if not lookup_exp_check(lookup_version):
            exit(1)

    if exp_flag:
        # run exploitability tests
        run_exp_tests(glibc_path)

    if check_flag:
        # run security check tests
        run_check_tests(glibc_path)

    statistics()
    return


if __name__ == "__main__":
    main()
