#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main() {
 	int world_rank;
  	int world_size;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
 	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  	

 	int recived;
	int aux = 1;
	int qtd = 0;
	long tam = 0;
	int start,end;
	int sum = 0;
	int recived_count;
	int recived_limit;
	int q;
	FILE* f;
	f = fopen("out.txt","rb");
	fread(&aux,sizeof(int),1,f);
	fseek ( f , 0 , SEEK_END);
	tam = ftell(f)/sizeof(int);
	fseek ( f , tam , SEEK_SET);
	qtd = (tam/(world_size-1));
	//printf("quantidade de cada bloco : %d\n", qtd);
	recived_limit = world_size;

  	if(world_rank == 0){
		printf("controlador\n");
  		for (q = 1; q < world_size; q++) {
			int status = MPI_Recv(&recived, 1, MPI_INT, q,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("%d\n", status);
			printf("%d\n", recived);
			sum += recived;
		}
	/*	while(recived_count < recived_limit){
			
		}*/

		printf("o total eh de \n",sum);
  		printf("Im a controller\n");

  	}else{ 	
		//printf("meu range eh de %d ate: %d  \n",qtd*(world_rank-1), (qtd*world_rank)-1);
		start = qtd*(world_rank-1);
		end =  (qtd*world_rank)-1;
		fseek ( f , start*sizeof(int),0);
		for (int i = start  ;  i < end ;i ++ ){  
			sum += aux;		
  			fread(&aux,sizeof(int),1,f);
  			
  		}
  		MPI_Send(&sum, 1, MPI_INT, 0, 0,MPI_COMM_WORLD);



  	}
  	  MPI_Finalize();
return 0;


}