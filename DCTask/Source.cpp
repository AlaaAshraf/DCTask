#include <iostream>
#include <omp.h>
#include <cmath>
#include <algorithm>
using namespace std;
double fRand(double fMin, double fMax)
{
	double f = (double)rand() / RAND_MAX;
	return fMin + f * (fMax - fMin);
}
int main()
{
	const int n = 4;
	double A[n][n], b[n], x[n], x2[n];
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
	omp_set_num_threads(min(8,n));
	while(true)
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
		if (diff < 0.000001)
			break;
	}

	for (int i = 0; i < n; i++)
		cout << x[i] << endl;
	
}