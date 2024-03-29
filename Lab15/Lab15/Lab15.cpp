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
	double sum;
	int i, j, n, m, ths, chunk, avg, c;

	setlocale(LC_ALL, "Rus");

	printf("Введите количество строк: ");
	scanf_s("%d", &n);
	printf("Введите количество столбцов: ");
	scanf_s("%d", &m);
	printf("Введите количество элементов: ");
	scanf_s("%d", &avg);

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

	ofstream os("C:\\lab15_100.csv", ios::app);
	//saveCSVthreads("C:\\lab15_100.csv");
	//os << "\n\n";

	for (ths = 5; ths <= 50; ths += 5) {
		a = (double*)malloc(n * m * sizeof(double));
		//loadArr(a, n*m, "C:\\100.bin");
		//loadArr(a, n*m, "C:\\1000.bin");
		//loadArr(a, n*m, "C:\\2000.bin");
		//loadArr(a, n*m, "C:\\4000.bin");
		//loadArr(a, n*m, "C:\\6000.bin");
		//loadArr(a, n*m, "C:\\8000.bin");
		loadArr(a, n*m, "C:\\10000.bin");

		c = 0;
		sum = 0;
		chunk = n * 0.1;
		b = (double*)calloc(ceil((n*m) / avg), sizeof(double));

		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start_p);

        #pragma omp parallel for private(i) default(shared) reduction(+:c) schedule(dynamic,chunk) num_threads(ths)
		for (i = 0; i < n*m; i++)
		{
			if (i%avg == avg - 1) {
				sum += a[i];
				b[(i + 1) / avg - 1] = sum;
				sum = 0;
				c++;
			}
			else
			{
				sum += a[i];
			}
		}

		QueryPerformanceCounter(&finish_p);

		printf("Количество средних значений: %d\n", c);
		double time = (finish_p.LowPart - start_p.LowPart) * 1000.0f / frequency.LowPart;
		printf("Время выполения = %f\n", time);

		os << time << ";";

		free(a);
		free(b);
	}
	os << "\n";
	os.close();
}