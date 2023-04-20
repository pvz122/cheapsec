import pty
import os

pid, fd = pty.fork()
if pid == 0:
    # child
    os.execvp("timeout", ["timeout", "1s", "./a.out"])
else:
    # parent
    ret_content = b""
    while True:
        try:
            ret_content += os.read(fd, 1024)
        except:
            break

print(ret_content)
