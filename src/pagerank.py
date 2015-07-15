#!/usr/bin/env python
from __future__ import print_function
from pyspark import SparkContext, SparkConf


# use this when running at local
# spark_conf = SparkConf().setMaster("local")
# use this when running on yarn
spark_conf = SparkConf()


sc = SparkContext(conf = spark_conf)

input_filename = "dec_input"


dec_input = sc.textFile(input_filename, use_unicode = False)

# input: URL url url url ...
#        URL points to those urls
# output: tuple of (URL, (pagerank value, url list))
def parse_input_line(line):
    nums = line.split()

    assert(len(nums) >= 1)

    URL = int(nums[0])

    pr = 1.0

    urls = list()

    for url in nums[1:]:
        urls.append(int(url))

    return (URL, (pr, urls))


# input: (URL, (pagerank value, url list))
# output: (URL, (0, url list))
#         (url, (pagerank value / size of url list, []))
def pagerank_map(item):
    return_value = list()
    
    URL = item[0]
    pr = item[1][0]
    urls = item[1][1]

    return_value.append((URL, (0, urls)))
    
    for url in urls:
        return_value.append((url, (pr / len(urls), [])))
    
    return return_value

# input: 2 * (pagerank value, url list1)
#        at most 1 url list is non-empty
# output: (sum of two pagerank values, url list which is non-empty)

def pagerank_reduce(a, b):
    return (a[0] + b[0], a[1] if len(a[1]) else b[1])

# multiply each pagerank value with constant d
def pagerank_reduce2(item):
    d = 0.85
    return (item[0], (item[1][0] * d + 1 - d, item[1][1]))
    
def remove_url_list(item):
    return (item[1][0], item[0])



items = dec_input.map(parse_input_line)


n_iterations = 5;

for i in range(n_iterations):
    items = items.flatMap(pagerank_map).reduceByKey(pagerank_reduce).map(pagerank_reduce2)

    #for j in items.collect():
    #    print(j)
    #print("----------------------------")


items = items.map(remove_url_list)
items = items.sortByKey(0)

items.saveAsTextFile("dec_output")


