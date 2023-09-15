import json
import pyperclip

with open("outputCode.txt", "w") as f1:
    with open("input.txt", "r") as f:
        for line in f.readlines():
            pos = 0
            for i in range(len(line)):
                if not line[i].isdigit():
                    pos = i
                    break
            line = line[pos+1:]
            f1.write(line)

with open("outputCode.txt", "r") as f:
    pyperclip.copy(f.read())
