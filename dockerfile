FROM archlinux

RUN pacman -Sy --noconfirm vim make gcc neofetch valgrind glibc strace lldb gdb

ENV DEBUGINFOD_URLS="https://debuginfod.archlinux.org"

COPY . /checking_files

WORKDIR /checking_files
