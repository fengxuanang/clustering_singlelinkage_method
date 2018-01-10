#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <math.h>

#include "singlelink.h"

//global variables
Item* data; //input file data
Cluster* cluster; // cluster array
//double matrix[DIMENSION + 1][DIMENSION + 1]; // matrix for processs
//double originalMatrix[DIMENSION + 1][DIMENSION + 1]; // original matrix backup
double **matrix; // matrix for processs
//double **originalMatrix;// original matrix backup
//int maxClusterNum [DIMENSION][1 + 5]; // max number information in cluster
int DIMENSION;
char READ_FILE_PATH[256];

/****************************
 Main
 ***************************/
int main(int argc, const char * argv[]) {
    //printf("main start\n");// debug
    if(argc != 2){
      printf("Usage: %s (input file)\n",argv[0]);
      exit(8);
    }
    
    memset(READ_FILE_PATH, 0x00, sizeof(READ_FILE_PATH));
    strncpy(READ_FILE_PATH, argv[1],256);
    
    printf("Input file: %s\n",READ_FILE_PATH);
    //init
    initialization();
    //process
    process();
    //termination
    termination();

    //printf("main over\n");// debug
    
    return 0;
}

/****************************
 initialization
 ***************************/
void initialization() {

    //printf("initialization start\n");// debug
    //read file
    readFile();
    //allocate memory
    allocateMem();
    // initCluster
    initCluster();
    // makeDataMatrix
    initMatrix();
    // writeOriginalMatrix
    //writeOriginalMatrix();

    //printf("initialization end\n");// debug
 
    return;
}

/****************************
 process
 ***************************/
void process() {
    //
    int cnt = DIMENSION + 1;
    int row, colum = 0;
    int rowName, columName = 0;
    int loopCnt = 0;

    //printf("process start\n");// debug
    //
    while (cnt - 1 > CLUSTER_NUM) {
        // find the min distance
        findMinInfo(cnt, &row, &colum, &rowName, &columName);

        //printf("row = %d\n", row); //debug
        //printf("colum = %d\n", colum); //debug
        //printf("rowName = %d\n", rowName); //debug
        //printf("columName = %d\n", columName); //debug
        // update matrix shape and data.
        updateMatrix(cnt, row, colum);
        // update clusterID in cluster
        updateCluster(cnt, rowName - 1, columName - 1);
        // update max num information in cluster
        //updateMaxClusterNum(loopCnt);
        //
        cnt--; // important!
        //
        loopCnt++;
    }

    // write the temp matrix result into csv file
    //writeTempMatrix(cnt);//debug

    //adjust clusterID in cluster
    adjustCluster();

    //calculate center
    //calCenter();

    //debug
    /*
    int i;
    for(i = 0; i < DIMENSION; i++){
        //
        printf("cluster[%d].clusterID = %d\n", i, cluster[i].clusterID);
    }*/

    //printf("process end\n");// debug

    return;
}

/****************************
 termination
 ***************************/
void termination() {
    //
    //printf("termination start\n");// debug
    // write the result file
    writeResultFile();
    //writeMaxClusterNumFile();

    free(data);
    free(cluster);
    free(matrix);
    //free(originalMatrix);

    //printf("termination end\n");// debug
    return;
}

// allocate the memory
void allocateMem() {

    //printf("allocateMem start\n");// debug
    //
    int i = 0;
    cluster = NULL;
    // allocate cluster
    cluster = (Cluster*)malloc(sizeof(struct cluster_tag) * (DIMENSION + 1));
    if (data == NULL) {
        printf("malloc error:cluster\n");  
        exit(8);
    }

    // matrix
    matrix = NULL;
    matrix = (double**)malloc(sizeof(double*) * (DIMENSION + 1));
    for (i = 0; i < DIMENSION + 1; i++) {
        //
        matrix[i] = (double*)malloc(sizeof(double) * (DIMENSION + 1));
    }
    if (matrix == NULL) {
        printf("malloc error:matrix\n");  
        exit(8);
    }

    // originalMatrix
    /*
    originalMatrix = NULL;
    originalMatrix = (double**)malloc(sizeof(double*) * (DIMENSION + 1));
    for (i = 0; i < DIMENSION + 1; i++) {
        //
        originalMatrix[i] = (double*)malloc(sizeof(double) * (DIMENSION + 1));

    }
    if (originalMatrix == NULL) {
        printf("malloc error:matrix\n");  
        exit(8);
    }*/

    //printf("allocateMem end\n");// debug
    
    return;
}

