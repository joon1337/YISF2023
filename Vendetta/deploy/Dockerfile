FROM ubuntu:20.04
ENV DEBIAN_FRONTEND=noninteractive\
    LC_CTYPE=C.UTF-8

# 포트 설정
EXPOSE 5000

# 레포지토리 변경
RUN sed -i "s/archive.ubuntu/mirror.kakao/g" /etc/apt/sources.list

# 업데이트
RUN apt update && apt upgrade -y
RUN apt install -y xinetd ssh
RUN apt install -y --no-install-recommends tzdata

# 타임존 설정
RUN echo "Asia/Seoul" > /etc/timezone
RUN rm -f /etc/localtime
RUN dpkg-reconfigure --frontend noninteractive tzdata

# 서비스 설정
RUN touch /var/log/prob.log
RUN touch /etc/xinetd.d/prob
RUN echo "service prob                         " >> /etc/xinetd.d/prob
RUN echo "{                                       " >> /etc/xinetd.d/prob
RUN echo "    flags = REUSE                       " >> /etc/xinetd.d/prob
RUN echo "    disable = no                        " >> /etc/xinetd.d/prob
RUN echo "    socket_type = stream                " >> /etc/xinetd.d/prob
RUN echo "    wait = no                           " >> /etc/xinetd.d/prob
RUN echo "    user = user                         " >> /etc/xinetd.d/prob
RUN echo "    server = /home/user/prob            " >> /etc/xinetd.d/prob
RUN echo "    port = 5000                         " >> /etc/xinetd.d/prob
RUN echo "    protocol = tcp                      " >> /etc/xinetd.d/prob
RUN echo "    log_type = FILE /var/log/prob.log" >> /etc/xinetd.d/prob
RUN echo "    log_on_success = HOST DURATION EXIT " >> /etc/xinetd.d/prob
RUN echo "}                                       " >> /etc/xinetd.d/prob
RUN echo "prob 5000/tcp" >> /etc/services


# 계정 설정
RUN useradd user -m -s /bin/bash
RUN echo "user:gpffhemflagor!" | chpasswd
RUN echo "root:gpffhemflagor!root" | chpasswd

# ASLR 설정
RUN touch /etc/sysctl.d/01-disable-aslr.conf
RUN echo "kernel.randomize_va_space = 2" >> /etc/sysctl.d/01-disable-aslr.conf

# 기본 권한 설정
RUN chmod -R 700 /bin
RUN chmod -R 700 /sbin
RUN chmod -R 700 /usr/bin
RUN chmod -R 700 /usr/sbin
RUN chmod 755 /bin
RUN chmod 755 /sbin
RUN chmod 755 /usr/bin
RUN chmod 755 /usr/sbin
RUN chmod 755 /bin/ls
RUN chmod 755 /bin/cat
RUN chmod 755 /bin/sh
RUN chmod 755 /bin/dash
RUN chmod 755 /bin/bash
RUN chmod 600 /etc/passwd
RUN chmod 600 /etc/passwd-
RUN chmod 600 /etc/group
RUN chmod 600 /etc/group-
RUN chmod 600 /etc/shadow
RUN chmod 600 /etc/shadow-
RUN chmod -R 700 /var/log
RUN chmod -R 700 /var/tmp
RUN chmod 700 /var
RUN chmod 700 /tmp
RUN chmod 600 /dev/shm

# 바이너리 복사
COPY flag /home/user/flag
COPY chall /home/user/prob
RUN ln -s /home/user/flag /flag

# 홈 디렉토리 설정
RUN rm -rf /home/user/.bash_history
RUN rm -rf /home/user/.bash_logout
RUN rm -rf /home/user/.bashrc
RUN rm -rf /home/user/.profile
RUN rm -rf /home/user/.cache
RUN ln -s /dev/null /home/user/.bash_history
RUN ln -s /dev/null /home/user/.bash_logout
RUN ln -s /dev/null /home/user/.bashrc
RUN ln -s /dev/null /home/user/.profile
RUN ln -s /dev/null /home/user/.cache
RUN chown user.user /home/user/.bash_history
RUN chown user.user /home/user/.bash_logout
RUN chown user.user /home/user/.bashrc
RUN chown user.user /home/user/.profile
RUN chown user.user /home/user/.cache
RUN chown user.user /home/user
RUN chown user.user /home/user/flag
RUN chown user.user /home/user/prob

# --------------------------------- #

RUN chmod 555 /usr/bin/clear
RUN chmod 500 /home/user
RUN chmod 400 /home/user/flag
RUN chmod 500 /home/user/prob

CMD ["/usr/sbin/xinetd","-dontfork"]
