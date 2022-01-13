import subprocess
import os

from pathlib import Path

def run_clang_format(path, extension):
    files = []
    for v in path.iterdir():
        f, ext = os.path.splitext(v)
        if (ext == extension):
            files.append(f + ext)

    subprocess.run(['clang-format', '-i'] + files)

run_clang_format(Path('../src/'), '.cpp')
run_clang_format(Path('../test/'), '.cpp')
run_clang_format(Path('../include/'), '.h')
