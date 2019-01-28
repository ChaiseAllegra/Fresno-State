
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>
#include <fstream>

using namespace std;

void mergesort(int *A, int *temp, int left, int right) {
	if (left == right) return; // List has one record
	int mid = (left + right) / 2; // Select midpoint
	mergesort(A, temp, left, mid); // Mergesort first half
	mergesort(A, temp, (mid + 1), right); // Mergesort second half
	for (int i = left; i <= right; i++)  // Copy subarray to temp
	{
		temp[i] = A[i];
		assignsMerge++;
	}
	// Do the merge operation back to A
	int i1 = left;
	int i2 = mid + 1;
	for (int curr = left; curr <= right; curr++) {
		/* 1 */
		compsMerge++;
		if (i1 == mid + 1)
		{
			A[curr] = temp[i2++];
			assignsMerge++;
		}
		else if (i2 > right)   // Right sublist exhausted
		{
			A[curr] = temp[i1++];
			assignsMerge++;
		}
		else if (temp[i1] <= temp[i2])    // Get smaller value
		{
			A[curr] = temp[i1++];
			assignsMerge++;
		}
		else
			A[curr] = temp[i2++];
	}
}

void Merge(int *a, int low, int high, int mid)
{
	// We have low to mid and mid+1 to high already sorted.
	int i, j, k, temp[high-low+1];
	i = low;
	k = 0;
	j = mid + 1;
 
	// Merge the two parts into temp[].
	while (i <= mid && j <= high)
	{
		if (a[i] < a[j])
		{
			temp[k] = a[i];
			k++;
			i++;
		}
		else
		{
			temp[k] = a[j];
			k++;
			j++;
		}
	}
 
	// Insert all the remaining values from i to mid into temp[].
	while (i <= mid)
	{
		temp[k] = a[i];
		k++;
		i++;
	}
 
	// Insert all the remaining values from j to high into temp[].
	while (j <= high)
	{
		temp[k] = a[j];
		k++;
		j++;
	}
 
 
	// Assign sorted data stored in temp[] to a[].
	for (i = low; i <= high; i++)
	{
		a[i] = temp[i-low];
	}
}
 