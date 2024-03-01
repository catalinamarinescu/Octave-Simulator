#include <stdio.h>
#include <stdlib.h>
#define MODULO 10007

int **alloc_matrix(int m, int n)
{
	int **mat = (int **)malloc(m * sizeof(int *));
	if (mat == 0)
		return NULL;

	for (int i = 0; i < m; i++) {
		//alloc every line
		mat[i] = (int *)malloc(n * sizeof(int));
		if (mat[i] == 0) {
			return NULL;
			free(mat[i]);
		}
	}
	return mat;
}

void read_matrix(int **matrix, int m, int n)
{
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++)
			scanf("%d", &matrix[i][j]);
	}
}

void realloc_arr(int ****v, int **lines, int **col, int nr_matrix)
{
	*v = realloc(*v, nr_matrix * sizeof(int **));
	*lines = realloc(*lines, nr_matrix * sizeof(int));
	*col = realloc(*col, nr_matrix * sizeof(int));
}

void print_matrix(int nr_matrix, int ***v, int *lines, int *col)
{
	int index;
	scanf("%d", &index);
	if (index >= nr_matrix || index < 0) {
		printf("No matrix with the given index\n");
	} else if (index < nr_matrix) {
		for (int i = 0; i < lines[index]; i++) {
			for (int j = 0; j < col[index]; j++)
				//print the matrix at the given index
				printf("%d ", v[index][i][j]);

			printf("\n");
		}
	}
}

void resize_matrix(int ***v, int nr_matrix, int *lines, int *col)
{
	int l, c, index;
	int *indices_lines, *indices_col;

	scanf("%d", &index);
	scanf("%d", &l);

	// alloc memory to the line indices
	indices_lines = malloc(l * sizeof(int));
	for (int i = 0; i < l; i++)
		scanf("%d", &indices_lines[i]);

	scanf("%d", &c);

	//alloc memory to the column indices
	indices_col = malloc(c * sizeof(int));
	for (int i = 0; i < c; i++)
		scanf("%d", &indices_col[i]);

	if (index >= nr_matrix || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		//alloc memory to the new matrix
		int **tmp = (int **)malloc(l * sizeof(int *));
		for (int i = 0; i < l; i++)
			//alloc the lines
			tmp[i] = malloc(c * sizeof(int *));

		for (int i = 0; i < l; i++) {
			for (int j = 0; j < c; j++)
				tmp[i][j] = v[index][indices_lines[i]][indices_col[j]];
		}

		//freeing the lines of the original matrix
		for (int i = 0; i < lines[index]; i++)
			free(v[index][i]);

		//freeing the original matrix
		free(v[index]);

		//update the arrays with the new dimensions
		v[index] = tmp;
		lines[index] = l;
		col[index] = c;
	}

	free(indices_lines);
	free(indices_col);
}

int **mult(int *lines, int *col, int ***v, int index, int index1, int nr_matrix)
{
	int **mat = NULL;

	if (index >= nr_matrix || index1 >= nr_matrix || index < 0 || index1 < 0) {
		printf("No matrix with the given index\n");
	} else if (col[index] != lines[index1]) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		//alloc the result matrix
		mat = alloc_matrix(lines[index], col[index1]);
		int **first = v[index], **second = v[index1];
		for (int i = 0; i < lines[index]; i++) {
			for (int j = 0; j < col[index1]; j++) {
				mat[i][j] = 0;
				for (int k = 0; k < lines[index1]; k++) {
					mat[i][j] += first[i][k] * second[k][j];
					mat[i][j] %= MODULO;

					if (mat[i][j] < 0)
						mat[i][j] += MODULO;
				}
			}
		}
	}
	return mat;
}

void sort_matrix(int *lines, int *col, int nr_matrix, int ***v)
{
	//alloc the sum array
	int *s = (int *)malloc(nr_matrix * sizeof(int));
	for (int i = 0; i < nr_matrix; i++) {
		s[i] = 0;

		//calculate the sum of the elements
		for (int j = 0; j < lines[i]; j++) {
			for (int k = 0; k < col[i]; k++) {
				s[i] += v[i][j][k];
				s[i] %= MODULO;

				if (s[i] < 0)
					s[i] += MODULO;
			}
		}
	}

	int aux1, **aux2;

	for (int i = 0; i < nr_matrix - 1; i++) {
		for (int j = i + 1; j < nr_matrix; j++) {
			if (s[i] > s[j]) {
				//sort the sums
				aux1 = s[i];
				s[i] = s[j];
				s[j] = aux1;

				//sort the matrix array
				aux2 = v[i];
				v[i] = v[j];
				v[j] = aux2;

				//sort the lines array
				aux1 = lines[i];
				lines[i] = lines[j];
				lines[j] = aux1;

				//sort the columns array
				aux1 = col[i];
				col[i] = col[j];
				col[j] = aux1;
			}
		}
	}

	//free the sum array
	free(s);
}

void transpose_matrix(int *lines, int *col, int ***v, int nr_matrix)
{
	int aux, index;
	scanf("%d", &index);
	if (index < nr_matrix && index >= 0) {
		//alloc the transpose matrix
		int **matrix = (int **)malloc(col[index] * sizeof(int *));
		for (int i = 0; i < col[index]; i++)
			matrix[i] = (int *)malloc(lines[index] * sizeof(int));

		for (int i = 0; i < col[index]; i++) {
			for (int j = 0; j < lines[index]; j++)
				matrix[i][j] = v[index][j][i];
		}

		for (int i = 0; i < lines[index]; i++)
			free(v[index][i]);

		//free the original matrix
		free(v[index]);

		//switches the lines and columns
		aux = lines[index];
		lines[index] = col[index];
		col[index] = aux;

		//update the matrix in the matrix array
		v[index] = matrix;
	} else {
		printf("No matrix with the given index\n");
	}
}

