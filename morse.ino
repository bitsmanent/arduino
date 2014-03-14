#define PIN		9
#define FREQ		540
#define DELAY		50

#define LENGTH(X)	(sizeof X / sizeof X[0])
#define CSENSITIVE	0

char buf[1024];

struct cvt_t {
	char c;
	long int i;
};

struct cvt_t cvtmap[] = {
	{ 'A', 12 },
	{ 'B', 2111 },
	{ 'C', 2121 },
	{ 'D', 211 },
	{ 'E', 1 },
	{ 'F', 1121 },
	{ 'G', 221 },
	{ 'H', 1111 },
	{ 'I', 11 },
	{ 'J', 1222 },
	{ 'K', 212 },
	{ 'L', 1211 },
	{ 'M', 22 },
	{ 'N', 21 },
	{ 'O', 222 },
	{ 'P', 1221 },
	{ 'Q', 2212 },
	{ 'R', 121 },
	{ 'S', 111 },
	{ 'T', 2 },
	{ 'U', 112 },
	{ 'V', 1112 },
	{ 'W', 122 },
	{ 'X', 2112 },
	{ 'Y', 2122 },
	{ 'Z', 2211 },
	{ '0', 22222 },
	{ '1', 12222 },
	{ '2', 11222 },
	{ '3', 11122 },
	{ '4', 11112 },
	{ '5', 11111 },
	{ '6', 21111 },
	{ '7', 22111 },
	{ '8', 22211 },
	{ '9', 22221 },
	{ '.', 121212 },
	{ ',', 221122 },
	{ '*', 112211 },
};

/* function declarations */
int slen(char *);
struct cvt_t *cvtmatch(char c);
void out(char *);
void cvtrun(char *);

/* function implementations */
int
slen(char *s) {
	int len = 0;
	while(*(s++))
		++len;
	return len;
}

struct cvt_t * 
cvtmatch(char c) {
	int i;
	char t;

	for(i = 0; i < LENGTH(cvtmap); ++i) {
		if(c == cvtmap[i].c) {
			return (cvtmap + i);
		}
	}

	return NULL;
}

void
out(char *s) {
	if(!*s) {
		delay(DELAY);
		return;
	}

	int t;
	switch(*s - '0') {
		case 1: t = 75; break;
		case 2: t = 150; break;
		default: t = 0; break;
	}

	tone(PIN, FREQ, t);
	delay(DELAY + t);
	out(s + 1);
}

void
cvtrun(char *s) {
	struct cvt_t *cvt;
	int i, cvti, len = slen(s);
	char c, str[8];

	for(i = 0; i < len; ++i) {
		c = s[i];
#if !CSENSITIVE
		if(c >= 'a' && c <= 'z')
			c -= 32;
#endif
		cvt = cvtmatch(c);
		if(!cvt)
			continue;

		snprintf(str, sizeof str, "%ld", cvt->i);

		out(str);
	}
}

void
setup(void) {
	Serial.begin(9600);
	pinMode(PIN, OUTPUT);
}

void
loop(void) {
	char c;

	if(Serial.available() <= 0)
		return;

	c = Serial.read();
	if(c != 13) {
		Serial.print(c);
		snprintf(buf, sizeof buf, "%s%c", buf, c);
		return;
	}
	cvtrun(buf);
	buf[0] = '\0';

	Serial.println();
	Serial.print("> ");
}
