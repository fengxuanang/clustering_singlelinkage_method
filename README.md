# Single Linkage Clustering Method

A C implementation of the single linkage hierarchical clustering algorithm for grouping data points based on distance matrices.

## Overview

This program implements the **single linkage clustering** method, also known as the nearest neighbor method. It's a hierarchical clustering algorithm that groups data points by iteratively merging the closest clusters based on minimum pairwise distances.

### Algorithm

The single linkage method works as follows:
1. Start with each data point as its own cluster
2. Find the two closest clusters (minimum distance)
3. Merge these clusters into one
4. Update the distance matrix using the minimum distance rule
5. Repeat steps 2-4 until the desired number of clusters is reached

## Features

- Dynamic memory allocation for flexible dataset sizes
- CSV input format support
- Configurable number of output clusters
- Efficient matrix-based distance calculations
- Clean, well-documented code

## Requirements

- C compiler (GCC, Clang, or MSVC)
- Standard C library

## Installation

### Compile the program:

```bash
gcc -o singlelink singlelink.c -lm
```

The `-lm` flag links the math library (required for `sqrt` function).

## Usage

### Basic Usage

```bash
./singlelink <input_file>
```

### Example

```bash
./singlelink distance.csv
```

### Input File Format

The input file should be a CSV file containing a distance matrix in the following format:

```
index_1,index_2,distance
1.000,1.000,0.000
1.000,2.000,206
1.000,3.000,429
...
```

- **index_1**: First data point index
- **index_2**: Second data point index  
- **distance**: Distance between the two points

The file should contain all pairwise distances in a square matrix format (N×N entries for N data points).

### Output

The program generates a `result.csv` file containing the clustering results:

```
index,clusterID
1,1
2,1
3,1
4,2
...
```

Each row shows the data point index and its assigned cluster ID.

## Configuration

You can modify the following parameters in `singlelink.h`:

- **CLUSTER_NUM**: Number of clusters to generate (default: 2)
- **RESULT_FILE_PATH**: Output file path (default: "result.csv")
- **READ_FILE_RECORD_SIZE**: Maximum record size for input (default: 1024)

### Example: Changing Number of Clusters

Edit `singlelink.h`:

```c
#define CLUSTER_NUM 3  // Change from 2 to 3 clusters
```

Then recompile the program.

## Example Dataset

The included `distance.csv` file contains a 9×9 distance matrix representing distances between 9 data points (e.g., cities). This sample dataset can be used to test the clustering algorithm.

## Project Structure

```
clustering_singlelinkage_method-master/
├── singlelink.c       # Main implementation
├── singlelink.h       # Header file with definitions
├── distance.csv       # Sample input data
└── README.md          # This file
```

## Algorithm Complexity

- **Time Complexity**: O(n³) where n is the number of data points
- **Space Complexity**: O(n²) for the distance matrix

## Implementation Details

### Key Functions

- `readFile()`: Reads the distance matrix from CSV file
- `initMatrix()`: Initializes the distance matrix
- `findMinInfo()`: Finds the minimum distance in the matrix
- `combineData()`: Merges two clusters using minimum linkage
- `updateMatrix()`: Updates the distance matrix after merging
- `adjustCluster()`: Adjusts cluster IDs to be sequential
- `writeResultFile()`: Outputs the clustering results

### Data Structures

- **Item**: Stores distance matrix entries (index_1, index_2, distance)
- **Cluster**: Stores cluster information (index, clusterID, dispersion)

## Limitations

- Requires complete distance matrix as input
- Single linkage can produce elongated clusters (chaining effect)
- Number of clusters must be specified in advance

## License

This project is open source and available for educational and research purposes.

## Author

Created by Feng (2017/08/24)

