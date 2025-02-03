// morse.c - print arguments as Morse code

#include <stdio.h>

int main(int argc, char *argv[]) {
  char *sigv[123] = {0};

  sigv['A'] = sigv['a'] = ".-";
  sigv['B'] = sigv['b'] = "-...";
  sigv['C'] = sigv['c'] = "-.-.";
  sigv['D'] = sigv['d'] = "-..";
  sigv['E'] = sigv['e'] = ".";
  sigv['F'] = sigv['f'] = "..-.";
  sigv['G'] = sigv['g'] = "--.";
  sigv['H'] = sigv['h'] = "....";
  sigv['I'] = sigv['i'] = "..";
  sigv['J'] = sigv['j'] = ".---";
  sigv['K'] = sigv['k'] = "-.-";
  sigv['L'] = sigv['l'] = ".-..";
  sigv['M'] = sigv['m'] = "--";
  sigv['N'] = sigv['n'] = "-.";
  sigv['O'] = sigv['o'] = "---";
  sigv['P'] = sigv['p'] = ".--.";
  sigv['Q'] = sigv['q'] = "--.-";
  sigv['R'] = sigv['r'] = ".-.";
  sigv['S'] = sigv['s'] = "...";
  sigv['T'] = sigv['t'] = "-";
  sigv['U'] = sigv['u'] = "..-";
  sigv['V'] = sigv['v'] = "...-";
  sigv['W'] = sigv['w'] = ".--";
  sigv['X'] = sigv['x'] = "-..-";
  sigv['Y'] = sigv['y'] = "-.--";
  sigv['Z'] = sigv['z'] = "--..";

  sigv['1'] = ".----";
  sigv['2'] = "..---";
  sigv['3'] = "...--";
  sigv['4'] = "....-";
  sigv['5'] = ".....";
  sigv['6'] = "-....";
  sigv['7'] = "--...";
  sigv['8'] = "---..";
  sigv['9'] = "----.";
  sigv['0'] = "-----";

  sigv[ '.'] = ".-.-.-";
  sigv[ ','] = "--..--";
  sigv[ ':'] = "---...";
  sigv[ '?'] = "..--..";
  sigv['\''] = ".----.";
  sigv[ '-'] = "-....-";
  sigv[ '/'] = "-..-.";
  sigv[ '('] = "-.--.";
  sigv[ ')'] = "-.--.-";
  sigv['\"'] = ".-..-.";
  sigv[ '='] = "-...-";
  sigv[ '+'] = ".-.-.";
  sigv[ '@'] = ".--.-.";

  while (*++argv)
    while (**argv) {
      fputs(sigv[**argv], stdout);
      putchar(*++*argv ? ' ' : '\n');
    }
}
