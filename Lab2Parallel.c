#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <omp.h>

/********************************
 Rules:
 0. a neighbor is defined by a cell that is vertically, horizontally, or diagonally adjacent. Cells have 8 neighbors, with the exception of the corner and border cells, which have fewer.
 1. an alive cell with fewer than two alive neighbors dies in the next generation
 2. an alive cell with more than three alive neighbors dies in the next generation
 3. an alive cell with exactly two or three alive neighbors stays alive in the next generation
 4. a dead cell with exactly three alive neighbors becomes alive in the next generation
 ********************************/
void ParallelCellTable(int gens, int N, int thread_count, const char *filename)
{
    //initialize the global array
    int *globalArray = (int *)calloc(N * N, sizeof(int));
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        printf("%s%s%s\n","File ", filename," did not open" );
    }
    else
    {
        while(!feof(fptr))
        {
            for (int i = 0; i < N * N; i++)
            {
                fscanf(fptr, "%d", &globalArray[i]);   
            }
        }
    }
    fclose(fptr);
    int *global_next_gen = (int *)calloc(N*N, sizeof(int));
    double totalTime = 0;
    for (int gen = 0; gen < gens; gen++)
    {
        double startTime = omp_get_wtime();
        #pragma omp parallel num_threads(thread_count)
        {
            int *localNextGen = (int *)calloc(N * N, sizeof(int));
            int counter = 0;
            //set the 
            #pragma omp for nowait
            //find the rows of the next generation in parallel
            for (int rowIndex = 0; rowIndex < N; rowIndex++)
            {
                for (int i = 0; i < N; i++)
                {
                    counter = 0;
                    if (rowIndex == 0)// check the top border
                    {
                        //top left corner
                        if (i == 0)
                        {
                            if (globalArray[(rowIndex + 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                        }
                        //top right corner
                        else if (i == N - 1)
                        {
                            if (globalArray[(rowIndex) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i] == 1)
                            {
                                counter++;
                            }                
                        }
                        //top middle
                        else
                        {
                            if (globalArray[(rowIndex + 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex) * N + i + 1] == 1)
                            {
                                counter++;
                            } 
                            if (globalArray[(rowIndex) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i-1] == 1)
                            {
                                counter++;
                            } 
                        }
                    }


                    //bottom row
                    else if (rowIndex == N - 1)
                    {
                        // bottom left corner
                        if (i == 0)
                        {
                            if(globalArray[(rowIndex - 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                        }
                        //bottom right corner
                        else if (i == N - 1)
                        {
                            if(globalArray[(rowIndex - 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                        }
                        //bottom middle
                        else
                        {
                            if(globalArray[(rowIndex - 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                        }
                    }

                    
                    //middle row
                    else
                    {
                        //left border
                        if (i == 0)
                        {
                            if (globalArray[(rowIndex + 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                        }
                        //right border
                        else if (i == N - 1)
                        {
                            if (globalArray[(rowIndex + 1) * N + i-1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i - 1] == 1)
                            {
                                counter++;
                            }        
                        }
                        //middle cell
                        else
                        {
                            if (globalArray[(rowIndex + 1) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i - 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex + 1) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if (globalArray[(rowIndex) * N + i + 1] == 1)
                            {
                                counter++;
                            }
                            if(globalArray[(rowIndex - 1) * N + i + 1] == 1)
                            {
                                counter++;
                            }                
                        }
                        
                    }

                    if (globalArray[rowIndex * N + i] == 1)
                    {
                        if (counter !=2 && counter != 3)
                        {
                            localNextGen[rowIndex * N + i] =  0;
                        }
                        else
                        {
                            localNextGen[rowIndex * N + i] = 1;
                        }
                        
                    }

                    else if (globalArray[rowIndex* N + i] == 0)
                    {
                        if (counter == 3)
                        {
                            localNextGen[rowIndex * N + i] = 1;
                        }
                        else
                        {
                            localNextGen[rowIndex * N + i] = 0;
                        }
                        
                    }   
                }
                for (int i = 0; i < N; i++)
                {
                        global_next_gen[rowIndex * N + i] = localNextGen[rowIndex * N + i];
                }
            }
            free(localNextGen);
        }
        double endtime = omp_get_wtime();
        totalTime += (endtime - startTime);
        for (int i = 0; i < N * N; i++)
        {
            globalArray[i] = global_next_gen[i];
        }
    }
    free(global_next_gen);
    printf("%s%d%s%f", "The average time per generation with ", thread_count," threads:", (totalTime/gens));
    //write file
    char *output;
	int len = 0;
	//get the length of the filename for malloc
	for (int i = 0; filename[i] != '\0'; i++)
		len++;
	//allocate space based on initial file name
	output = (char *)malloc(len + 4);
	for (int i = 0; i < len; i++)
	{
		output[i] = filename[i];
	}
	output[len] = '.';
	output[len + 1] = 'o';
	output[len + 2] = 'u';
	output[len + 3] = 't';
    FILE *fptr1;
    fptr1 = fopen(output, "w");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            fprintf(fptr1, "%d%s", globalArray[i * N + j], " ");
        }
        fprintf(fptr1,"%s\n", "");
    }
    fclose(fptr1);
    free(output);
    free(globalArray);
}
int main(int argc, const char *argv[])
{
    int gens = atoi(argv[1]);
    int N = atoi(argv[2]);
    int thread_count = atoi(argv[3]);
    ParallelCellTable(gens, N, thread_count, argv[4]);
    
}