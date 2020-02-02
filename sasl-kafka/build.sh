#!/bin/bash

cd kafka
docker build -t confluentinc/cp-kafka-sasl:5.2.1 .

cd ../zookeeper
docker build -t confluentinc/cp-zookeeper-sasl:5.2.1 .
