//
//  cluster.h
//  cluster
//
//  Created by Feng on 2017/08/24.
//

#ifndef singlelink_h
#define singlelink_h

// read file
//#define READ_FILE_PATH "distance.csv" //us cities testdata
#define READ_FILE_RECORD_SIZE 1024

// write file
#define RESULT_FILE_PATH "result.csv"
//#define MAXCLUSTER_FILE_PATH "maxClusterInfo.csv"
//#define DATA_MATRIX_PATH "matrix.csv" //debug file
//define TEMP_MATRIX_PATH "tempMatrix.csv" //debug file
//#define MATRIX_RECORD_SIZE 1024

//data dimesion
//#define DIMENSION 9

// cluster
#define CLUSTER_NUM 2

// data item
struct item_tag {  
    double index_1;   //column1  
    double index_2;   //column2
    double distance;
};
typedef struct item_tag Item;

// cluster infomation
struct cluster_tag {
    int    index;
    int    clusterID;
    double dispersion;
};
typedef struct cluster_tag Cluster;

// function
int main(int argc, const char * argv[]);
void initialization();
void allocateMem();
void readFile();
void initCluster();
void initMatrix();
//void writeOriginalMatrix(); //debug
void process();
void findMinInfo(int cnt, int* row, int* colum, int* rowName, int* columName);
void updateMatrix(int cnt, int row, int colum);
void combineData(int cnt, int colum1, int colum2);
void deleteColum(int cnt, int colum);
void deleteRow(int cnt, int row);
void updateCluster(int cnt, int index1, int index2);
//void updateMaxClusterNum(int index);
//int cmp (const void *a , const void *b);
//void writeTempMatrix(int cnt); //debug
void adjustCluster();
//void calCenter();
void termination();
void writeResultFile();
//void writeMaxClusterNumFile();


#endif /* singlelink_h */