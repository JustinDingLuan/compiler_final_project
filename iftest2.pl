PROGRAM iftest2;
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
  IF a>=b THEN
    BEGIN
      bigger:=a;
      WRITE(msg3,bigger);
    END;
  IF b>a THEN
    BEGIN
      bigger:=b;
      WRITE(msg3,bigger);
    END;
END.
