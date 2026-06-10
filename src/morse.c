// morse.c - translate arguments into Morse code

#include <stdio.h>

int main(int argc, char *argv[]) {
  char *sigv[123] = {0};

  sigv['A'] = sigv['a'] = ".-";   sigv['1'] = ".----";
  sigv['B'] = sigv['b'] = "-..."; sigv['2'] = "..---";
  sigv['C'] = sigv['c'] = "-.-."; sigv['3'] = "...--";
  sigv['D'] = sigv['d'] = "-..";  sigv['4'] = "....-";
  sigv['E'] = sigv['e'] = ".";    sigv['5'] = ".....";
  sigv['F'] = sigv['f'] = "..-."; sigv['6'] = "-....";
  sigv['G'] = sigv['g'] = "--.";  sigv['7'] = "--...";
  sigv['H'] = sigv['h'] = "...."; sigv['8'] = "---..";
  sigv['I'] = sigv['i'] = "..";   sigv['9'] = "----.";
  sigv['J'] = sigv['j'] = ".---"; sigv['0'] = "-----";
  sigv['K'] = sigv['k'] = "-.-";
  sigv['L'] = sigv['l'] = ".-.."; sigv['.'] = ".-.-.-";
  sigv['M'] = sigv['m'] = "--";   sigv[','] = "--..--";
  sigv['N'] = sigv['n'] = "-.";   sigv[':'] = "---...";
  sigv['O'] = sigv['o'] = "---";  sigv['?'] = "..--..";
  sigv['P'] = sigv['p'] = ".--."; sigv['\''] = ".----.";
  sigv['Q'] = sigv['q'] = "--.-"; sigv['-'] = "-....-";
  sigv['R'] = sigv['r'] = ".-.";  sigv['/'] = "-..-.";
  sigv['S'] = sigv['s'] = "...";  sigv['('] = "-.--.";
  sigv['T'] = sigv['t'] = "-";    sigv[')'] = "-.--.-";
  sigv['U'] = sigv['u'] = "..-";  sigv['"'] = ".-..-.";
  sigv['V'] = sigv['v'] = "...-"; sigv['='] = "-...-";
  sigv['W'] = sigv['w'] = ".--";  sigv['+'] = ".-.-.";
  sigv['X'] = sigv['x'] = "-..-"; sigv['@'] = ".--.-.";
  sigv['Y'] = sigv['y'] = "-.--";
  sigv['Z'] = sigv['z'] = "--..";

  while (*++argv)
    while (**argv) {
      fputs(sigv[**argv], stdout);
      putchar(*++*argv ? ' ' : '\n');
    }
}
