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
	for(int i=5;i<=50;i+=5)
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

	double *a;
    double sum;
	int i, j, n, m, ths, chunk, q = 0;

	setlocale(LC_ALL, "Rus");

	printf("Введите количество строк: ");
	scanf_s("%d", &n);
	printf("Введите количество столбцов: ");
	scanf_s("%d", &m);
	/*printf("Введите количество потоков: ");
	scanf_s("%d", &ths);*/

	printf("Коэффициент умножения: ");
	scanf_s("%d", &q);

	/*a = (double*)malloc(n * m * sizeof(double));

	for (i = 0; i < n; i++)
	{
		for (j = 0; j < m; j++)
		{
			a[i * m + j] = random(0, 100);
		}
	}

	saveArr(a, n*m, "C:\\100.bin");*/
	//saveArr(a, n*m, "C:\\1000.bin");
	//saveArr(a, n*m, "C:\\2000.bin");
	//saveArr(a, n*m, "C:\\4000.bin");
	//saveArr(a, n*m, "C:\\6000.bin");
	//saveArr(a, n*m, "C:\\8000.bin");
	//saveArr(a, n*m, "C:\\10000.bin");

	ofstream os("C:\\lab11_100.csv", ios::app);
	//saveCSVthreads("C:\\lab11_100.csv");
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

		sum = 0;
		chunk = n * 0.1;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start_p);

        #pragma omp parallel for private(i,j) reduction(+:sum) schedule(dynamic,chunk) num_threads(ths)
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < m; j++)
			{
				a[i * m + j] *= q;
				sum += a[i * m + j];
			}
		}

		QueryPerformanceCounter(&finish_p);

		printf("Сумма: %f\n", sum);

		double time = (finish_p.LowPart - start_p.LowPart) * 1000.0f / frequency.LowPart;
		printf("Время выполения = %f\n", time);

		os << time << ";";
		free(a);
	}
	os << "\n";
	os.close();
}