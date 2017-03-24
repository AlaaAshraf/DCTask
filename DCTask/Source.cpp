#include <iostream>
#include <omp.h>
#include <cmath>
#include <algorithm>
#include <time.h> 
using namespace std;
#define EPSILON 0.0000001
double **A, *b, *x, *x2;
int n;

double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}
void parallel(int threadNum)
{
	int c = 0;
	omp_set_num_threads(threadNum);
	while (true)
	{
		int i;
#pragma omp parallel for private(i)

		for (i = 0; i < n; i++)
		{
			double alpha = 0;
			for (int j = 0; j < n; j++)
			{
				if (j != i)
				{
					alpha += A[i][j] * x[j];
				}
				//printf("I = %d, J = %d, Thread = %d \n", i, j, omp_get_thread_num());
			}
			x2[i] = (1 / A[i][i])*(b[i] - alpha);
		}
		double diff = 0;
		for (int i = 0; i < n; i++)
		{
			diff += abs(x[i] - x2[i]);
			x[i] = x2[i];
		}
		diff /= n;
		c++;
		if (diff < EPSILON||c>=333)
			break;
	}
}
void serial()
{
	int c = 0;
	while (true)
	{
		int i;
		for (i = 0; i < n; i++)
		{
			double alpha = 0;
			for (int j = 0; j < n; j++)
			{
				if (j != i)
				{
					alpha += A[i][j] * x[j];
				}
			}
			x2[i] = (1 / A[i][i])*(b[i] - alpha);
		}
		double diff = 0;
		for (int i = 0; i < n; i++)
		{
			diff += abs(x[i] - x2[i]);
			x[i] = x2[i];
		}
		diff /= n;
		c++;
		if (diff < EPSILON||c>=333)
			break;
	}
}
int main()
{
	cout << "Enter N: " << endl;
	cin >> n;

	b = new double[n], x = new double[n], x2 = new double[n];
	A = new double*[n];

	for (int i = 0; i < n; i++)
		A[i] = new double[n];
	cout << "Enter A:" << endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cin >> A[i][j];
		}
	}
	cout << "Enter b:" << endl;
	for (int i = 0; i < n; i++)
	{
		cin >> b[i];
		x[i] = fRand(-1000,1000);
	}
	
	char c;
	cout << "For parallel press P, for serial press S: ";
	cin >> c;
	clock_t t;
	if (c == 'S')
	{
		t = clock();
		serial();
		t = clock() - t;
	}
	else if(c=='P')
	{
		cout << "Enter number of threads: ";
		int threadNum;
		cin >> threadNum;
		t = clock();
		parallel(threadNum);
		t = clock() - t;
	}

	for (int i = 0; i < n; i++)
	{
		x[i] = round(x[i] * 1000) / 1000;
		cout << x[i] << endl;
	}
	cout << "Time elapsed = " << ((float)t) / CLOCKS_PER_SEC << " sec." << endl;

	delete[]b, delete[]x, delete[]x2;
	for (int i = 0; i < n; i++)
		delete[]A[i];
	delete[]A;
	
}