int buffer[MAX];
int prod = 0;
int cons = 0;

void produz(int valor) {
buffer[prod] = valor; // f1
prod = (prod + 1) % MAX; // f2
}

int consome() {
int tmp = buffer[cons]; // g1
cons = (cons + 1) % MAX; // g2
return tmp;
}