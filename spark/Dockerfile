FROM openjdk:8u232-jdk

RUN apt update

# Install R
RUN apt install r-base -y


# Install Python 3.7.1
RUN apt install build-essential checkinstall libreadline-gplv2-dev libncursesw5-dev libssl-dev libsqlite3-dev tk-dev libgdbm-dev libc6-dev libbz2-dev libffi-dev zlib1g-dev -y
COPY ./Python-3.7.1.tgz /tmp/
RUN chmod 755 /tmp/Python-3.7.1.tgz && tar -xzf /tmp/Python-3.7.1.tgz -C /tmp/
RUN cd /tmp/Python-3.7.1 && ./configure --enable-optimizations && make altinstall
RUN update-alternatives --install /usr/bin/pip pip /usr/local/bin/pip3.7 1
RUN update-alternatives --install /usr/bin/python python /usr/local/bin/python3.7 1
RUN update-alternatives --install /usr/bin/pydoc pydoc /usr/local/bin/pydoc3.7 1


# Add Spark 2.4.4 src
COPY ./spark-2.4.4.tgz /tmp/
RUN mkdir /spark/ && mkdir /spark/out/
RUN chmod 755 /tmp/spark-2.4.4.tgz && tar -xzf /tmp/spark-2.4.4.tgz -C /spark/ && mkdir /spark/spark-2.4.4/dist/

WORKDIR /spark/

COPY ./build-distrib.sh /spark/
RUN chmod 755 /spark/build-distrib.sh

CMD ["/bin/bash"]
