PROGRAM readwrite2;
CONST
  msg1=" please key in two numbers: ",
  msg2=" numbers keyed in are ",
  msg3=" sum is ";
VAR
  m, n, sum;
BEGIN
  WRITE(msg1);
  READ(m,n);
  WRITE(msg2,m,n);
  sum := m+n;
  WRITE(msg3,sum);
END.
