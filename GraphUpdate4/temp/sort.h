#ifndef SORT_H
#define SORT_H
#include <vector>
#include <climits>

//QuichSort(A, start, size)
template <class T>
void quickSort(T A[], int start, int size)
{
	if(size <= 1)
		return;
	//std::cout<<start<<" "<<A[start]<<std::endl;

	T pivot = A[start];
	int left = start+1;
	int right = start+size-1;

	//std::cout<<"Pivoit: "<<pivot<<" "<<right<<std::endl;

	while(left <= right)
	{
		while(left <= right && pivot > A[left])
			left++;
		while(left <= right && pivot <= A[right])
			right--;

		if(left < right)
		{
			T temp;
			temp = A[left];
			A[left] = A[right];
			A[right] = temp;
		}
	}

	A[start] = A[right];
	A[right] = pivot;

	std::cout<<"Size: "<<size<<" Pivoit: "<<pivot<<" "<<right<<std::endl;

	quickSort(A, start, right-start);
	quickSort(A, right+1, start+size-1-right);
}

//Max-Heapify(A, i, n)
//    l = Left(i)
//    r = Right(i)
//    if l < n and A[l] > A[i]
//         largest = l
//    else
//         largest = i
//    if r <= n and A[r] > A[largest]
//         largest = r
//    if largest != i
//         exchange A[i] with A[largest]
//         Max-Heapify(A, largest, n)
template <class T>
void maxHeapify(T A[], int i, int n)
{
	int l = 2*i;
	int r = 2*i+1;
	if(l > n) return;//check if left node exist

	int largest = i;
	if(l < n && A[l] > A[i])
		largest = l;
	if(r <= n && A[r] > A[largest])
		largest = r;
	if(largest != i)
	{
		T temp;
		temp = A[i];
		A[i] = A[largest];
		A[largest] = temp;
		maxHeapify(A, largest, n);
	}
}

//Build-Max-Heap(A, n)
//    for i = floor(n/2) downto 1
//         Max-Heapify(A, i, n)
template <class T>
void buildHeap(T A[], int n)
{
	for(int i = n/2; i >= 1; --i)
	{
		maxHeapify(A, i, n);
	}
}

//HeapSort(A, n)
//    Build-Max-Heap(A, n)
//    for i = n downto 2
//         exchange A[1] with A[i]
//         Max-Heapify(A, 1, i-1)
template <class T>
void heapSort(T A[], int n)
{
	buildHeap(A, n);

	for(int i = n; i > 1; --i)
	{
		T temp;
		temp = A[1];
		A[1] = A[i];
		A[i] = temp;
		maxHeapify(A, 1, i-1);
	}
}

//MERGE(A, p, q, r)
//    n1 = q-p+1
//    n2 = r-q
//    Let L[1..n1+1] and R[1..n2+1] be new arrays
//    for i = 1 to n1
//         L[i] = A[p+i-1]
//    for j = 1 to n2
//         R[j] = A[q+j]
//    L[n1+1] = inf
//    R[n2+1] = inf
//    i = 1
//    j = 1
//    for k = p to r
//        if L[i] <= R[j]
//            A[k] = L[i]
//            i = i + 1
//        else A[k] = R[j]
//            j = j+1
template <class T>
void merge(T A[], int p, int q, int r)
{
	int n1 = q - p +1;
	int n2 = r - q;
	T L[n1], R[n2];
	for(int i = 0; i < n1; ++i)
		L[i] = A[p+i];
	for(int i = 0; i < n2; ++i)
		R[i] = A[q+i+1];
	int i = 0;
	int j = 0;
	for(int k = p; k <= r; ++k)
	{
		if(i < n1 && j < n2)
		{
			if(L[i] <= R[j])
			{
				A[k] = L[i];
				++i;
			}
			else
			{
				A[k] = R[j];
				++j;
			}
		}
		else if(i < n1 && j >= n2)
		{
			A[k] = L[i];
			++i;
		}
		else if(i >= n1 && j < n2)
		{
			A[k] = R[j];
			++j;
		}

		/*if(L[i] <= R[j] || j >= n2)
		{
			A[k] = L[i];
			++i;
		}
		else
		{
			A[k] = R[j];
			++j;
		}*/
	}
}

template <class T>
void merge(std::vector<T> &v, int p, int q, int r)
{
	int n1 = q - p +1;
	int n2 = r - q;
	T L[n1+1], R[n2+1];
	for(int i = 0; i < n1; ++i)
		L[i] = v[p+i];
	for(int i = 0; i < n2; ++i)
		R[i] = v[q+i+1];
	int i = 0;
	int j = 0;
	for(int k = p; k <= r; ++k)
	{
		if(i < n1 && j < n2)
		{
			if(L[i] <= R[j])
			{
				v[k] = L[i];
				++i;
			}
			else
			{
				v[k] = R[j];
				++j;
			}
		}
		else if(i < n1 && j >= n2)
		{
			v[k] = L[i];
			++i;
		}
		else if(i >= n1 && j < n2)
		{
			v[k] = R[j];
			++j;
		}

		/*if(L[i] <= R[j] || j >= n2)
		{
			v[k] = L[i];
			++i;
		}
		else
		{
			v[k] = R[j];
			++j;
		}*/
	}
}

//MERGE-SORT(A, p, r)
//if p < r
//     q = floor((p+r)/2)
//     MERGE-SORT(A, p, q)
//     MERGE-SORT(A, q+1, r)
//     MERGE(A, p, q, r)
template <class T>
void mergeSort(T A[], int p, int r)
{
	if(p < r)
	{
		int q = (p+r)/2;
		mergeSort(A, p, q);
		mergeSort(A, q+1, r);
		merge(A, p, q, r);
	}
}

template <class T>
void mergeSort(std::vector<T> &v, int p, int r)
{
	if(p < r)
	{
		int q = (p+r)/2;
		mergeSort(v, p, q);
		mergeSort(v, q+1, r);
		merge(v, p, q, r);
	}
}

//INSERTION-SORT(A, n)
//for j = 1 to A.length-1
//    key = A[j];
//    i = j-1;
//    while i >= 0 and A[i] > key
//          A[i+1] = A[i];
//          --i;
//    A[i+1] = key;
template <class T>
void insertionSort(T A[], int n)
{
	for(int j = 1; j < n; ++j)
	{
		T key = A[j];
		int i = j -1;
		while(i >= 0 && A[i] > key)
		{
			A[i+1] = A[i];
			--i;
		}
		A[i+1] = key;
	}
}

template <class T>
void insertionSort (std::vector<T>& v)
{
  for (int i = 1; i < v.size(); 
       ++i)
    {
     T key = v[i];
     int j = i - 1;
     while (j >= 0 && key < v[j]) {
       v[j+1] = v[j];
       --j;
     }
     // Insert the new value
     v[j+1] = key;
   }
}

#endif
