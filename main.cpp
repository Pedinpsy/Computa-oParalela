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
	int message[3];
	int bigger;
  	int smaller;
  	int smallers[world_size];
  	int biggers[world_size];
  	int count = 0;  
	

	FILE* f;
	f = fopen("out.txt","rb");
	fread(&aux,sizeof(int),1,f);
	fseek ( f , 0 , SEEK_END);
	tam = ftell(f)/sizeof(int);
	fseek ( f , tam , SEEK_SET);

	if(tam % (world_size-1) == 0 ){
		qtd = (tam/(world_size-1));
	}else

	qtd = (tam/(world_size-1));
	//printf("quantidade de cada bloco : %d\n", qtd);
	recived_limit = world_size;

  	if(world_rank == 0){
  		for (q = 1; q < world_size; q++) {
			int status = MPI_Recv(message, 3, MPI_INT, q,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			//printf("%d\n", recived);
			sum += message[0];
			smallers[q] = message[2];
			biggers[q] = message[1];
			if(q ==1){
				smaller = message[2];
				bigger = message[1]; 
			}
		}
			for (q = 1; q < world_size; q++) {
			if(bigger < biggers[q] ){
  					bigger = biggers[q];
  				}
  				if(smaller > smallers[q] ){
  					smaller = smallers[q];
  				}


		}	

		printf("O total de registros do periodo eh de : %d\n",sum);
		printf("O maior numero eh : %d\n", bigger);
		printf("O menor numero eh : %d \n",smaller);
		printf("O total de dias no periodo eh de %d \n",tam);
 

  	}else{ 

  			
		//printf("meu range eh de %d ate: %d  \n",qtd*(world_rank-1), (qtd*world_rank)-1);
		start = qtd*(world_rank-1);
		end =  (qtd*world_rank)-1;
		fseek ( f , start*sizeof(int),0);
		for (int i = start;i<= end;i++ ){
			count ++;
			fread(&aux,sizeof(int),1,f);
			sum += aux;		
  			
  			if(i ==  start){
				bigger = aux;
				smaller = aux;
  			}else{
  				if(bigger < aux ){
  					bigger = aux;
  				}
  				if(smaller > aux ){
  					smaller = aux;
  				}
  			}


  		}
  		if(world_rank  == world_size-1){
  		

			for(int i = 0 ; i <tam %  (world_size-1); i ++){
				fread(&aux,sizeof(int),1,f);
				sum += aux;	

			}
  		}
  		message[0] = sum;
  		message[1] = bigger;
  		message[2] = smaller;

  		MPI_Send(message, 3, MPI_INT, 0, 0,MPI_COMM_WORLD);



  	}
  	  MPI_Finalize();
return 0;


}