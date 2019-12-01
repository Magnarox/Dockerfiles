FROM openjdk:8u232-jdk

COPY ./spark-2.4.4.tgz /tmp/
RUN mkdir /spark/ && mkdir /spark/out/
RUN chmod 755 /tmp/spark-2.4.4.tgz && tar -xzf /tmp/spark-2.4.4.tgz -C /spark/ && mkdir /spark/spark-2.4.4/dist/

WORKDIR /spark/

COPY ./build-distrib.sh /spark/
RUN chmod 755 /spark/build-distrib.sh

CMD ["/bin/bash"]
