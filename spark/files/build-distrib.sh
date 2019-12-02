#!/bin/bash

cd /spark/

chown -R root:root out/
chmod -R 755 out/

cd /spark/spark-2.4.4/

export MAVEN_OPTS="-Xmx2g -XX:ReservedCodeCacheSize=512m"
./dev/make-distribution.sh --name magnarox-spark --pip --r --tgz -Psparkr -Phadoop-3.1 -Dhadoop.version=3.1.2 -Phive -Phive-thriftserver -Pkubernetes

cp *.tgz ../out/
