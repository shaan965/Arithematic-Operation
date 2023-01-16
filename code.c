/* Purpose: Arithematic Operation in String Representation
 * Author : CMPUT 201
 * Date   : 16th October, 2021
 */

#include "lab08.h"

int main(int argc, char *argv[]) {
	if (argc != 3)
		return 0;
	check_validate(argv[1], __LINE__, __FILE__);
	check_validate(argv[2], __LINE__, __FILE__);
    	check_add(argv[1], argv[2], __LINE__, __FILE__);
    	check_gt(argv[1], argv[2], __LINE__, __FILE__);
    	check_eq(argv[1], argv[2], __LINE__, __FILE__);
    	check_diff(argv[1], argv[2], __LINE__, __FILE__);
    	check_multiply(argv[1], argv[2], __LINE__, __FILE__);
    	check_divide(argv[1], argv[2], __LINE__, __FILE__);
    	check_modulo(argv[1], argv[2], __LINE__, __FILE__);
	return 0;
}


bool validate(const char *p) {
	// Case 1: Negative Number
	if (p[0] == '-')
		return false;

	// Case 2: Leading Zero Situation
	if (p[0] == '0' && p[1] == '0')
		return false;

	// Case 3: Check if there is non digit character in the string
	for (int i = 0; i < strlen(p); i++) {
		if (p[i] < 48 || p[i] > 57) 
			return false;
	}

	return true;
}


char *add(const char *m, const char *n) {
	// Find out the maximum capacity for sum result
	int length_result = (strlen(m) > strlen(n)) ? strlen(m) + 1 : strlen(n) + 1;

	int sum, carry = 0;
	int i = strlen(m) - 1;		// index of m, starting from the last (far-right)
	int j = strlen(n) - 1;		// index of n, starting from the last (far-right)
	int k = length_result - 1;	// index of result, starting from the last index
	char *result = calloc(length_result, sizeof(char));

	while (i >= 0 && j >= 0) {
		sum = m[i--] - '0' + n[j--] - '0' + carry;
		carry = sum / 10;
		result[k--] = (sum % 10) + '0';
	}

	// Handle the remaining digits in string respresentation m, (n is exhausted)
	while (i >= 0) {
		sum = m[i--] - '0' + carry;
		carry = sum / 10;
		result[k--] = (sum % 10) + '0';
	}

	// Handle the remaining digits in string respresentation n, (m is exhausted)
	while (j >= 0) {
		sum = n[j--] - '0' + carry;
		carry = sum / 10;
		result[k--] = (sum % 10) + '0';
	}

	// If there is remaining carry
	if (carry == 1)
		result[k--] = '1';

	// Handle leading zero in the result array	
	if (result[0] == 0) {
		for (int a = 1; a < length_result; a++) 
			result[a - 1] = result[a];

		result[length_result - 1] = '\0';
	}

	return result;
}


bool gt(const char *m, const char *n) {
	// Compare the length of string m and n
	if (strlen(m) > strlen(n)) 
		return true;
	else if (strlen(m) < strlen(n))
		return false;

	// Compare each corresponding index digit
	for (int i = 0 ; i < strlen(m); i++) {
		if (m[i] > n[i])
			return true;
		else if (m[i] < n[i])
			return false;
	}
	
	return false; 	// Indicate m == n
}


bool eq(const char *m, const char *n) {
	// Compare the length of string m and n
	if (strlen(m) != strlen(n))
		return false;

	// Compare each corresponding index digit
	for (int i = 0; i < strlen(m); i++) {
		if (m[i] != n[i])
			return false;
	}

	return true;
}


