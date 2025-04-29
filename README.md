# STARS

This code is the implementation of our research paper "**STARS: A Sampling and Threshold Sharing Solution for Network  K-function Analytics**". Based on our theoretiacl analysis in this paper, this method can further reduce the tiem complexity for generating Network K-function plot.

This code integrates the methods from [NeighbourSharing](https://github.com/edisonchan2013928/Network-K-function) and extends the implementation of our STARS method.


## Code Descriptions

The file "[sam_count.cpp](./src/sam_count.cpp)" shows the implemetnation of the method STARS.



## Quick Start
### 1. Clone Repository
```bash
$ git clone https://github.com/yeswin411/STARS.git
$ cd STARS
```
### 2. Build with CMake (Recommended)

```bash
$ mkdir build && cd build
$ cmake .. -DCMAKE_BUILD_TYPE=Release
$ make -j4
```

### 3. Run Example
```
./STARS data/example_network ./example_output 7 5 5 2 2 0.1
```

### Alternative Compilation
For direct compilation without CMake:
```bash
g++ ./src/*.cpp -O3 -o STARS
```

### 

## Data format 


See see [file_structure.txt](./file_structure.txt) for specifications and [example dataset](./data/example_network) for a concrete example.

## Usage

**Parameter Specification**

```bash
./STARS DATASET_DIR RESULT_DIR METHOD L D START_TAU INCR_TAU [SAMPLING_THRESHOLD]
```



| Parameter | Type | Description | Constraints |
|-----------|------|-------------|-------------|
| `DATASET_DIR` | Path | Input dataset directory | Must follow file structure |
| `RESULT_DIR` | Path | Output file path | Directory must exist |
| `METHOD` | Integer [1-8] | Algorithm selection | See method table |
| `L` | Integer ≥1 | Total datasets (1 real + L-1 randomly generated) |  |
| `D` | Integer ≥1 | Number of distance thresholds |  |
| `START_TAU` | Float | Initial threshold | >0 |
| `INCR_TAU` | Float | Threshold increment | >0 |
| `SAMPLING_THRESHOLD` | Float [0-1] | Sampling rate (Methods 7/8) |  |



### Method Taxonomy

|**Code**|	**Method**|	**Description**|
|:----|:----|:----|
1	|RQS	|Range-Query Searching
2	|SPS	|Shortest Path Sharing
3	|CA	 |Count Aggregation
4	|NS	|Neighbor Sharing
5	|CA<sup>(ASPS)</sup>	|Adaptive SPS with CA
6	|NS<sup>(ASPS)</sup>	|Adaptive SPS with NS
7	|STARS	|Proposed Method (Threshold Sharing + Sampling)
8	|SAMPLING+NS	|NS with sampling


### **Example Configurations**
**STARS Method**

```bash
./STARS data/Chicago result/chicago_stars 7 5 20 100 100 0.1
```

**Baseline Method (RQS)**
```bash
./STARS data/Chicago result/chicago_rqs 1 5 20 100 100 0.0
```


