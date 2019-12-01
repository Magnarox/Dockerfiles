#!/bin/bash

cd /spark/spark-2.4.4/

chown -R root:root dist/
chmod -R 755 dist/

export MAVEN_OPTS="-Xmx2g -XX:ReservedCodeCacheSize=512m"
./dev/make-distribution.sh --name magnarox-spark --pip --r --tgz -Psparkr -Phadoop-3.1 -Dhadoop.version=3.1.2 -Phive -Phive-thriftserver -Pkubernetes
