function main() {
  var i, sum;
  i = 1;
  sum = 0;
  printf("  i   sum\n");
  while (i < 10) {
    i = add(i, 1);
    sum = add(sum, i);
    printf("  %ld   %ld\n", i, sum);
  }

	printf("     %ld\n", doubleSquare(sum));
  return 0;
}

function add(a1, a2) {
  var a;
  a = a1 + a2;
  return a;
}

function adl(a1, a2) {
  var a;
  a = a1 + a2;
  a1 = 1;
  a2 = 4;
      printf("  %ld   %ld\n", a1, a2);
  return a;
}


function doubleSquare(a1) {
	var a;
	a = (a1 * a1) * 2;
	return a;
}