void free_matrix(int ****v, int **lines, int **col, int index, int *nr_matrix)
{
	if (index >= *nr_matrix || index < 0) {
		printf("No matrix with the given index\n");
	} else {
		//freeing the lines
	    for (int j = 0; j < (*lines)[index]; j++)
			free((*v)[index][j]);

		//free the original matrix
		free((*v)[index]);

	    for (int i = index; i < (*nr_matrix) - 1; i++) {
		    (*v)[i] = (*v)[i + 1];
		    (*lines)[i] = (*lines)[i + 1];
		    (*col)[i] = (*col)[i + 1];
	    }

		//decrease the total number of matrices
		(*nr_matrix)--;
	}
}

void matrix_release(int ***v, int *lines, int *col, int nr_matrix)
{
	//freeing all the lines
	for (int i = 0; i < nr_matrix; i++) {
		for (int j = 0; j < lines[i]; j++)
			free(v[i][j]);

		free(v[i]);
	}

	//freeing all the arrays
	free(v);
	free(lines);
	free(col);
}

void multiply(int **res, int **copy, int **aux, int l, int c)
{
	for (int i = 0; i < l; i++) {
		for (int j = 0; j < c; j++) {
			res[i][j] = 0;
			for (int k = 0; k < l; k++) {
				res[i][j] += aux[i][k] * copy[k][j];
				res[i][j] %= MODULO;

				if (res[i][j] < 0)
					res[i][j] += MODULO;
			}
		}
	}

	for (int i = 0; i < l; i++) {
		for (int j = 0; j < c; j++)
			aux[i][j] = res[i][j];
	}
}

void rise(int ***v, int *lines, int *col, int nr, long long power, int index)
{
	if (index >= nr || index < 0) {
		printf("No matrix with the given index\n");
	} else if (power < 0) {
		printf("Power should be positive\n");
	} else if (col[index] != lines[index]) {
		printf("Cannot perform matrix multiplication\n");
	} else {
		//alloc all the matrices that will be used
		int **result = alloc_matrix(lines[index], col[index]);
		int **copy = alloc_matrix(lines[index], col[index]);
		int **tmp = alloc_matrix(lines[index], col[index]);
		int **aux = alloc_matrix(lines[index], col[index]);

		//create the aux matrix
		for (int i = 0; i < lines[index]; i++) {
			for (int j = 0; j < col[index]; j++) {
				if (i == j)
					aux[i][j] = 1;
				else
					aux[i][j] = 0;
			}
		}

		//copy the original matrix
		for (int i = 0; i < lines[index]; i++) {
			for (int j = 0; j < col[index]; j++)
				copy[i][j] = v[index][i][j];
		}

		//rising the matrix to the given power
		while (power > 0) {
			if (power % 2 != 0) {
				multiply(result, copy, aux, lines[index], col[index]);
				power--;
			}

			multiply(tmp, copy, copy, lines[index], col[index]);
			power /= 2;
		}

		//freeing all the matrices that were allocated
		for (int i = 0; i < lines[index]; i++) {
			free(copy[i]);
			free(tmp[i]);
			free(aux[i]);
		}
		free(copy);
		free(aux);
		free(tmp);

		//freeing the original matrix
		for (int j = 0; j < lines[index]; j++)
			free(v[index][j]);

		free(v[index]);

		//update the matrix array with the result
		v[index] = result;
	}
}

int main(void)
{
	int nr_matrix = 0, index, index1;
	long long power;
	char start = 'A';
	int ***v, **matrix, *lines, *col;

	lines = malloc(sizeof(int));
	col = malloc(sizeof(int));
	v = malloc(sizeof(int **));

	do {
		scanf("%c", &start);

		switch (start) {
		case 'L':
			realloc_arr(&v, &lines, &col, nr_matrix + 1);
			int m, n;
			scanf("%d", &m);
			scanf("%d", &n);

			v[nr_matrix] = alloc_matrix(m, n);
			read_matrix(v[nr_matrix], m, n);

			lines[nr_matrix] = m;
			col[nr_matrix] = n;
			nr_matrix++;

			break;
		case 'D':
			scanf("%d", &index);
			if (index >= nr_matrix)
				printf("No matrix with the given index\n");
			else
				printf("%d %d\n", lines[index], col[index]);

			break;
		case 'P':
			print_matrix(nr_matrix, v, lines, col);
			break;
		case 'C':
			resize_matrix(v, nr_matrix, lines, col);
			break;
		case 'M':
			scanf("%d %d", &index, &index1);
			matrix = mult(lines, col, v, index, index1, nr_matrix);

			if (matrix) {
				realloc_arr(&v, &lines, &col, nr_matrix + 1);
				//update the arrays with the new values
				v[nr_matrix] = matrix;
				lines[nr_matrix] = lines[index];
				col[nr_matrix] = col[index1];
				nr_matrix++;
			}
			break;
		case 'O':
			sort_matrix(lines, col, nr_matrix, v);
			break;
		case 'T':
			transpose_matrix(lines, col, v, nr_matrix);
			break;
		case 'R':
			scanf("%d %lld", &index, &power);
			rise(v, lines, col, nr_matrix, power, index);
			break;
		case 'F':
			scanf("%d", &index);
			free_matrix(&v, &lines, &col, index, &nr_matrix);
			break;
		case 'Q':
			matrix_release(v, lines, col, nr_matrix);
			break;
		case '\n':
			break;
		default:
			printf("Unrecognized command\n");
			break;
		}
	} while (start != 'Q');
	return 0;
}
