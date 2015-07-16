#!/bin/sh                                                                       
                                                                                
export HADOOP_CONF_DIR=/home/centos/hadoop-2.7.1/etc/hadoop/                    
                                                                                
SPARK_PATH=~/spark-1.4.0-bin-hadoop2.6/                                         
                                                                                
${SPARK_PATH}/bin/spark-submit \                                                
--master yarn-cluster \                                                         
--num-executors 3 \                                                             
--driver-memory 3g \                                                            
--executor-memory 3g \                                                          
--executor-cores 1 \                                                            
--queue default \                                                               
pagerank.py

