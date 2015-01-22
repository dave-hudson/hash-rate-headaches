/*
 * hrh.c
 *	Poisson Process estimator used to generate the data for the "Hash Rate
 *	Headaches" article on hashingit.com.
 *
 * Copyright (C) 2015 David Hudson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <mpfr.h>

/*
 * calculate()
 *	Work out the per-day changes in the network.
 */
static void calculate(unsigned int first_event, unsigned int last_event, double average_events)
{
	printf("---------------- %5d, %5d, %7.1f -----------------\n", first_event, last_event, average_events);

	mpfr_t lambda;
	mpfr_init2(lambda, 1024);
	mpfr_set_d(lambda, average_events, MPFR_RNDN);

	mpfr_t neg_lambda;
	mpfr_init2(neg_lambda, 1024);
	mpfr_neg(neg_lambda, lambda, MPFR_RNDN);

	mpfr_t e_pow_neg_lambda;
	mpfr_init2(e_pow_neg_lambda, 1024);
	mpfr_exp(e_pow_neg_lambda, neg_lambda, MPFR_RNDN);

	mpfr_t lambda_pow_n;
	mpfr_init2(lambda_pow_n, 1024);

	mpfr_t n_fac;
	mpfr_init2(n_fac, 1024);

	mpfr_t num;
	mpfr_init2(num, 1024);

	mpfr_t p;
	mpfr_init2(p, 1024);

	mpfr_t cumulative;
	mpfr_init2(cumulative, 1024);
	mpfr_set_ui(cumulative, 0, MPFR_RNDN);

	for (unsigned int n = 0; n < last_event; n++) {
		mpfr_pow_ui(lambda_pow_n, lambda, n, MPFR_RNDN);
		mpfr_fac_ui(n_fac, n, MPFR_RNDN);
		mpfr_mul(num, e_pow_neg_lambda, lambda_pow_n, MPFR_RNDN);
		mpfr_div(p, num, n_fac, MPFR_RNDN);
		mpfr_add(cumulative, cumulative, p, MPFR_RNDN);

		printf("%5u | %14g | %14g | %14g\n", n, (double)n / average_events, mpfr_get_d(p, MPFR_RNDN), mpfr_get_d(cumulative, MPFR_RNDN));
	}

	mpfr_clear(cumulative);		
	mpfr_clear(p);		
	mpfr_clear(num);		
	mpfr_clear(n_fac);		
	mpfr_clear(lambda_pow_n);		
	mpfr_clear(e_pow_neg_lambda);		
	mpfr_clear(neg_lambda);		
	mpfr_clear(lambda);		
}

int main(int argc, char **argv)
{
	calculate(0, 14, 6);
	calculate(0, 14, 6.6);
	calculate(1900, 2200, 2016);
}

