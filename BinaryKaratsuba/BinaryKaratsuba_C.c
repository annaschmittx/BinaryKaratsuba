//karatsuba para binários passados em string
//2024/2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//inverter a string
void strrev(char* str) {
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; ++i) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

//igualar o tamanho das strings
void igualar_tamanho(const char* str1, const char* str2, char* res1, char* res2) {
    size_t len1 = strlen(str1);
    size_t len2 = strlen(str2);
    size_t new_len = (len1 > len2) ? len1 : len2;

    memset(res1, '0', new_len - len1);
    strcpy(res1 + new_len - len1, str1);
    res1[new_len] = '\0';

    memset(res2, '0', new_len - len2);
    strcpy(res2 + new_len - len2, str2);
    res2[new_len] = '\0';
}

//soma os dois binários como strings
void somar_bits(const char* primeiro, const char* segundo, char* resultado) {
    size_t comprimento = strlen(primeiro);
    int carry = 0;

    for (size_t i = comprimento; i > 0; --i) {
        int bit_primeiro = primeiro[i - 1] - '0';
        int bit_segundo = segundo[i - 1] - '0';
        int soma = (bit_primeiro ^ bit_segundo ^ carry);
        resultado[i - 1] = soma + '0';
        carry = (bit_primeiro & bit_segundo) | (bit_segundo & carry) | (bit_primeiro & carry);
    }

    if (carry) {
        resultado[0] = '1';
        resultado[comprimento + 1] = '\0';
    } else {
        memmove(resultado, resultado + 1, comprimento);
        resultado[comprimento - 1] = '\0';
    }
}

//binário para decimal
unsigned int binario_para_decimal(const char* binario) {
    unsigned int decimal = 0;
    while (*binario) {
        decimal = (decimal << 1) | (*binario++ - '0');
    }
    return decimal;
}

//decimal para binario
void decimal_para_binario(unsigned int decimal, char* binario) {
    int i = 0;
    while (decimal > 0) {
        binario[i++] = (decimal & 1) ? '1' : '0';
        decimal >>= 1;
    }
    binario[i] = '\0';
    strrev(binario);
}

//karatsuba
void karatsuba(const char* X, const char* Y, char* resultado_binario, unsigned int* resultado_decimal) {
    size_t n = strlen(X) > strlen(Y) ? strlen(X) : strlen(Y);
    char X_padded[n + 1];
    char Y_padded[n + 1];
    igualar_tamanho(X, Y, X_padded, Y_padded);

    if (n == 0) {
        resultado_binario[0] = '0';
        resultado_binario[1] = '\0';
        *resultado_decimal = 0;
        return;
    }
    if (n == 1) {
        *resultado_decimal = (X_padded[0] - '0') * (Y_padded[0] - '0');
        decimal_para_binario(*resultado_decimal, resultado_binario);
        return;
    }

    size_t fh = n / 2; // 1/2
    size_t sh = n - fh; // 2/2

    char Xl[fh + 1];
    char Xr[sh + 1];
    char Yl[fh + 1];
    char Yr[sh + 1];
    strncpy(Xl, X_padded, fh);
    Xl[fh] = '\0';
    strncpy(Xr, X_padded + fh, sh);
    Xr[sh] = '\0';
    strncpy(Yl, Y_padded, fh);
    Yl[fh] = '\0';
    strncpy(Yr, Y_padded + fh, sh);
    Yr[sh] = '\0';

    char P1_bin[1000];
    char P2_bin[1000];
    char P3_bin[1000];
    unsigned int P1_dec, P2_dec, P3_dec;

    unsigned int Xl_dec = binario_para_decimal(Xl);
    unsigned int Xr_dec = binario_para_decimal(Xr);
    unsigned int Yl_dec = binario_para_decimal(Yl);
    unsigned int Yr_dec = binario_para_decimal(Yr);

    char sum_X_bin[fh + 1];
    char sum_Y_bin[fh + 1];
    decimal_para_binario(Xl_dec + Xr_dec, sum_X_bin);
    decimal_para_binario(Yl_dec + Yr_dec, sum_Y_bin);

    char P1_result[1000];
    char P2_result[1000];
    char P3_result[1000];
    unsigned int P1, P2, P3;

    karatsuba(Xl, Yl, P1_result, &P1);
    karatsuba(Xr, Yr, P2_result, &P2);
    karatsuba(sum_X_bin, sum_Y_bin, P3_result, &P3);

    P1_dec = binario_para_decimal(P1_result);
    P2_dec = binario_para_decimal(P2_result);
    P3_dec = binario_para_decimal(P3_result);

    *resultado_decimal = P1_dec * (1 << (2 * sh)) +
                         (P3_dec - P1_dec - P2_dec) * (1 << sh) +
                         P2_dec;
    decimal_para_binario(*resultado_decimal, resultado_binario);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <valor1> <valor2>\n", argv[0]);
        return 1;
    }

    const char* valor1 = argv[1];
    const char* valor2 = argv[2];

    char resultado_binario[1000];
    unsigned int resultado_decimal;

    karatsuba(valor1, valor2, resultado_binario, &resultado_decimal);

    printf("valor 1: %s\n", valor1);
    printf("valor 2: %s\n", valor2);
    printf("saída: %s (binário), %u (decimal)\n", resultado_binario, resultado_decimal);

    return 0;
}
