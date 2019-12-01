# Spark

## Objective

Build Spark 2.4.4 distrib with :

* Scala 2.11
* Hadoop 3.1.2
* Python 3.7.1
* R 3.3.3
* Kubernetes compatibility

## Create your docker images to build distrib

Run `docker-builder.sh`

Launch docker container and build distrib : 

    docker run -v out:/spark/spark-2.4.4/dist/ spark-builder

After a long time, build is in `out` directory.
    


