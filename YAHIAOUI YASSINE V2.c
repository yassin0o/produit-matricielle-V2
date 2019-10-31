//YAHIAOUI MOHAMED YASSINE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <math.h>


#define NBLigne_A 3
#define NBColonne_A 3
#define NBLigne_B 3
#define NBColonne_B 3
#define NBThread 9

int A[NBLigne_A][NBColonne_A];
int B[NBLigne_B][NBColonne_B];
int resultp[NBLigne_A][NBColonne_B];
int results[NBLigne_A][NBColonne_B];



void *produit(void *coord[]);



int main(int argc,char *argv[]){
	int th1,th2,rc,i,j,m;
	int threadPerLine,threadPerColumn;
	threadPerLine=sqrt(NBThread);
	threadPerColumn=NBThread/threadPerLine;
	pthread_t threads[threadPerLine][threadPerColumn];
	double TempsDeb,TempsFin,TempsSeq,TempsParallel;
	
	srand(time(NULL));
	
	
	
	//initialisation des matrices
	for(i=0;i<NBLigne_A;i++){
		for(j=0;j<NBColonne_A;j++){
			A[i][j]=rand()%NBLigne_A;
		}
	}
	for(i=0;i<NBLigne_B;i++){
		for(j=0;j<NBColonne_B;j++){
			B[i][j]=rand()%NBLigne_B;
		}
	}
	for(i=0;i<NBLigne_A;i++){
		for(j=0;j<NBColonne_B;j++){
			
			resultp[i][j]=0;
			results[i][j]=0;
		}
	}
	
	

	//debut de calcule sequantielle
	TempsDeb=clock();
	for(m=0;m<NBLigne_A;m++){
		for(i=0;i<NBColonne_B;i++){
			for( j=0;j<NBLigne_B;j++){
				results[m][i]=results[m][i]+A[m][j]*B[j][i];
			}
		}
	}
	TempsFin=clock();
	TempsSeq=(double)(TempsFin-TempsDeb)/CLOCKS_PER_SEC;
	
	
	
	
	//affichage du resultat sequentielle
	
	printf("resutat sequantielle : \n");
	for(i=0;i<NBLigne_A;i++){
		for(j=0;j<NBColonne_B;j++){
			printf("%d  ",results[i][j]);
		}
		printf("\n");
	}
	printf("le temps d execution =%f\n",TempsSeq);
    
	
	
	
	
	//debut de calcule paralelle
	
	TempsDeb=clock();
	for(th1=0;th1<threadPerLine;th1++){
		for(th2=0;th2<threadPerColumn;th2++){
			void *coord[]={th1,th2};
			rc=pthread_create(&threads[th1][th2],NULL,produit,coord);
			pthread_join(threads[th1][th2],NULL);
			if(rc){
				printf("err 0");
				exit(-1);
			}
		}
	}

	
	TempsFin=clock();
	TempsParallel=(double)(TempsFin-TempsDeb)/CLOCKS_PER_SEC;
	
	
	
	
	
	//affichage du resultat paralelle
	printf("resutat paralelle : \n");
	for(i=0;i<NBLigne_A;i++){
		for(j=0;j<NBColonne_B;j++){
			printf("%d  ",resultp[i][j]);
		}
		printf("\n");
	}
	printf("le temps d execution =%f\n",TempsParallel);

	
}



void *produit(void *coord[]){
	
	int ti=*((int*)(&coord[0]));
	int tj=*((int*)(&coord[1]));
	int i,j,z,startLine,endLine,startColumn,endColumn;
	int threadPerLine,threadPerColumn;
int nbt=NBThread;
threadPerLine=sqrt(nbt);
threadPerColumn=nbt/threadPerLine;
	startLine=ti*(NBLigne_A/threadPerLine);
	startColumn=tj*(NBColonne_B/threadPerColumn);
	if(ti!=threadPerLine-1){
	    endLine=startLine+(NBLigne_A/threadPerLine);
	}else{
	    endLine=NBLigne_A;
		
	}
	if(tj!=threadPerColumn-1){
	    endColumn=startColumn+(NBColonne_B/threadPerColumn);
	}else{
	    endColumn=NBColonne_B;
		
	}

    for(z=startLine;z<endLine;z++){
		for(i=startColumn;i<endColumn;i++){
			for( j=0;j<NBLigne_B;j++){
				resultp[z][i]=resultp[z][i]+A[z][j]*B[j][i];
			}
		}
	}
	
}


