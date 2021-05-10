#include <stdio.h>
#include <math.h>
#include <string.h>

extern double f1(double x);
extern double f2(double x);
extern double f3(double x);
extern double ff1(double x);
extern double ff2(double x);
extern double ff3(double x);

int it;

double F(double (*f)(double),
         double (*g)(double),
         double x)
{
    return f(x) - g(x);                                                             // Для решения f(x) = g(x)
}

double root(double (*f)(double),
            double (*g)(double),
            double a,
            double b,
            double eps,
            double (*ff)(double),
            double (*gg)(double))
{
    double x, c, t;
    it = 0;
    if (F(f, g, a) * (F(f, g, (a + b)) / 2 - (F(f, g, a) + F(f, g, b)) / 2) < 0)    // Проверка знака F`(x) * F``(x)
    {
        x = a;
    }
    else
    {
        x = b;
        eps = -eps;
    }
    c = x - F(f, g, x) / F(ff, gg, x);
    while (F(f, g, c) * F(f, g, c + eps) > 0)
    {
        t = c;
        c = x - F(f, g, x) / F(ff, gg, x);                                          // Реализация метода касательных
        x = t;
        it++;
    }
    return c;
}

double integral(double (*f)(double),
                double a,
                double b,
                double eps)
{
    double In, I2n = 0, S1_2n = 0, S2_2n = 0, st, r = 1, p = 1 / 15, x;
    int n = 10, i = 0;
    while (r >= eps)                                                                // Проверка правила Рунге
    {
        st = (b - a) / (2 * n);                                                     // Деление [a, b] на 2n равных частей
        S1_2n = 0;
        S2_2n = 0;
        x = a;
        for (i = 1; i <= n; i++)                                                    // Вычисление сумм для формулы Симпсона
        {
            if (i != n)
            {
                S1_2n += f(x + st * (2 * i));
            }
            S2_2n += f(x + st * (2 * i - 1));
        }
        In = I2n;                                                                   // Сохранение вычесленного интеграла с предыдущей точностью
        I2n = (st / 3) * (f(a) + f(b) + 2 * S1_2n + 4 * S2_2n);                     // Вычисление интеграла по формуле Симпсона
        n *= 2;                                                                     // Увеличение точности
        r = p * fabs(In - I2n);                                                     // Вычисление точности по правилу Рунге
    }
    return I2n;
}

void testroot(void)                                                                 // Для тестирования функции root
{
    double (*p1)(double), (*p2)(double), (*pp1)(double), (*pp2)(double);
    int F1, F2;
    double a, b, eps;
    scanf("%d%d", &F1, &F2);                                                        // Считывает параметры для определения тестируемых функций
    switch (F1)                                                                     // В зависимости от параметров выбирает указатели на нужные функции
    {
        case 1:
            p1 = f1;
            pp1 = ff1;
        break;
        case 2:
            p1 = f2;
            pp1 = ff2;
        break;
        case 3:
            p1 = f3;
            pp1 = ff3;
    }
    switch (F2)
    {
        case 1:
            p2 = f1;
            pp2 = ff1;
        break;
        case 2:
            p2 = f2;
            pp2 = ff2;
        break;
        case 3:
            p2 = f3;
            pp2 = ff3;
    }
    scanf("%lf%lf%lf", &a, &b, &eps);                                               // Считывает все прочие параметры
    double x = root(p1, p2, a, b, eps, pp1, pp2);
    printf("Testroot:\n%lf\n", x);
}

void testintegral(void)                                                             // Для тестирования функциии integral
{
    double (*p)(double);
    int F;
    double a, b, eps;
    scanf("%d", &F);                                                                // Считывает параметр для тестируемой функции
    switch (F)                                                                      // В зависимости от параметра выбирает указатель на нужную функцию
    {
        case 1:
            p = f1;
        break;
        case 2:
            p = f2;
        break;
        case 3:
            p = f3;
    }
    scanf("%lf%lf%lf", &a, &b, &eps);                                               // Считывает все прочие параметры
    double x = integral(p, a, b, eps);
    printf("Testroot:\n%lf\n", x);
}

int main(int argc, char *argv[])
{
    double x12, x23, x13, I1, I2, I3, eps1 = 0.01, eps2 = 0.0001, A = eps1, B = 3, S, i1 = 1, i2 = 1;
    int it1, it2, it3, i;
    while (i1 + i2 > 0.0005)                                                        // Цикл вычисляет необходимую точность для поиска абсцисс точек пересечения кривых
    {
        eps1 /= 10;
        i1 = integral(f3, A - eps1, A, eps2);
        i2 = integral(f3, B, B + eps1, eps2);
    }
    A = eps1;
    x12 = root(f1, f2, A, B, eps1, ff1, ff2);
    it1 = it;
    x23 = root(f2, f3, A, B, eps1, ff2, ff3);
    it2 = it;
    x13 = root(f1, f3, A, B, eps1, ff1, ff3);
    it3 = it;
    I1 = integral(f1, x13, x12, eps2);
    I2 = integral(f2, x23, x12, eps2);
    I3 = integral(f3, x13, x23, eps2);
    S = I1 - I2 - I3;
    printf("%.3lf\n", S);
    for (i = 1; i < argc; i++)                                                      // Цикл проверяет наличие ключей и выполняет заданные ими команды
    {
        if (strcmp(argv[i] , "abs") == 0)
        {
            printf("Абсциссы точек пересечения кривых между кривыми соответственно 1 и 2, 2 и 3, 1 и 3:\n%lf\t%lf\t%lf\n", x12, x23, x13);
        }
        if (strcmp(argv[i], "iter") == 0)
        {
            printf("Число операций, необходимое для поиска точек пересечения кривых соответственно 1 и 2, 2 и 3, 1 и 3:\n%d\t%d\t%d\n", it1, it2, it3);
        }
        if (strcmp(argv[i], "test") == 0)
        {
            if (strcmp(argv[i + 1], "root") == 0)
            {
                testroot();
            }
            if (strcmp(argv[i + 1], "integral") == 0)
            {
                testintegral();
            }
        }
        if (strcmp(argv[i] , "-help") == 0)
        {
            printf("-help\t\tВыводит на печать все допустимые ключи командной строки\n");
            printf("abs\t\tВыводит на печать абсциссы точек пересечения кривых\n");
            printf("iter\t\tВыводит на печать число операций, необходимых для вычисления каждой из точек пересечения кривых\n");
            printf("test root\tПозволяет протестировать функцию root, вычисляющую абсциссы точек пересечения заданных кривых на заданном отрезке с заданной точностью соответственно\n");
            printf("test integral\tПозволяет протестировать функцию integral, вычисляющую интеграл заданной функции на заданном отрезке с заданной точностью соответственно\n");
        }
    }
    return 0;
}
