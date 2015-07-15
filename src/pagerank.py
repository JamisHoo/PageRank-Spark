#!/usr/bin/env python
from __future__ import print_function
from pyspark import SparkContext, SparkConf


# use this when running at local
# spark_conf = SparkConf().setMaster("local")
# use this when running on yarn
spark_conf = SparkConf()

sc = SparkContext(conf = spark_conf)

input_filename = "bin_input"

delimiter = "ABCDEF\n"

# rdd input is (offset, line)
rdd = sc.newAPIHadoopFile(
    #path
    input_filename,
    # inputFormatClass
    'org.apache.hadoop.mapreduce.lib.input.TextInputFormat',
    # valueClass
    'org.apache.hadoop.io.LongWritable',
    # keyClass
    'org.apache.hadoop.io.Text',
    # keyConverter
    None,
    # valueConverter
    None,
    # conf
    { "textinputformat.record.delimiter": delimiter })

rdd = rdd.map(lambda x: x[1])




