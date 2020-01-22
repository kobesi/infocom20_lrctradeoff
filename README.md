In this file, we briefly introduce how to deploy and run the lrctradeoff prototype. More design details can be referred to our paper "__Si Wu, Zhirong Shen, Patrick P. C. Lee__, On the Optimal Repair-Scaling Trade-off in Locally Repairable Codes" which appears at _InfoCom 2020_. If you have any questions regarding the deployment and the code, please feel free to contact me at siwu5938@gmail.com.

## 1. Files and Descriptions

- Config.hh, Config.cc: the analysis of the configuration file.

- Socket.hh, Socket.cc: the implementation of the network socket operations, including sending and receiving data over the network.

- Metadata.hh, Metadata.cc: the implementation of the metadata, including metadata write, read, and update.

- Coordinator.hh, Coordinator.cc: the implementation of the Coordinator (CN), which sends commands to the Datanodes (DNs) and receives acks.

- Datanode.hh, Datanode.cc: the implementation of the DNs, which receive commands and execute the actual data read, write, and transfer in parallel, and finally reply acks to the CN.

## 2. Configuration

#### 2.1. Introduction to the configuration

The configuration is realized by a XML file called “configuration.xml”. This file specifies the following parameters (and also the physical meanings):

| Parameter           | Physical meaning                                             |
| ------------------- | ------------------------------------------------------------ |
| k                   | Number of data blocks in a LRC-coded stripe                  || l_f                 | Number of local parity blocks in a fast LRC-coded stripe     || g                   | Number of global parity blocks in a LRC-coded stripe         || l_c                 | Number of local parity blocks in a compact LRC-coded stripe  || place_method        | Placing method, 1 for Opt-S, 2 for Opt-R, and 3 for Flat     || rack_num            | Number of racks/ clusters                                    || cn_ip               | IP address of the CN                                         || gw_ip               | IP address of the gateway node                               || chunk_size          | Size of a block, e.g., 64MB                                  || packet_size         | Size of a packet in network transmission, e.g., 1MB          || data_path           | Absolute path that stores the data blocks in each DN         || /rack1, /rack2, …   | The rack to node mappings                                    |
#### 2.2. Configuration example

We give an example configuration as follows:

```xml
<setting>
<attribute><name>k</name><value>4</value></attribute>
<attribute><name>l_f</name><value>2</value></attribute>
<attribute><name>g</name><value>2</value></attribute>
<attribute><name>l_c</name><value>1</value></attribute>
<attribute><name>place_method</name><value>1</value></attribute>
<attribute><name>rack_num</name><value>2</value></attribute>
<attribute><name>cn_ip</name><value>192.168.0.22</value></attribute>
<attribute><name>gw_ip</name><value>192.168.0.19</value></attribute>
<attribute><name>chunk_size</name><value>64</value></attribute>
<attribute><name>packet_size</name><value>1</value></attribute>
<attribute><name>data_path</name><value>/home/jhli/WUSI/lrctradeoff/data/</value></attribute>
<attribute><name>/rack1</name>
<value>192.168.0.12</value>
<value>192.168.0.13</value>
<value>192.168.0.14</value>
<value>192.168.0.15</value>
</attribute>
<attribute><name>/rack2</name>
<value>192.168.0.24</value>
<value>192.168.0.25</value>
<value>192.168.0.18</value>
</attribute>
</setting>
```

Note that this scaling operation is from fast LRC (k, l, g) = (4, 2, 2) to compact LRC (k, l’, g) = (4, 1, 2). The node “192.168.0.22” acts as a CN, while the node “192.168.0.19” acts a gateway. Nodes “12, 13, 14, 15” reside in the 1st rack/ cluster, while nodes “24, 25, 18” reside in the 2nd rack/ cluster. Note also that we omit the rack/ cluster for storing the global parity blocks. 

In each rack/ cluster, nodes communicate with each other with low latency links. Cross-cluster transfers must traverse the gateway node. Since the in and out links of the gateway node are with high latency, cross-cluster transfers are the performance bottleneck. In our experiments, we use the Wonder Shaper tool (https://github.com/magnific0/wondershaper) to control the in and out bandwidth of the gateway node.

## 3. Deployment of the lrctradeoff prototype

Our code is implemented with C++. We show how to compile and run the code on Ubuntu with g++ and make (We run our experiments on Ubuntu 16.04.5 with g++ version 5.4.0 and make version 4.1).

#### 3.1. Compile

Compiling:

```shell
$ make
```

After successfully make, you will find two executables namely LRCCN and LRCDN, which represent the CN and the DN, respectively.

#### 3.2. Distribute the code package to the DNs

You should first modify the dist.sh shell, and then running:

```shell
$ ./dist.sh
```

#### 3.3. Run the prototype

- At the CN, you should first generate a file whose name is composed of six characters, e.g., 

```shell
$ dd if=/dev/urandom of=FI0000 bs=1M count=256
```

Then you should run the CN:

```shell
$ ./LRCCN
```
- At each DN, you should run the DN:

```shell
$./LRCDN
```

#### 3.4. Different commands from the CN

At the CN terminal, you can input different commands to execute the file upload/ download/ upcode/ downcode operations. 

- "ul FI0000": upload the file to the DNs.

- "dl FI0000": download the file to the CN. If there is any block missing, the CN will trigger decode, and then download the file again.

- "uc FI0000": upcode the file from fast LRC into compact LRC.

- "dc FI0000": downcode the file from compact LRC into fast LRC again.

- "te FI0000": collectively run upload, download (decode), upcode, and downcode, test the performance of each operation.