//read data from file
void readFile() {
    //
    //printf("readFile start\n");// debug
    //
    int  Row = 0;
    double d1,d2,d3 = 0.0;
    char str1[128];
    char str2[128];
    char str3[128];
    int nColum = 0;
    char RecordBuf[READ_FILE_RECORD_SIZE];
    FILE *fp = NULL;
    //
    memset(str1, 0x00, sizeof(str1));
    memset(str2, 0x00, sizeof(str2));
    memset(str3, 0x00, sizeof(str3));
    memset(RecordBuf, 0x00, sizeof(RecordBuf));
    
    //file open
    fp = fopen(READ_FILE_PATH, "r");
    if ( fp == NULL ) {
        printf("Datafile is not found\n");
        exit(8); //
    }
    
    //csv file read (backup)
    while (fgets(RecordBuf, READ_FILE_RECORD_SIZE, fp)) {
        // allocate memory
        if (Row == 0) {
            //
            data = (Item*)malloc(sizeof(struct item_tag) * (1));
            if (data == NULL) {
                //
                printf("malloc error:data\n");
                exit(8);
            }
        } else {
            //
            if (data != NULL) {
                //
                data = (Item*)realloc(data, sizeof(struct item_tag) * (Row + 1));
                //
                if (data == NULL) {
                    //
                    printf("realloc error:data\n");
                    exit(8);
                }
            }
        }
        sscanf(RecordBuf, "%lf,%lf,%lf", &data[Row].index_1,
                                         &data[Row].index_2,
                                         &data[Row].distance);

        //printf("RecordBuf = %s\n",RecordBuf);//debug
        //printf("data[%d][0] = %lf\n",  Row, data[Row].index_1);//debug
        //printf("data[%d][1] = %lf\n",  Row, data[Row].index_2);//debug
        //printf("data[%d][2] = %lf\n",  Row, data[Row].distance);//debug

        Row++;
    }

    //printf("totol Row = %d\n", Row);

    // gnu file read
    /*
    while (fgets(RecordBuf, READ_FILE_RECORD_SIZE, fp)) {
        //
        nColum = sscanf(RecordBuf, " %lf %lf %lf", &d1, &d2, &d3);
        
        if (nColum == 3) {
            //
            sscanf(RecordBuf, " %s %s %s", str1, str2, str3);
            if (strlen(str3) != 1) {
                // copy string data to double
                sscanf(RecordBuf, " %lf %lf %lf", &data[Row].index_1, &data[Row].index_2, &data[Row].distance);

                Row++;
                memset(RecordBuf, 0x00, sizeof(RecordBuf));
                memset(str1,      0x00, sizeof(str1));
                memset(str2,      0x00, sizeof(str2));
                memset(str3,      0x00, sizeof(str3));
            }
        }
    }
    */
    //calculate the dimension
    DIMENSION = sqrt(Row);
    //printf("readFile end\n");// debug

    return;
}

//init cluster array
void initCluster() {
    //
    int i = 0;
    //printf("initCluster start\n");// debug
    //index 
    for (i = 0; i < DIMENSION; i++) {
        cluster[i].index = i + 1;     // index from 1 to dimension
        cluster[i].clusterID = i + 1; // init id
        cluster[i].dispersion = 0.0;  // init dispersion
    }

    //printf("initCluster end\n");// debug

    return;

}


//create the original matrix
void initMatrix() {
    //
    int i,m,n = 0;
    int cnt = 0;

    //printf("initMatrix start\n");// debug

    //index header
    for (i = 0; i < DIMENSION + 1; i++) {
        // process matrix
        matrix[i][0] = i;
        matrix[0][i] = i;
        // original matrix
        //originalMatrix[i][0] = i;
        //originalMatrix[0][i] = i;
        // max cluster number matrix
        //maxClusterNum[i][0] = i + 1;
    }

    // data
    for (n = 1; n < DIMENSION + 1; n++) {
        for (m = 1; m < DIMENSION + 1; m++) {
            // process matrix
            matrix[m][n] = data[cnt].distance;
            // original matrix
            //originalMatrix[m][n] = data[cnt].distance;
            cnt++;
        }
    }

    //check (debug)
    /*
    for (m = 0; m < DIMENSION + 1; m++) {
        for (n = 0; n < DIMENSION + 1; n++) {
            printf("originalMatrix[%d][%d] = %f\n", m, n, matrix[m][n]);
        }
    } */

    //printf("initMatrix end\n");// debug

    return;
}

//find the min value, index information in matrix
void findMinInfo(int cnt, int* row, int* colum, int* rowName, int* columName) { // todo
    //
    int i, j = 0;
    double min = 9999.999; // max value
    //printf("findMinDistance start\n"); // debug

    for (i = 2; i < cnt; i++) { // i = 2 start, please look at the matrix.csv
        for(j = 1; j < cnt; j++) {
            //
            if ( i != j ) {
                //
                if (matrix[i][j] < min) {
                    //
                    min = matrix[i][j];
                    *row = i; //
                    *rowName = matrix[i][0];
                    *colum = j; //
                    *columName = matrix[0][j];
                }
            } else {
                //
                break;
            }

        }
    }

    //printf("min in matrix is %f\n", min);// debug

    //printf("findMinDistance end\n");// debug

    return;
}