char *diff(const char *m, const char *n) {
	// Special Case #1: m - n == 0
	if (eq(m , n)) {
		char *result = calloc(2, sizeof(char));
		result[0] = '0';

		return result;
	}


	int length_result;
	const char *larger, *smaller;

	// Decide which is a larger string respresentation -> *larger
	//        which is a smaller string representation -> *smaller
	if (gt(m , n)) {
		larger = m;
		smaller = n;
		length_result = strlen(m);
	} else {
		larger = n;
		smaller = m;
		length_result = strlen(n);
	}	


	int borrow = 0;
	int i = strlen(larger) - 1;	// starting index of larger string representation
	int j = strlen(smaller) - 1;	// starting index of smaller string representation
	int k = length_result - 1;	// starting index in the result array
	char *result = calloc(length_result, sizeof(char));

	while (j >= 0) {
		if (larger[i] - borrow >= smaller[j]) {
			result[k--] = larger[i--] - borrow - smaller[j--] + '0';
			borrow = 0;
		} else {
			result[k--] = larger[i--] - borrow + 10 - smaller[j--] + '0';
			borrow = 1;
		}
	}

	while (i >= 0) {
		if (larger[i] - borrow >= '0') {
			result[k--] = larger[i--] - borrow;
			borrow = 0;
		} else {
			result[k--] = larger[i--] + 10 - borrow;
			borrow = 1;
		}
	}

	// Handle leading-zero in the string representation
	while (result[0] == '0') {
		for (int i = 1; i < length_result; i++) 
			result[i - 1] = result[i];

		result[--length_result] = '\0';
	}


	return result;
}


char *multiply(const char *m, const char *n) {
	// Special Case #1: One of the string representation is zero	
	if (!strcmp(m, "0") || !strcmp(n, "0")) {
		char *result = calloc(2, sizeof(char));
		result[0] = '0';

		return result;
	}


	int *int_result = calloc(strlen(m) + strlen(n), sizeof(int));

	// Calculate the multiplication result in each index position, starting from right to left ( m and n )
	for (int i = strlen(m) - 1; i >= 0; i--)
		for (int j = strlen(n) - 1; j >= 0; j--)
			int_result[i + j + 1] += ((m[i] - '0') * (n[j] - '0'));

	// Find the digits and the carry
	for (int i = strlen(m) + strlen(n) - 1; i > 0; i--) {
		int_result[i - 1] += int_result[i] / 10;
	       	int_result[i] = int_result[i] % 10;
	}

	int k = 0;		// Indicate the index position in char_result
	char *char_result;	// Contains the string representation of multiplication result

	if (int_result[0] == 0) {
		char_result = calloc(strlen(m) + strlen(n) - 1, sizeof(char));
		for (int i = 1; i < strlen(m) + strlen(n); i++)
			char_result[k++] = int_result[i] + '0';
	} else {
		char_result = calloc(strlen(m) + strlen(n), sizeof(char));
		for (int i = 0; i < strlen(m) + strlen(n); i++)
			char_result[k++] = int_result[i] + '0';
	}

	return char_result;
}


char *divide(const char *m, const char *n) {
	// Special Case #1: n == 0
	if (!strcmp(n, "0"))
		return NULL;

	// Special Case #2: if n > m
	if (gt(n , m)) {
		char *result = calloc(2, sizeof(char));
		result[0] = '0';

		return result;
	}

	char *new_n;
	char *new_m = calloc(strlen(m) + 1, sizeof(char));
	int length = (strlen(m) >= strlen(n) ? strlen(m) - strlen(n) + 2 : 2);
	char *result = calloc(length, sizeof(char));

	strcpy(new_m, m);

	for (int i = length - 2; i >= 0; i--) {
		// Find the new representation of new_n by adding '0'
		new_n = calloc(strlen(n) + i + 1, sizeof(char));
	    	strcpy(new_n, n);

		for (int j = strlen(n); j < strlen(n) + i; j++)
			new_n[j] = '0';

		// Record the number of execution of difference
		int digit = 0;
		while (!gt(new_n, new_m)) {
			digit++;
			char *remaining = diff(new_m, new_n);
			free(new_m);
			new_m = remaining;
		}

		result[length - 2 - i] = digit + '0';
		free(new_n);
	}
	free(new_m);

	// Handle Leading Zero
	while (result[0] == '0') {
		for (int r = 0; r < length; r++)
			result[r] = result[r+1];
		length--;
		result = realloc(result, length + 1);
	}

	return result;
}


char *modulo(const char *m, const char *n) {
	if (!strcmp(n, "0")) {
        	return NULL;
	}

	char *quotient = divide(m, n);
	char *product = multiply(quotient, n);
	char *result = diff(m, product);
	free(quotient);
	free(product);

	return result;
}

