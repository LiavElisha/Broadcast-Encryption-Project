#include <stdio.h>

int f1(int clientIndex)
{
	switch(clientIndex)
	{
		case 0:
			return 1;
		case 1:
			return 2;
		case 2:
			return 1;
		case 3:
			return 3;
		case 4:
			return 4;
		case 5:
			return 0;
		case 6:
			return 2;
		case 7:
			return 0;
		case 8:
			return 2;
		case 9:
			return 1;
		default:
			printf("The scheme doesn't support more than 10 clients\n");
			return -1;
	}
}


int f2(int clientIndex)
{
	switch(clientIndex)
	{
		case 0:
			return 6;
		case 1:
			return 1;
		case 2:
			return 1;
		case 3:
			return 0;
		case 4:
			return 0;
		case 5:
			return 0;
		case 6:
			return 0;
		case 7:
			return 2;
		case 8:
			return 0;
		case 9:
			return 1;
		default:
			printf("The scheme doesn't support more than 10 clients\n");
			return -1;
	}
}


int f3(int clientIndex)
{
	switch(clientIndex)
	{
		case 0:
			return 6;
		case 1:
			return 6;
		case 2:
			return 2;
		case 3:
			return 1;
		case 4:
			return 2;
		case 5:
			return 6;
		case 6:
			return 4;
		case 7:
			return 1;
		case 8:
			return 1;
		case 9:
			return 2;
		default:
			printf("The scheme doesn't support more than 10 clients\n");
			return -1;
	}
}


int f4(int clientIndex)
{
	switch(clientIndex)
	{
		case 0:
			return 2;
		case 1:
			return 2;
		case 2:
			return 7;
		case 3:
			return 4;
		case 4:
			return 4;
		case 5:
			return 4;
		case 6:
			return 4;
		case 7:
			return 4;
		case 8:
			return 2;
		case 9:
			return 5;
		default:
			printf("The scheme doesn't support more than 10 clients\n");
			return -1;
	}
}


int f5(int clientIndex)
{
	switch(clientIndex)
	{
		case 0:
			return 3;
		case 1:
			return 6;
		case 2:
			return 6;
		case 3:
			return 6;
		case 4:
			return 3;
		case 5:
			return 4;
		case 6:
			return 3;
		case 7:
			return 3;
		case 8:
			return 2;
		case 9:
			return 6;
		default:
			printf("The scheme doesn't support more than 10 clients\n");
			return -1;
	}
}


int f6(int clientIndex)
{
	switch(clientIndex)
	{
		case 0:
			return 1;
		case 1:
			return 5;
		case 2:
			return 1;
		case 3:
			return 1;
		case 4:
			return 1;
		case 5:
			return 1;
		case 6:
			return 1;
		case 7:
			return 1;
		case 8:
			return 7;
		case 9:
			return 7;
		default:
			printf("The scheme doesn't support more than 10 clients\n");
			return -1;
	}
}


int f7(int clientIndex)
{
	switch(clientIndex)
	{
		case 0:
			return 0;
		case 1:
			return 5;
		case 2:
			return 5;
		case 3:
			return 5;
		case 4:
			return 7;
		case 5:
			return 7;
		case 6:
			return 7;
		case 7:
			return 5;
		case 8:
			return 5;
		case 9:
			return 7;
		default:
			printf("The scheme doesn't support more than 10 clients\n");
			return -1;
	}
}


int f(int functionIndex, int clientIndex)
{
	switch(functionIndex)
	{
	    case 1:
	        return f1(clientIndex);
	    case 2:
	        return f2(clientIndex);
	    case 3:
	        return f3(clientIndex);
	    case 4:
	        return f4(clientIndex);
	    case 5:
	        return f5(clientIndex);
	    case 6:
	        return f6(clientIndex);
	    case 7:
	        return f7(clientIndex);
		default:
			printf("The scheme doesn't support more than 10 clients\n");
			return -1;
	}
}