// update value in matrix
void updateMatrix(int cnt, int row, int colum) {

    //printf("updateMatrix start\n");// debug

    // combineColum
    combineData(cnt, row, colum);

    // deleteColum
    deleteColum(cnt, row); // number row

    // deleteRow
    deleteRow(cnt, row);

    //printf("updateMatrix end\n");// debug

    return;
}

// combine two colums in one
// combine two rows in one
void combineData(int cnt, int colum1, int colum2) {
    //printf("combineColum start\n");// debug
    int i = 0;

    for (i = 1; i < cnt; i++) {
        // minimum 
        if (matrix[i][colum1] < matrix[i][colum2]) {
            // update colum
            matrix[i][colum2] = matrix[i][colum1];
            // update row
            matrix[colum2][i] = matrix[i][colum2];
        }
    }

    //printf("combineColum end\n");// debug

    return;
}

// delete the colum in matrix
void deleteColum(int cnt, int colum) {
    int i,j = 0;

    //printf("deleteColum start\n");// debug
    for(i = 0; i < cnt; i++) {
        for(j = colum; j < cnt - 1; j++) {
            // cover colum data
            matrix[i][j] = matrix[i][j + 1];
        }
    }

    //printf("deleteColum end\n");// debug

    return;
}

// delete the row in matrix
void deleteRow(int cnt, int row) {
    int i,j = 0;

    //printf("deleteRow start\n");// debug
    for(i = row; i < cnt - 1; i++) {
        for(j = 0; j < cnt; j++)
            // cover row datas
            matrix[i][j] = matrix[i + 1][j];
    }

    //printf("deleteRow end\n");// debug
    return;
}


//update the cluster array
void updateCluster(int cnt, int index1, int index2) {
    //printf("updateCluster start\n");// debug

    int i = 0;
    int temp = 0;

    // temp value (need to be changed as cluster[index2].clusterID)
    temp = cluster[index1].clusterID;

    //update
    for (i = 0; i < DIMENSION; i++) {
        //
        if (cluster[i].clusterID == temp) {
            // cover id
            cluster[i].clusterID = cluster[index2].clusterID;
        }
    }

    //printf("updateCluster end\n");// debug

    return;
}

//
/*
void updateMaxClusterNum(int index) {
    //
    int i, j = 0;
    int cnt = 0;
    int idArray[DIMENSION];
    //printf("updateMaxClusterNum start\n");// debug

    for (i = 0; i < DIMENSION; i++) {
        //
        for (j = 0; j < DIMENSION; j++) {
            // find same id in cluster
            if (i + 1 == cluster[j].clusterID) {
                //
                cnt++;
            }
        }
        // copy count to id array
        idArray[i] = cnt;
        // init counter
        cnt = 0;
    }

    for (i = 0; i < DIMENSION; i++) {
        // quick sort (denscending)
        qsort(idArray, DIMENSION, sizeof(idArray[0]), cmp);
    }

    for (i = 0; i < 5; i++) {
        // copy max 5 id number to maxClusterNum
        maxClusterNum[index][i + 1] = idArray[i];
    }

    //printf("updateMaxClusterNum end\n");// debug

    return;
}

//compare function for quick sort
int cmp ( const void *a , const void *b ) {
    //
    //return *(int *)a - *(int *)b;  //
    return *(int *)b - *(int *)a; //descending

}
*/

// adjust clusterID in cluster array
void adjustCluster() {
    //
    int i, j, k = 0;
    int breakNum = 0;
    int numCnt = 0;
    //printf("adjustCluster start\n");// debug

    //clustering process is over, need to adjust clusterID
    //find the break number in cluster
    for (i = 1; i < CLUSTER_NUM + 1; i++) {
        for(j = 0; j < DIMENSION; j++) {
            //
            numCnt = 0;
            //
            if (cluster[j].clusterID == i) {
                //
                numCnt++;
                break;
            } 
        }

        if (numCnt == 0) {
            // find
            breakNum = i;
            break;
        }
    }

    // need to adjust clusterID
    if (breakNum != 0) {
        //
        for (i = 0; i < DIMENSION; i++) {
            // adjust clusterID in cluster with breakNum
            if (cluster[i].clusterID > breakNum) {
                // modify the clusterID to real value
                k = i + 1;
                for (j = k; j < DIMENSION; j++ ) {
                    if (cluster[i].clusterID == cluster[j].clusterID) {
                        //
                        cluster[j].clusterID = breakNum;
                    }
                }
                //
                cluster[i].clusterID = breakNum;
                breakNum++;
            }
        }
    }

    //printf("adjustCluster end\n");// debug

    return;

}

