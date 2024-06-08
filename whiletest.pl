PROGRAM whiletest;
CONST
  msg1=" keyin a number (a): ",
  msg2=" keyin a number (b): ",
  msg3="H.C.F =";
VAR
  a,b,c,q,r,hcf;
BEGIN
  WRITE(msg1);
  READ(a);
  WRITE(msg2);
  READ(b);
  IF a<b THEN
    BEGIN
      c:=a;
      a:=b;
      b:=c;
    END;
  WHILE b > 0 DO
    BEGIN
      q := a/b;
      r := a-b*q;
      a := b;
      b := r;
    END;
  hcf := a;
  WRITE(msg3,hcf);
END.
