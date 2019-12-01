#!/bin/bash

cd /spark/spark-2.4.4/
./dev/make-distribution.sh --name magnarox-spark --pip --r --tgz -Psparkr -Phadoop-3.1 -Dhadoop.version=3.1.2 -Phive -Phive-thriftserver -Pkubernetes
