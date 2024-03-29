#include "pch.h"

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <windows.h>

using namespace std;

void saveArr(const double* pdata, size_t nm, const char* file_path)
{
	ofstream os(file_path, ios::binary | ios::out);
	if (!os.is_open())
		return;
	os.write((const char*)(pdata), nm * sizeof(double));
	os.close();
}

void loadArr(double* pdata, size_t nm, const char* file_path)
{
	ifstream is(file_path, ios::binary | ios::in);
	if (!is.is_open())
		return;
	is.read((char*)(pdata), nm * sizeof(double));
	is.close();
}

void saveCSVthreads(const char *file_path)
{
	ofstream os(file_path, ios::app);
	if (!os.is_open())
		return;
	for (int i = 5; i <= 50; i += 5)
		os << i << ";";
	os << "\n";
	os.close();
}

double random(double min, double max)
{
	//return ((min + (double)(rand()) / RAND_MAX * max) * 100) / 100;
	return min + (double)(rand()) / RAND_MAX * max;
}

int main()
{
	LARGE_INTEGER frequency, start_p, finish_p;

	double *a, *b;

	int i, j, n, m, ths, chunk, count;

	setlocale(LC_ALL, "Rus");

	printf("Введите количество строк: ");
	scanf_s("%d", &n);
	printf("Введите количество столбцов: ");
	scanf_s("%d", &m);

	/*a = (double*)malloc(n * m * sizeof(double));

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			a[i * m + j] = random(0, 100);
		}
	}*/

	//saveArr(a, n*m, "C:\\100.bin");
	//saveArr(a, n*m, "C:\\1000.bin");
	//saveArr(a, n*m, "C:\\2000.bin");
	//saveArr(a, n*m, "C:\\4000.bin");
	//saveArr(a, n*m, "C:\\6000.bin");
	//saveArr(a, n*m, "C:\\8000.bin");
	//saveArr(a, n*m, "C:\\10000.bin");

	ofstream os("C:\\lab12_100.csv", ios::app);
	//saveCSVthreads("C:\\lab12_100.csv");
	//os << "\n\n";

	for(ths = 5; ths <= 50; ths += 5) {
		a = (double*)malloc(n * m * sizeof(double));
		//loadArr(a, n*m, "C:\\100.bin");
		//loadArr(a, n*m, "C:\\1000.bin");
		//loadArr(a, n*m, "C:\\2000.bin");
		//loadArr(a, n*m, "C:\\4000.bin");
		//loadArr(a, n*m, "C:\\6000.bin");
		//loadArr(a, n*m, "C:\\8000.bin");
		loadArr(a, n*m, "C:\\10000.bin");

		count = 0;
		chunk = n * 0.1;
		b = (double*)calloc(n * m, sizeof(double));

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start_p);

        #pragma omp parallel for private(i,j) reduction(+:count) schedule(dynamic,chunk) num_threads(ths)
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < m; j++)
			{
				if (a[i*m + j] != 0)
				{
					b[i*m + j] = a[i*m + j];
					count++;
				}
			}
		}

		QueryPerformanceCounter(&finish_p);
		j = 0;
		for (i = 0; i < n*m; i++)
		{
			if (b[i] != 0)
			{
				b[j] = b[i];
				j++;
			}
		}
		b = (double*)realloc(b, count * sizeof(double));
		printf("Количество эелементов: %d\n", count);

		double time = (finish_p.LowPart - start_p.LowPart) * 1000.0f / frequency.LowPart;
		printf("Время выполения = %f\n", time);

		os << time << ";";

		free(a);
		free(b);
	}
	os << "\n";
	os.close();
}