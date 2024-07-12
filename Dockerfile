FROM ubuntu:24.04
RUN apt update && apt install -y libncursesw5-dev && \
    apt install -y  ncurses-doc && apt -y install openssh-server && \
    apt install -y nano
RUN apt install -y gcc && apt install -y make
RUN echo "PermitRootLogin yes" >> /etc/ssh/sshd_config