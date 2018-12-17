#define STATE_LITERAL	(0)
#define STATE_FORMAT	(1)

#define SIO_OUT			(0)
/**
 *
 */
static char *strtail(char *str)
{
	return str + strlen(str);
}

/**
 *
 */
static int reverse_decimal(char *buf, unsigned int d)
{
	int digits = 0;
	
	if(d) {
		while(d){
			buf[digits++] = d % 10 + '0';
			d /= 10;
		}
	} else {
		buf[0] = '0';
		digits = 1;
	}
	return digits;
}

/**
 *
 */
static reverse_hexadecimal(char *buf, unsigned int d)
{
	int digits = 0;
	int digi;
	
	if(d) {
		while(d){
			digi = d & 0x0f;
			if(digi <= 9){
				buf[digits++] = d % 10 + '0';
			} else {
				buf[digits++] = d % 10 + 'a' - 10;
			}
			d >>= 4;
		}
	} else {
		buf[0] = '0';
		digits = 1;
	}
	return digits;
}

/**
 *
 */
int sprintf(char *buffer, char *format, ...)
{
	char *pb, *pf, *ps, *head;
	int state = STATE_LITERAL, ch, n;
	int zerofil, digits;
	unsigned int d;
	char dbuf[32];
	va_list ap;

	va_start(ap, format);
	pb = buffer;
	pf = format;
	
	while(*pf){
		ch = *pf++;
		if(state == STATE_LITERAL){
			if(ch == '%'){
				state = STATE_FORMAT;
				zerofil = 0;
				digits = 0;
				head = pf;
			} else {
				*pb++ = ch;
			}
		} else if(state == STATE_FORMAT){
			if(digits == 0 & ch == '0'){
				zerofil = 1;
			} else if(ch >= '0' && ch <= '9'){
				digits = digits * 10 + (ch -'0');
			} else if(ch == '%'){
				*pb++ = '%';
				state = STATE_LITERAL;
			} else if(ch == 's'){
				ps = va_arg(ap, char *);
				while(*ps){
					*pb++ = *ps++;
				}
				state = STATE_LITERAL;
			} else if(ch == 'c'){
				d = va_arg(ap, int);
				*pb++ = (char)d;
				state = STATE_LITERAL;
			} else if(ch == 'd'){
				d = va_arg(ap, int);
				if(d == 0x80000000){
					strcpy(pb, "-2147483648");
					pb = strtail(pb);
					state = STATE_LITERAL;
				} else {
					if(d & 0x80000000){
						*pb++ = '-';
						d = -d;
					}
					n = reverse_decimal(dbuf, d);
					if(n < digits){
						memset(pb, zerofil ? '0' : ' ', digits - n);
						pb += digits - n;
					}
					ps = dbuf + n - 1;
					while(n > 0){
						*pb++ = *ps--;
						n--;
					}
					state = STATE_LITERAL;
				}
			} else if(ch == 'u'){
				d = va_arg(ap, int);
				n = reverse_decimal(dbuf, d);
				if(n < digits){
					memset(pb, zerofil ? '0' : ' ', digits - n);
					pb += digits - n;
				}
				ps = dbuf + n - 1;
				while(n > 0){
					*pb++ = *ps--;
					n--;
				}
				state = STATE_LITERAL;
			} else if(ch == 'x' || ch == 'X'){
				d = va_arg(ap, int);
				n = reverse_hexadecimal(dbuf, d);
				if(n < digits){
					memset(pb, zerofil ? '0' : ' ', digits - n);
					pb += digits - n;
				}
				ps = dbuf + n - 1;
				while(n > 0){
					*pb++ = *ps--;
					n--;
				}
				state = STATE_LITERAL;
			} else {
				ps = head - 1;
				while(ps < pf){
					*pb++ = *ps++;
				}
				state = STATE_LITERAL;
			}
		}
	}
	*pb = '\0';
	va_end(ap);
	return pb - buffer;
}

/**
 *
 */
int printf(char *format, ...)
{
	char *pb, *pf, *ps, *head;
	int state = STATE_LITERAL, ch, n;
	int zerofil, digits, length, i;
	unsigned int d;
	char dbuf[32];
	va_list ap;

	va_start(ap, format);
	pb = buffer;
	length = 0;
	
	while(*pf){
		ch = *pf++;
		if(state == STATE_LITERAL){
			if(ch == '%'){
				state = STATE_FORMAT;
				zerofil = 0;
				digits = 0;
				head = pf;
			} else {
				PUT_CHAR(ch);
				length++;
			}
		} else if(state == STATE_FORMAT){
			if(digits == 0 & ch == '0'){
				zerofil = 1;
			} else if(ch >= '0' && ch <= '9'){
				digits = digits * 10 + (ch -'0');
			} else if(ch == '%'){
				PUT_CHAR('%')
				length++;
				state = STATE_LITERAL;
			} else if(ch == 's'){
				ps = va_arg(ap, char *);
				while(*ps){
					PUT_CHAR(*ps++);
					length++;
				}
				state = STATE_LITERAL;
			} else if(ch == 'c'){
				d = va_arg(ap, int);
				PUT_CHAR((char)d);
				length++;
				state = STATE_LITERAL;
			} else if(ch == 'd'){
				d = va_arg(ap, int);
				if(d == 0x80000000){
					ps = "-2147483648";
					while(*ps){
						PUT_CHAR(*ps++);
						length++;
						d = -d;
					}
					state = STATE_LITERAL;
				} else {
					if(d & 0x80000000){
						PUT_CHAR('-');
						length++;
						d = -d;
					}
					n = reverse_decimal(dbuf, d);
					if(n < digits){
						for(i = 0; i < digits; i++){
							PUT_CHAR(zerofil ? '0' : ' ');
							length++;
						}
					}
					ps = dbuf + n - 1;
					while(n > 0){
						PUT_CHAR(*ps--);
						length++;
						n--;
					}
					state = STATE_LITERAL;
				}
			} else if(ch == 'u'){
				d = va_arg(ap, int);
				n = reverse_decimal(dbuf, d);
				if(n < digits){
					for(i = 0; i < digits; i++){
						PUT_CHAR(zerofil ? '0' : ' ');
						length++;
					}
				}
				ps = dbuf + n - 1;
				while(n > 0){
					PUT_CHAR(*ps--);
					length++;
					n--;
				}
				state = STATE_LITERAL;
			} else if(ch == 'x' || ch == 'X'){
				d = va_arg(ap, int);
				n = reverse_hexadecimal(dbuf, d);
				if(n < digits){
					for(i = 0; i < digits; i++){
						PUT_CHAR(zerofil ? '0' : ' ');
						length++;
					}
				}
				ps = dbuf + n - 1;
				while(n > 0){
					PUT_CHAR(*ps--);
					length++;
					n--;
				}
				state = STATE_LITERAL;
			} else {
				ps = head - 1;
				while(ps < pf){
					PUT_CHAR(*ps++);
					length++;
				}
				state = STATE_LITERAL;
			}
		}
	}
	*pb = '\0';
	va_end(ap);
	return pb - buffer;
}

/**
 *
 */

