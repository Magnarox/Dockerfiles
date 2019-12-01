# Spark

## Objective

Build Spark 2.4.4 distrib with hadoop 3.1.2 client and Python 3.7.1 bindings.

## Create your docker images to build distrib

Run `docker-builder.sh`

Launch docker container and build distrib : 

    docker run -it spark-builder -v ./out/:/spark/spark-2.4.4/dist/ /bin/bash
    > ./build-distrib.sh


