# include <stdio.h>
# include <stdlib.h>
int main()
{
	int i = 0;
	int j ;
	int cur, start, end;
		char stringtest[20];
	char filename[20] = "filetest0.txt";		//���� �̸� 

	FILE* stream = fopen(filename, "w");		//���� ���� 
	if(stream == NULL)							//���� Ȯ�� 
	{
		printf("file open error \n");
		exit(1);
	}

	while(i < 10)								//�ݺ� 
	{
		if(ftell(stream) > 100)				//�� ������ ũ�� �˻� 
		{
			printf("new file opened \n");	
			if(fclose(stream) == EOF)			//ũ�⸦ ����� ���ϴ��� 
			{
				printf("file close error \n");
				exit(1);
			}
			
			filename[8]++;						//���ο� ������ �������� �ܰ� 
			
			stream = fopen(filename, "w");		//�� ���� ���� 
			
			if(stream ==NULL)		//���� Ȯ�� 
			{
				printf("file open error \n");
				exit(1);
			}
			i++;			//���α׷� ���� ���� ----���� �� 
		}
		
		scanf("%d", &j);			//������ �Է� ------------------------
		
		fprintf(stream,"test : %d ", j);	//���� �Է� 
		
		end = ftell(stream);//--------------------------------------------------------
		
		printf("file end pointer loc : %d \n",end);//-----------------------
		
		if(j == -1)				//���α׷� �������� -------------------------------------
		{
			if(fclose(stream) == EOF)
			{
				printf("file close error \n");
				exit(1);
			}
			exit(0);
		}
	}
	

	exit(0);
}
