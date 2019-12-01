FROM openjdk:8u232-jdk

COPY ./spark-2.4.4.tgz /tmp/
RUN mkdir /spark/ 
RUN chmod 755 /tmp/spark-2.4.4.tgz
RUN tar -xzf /tmp/spark-2.4.4.tgz -C /spark/

CMD ["/bin/bash"]
