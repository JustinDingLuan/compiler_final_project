PROGRAM iftest;
CONST
  msg1=" please key in a number (a): ",
  msg2=" please key in a number (b): ",
  msg3=" the bigger is ";
VAR
  a, b, bigger;
BEGIN
  WRITE(msg1);
  READ(a);
  WRITE(msg2);
  READ(b);
  IF a>=b THEN bigger:=a;
  IF b>a THEN bigger:=b;
  WRITE(msg3,bigger);
END.