//calculate center of cluster
/*
void calCenter() {
    //
    int i,j = 0;
    int id = 0;
    int index1,index2 = 0;
    printf("calCenter start\n");// debug

    for (i = 0; i < DIMENSION; i++) {
        //
        id = cluster[i].clusterID;
        index1 = cluster[i].index;
        for (j = 0; j < DIMENSION; j++) {
            if (id == cluster[j].clusterID) {
                //
                cluster[i].dispersion += (originalMatrix[i + 1][j + 1] * originalMatrix[i + 1][j + 1]);
            }
        }

    }
    printf("calCenter end\n");// debug
    return;
}
*/

//write result file
void writeResultFile() {
    //
    int m,n = 0;

    //printf("writeResultFile start\n");// debug
    FILE *fp = NULL;

    fp = fopen(RESULT_FILE_PATH, "w");
    if (fp == NULL) {
        printf("result file is not found\n");
        exit(8);
    }
    /*
    for (m = 0; m < DIMENSION; m++) {
        for (n = 0; n < 3; n++) {
            if (n == 2) {
                fprintf(fp, "%lf", cluster[m].dispersion);
            } else if (n== 1) {
                fprintf(fp, "%d,", cluster[m].clusterID);
            } else {
                fprintf(fp, "%d,", cluster[m].index);
            }
        }
        fprintf(fp, "\n");
    }*/

    for (m = 0; m < DIMENSION; m++) {
        for (n = 0; n < 2; n++) {
            if (n== 1) {
                fprintf(fp, "%d", cluster[m].clusterID);
            } else {
                fprintf(fp, "%d,", cluster[m].index);
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    fp = NULL;

    //printf("writeResultFile end\n");// debug

    return;
}

//write result file
/*
void writeMaxClusterNumFile() {
    //
    int m,n = 0;

    printf("writeMaxClusterNumFile start\n");// debug
    FILE *fp = NULL;

    fp = fopen(MAXCLUSTER_FILE_PATH, "a");
    if (fp == NULL) {
        printf("maxClusterNum file is not found\n");
        exit(8);
    }
    
    // header
    fprintf(fp, "%s,%s,%s,%s,%s,%s\n", "step", "MaxClusterNum1",
                                               "MaxClusterNum2",
                                               "MaxClusterNum3",
                                               "MaxClusterNum4",
                                               "MaxClusterNum5");

    for (m = 0; m < DIMENSION; m++) {
        for (n = 0; n < 6; n++) {
            if (n != 5) {
                fprintf(fp, "%d,", maxClusterNum[m][n]);
            } else {
                fprintf(fp, "%d", maxClusterNum[m][n]);
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    fp = NULL;

    printf("writeMaxClusterNumFile end\n");// debug

    return;
}
*/



//debug function
// write original matrix data into csv file
/*
void writeOriginalMatrix() {
    //
    int m,n = 0;

    //printf("writeOriginalMatrix start\n");// debug
    FILE *fp = NULL;

    fp = fopen(DATA_MATRIX_PATH, "w");
    if (fp == NULL) {
        printf("originalMatrixfile is not found\n");
        exit(8);
    }

    //debug
    for (m = 0; m < DIMENSION + 1; m++) {
        for (n = 0; n < DIMENSION + 1; n++) {
            if (n == DIMENSION) {
                fprintf(fp, "%f", originalMatrix[m][n]);
            } else {
                fprintf(fp, "%f,", originalMatrix[m][n]);
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    fp = NULL;

    //printf("writeOriginalMatrix end\n");// debug

    return;
}*/

//debug function
// write temp matrix data
/*
void writeTempMatrix(int cnt) {
    //
    int m,n = 0;

    //printf("writeTempMatrix start\n");// debug
    FILE *fp = NULL;

    fp = fopen(TEMP_MATRIX_PATH, "w");
    if (fp == NULL) {
        printf("TempMatrixfile is not found\n");
        exit(8); //
    }

    //debug
    for (m = 0; m < cnt; m++) {
        for (n = 0; n < cnt; n++) {
            //printf("matrix[%d][%d] = %f\n", m, n, matrix[m][n]);//debug
            // debug
            if ( matrix[0][n] != n || matrix[m][0] != m) {
                //
                matrix[0][n] = n;
                matrix[m][0] = m;
            }

            if (n == cnt - 1) {
                fprintf(fp, "%f", matrix[m][n]);
            } else {
                fprintf(fp, "%f,", matrix[m][n]);
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    fp = NULL;

    //printf("writeTempMatrix end\n");// debug

    return;
}
*/
